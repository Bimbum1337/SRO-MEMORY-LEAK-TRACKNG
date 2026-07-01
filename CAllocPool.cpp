#include "CAllocPool.h"
#include <cstdlib>
#include <cstring>
#include <windows.h>

extern bool EngineDebugAssert(int nLine, const char *pszFile, const char *pszExpr, char chContext);

static const char s_pszIMemoryFile[] = "c:\\vss-od\\jmx_library\\enginecommon\\IMemory.h";

struct SPoolBlockHeader
{
    SPoolBlockHeader *m_pNext;
    CAllocPool *m_pOwner;
};

CAllocPool::CAllocPool(unsigned int dwTypeTag1, unsigned int dwTypeTag2, int nElementSize, int nCommitBatch, int nUnused, bool bAlign16)
{
    m_dwUnk04 = 0;
    m_dwUnk08 = 0;
    m_nInUseCount = 0;
    m_dwUnk10 = 0;
    m_pChunkListHead = 0;
    m_pChunkListTail = 0;
    m_nCommitCount = 0;
    m_pFreeListHead = 0;
    m_pFreeListTail = 0;
    m_nFreeCount = 0;

    m_chTypeTag1[0] = static_cast<char>(dwTypeTag2 >> 24);
    m_chTypeTag1[1] = static_cast<char>(dwTypeTag2 >> 16);
    m_chTypeTag1[2] = static_cast<char>(dwTypeTag2 >> 8);
    m_chTypeTag1[3] = static_cast<char>(dwTypeTag2);
    m_chTypeTag2[0] = static_cast<char>(dwTypeTag1 >> 24);
    m_chTypeTag2[1] = static_cast<char>(dwTypeTag1 >> 16);
    m_chTypeTag2[2] = static_cast<char>(dwTypeTag1 >> 8);
    m_chTypeTag2[3] = static_cast<char>(dwTypeTag1);
    m_bUnk48 = false;

    if (m_chTypeTag1[0] == 0 && !EngineDebugAssert(246, s_pszIMemoryFile, "m_chTypeName[0]", m_chTypeTag1[0]))
        DebugBreak();

    m_bDebugGuardBytes = false;
    m_nInUseCount = 0;
    m_nElementSize = nElementSize;
    m_nStride = nElementSize + 8;
    if (bAlign16)
        m_nStride = (nElementSize + 23) & ~0xF;
    m_nElementSizeDup = nElementSize;
    m_dwUnk20 = 0;
    m_nCommitBatch = nCommitBatch;

    (void)nUnused;
}

CAllocPool::~CAllocPool()
{
    ReleaseAllChunks();
}

void CAllocPool::ReleaseAllChunks()
{
    void *pChunk = m_pChunkListHead;
    while (pChunk)
    {
        void *pNext = *reinterpret_cast<void **>(pChunk);
        std::free(pChunk);
        pChunk = pNext;
    }

    m_pChunkListTail = 0;
    m_pChunkListHead = 0;
    m_nCommitCount = 0;
    m_pFreeListTail = 0;
    m_pFreeListHead = 0;
    m_nFreeCount = 0;
    m_nInUseCount = 0;
    m_dwUnk08 = 0;
}

void *CAllocPool::GrowPool()
{
    int nBatch = m_nCommitBatch;
    if (nBatch == -1)
    {
        if (m_nCommitCount)
            nBatch = m_nCommitCount > 10 ? 100 : 50;
        else
            nBatch = 1;
    }

    char *pChunk = static_cast<char *>(std::malloc(nBatch * m_nStride + 48));
    *reinterpret_cast<void **>(pChunk) = 0;
    *reinterpret_cast<int *>(pChunk + 4) = nBatch;
    *reinterpret_cast<int *>(pChunk + 8) = reinterpret_cast<int>(pChunk) - (reinterpret_cast<int>(pChunk) & 0xFFFFFFF0) + 8;
    ++m_nCommitCount;

    if (m_pChunkListTail)
        *reinterpret_cast<void **>(m_pChunkListTail) = pChunk;
    else
        m_pChunkListHead = pChunk;
    m_pChunkListTail = pChunk;

    char *pNode = pChunk + *reinterpret_cast<int *>(pChunk + 8) + 16;
    void *pResult = pNode;
    for (int i = 0; i < nBatch; ++i)
    {
        SPoolBlockHeader *pHeader = reinterpret_cast<SPoolBlockHeader *>(pNode);
        pHeader->m_pOwner = 0;
        if (m_bDebugGuardBytes)
            std::memset(pNode + m_nElementSize + 8, 0xCE, 16);
        ++m_nFreeCount;
        pHeader->m_pNext = 0;

        if (m_pFreeListHead)
            reinterpret_cast<SPoolBlockHeader *>(m_pFreeListTail)->m_pNext = pHeader;
        else
            m_pFreeListHead = pHeader;
        m_pFreeListTail = pHeader;

        pNode += m_nStride;
    }

    return pNode;
}

