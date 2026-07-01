#include "CCompChar.h"
#include "CRTGeometry.h"
#include "CMemPool.h"
#include <map>
#include <new>

static CMemPool<5994344087672782848ULL, CCompChar, 1000, 1> g_compCharPool;

extern void *FindAttachSocketByKind(CCompoundObj *pThis, int nKind);
extern void ReleaseCharSocketBindings(CCompoundObj *pThis);
extern void *FindMaterialSocketEntry(void *pModDriver, int nKind);
extern int RebuildAttachSocketVisual(void *pAttachedOwner, void *pMaterialSocket, int bForceReset);
extern void NotifyCharObjAiState5(int nCharObjHandle);
extern int GetCharObjDetailType(int nCharObjHandle);
extern void DispatchAiEndEvent(CCompChar *pThis, int nLoadResult);
extern bool AttachSourceMatchesSocketKind(void *pSource, int nSocketKind);
extern bool AttachGeometryMatchesSocketKind(CRTGeometry *pGeometry, int nSocketKind);
extern int GetAttachSocketTypeTag(void *pSocketData);
extern bool AttachSocketHasWordFlagFive(void *pSocketData);
extern bool GetAttachSocketExtraFlagSet(void *pSocketData);
extern void SwapModelVariantForSocket(CCompChar *pThis, CCompChar *pOwner, int nSocketKind);

CCompChar::CCompChar() : CCompoundObj()
{
}

CCompChar::~CCompChar()
{
    ResetState();
}

CCompChar *CCompChar::AllocateAndConstruct()
{
    void *pRaw = g_compCharPool.Alloc();
    if (!pRaw)
        return 0;
    return new (pRaw) CCompChar();
}

void CCompChar::ReleaseToPool(CCompChar *pChar)
{
    g_compCharPool.Free(pChar);
}

int CCompChar::InvokeUpdateHook()
{
    return LoadModel(0, 0, 0);
}

int CCompChar::RefreshModDriverFlags()
{
    CCompoundObj::RefreshModDriverFlags();

    void *pOwner = FindAttachSocketByKind(this, 1);
    if (!pOwner)
        return 0;

    CCompoundObj *pOwnerObj = static_cast<CCompoundObj *>(pOwner);
    bool bSingleLod = pOwnerObj->GetLodGeometryMapPtr()->size() == 1;

    void *pMaterialEntry = 0;
    if (m_pModDriver)
        pMaterialEntry = FindMaterialSocketEntry(m_pModDriver, 1);

    return RebuildAttachSocketVisual(pOwner, pMaterialEntry, bSingleLod ? 1 : 0);
}

int CCompChar::OnAttach(int nSocketKind)
{
    std::map<unsigned int, void *> *pMap = GetLodSourceMapPtr();
    for (std::map<unsigned int, void *>::iterator it = pMap->begin(); it != pMap->end(); ++it)
    {
        if (AttachSourceMatchesSocketKind(it->second, nSocketKind))
            return static_cast<int>(it->first);
    }
    return -1;
}

int CCompChar::OnDetach(int nSocketKind)
{
    std::map<unsigned int, CRTGeometry *> *pMap = GetLodGeometryMapPtr();
    for (std::map<unsigned int, CRTGeometry *>::iterator it = pMap->begin(); it != pMap->end(); ++it)
    {
        if (AttachGeometryMatchesSocketKind(it->second, nSocketKind))
            return OnRelease(static_cast<int>(it->first));
    }
    return 0;
}

int CCompChar::OnRelease(int nSocketKind)
{
    void *pSocketData = FindAttachSocketByKind(this, nSocketKind);
    if (!pSocketData)
        return 0;

    int nTypeTag = GetAttachSocketTypeTag(pSocketData);
    if (nTypeTag == 7)
        ClearAfterImagePos0();
    else if (nTypeTag == 6)
        ClearAfterImagePos1();

    bool bReleaseBindings = false;
    if (AttachSocketHasWordFlagFive(pSocketData))
        bReleaseBindings = GetAttachSocketExtraFlagSet(pSocketData);

    SwapModelVariantForSocket(this, this, nSocketKind);

    if (bReleaseBindings)
        ReleaseCharSocketBindings(this);

    return 1;
}

void CCompChar::NotifyAllChildrenDeviceEvent()
{
    ReleaseCharSocketBindings(this);
    CCompoundObj::NotifyAllChildrenDeviceEvent();
}

int CCompChar::LoadModel(int nCharObjHandle, int nResHandle, int a4)
{
    unsigned short wDetailType = *reinterpret_cast<unsigned short *>(nCharObjHandle + 112);
    if (wDetailType && wDetailType != 5)
        return -1;

    int nResult = CCompoundObj::LoadModel(nCharObjHandle, nResHandle, a4);
    if (nResult < 0)
        return nResult;

    if (*reinterpret_cast<unsigned short *>(nCharObjHandle + 112) == 5)
        NotifyCharObjAiState5(nCharObjHandle);

    if ((m_dwFlags072 & 1) && m_pModDriver && GetCharObjDetailType(nCharObjHandle) == 7)
        DispatchAiEndEvent(this, nResult);

    return nResult;
}
