#include "CRTModDriver.h"
#include "CMemPool.h"
#include <new>

extern void *EngineAllocFromPool(void *pPoolDescriptor);
extern void EngineFreeBlock(void *pBlock, unsigned int nSize);
extern void EngineFreeArray(void *pBegin, unsigned int nByteCount);
extern void *EngineTreeAllocSentinel();
extern void EngineTreeEraseNode(void *pTreeThis, void *pOutIter, void *pNode);
extern void *EngineTreeFind(void *pTreeThis, int nKey);
extern void EngineSoundBufferHolderInit(SDriverSoundBufferHolder *pHolder);
extern void EngineSoundBufferHolderDestroy(SDriverSoundBufferHolder *pHolder);
extern void EngineDetachAllGroups(CRTModDriver *pThis);

static CMemPool<5994344087555473408ULL, CRTModDriver, 1000, 1> g_driverPool;

CRTModDriver::CRTModDriver()
{
    EngineSoundBufferHolderInit(&m_soundBufferHolder);

    m_pWaveIdArrayBegin = 0;
    m_pWaveIdArrayEnd = 0;
    m_pWaveIdArrayCapacity = 0;

    m_pModSetMapHead = (SModsetMapNode *)EngineTreeAllocSentinel();
    m_pModSetMapHead->m_bIsNil = 1;
    m_pModSetMapHead->m_pParent = m_pModSetMapHead;
    m_pModSetMapHead->m_pLeft = m_pModSetMapHead;
    m_pModSetMapHead->m_pRight = m_pModSetMapHead;
    m_nModSetMapSize = 0;

    m_pOwnerRegistry = 0;
    m_pRegistryContext = 0;
}

CRTModDriver::~CRTModDriver()
{
    EngineDetachAllGroups(this);

    ClearModSetMap();

    ClearWaveIdArray();

    ClearSoundBufferHolder();
}

CRTModDriver *CRTModDriver::AllocateAndConstruct()
{
    void *pRaw = g_driverPool.Alloc();
    if (!pRaw)
        return 0;
    return new (pRaw) CRTModDriver();
}

void CRTModDriver::ReleaseToPool(CRTModDriver *pDriver)
{
    g_driverPool.Free(pDriver);
}

void CRTModDriver::BindOwnerRegistry(void *pOwner, void *pRegistry)
{
    m_pOwnerRegistry = pOwner;
    m_pRegistryContext = pRegistry;
}

void CRTModDriver::ClearSoundBufferHolder()
{
    EngineSoundBufferHolderDestroy(&m_soundBufferHolder);
}

void CRTModDriver::ClearWaveIdArray()
{
    if (m_pWaveIdArrayBegin)
        EngineFreeArray(m_pWaveIdArrayBegin, (unsigned int)((char *)m_pWaveIdArrayCapacity - (char *)m_pWaveIdArrayBegin));
    m_pWaveIdArrayBegin = 0;
    m_pWaveIdArrayEnd = 0;
    m_pWaveIdArrayCapacity = 0;
}

void CRTModDriver::ClearModSetMap()
{
    if (m_pModSetMapHead)
        EngineFreeBlock(m_pModSetMapHead, 48);
    m_pModSetMapHead = 0;
    m_nModSetMapSize = 0;
}

bool CRTModDriver::HasModSet(int nGroupId, int nKey) const
{
    (void)nGroupId;
    return EngineTreeFind((void *)&m_pModSetMapHead, nKey) != 0;
}

bool CRTModDriver::Dispatch(int nGroupId, int nKey, void *pAttachParams)
{
    if (!pAttachParams)
        return m_pOwnerRegistry ? Detach(nGroupId, nKey) : false;

    return true;
}

bool CRTModDriver::Detach(int nGroupId, int nKey)
{
    if (!HasModSet(nGroupId, nKey))
        return false;

    void *pFoundEntry = EngineTreeFind((void *)&m_pModSetMapHead, nKey);
    if (!pFoundEntry)
        return false;

    SModsetMapNode *pNode = (SModsetMapNode *)pFoundEntry;
    if (pNode->m_nRefCount > 1)
    {
        --pNode->m_nRefCount;
        return true;
    }

    EngineTreeEraseNode(&m_pModSetMapHead, 0, pFoundEntry);
    --m_nModSetMapSize;
    return false;
}

bool CRTModDriver::DetachGroup(int nGroupId)
{
    (void)nGroupId;
    EngineDetachAllGroups(this);
    return true;
}

bool CRTModDriver::Attach(int nGroupId, void *pWaveIdPair, int nParamA, int nParamB, void *pTag3, void *pTag3b, int nFlagsA, int nFlagsB)
{
    (void)nGroupId;
    (void)pWaveIdPair;
    (void)nParamA;
    (void)nParamB;
    (void)pTag3;
    (void)pTag3b;
    (void)nFlagsA;
    (void)nFlagsB;
    return true;
}