void *CAllocPool::Alloc()
{
    SPoolBlockHeader *pNode = static_cast<SPoolBlockHeader *>(m_pFreeListHead);
    if (pNode)
    {
        m_pFreeListHead = pNode->m_pNext;
        if (!pNode->m_pNext)
            m_pFreeListTail = pNode->m_pNext;
        --m_nFreeCount;
    }
    else
    {
        GrowPool();
        pNode = static_cast<SPoolBlockHeader *>(m_pFreeListHead);
        if (pNode)
        {
            m_pFreeListHead = pNode->m_pNext;
            if (!pNode->m_pNext)
                m_pFreeListTail = pNode->m_pNext;
            --m_nFreeCount;
        }
    }

    ++m_nInUseCount;
    pNode->m_pOwner = this;
    pNode->m_pNext = 0;

    void *pPayload = reinterpret_cast<char *>(pNode) + 8;
    if (m_bDebugGuardBytes)
        ValidateGuardBytes(pPayload);

    return pPayload;
}

void CAllocPool::Free(void *pElement)
{
    ValidateBlockOwnership(pElement);

    void **ppVtable = *reinterpret_cast<void ***>(pElement);
    ScalarDeletingDtorFn pDtor = reinterpret_cast<ScalarDeletingDtorFn>(ppVtable[0]);
    pDtor(pElement, 0);

    *reinterpret_cast<unsigned int *>(pElement) = 0xFEEEFEEE;

    SPoolBlockHeader *pHeader = reinterpret_cast<SPoolBlockHeader *>(static_cast<char *>(pElement) - 8);
    pHeader->m_pOwner = 0;
    ++m_nFreeCount;
    pHeader->m_pNext = 0;

    if (m_pFreeListHead)
        reinterpret_cast<SPoolBlockHeader *>(m_pFreeListTail)->m_pNext = pHeader;
    else
        m_pFreeListHead = pHeader;
    m_pFreeListTail = pHeader;

    if (--m_nInUseCount == 0)
        ReleaseAllChunks();
}

bool CAllocPool::IsAddressWithinChunks(void *pElement)
{
    unsigned int dwAddr = reinterpret_cast<unsigned int>(pElement);
    char *pChunk = static_cast<char *>(m_pChunkListHead);
    while (pChunk)
    {
        char *pNextChunk = *reinterpret_cast<char **>(pChunk);
        int nBatch = *reinterpret_cast<int *>(pChunk + 4);
        unsigned int dwChunkEnd = reinterpret_cast<unsigned int>(pChunk) + static_cast<unsigned int>(nBatch) * m_nStride + 48;
        if (dwAddr >= reinterpret_cast<unsigned int>(pChunk) && dwAddr < dwChunkEnd)
            return true;
        pChunk = pNextChunk;
    }
    return false;
}

void CAllocPool::ValidateBlockOwnership(void *pElement)
{
    SPoolBlockHeader *pHeader = reinterpret_cast<SPoolBlockHeader *>(static_cast<char *>(pElement) - 8);
    CAllocPool *pRecordedOwner = pHeader->m_pOwner;

    if (pRecordedOwner != this)
    {
        if (pRecordedOwner)
        {
            if (IsAddressWithinChunks(pHeader))
            {
                if (!EngineDebugAssert(484, s_pszIMemoryFile, "original memory dump", m_chTypeTag1[0]))
                    DebugBreak();
            }
            else
            {
                if (!EngineDebugAssert(489, s_pszIMemoryFile, "arg buff memory dump", m_chTypeTag1[0]))
                    DebugBreak();
            }
        }
        else
        {
            if (!EngineDebugAssert(474, s_pszIMemoryFile, "memory dump", m_chTypeTag1[0]))
                DebugBreak();
        }
    }

    if (m_bDebugGuardBytes)
        ValidateGuardBytes(pElement);
}

void CAllocPool::ValidateGuardBytes(void *pElement)
{
    static const unsigned char s_abGuard[16] =
    {
        0xCE, 0xCE, 0xCE, 0xCE, 0xCE, 0xCE, 0xCE, 0xCE,
        0xCE, 0xCE, 0xCE, 0xCE, 0xCE, 0xCE, 0xCE, 0xCE
    };

    if (m_bDebugGuardBytes
        && std::memcmp(static_cast<char *>(pElement) + m_nElementSize, s_abGuard, sizeof(s_abGuard)) != 0
        && !EngineDebugAssert(370, s_pszIMemoryFile, "guard bytes", m_chTypeTag1[0]))
    {
        DebugBreak();
    }
}
