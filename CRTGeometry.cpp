#include "CRTGeometry.h"
#include "CMemPool.h"
#include <new>

extern void EngineTreeInit(void *pTreeHead);
extern void *EngineTreeAllocSentinel();
extern void EngineFreeBlock(void *pBlock, unsigned int nSize);
extern void EngineFreeArray(void *pBegin, unsigned int nByteCount);
extern CRTSubMesh *EngineCreateSubMeshForMaterial(void *pMeshSlot, int nMaterialIndex);
extern void EnginePushBackPointer(void *pVector, void *pValue);
extern void EngineSkinBuildFromMesh(void *pSkinData, void *pMeshData);
extern void EngineSkinFinalize(void *pSkinData, void *pMeshData);
extern int EngineFindMaterialSlot(void *pMaterialMap, const char *pszName);
extern void EngineBoneListAnchorInit(char *pFlag, void **pHead, unsigned int *pReserved48);
extern void EngineBoneListAnchorDestroy(char *pFlag, void **pHead, unsigned int *pReserved48);
extern void EngineFreeVector12(void *pBegin, void *pEnd);
extern void EngineFreeVector32(void *pBegin, void *pEnd);

static CMemPool<5994344087555276800ULL, CRTGeometry, 1000, 1> g_geometryPool;

CRTGeometry::CRTGeometry()
{
    m_pSourceMesh = 0;
    m_pMeshData = reinterpret_cast<void *>(-1);
    m_pSubMeshBegin = 0;
    m_pSubMeshEnd = 0;
    m_pSubMeshCapacity = 0;
    m_nOwnerId = 0;
    m_bHasSkin = 0;
    m_nLodIndex = 0;

    EngineTreeInit(&m_pMaterialMapHead);
    m_pMaterialMapHead = (SGeometryMaterialSlot *)EngineTreeAllocSentinel();
    m_pMaterialMapHead->m_pNext = m_pMaterialMapHead;
    m_nMaterialMapSize = 0;

    m_bBatchesDirty = 0;
    m_nRefCount = 1;

    EngineBoneListAnchorInit(&m_bBoneListFlag, &m_pBoneWeightListHead, &m_dwBoneListUnk48);

    m_pRenderStateBegin = 0;
    m_pRenderStateEnd = 0;
    m_pRenderStateCapacity = 0;
    m_pSkinDataBegin = 0;
    m_pSkinDataEnd = 0;
    m_pSkinDataCapacity = 0;

    m_bBoneListLocked0 = 1;
    m_bBoneListLocked1 = 1;
    m_pUnk64 = 0;
    m_pUnk68 = 0;
}

CRTGeometry::~CRTGeometry()
{
    ClearSkinData();

    ClearRenderState();

    EngineBoneListAnchorDestroy(&m_bBoneListFlag, &m_pBoneWeightListHead, &m_dwBoneListUnk48);

    ClearMaterialLookup();

    ClearSubMeshBatches();
}

CRTGeometry *CRTGeometry::AllocateAndConstruct()
{
    void *pRaw = g_geometryPool.Alloc();
    if (!pRaw)
        return 0;
    return new (pRaw) CRTGeometry();
}

void CRTGeometry::ReleaseToPool(CRTGeometry *pGeometry)
{
    g_geometryPool.Free(pGeometry);
}

void CRTGeometry::ClearSubMeshBatches()
{
    if (m_pSubMeshBegin)
        EngineFreeArray(m_pSubMeshBegin, (unsigned int)((char *)m_pSubMeshEnd - (char *)m_pSubMeshBegin));
    m_pSubMeshBegin = 0;
    m_pSubMeshEnd = 0;
    m_pSubMeshCapacity = 0;
}

void CRTGeometry::ClearMaterialLookup()
{
    SGeometryMaterialSlot *pNode = m_pMaterialMapHead->m_pNext;
    while (pNode != m_pMaterialMapHead)
    {
        SGeometryMaterialSlot *pNext = pNode->m_pNext;
        EngineFreeBlock(pNode, 48);
        pNode = pNext;
    }
    EngineFreeBlock(m_pMaterialMapHead, 48);
    m_pMaterialMapHead = 0;
    m_nMaterialMapSize = 0;
}

void CRTGeometry::ClearSkinData()
{
    if (m_pSkinDataBegin)
        EngineFreeVector32(m_pSkinDataBegin, m_pSkinDataEnd);
    m_pSkinDataBegin = 0;
    m_pSkinDataEnd = 0;
    m_pSkinDataCapacity = 0;
}

void CRTGeometry::ClearRenderState()
{
    if (m_pRenderStateBegin)
        EngineFreeVector12(m_pRenderStateBegin, m_pRenderStateEnd);
    m_pRenderStateBegin = 0;
    m_pRenderStateEnd = 0;
    m_pRenderStateCapacity = 0;
}

int CRTGeometry::Build(CRTMesh *pSourceMesh, CRTBoneWeightSet *pBoneWeights, bool bBuildSkin, int nOwnerId, int nLodIndex)
{
    m_pSourceMesh = pSourceMesh;
    m_pMeshData = pBoneWeights;
    m_nOwnerId = nLodIndex;
    if (bBuildSkin)
        m_bHasSkin = EngineFindMaterialSlot(&m_pMaterialMapHead, (const char *)pBoneWeights);

    BuildSubMeshBatches();
    RebuildSkinData();
    return 1;
}

bool CRTGeometry::BuildSubMeshBatches()
{
    if (!m_pMeshData)
        return false;

    for (int i = 0; i < 1; ++i)
    {
        CRTSubMesh *pSub = EngineCreateSubMeshForMaterial(m_pMeshData, i);
        if (!pSub)
            return false;

        EnginePushBackPointer(&m_pSubMeshBegin, pSub);
    }

    m_bBatchesDirty = 1;
    return true;
}

int CRTGeometry::RebuildSkinData()
{
    EngineSkinBuildFromMesh(&m_pSkinDataBegin, m_pMeshData);
    if (m_pUnk64)
    {
        EngineSkinFinalize(&m_pSkinDataBegin, m_pMeshData);
        return 1;
    }
    return 0;
}

int CRTGeometry::FindMaterialIndex(const char *pszMaterialName) const
{
    return EngineFindMaterialSlot((void *)&m_pMaterialMapHead, pszMaterialName);
}

CRTMesh *CRTGeometry::GetSourceMesh() const
{
    return (CRTMesh *)m_pSourceMesh;
}

CRTSubMesh *const *CRTGeometry::GetSubMeshBegin() const
{
    return m_pSubMeshBegin;
}

CRTSubMesh *const *CRTGeometry::GetSubMeshEnd() const
{
    return m_pSubMeshEnd;
}

int CRTGeometry::GetSubMeshCount() const
{
    return (int)(m_pSubMeshEnd - m_pSubMeshBegin);
}

int CRTGeometry::GetLodIndex() const
{
    return m_nLodIndex;
}

int CRTGeometry::GetOwnerId() const
{
    return m_nOwnerId;
}
