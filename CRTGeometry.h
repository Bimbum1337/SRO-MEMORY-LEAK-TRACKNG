#ifndef CRTGEOMETRY_H
#define CRTGEOMETRY_H

#include <vector>
#include <map>

class CRTMesh;
class CRTSubMesh;
class CRTMaterial;
class CRTBoneWeightSet;

struct SGeometryMaterialSlot
{
    int m_nMaterialIndex;
    unsigned int m_nHash;
    SGeometryMaterialSlot *m_pNext;
};

class CRTGeometry
{
public:
    CRTGeometry();
    virtual ~CRTGeometry();

    static CRTGeometry *AllocateAndConstruct();
    static void ReleaseToPool(CRTGeometry *pGeometry);

    int Build(CRTMesh *pSourceMesh, CRTBoneWeightSet *pBoneWeights, bool bBuildSkin, int nOwnerId, int nLodIndex);
    bool BuildSubMeshBatches();
    int RebuildSkinData();
    int FindMaterialIndex(const char *pszMaterialName) const;

    CRTMesh *GetSourceMesh() const;
    CRTSubMesh *const *GetSubMeshBegin() const;
    CRTSubMesh *const *GetSubMeshEnd() const;
    int GetSubMeshCount() const;
    int GetLodIndex() const;
    int GetOwnerId() const;

protected:
    void ClearSubMeshBatches();
    void ClearMaterialLookup();
    void ClearSkinData();
    void ClearRenderState();

protected:
    void *m_pSourceMesh;
    void *m_pMeshData;
    void *m_pSubMeshVectorOwner;
    CRTSubMesh **m_pSubMeshBegin;
    CRTSubMesh **m_pSubMeshEnd;
    CRTSubMesh **m_pSubMeshCapacity;
    int m_nOwnerId;
    int m_bHasSkin;
    int m_nLodIndex;
    int m_dwMaterialMapReserved;
    SGeometryMaterialSlot *m_pMaterialMapHead;
    unsigned int m_nMaterialMapSize;
    int m_bBatchesDirty;
    int m_nRefCount;
    char m_bBoneListFlag;
    char m_abPad3C[3];
    void *m_pBoneWeightListHead;
    unsigned int m_dwBoneListUnk44;
    unsigned int m_dwBoneListUnk48;
    unsigned int m_adwBoneListReserved[4];
    int m_bBoneListLocked0;
    int m_bBoneListLocked1;
    void *m_pUnk64;
    void *m_pUnk68;
    int m_dwArrayReserved6C;
    void *m_pRenderStateBegin;
    void *m_pRenderStateEnd;
    void *m_pRenderStateCapacity;
    int m_dwArrayReserved7C;
    void *m_pSkinDataBegin;
    void *m_pSkinDataEnd;
    void *m_pSkinDataCapacity;
};

#endif
