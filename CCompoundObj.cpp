#include "CCompoundObj.h"
#include "CRTGeometry.h"
#include "CFxModManager.h"
#include <d3dx9math.h>
#include <cstring>
#include <cfloat>
#include <cmath>

extern void *AllocateWorldMatrixBlock();
extern void FreeWorldMatrixBlock(void *pBlock);
extern void DestroyAttachedObjectTree(std::list<void *> *pList);
extern void DestroyEventList(std::list<SCompoundObjEventRecord> *pList);
extern void DestroyList24(std::list<void *> *pList);
extern void DestroySocketList(std::list<SCompoundObjSocket> *pList);
extern void DestroyList48(std::list<SCompoundObjSocket> *pList);
extern void DestroyList52(std::list<void *> *pList);
extern void ReleaseComPointer(void *ppField);
extern void FreeMeshHolder(SCompoundObjMeshHolder *pHolder);
extern void OnResetState();
extern bool IsResourceLoaded(int nResHandle);
extern int ForEachAttachedGetMaxState(const std::list<void *> *pList);
extern int ForEachAttachedInvokeSetLightingState(std::list<void *> *pList, int a2);
extern int GetActiveModDriverVersion();
extern int LookupSocketOffsetTableImpl(unsigned int nIndex);
extern int AttachModDriverImpl(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
extern int PlaySoundEventImpl(int a1, int a2, int a3, int a4, int a5, int a6, int a7);
extern int *ReleaseAllSocketRefsImpl(std::list<SCompoundObjSocket> *pList);
extern void NotifyAllChildrenUnloadImpl(std::list<void *> *pList);
extern int InvokeUnloadHookImpl(CCompoundObj *pThis);
extern int LoadSocketResourcesImpl(CCompoundObj *pThis, int a2, int a3, int a4);
extern int SerializeStateImpl(CCompoundObj *pThis, int a2);
extern void *AllocateModDriverHandle();
extern void ReleaseModDriverHandle(void *pHandle);
extern void *AllocateModDriverHandle2();
extern void ReleaseModDriverHandle2(void *pHandle);
extern bool SetupModDriverGeometry(CCompoundObj *pThis);
extern bool AttachModDriverToTree(CCompoundObj *pThis);
extern int SocketCountIsOne(const int *pChildRaw);
extern bool IsAttachTreeEmpty(const CCompoundObj *pThis);
extern int LookupAttachedByKeyImpl(std::list<void *> *pList, int a2);
extern int FindAttachedByNameKeyImpl(std::list<void *> *pList, int a2, unsigned int *pOut);
extern int PropagateEventImpl(CCompoundObj *pThis, int a2);
extern double RenderImpl(CCompoundObj *pThis, int a2, int a3, float a4);
extern int InvokeUpdateHookImpl(CCompoundObj *pThis);
extern int GetModDriverRenderDataImpl(void *pModDriver, int a2);
extern int NotifyAttachedStateChangeImpl(CCompoundObj *pThis, int a2, int a3);
extern int NotifyModDriverAImpl(void *pModDriver2, int a2, int a3);
extern int NotifyModDriverBImpl(void *pModDriver2, int a2);
extern int RefreshModDriverFlagsImpl(CCompoundObj *pThis);
extern int AllChildrenLoadedImpl(const std::list<void *> *pList);
extern int AnyChildMatchesImpl(int a2, int a3, int a4);
extern int CopyFromTemplateImpl(CCompoundObj *pThis, int *pTemplate);
extern int ResolveOrBuildResourceImpl(int a2, int a3, int a4, unsigned int dwParam);
extern int ResolveResourceHandleImpl(void *pModDriver2, int a2, int a3);
extern void ClearAllAfterImagePosImpl(CCompoundObj *pThis);
extern void NotifyCharObjLoadBegin(void *pCharObjHandle);
extern void NotifyCharObjLoaded(CCompoundObj *pThis, int *pCharObjHandle);
extern void NotifyLoadProgress(CCompoundObj *pThis, float fProgress, int a3);
extern int GetGeneralInfoDetailType(const CCompoundObj *pThis);
extern int GetResFlags(int nType);
extern void InvalidateSocketMatrixList(int *pCharObjArray);
extern void CopyGeneralInfoBlock(CCompoundObj *pThis, int *pCharObjArray);
extern void ApplyResLoadFlags(unsigned int dwLodKey, int a2);
extern void MarkResUsed(unsigned int dwLodKey, int nResHandle, int a4);
extern bool BuildGeometryFromRes(CCompoundObj *pThis, unsigned int dwLodKey, int bHasModDriver, int nResHandle, int a4);
extern void UpdateModDriverLodOffset(int nByteOffset);
extern void SetActiveModDriverTimeRatio(float fRatio);
extern int GetActiveModDriverLightingParam(CCompoundObj *pThis);

CCompoundObj::CCompoundObj()
{
    m_szUnk088.clear();
    m_bUnk084 = false;

    m_mapLodSource.clear();
    m_mapLodGeometry.clear();

    m_v3BBoxMinX = FLT_MAX;
    m_v3BBoxMinY = FLT_MAX;
    m_v3BBoxMinZ = FLT_MAX;
    m_v3BBoxMaxX = -FLT_MAX;
    m_v3BBoxMaxY = -FLT_MAX;
    m_v3BBoxMaxZ = -FLT_MAX;

    m_bUnk212 = false;
    m_szUnk216.clear();

    for (int i = 0; i < 4; ++i)
        m_szMaterialName[i] = "default";

    m_listUnk400.clear();
    m_listSockets.clear();
    m_listAttachedObjects.clear();
    m_listSocketEvents.clear();

    m_apMeshHolder[0] = 0;
    m_apMeshHolder[1] = 0;

    m_fForwardX = 0.0f;
    m_fForwardY = 0.0f;
    m_fForwardZ = -1.0f;
    m_nLODByteOffset = 0;
    m_fLODRatio = -1.0f;
    m_nNextLodId = 0;

    m_dwUnk480 = 0;
    m_pModDriver = 0;
    m_pModDriver2 = 0;
    m_pFxAttachHandle = 0;
    m_dwDetailFactor = 0;
    m_bCollisionEnabled = false;
    m_generalInfo.SetFlags(0x3FFFF);
    m_dwAmbientARGB = 0xFF000000;

    m_pWorldMatrix = static_cast<D3DXMATRIX *>(AllocateWorldMatrixBlock());
    std::memset(m_pWorldMatrix, 0, sizeof(D3DXMATRIX));

    m_fAmbientR = 0.6f;
    m_fAmbientG = 0.6f;
    m_fAmbientB = 0.6f;
    m_fAmbientA = 1.0f;
    m_dwUnk612 = 0;
    m_bVisible = true;

    m_abAfterImagePos[0] = false;
    m_abAfterImagePos[1] = false;
    m_dwUnk476 = 0;

    m_afUnk524[0] = 0.0f;
    m_afUnk524[1] = 0.0f;
    m_afUnk524[2] = 0.0f;
    m_afUnk524[3] = 0.0f;
    m_v3CollisionCenterX = 0.0f;
    m_v3CollisionCenterY = 0.0f;
    m_v3CollisionCenterZ = 0.0f;

    m_fUnk556 = 1.0f;
    m_fLODRatio2 = 1.0f;

    m_dwUnk652 = 0;
    m_fYaw = 0.0f;
    m_dwFlags072 = 0;
    m_dwUnk588 = 5;

    m_v3CollisionCenterX2 = 0.0f;
    m_v3CollisionCenterY2 = 0.0f;
    m_v3CollisionCenterZ2 = 0.0f;
    m_fUnk636 = 1.0f;
    m_fUnk640 = 0.0f;
    m_fUnk644 = 0.0f;
    m_fUnk648 = 0.0f;

    ResetState();
}

CCompoundObj::~CCompoundObj()
{
    ResetState();

    if (m_pWorldMatrix)
    {
        FreeWorldMatrixBlock(m_pWorldMatrix);
        m_pWorldMatrix = 0;
    }

    DestroyAttachedObjectTree(&m_listAttachedObjects);

    DestroyEventList(&m_listSocketEvents);
    DestroyList24(&m_listUnk400);
    DestroySocketList(&m_listSockets);

    m_szUnk088.clear();
}

int CCompoundObj::ResetState()
{
    m_v3BBoxMinX = FLT_MAX;
    m_v3BBoxMinY = FLT_MAX;
    m_v3BBoxMinZ = FLT_MAX;
    m_v3BBoxMaxX = -FLT_MAX;
    m_v3BBoxMaxY = -FLT_MAX;
    m_v3BBoxMaxZ = -FLT_MAX;

    OnResetState();

    if (m_pWorldMatrix)
    {
        std::memset(m_pWorldMatrix, 0, sizeof(D3DXMATRIX));
        m_pWorldMatrix->_11 = 1.0f;
        m_pWorldMatrix->_22 = 1.0f;
        m_pWorldMatrix->_33 = 1.0f;
        m_pWorldMatrix->_44 = 1.0f;
    }

    for (int i = 0; i < 4; ++i)
        m_szMaterialName[i] = "default";

    DestroyList48(&m_listSockets);

    for (std::map<unsigned int, CRTGeometry *>::iterator it = m_mapLodGeometry.begin(); it != m_mapLodGeometry.end(); ++it)
        CRTGeometry::ReleaseToPool(it->second);
    m_mapLodGeometry.clear();
    m_mapLodSource.clear();
    m_nNextLodId = 0;

    ReleaseComPointer(&m_pModDriver);
    ReleaseComPointer(&m_pFxAttachHandle);

    for (int j = 0; j < 2; ++j)
    {
        if (m_apMeshHolder[j])
        {
            FreeMeshHolder(m_apMeshHolder[j]);
            m_apMeshHolder[j] = 0;
        }
    }

    m_bDirty = false;
    m_dwUnk480 = 0;

    DestroyList52(&m_listAttachedObjects);
    return 0;
}

int CCompoundObj::QueueSocketEvent(int a2, int a3)
{
    if (!m_pModDriver2)
        return 0;

    SCompoundObjEventRecord rec;
    rec.m_nEventType = 3;
    rec.m_nParam0 = a3;
    rec.m_nParam1 = 0;
    rec.m_nParam5 = a2;
    m_listSocketEvents.push_back(rec);
    return 0;
}

void CCompoundObj::SetLODRatioEx(float fRatio, int bForce)
{
    if (fRatio < 0.0f)
        fRatio = 0.0f;
    if (fRatio > 1.0f)
        fRatio = 1.0f;

    if (bForce || fRatio != m_fLODRatio)
    {
        m_fLODRatio = fRatio;
        m_nLODByteOffset = static_cast<int>(fRatio * 4.0f);
        if (m_pModDriver2)
            UpdateModDriverLodOffset(m_nLODByteOffset);
    }
}

int CCompoundObj::SetWorldMatrixRaw(const void *pMatrix44)
{
    if (!m_pWorldMatrix)
        return 0;
    std::memcpy(m_pWorldMatrix, pMatrix44, sizeof(D3DXMATRIX));
    m_fForwardX = -m_pWorldMatrix->_31;
    m_fForwardY = -m_pWorldMatrix->_32;
    m_fForwardZ = -m_pWorldMatrix->_33;
    return 1;
}

void *CCompoundObj::GetBoundBoxPtr()
{
    return &m_v3BBoxMinX;
}

int CCompoundObj::SetAmbientAlpha(unsigned char byAlpha)
{
    m_dwAmbientARGB = static_cast<unsigned int>(byAlpha) << 24;
    return static_cast<int>(m_dwAmbientARGB);
}

void *CCompoundObj::GetWorldMatrixPtr()
{
    return m_pWorldMatrix;
}

int CCompoundObj::SetModDriverParam(int a2)
{
    if (!m_pModDriver2)
        return 0;
    return GetModDriverRenderDataImpl(m_pModDriver2, a2);
}

double CCompoundObj::GetLODRatio2()
{
    return m_fLODRatio2;
}

void CCompoundObj::SetLODRatio2(float fRatio)
{
    m_fLODRatio2 = fRatio;
    if (m_pModDriver)
        SetActiveModDriverTimeRatio(fRatio);
}

int *CCompoundObj::SetWorldMatrixRow3(const int *pRow3)
{
    int *pRow = reinterpret_cast<int *>(reinterpret_cast<char *>(m_pWorldMatrix) + 48);
    pRow[0] = pRow3[0];
    pRow[1] = pRow3[1];
    pRow[2] = pRow3[2];
    return pRow;
}

void CCompoundObj::PropagateEvent(int a2)
{
    PropagateEventImpl(this, a2);
}

int CCompoundObj::SetYRotation(float fRadian)
{
    m_fYaw = fRadian;
    m_fForwardX = -std::sin(fRadian);
    m_fForwardY = 0.0f;
    m_fForwardZ = -std::cos(fRadian);
    return 1;
}

int CCompoundObj::GetLightingParam()
{
    return GetActiveModDriverLightingParam(this);
}

double CCompoundObj::Render(int a2, int a3, float a4)
{
    return RenderImpl(this, a2, a3, a4);
}

int CCompoundObj::InvokeUpdateHook()
{
    return InvokeUpdateHookImpl(this);
}

void CCompoundObj::SetFlag072Bit0(int bSet)
{
    if (bSet)
        m_dwFlags072 |= 1;
    else
        m_dwFlags072 &= ~1u;
}

int CCompoundObj::GetModDriverRenderData(int a2)
{
    if (!m_pModDriver)
        return 0;
    return GetModDriverRenderDataImpl(m_pModDriver, a2);
}

int CCompoundObj::SetCollisionInfo(float *pCenter, unsigned int *pFlags, int dwFlag)
{
    m_bDirty = true;
    m_v3CollisionCenterX2 = pCenter[0];
    m_v3CollisionCenterY2 = pCenter[1];
    m_v3CollisionCenterZ2 = pCenter[2];
    m_dwCollisionFlag0Copy = pFlags[0];
    m_dwCollisionFlag1Copy = pFlags[1];
    m_dwCollisionFlag2Copy = pFlags[2];
    m_dwCollisionParam = static_cast<unsigned int>(dwFlag);
    m_dwUnk588 = 0;
    m_bCollisionEnabled = true;

    m_v3CollisionCenterX = pCenter[0];
    m_v3CollisionCenterY = pCenter[1];
    m_v3CollisionCenterZ = pCenter[2];
    m_fCollisionDerivedX = m_v3CollisionCenterX;
    m_fCollisionDerivedY = m_v3CollisionCenterY;
    m_fCollisionDerivedZ = m_v3CollisionCenterZ;
    return static_cast<int>(m_fCollisionDerivedZ);
}

void CCompoundObj::ClearDirtyFlag()
{
    m_bDirty = false;
}

float *CCompoundObj::GetPositionRaw(float *pOut)
{
    pOut[0] = m_pWorldMatrix->_41;
    pOut[1] = m_pWorldMatrix->_42;
    pOut[2] = m_pWorldMatrix->_43;
    return pOut;
}

void CCompoundObj::SetAmbientColorF(float r, float g, float b)
{
    m_fAmbientR = r;
    m_fAmbientG = g;
    m_fAmbientB = b;
}

void CCompoundObj::NotifyAttachedStateChange(int a2, int a3)
{
    NotifyAttachedStateChangeImpl(this, a2, a3);
}

int CCompoundObj::NotifyModDriverA(int a2, int a3)
{
    if (!m_pModDriver)
        return 0;
    return NotifyModDriverAImpl(m_pModDriver2, a2, a3);
}

int CCompoundObj::NotifyModDriverB(int a2)
{
    if (!m_pModDriver)
        return 0;
    return NotifyModDriverBImpl(m_pModDriver2, a2);
}

int CCompoundObj::SetUnk476(int a2)
{
    m_dwUnk476 = static_cast<unsigned int>(a2);
    return a2;
}

int CCompoundObj::SetUnk480(int a2)
{
    m_dwUnk480 = static_cast<unsigned int>(a2);
    if (a2)
        m_dwFlags072 |= 1;
    else
        m_dwFlags072 &= ~1u;
    return a2;
}

int CCompoundObj::RefreshModDriverFlags()
{
    return RefreshModDriverFlagsImpl(this);
}

int CCompoundObj::QueryModDriverField(int a2)
{
    if (!m_pModDriver2)
        return -1;
    if (!a2)
        return -1;
    return *reinterpret_cast<int *>(a2 + 12);
}

int CCompoundObj::QueueEventType0(int a2, float a3, int a4, float a5, int a6)
{
    if (!m_pModDriver2)
        return 0;

    SCompoundObjEventRecord rec;
    rec.m_nEventType = 0;
    rec.m_nParam2 = a2;
    rec.m_fParam0 = a3;
    rec.m_nParam5 = a4;
    rec.m_fParam1 = a5;
    m_listSocketEvents.push_back(rec);
    return 0;
}

int CCompoundObj::QueueEventType1(int a2, int a3, int a4, float a5, int a6)
{
    if (!m_pModDriver2)
        return 0;

    SCompoundObjEventRecord rec;
    rec.m_nEventType = 1;
    rec.m_nParam2 = a2;
    rec.m_nParam3 = a3;
    rec.m_nParam4 = a4;
    rec.m_fParam1 = a5;
    m_listSocketEvents.push_back(rec);
    return 1;
}

int CCompoundObj::SetSocketLocalOffset(int a2, float a3, float a4, float a5, float a6)
{
    if (!m_pModDriver2)
        return 0;
    return SocketCountIsOne(0);
}

int CCompoundObj::ClearSocketLocalOffset(int a2, int a3)
{
    if (!m_pModDriver2)
        return 0;
    return SocketCountIsOne(0);
}

int CCompoundObj::PlaySoundEventA(int a1, int a2, int a3, int a4, int a5, int a6)
{
    return PlaySoundEventImpl(0, a1, a2, a3, a4, a5, a6);
}

int CCompoundObj::PlaySoundEventB(int a1, int a2, int a3, int a4, int a5, int a6)
{
    return PlaySoundEventImpl(1, a1, a2, a3, a4, a5, a6);
}

int CCompoundObj::ClearAllAfterImagePos()
{
    ClearAllAfterImagePosImpl(this);
    return 1;
}

int CCompoundObj::ClearAfterImagePos0()
{
    m_abAfterImagePos[0] = false;
    return 1;
}

int CCompoundObj::ClearAfterImagePos1()
{
    m_abAfterImagePos[1] = false;
    return 1;
}

int CCompoundObj::AllChildrenLoaded()
{
    return AllChildrenLoadedImpl(&m_listAttachedObjects);
}

int CCompoundObj::GetAttachedGeneralInfoField()
{
    int hChild = LookupAttachedByKey(1);
    if (!hChild)
        return 0;
    return *reinterpret_cast<int *>(hChild + 696);
}

int CCompoundObj::AnyChildMatches(int a2, int a3, int a4)
{
    return AnyChildMatchesImpl(a2, a3, a4);
}

int CCompoundObj::CopyFromTemplate(int *pTemplate)
{
    return CopyFromTemplateImpl(this, pTemplate);
}

int CCompoundObj::ResolveOrBuildResource(int a2, int a3, int a4)
{
    if (!m_pModDriver2 || !IsResourceLoaded(a2))
        return 0;
    return ResolveOrBuildResourceImpl(a2, a3, a4, m_dwDetailFactor);
}

int CCompoundObj::ResolveResourceHandle(int a2, int a3)
{
    if (!m_pModDriver2 || !a3)
        return 0;
    if (!IsResourceLoaded(a2))
        return 0;
    return ResolveResourceHandleImpl(m_pModDriver2, a2, a3);
}

void CCompoundObj::QueueEventType2(int a2, int a3)
{
    if (!m_pModDriver2)
        return;

    SCompoundObjEventRecord rec;
    rec.m_nEventType = 2;
    rec.m_nParam2 = a2;
    rec.m_nParam3 = a3;
    m_listSocketEvents.push_back(rec);
}

void CCompoundObj::QueueEventType5(int a2, int a3)
{
    if (!m_pModDriver2)
        return;

    SCompoundObjEventRecord rec;
    rec.m_nEventType = 5;
    rec.m_nParam2 = a2;
    rec.m_nParam3 = a3;
    m_listSocketEvents.push_back(rec);
}

int CCompoundObj::QueueEventType6(int a2, float a3)
{
    if (!m_pModDriver2)
        return 0;

    SCompoundObjEventRecord rec;
    rec.m_nEventType = 6;
    rec.m_fParam1 = a3;
    rec.m_nParam2 = a2;
    m_listSocketEvents.push_back(rec);
    return 0;
}

unsigned int CCompoundObj::LookupSocketOffsetTable(unsigned int a1, float a2)
{
    if (a1 >= 0x26)
        return a1;
    return LookupSocketOffsetTableImpl(a1);
}

int CCompoundObj::AttachModDriver(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    if (!IsResourceLoaded(a1))
        return 0;
    return AttachModDriverImpl(a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

int CCompoundObj::PlaySoundOnChild(int a2, int a3, int a4, int a5, int a6, float a7, int a8)
{
    return PlaySoundEventImpl(a2, a3, a4, a5, a6, static_cast<int>(a7), a8);
}

int *CCompoundObj::ReleaseAllSocketRefs()
{
    return ReleaseAllSocketRefsImpl(&m_listSockets);
}

double CCompoundObj::GetYaw()
{
    return m_fYaw;
}

void CCompoundObj::NotifyAllChildrenUnload()
{
    NotifyAllChildrenUnloadImpl(&m_listAttachedObjects);
}

int CCompoundObj::QueueEventType7(int a2, int a3, float a4, int a5, float a6, int a7)
{
    if (!m_pModDriver2)
        return 0;

    SCompoundObjEventRecord rec;
    rec.m_nEventType = 7;
    rec.m_nParam0 = a2;
    rec.m_nParam1 = a3;
    rec.m_fParam0 = a4;
    rec.m_nParam5 = a5;
    m_listSocketEvents.push_back(rec);
    return 0;
}

int CCompoundObj::GetModDriverExtra()
{
    int hChild = LookupAttachedByKey(1);
    if (!hChild)
        return 0;
    return hChild + 504;
}

char CCompoundObj::GetVisible()
{
    return m_bVisible ? 1 : 0;
}

char CCompoundObj::SetVisibleRaw(char bVisible)
{
    m_bVisible = bVisible != 0;
    return bVisible;
}

int CCompoundObj::OnAttach(int nSocketKind)
{
    return 0;
}

int CCompoundObj::OnDetach(int nSocketKind)
{
    return 0;
}

int CCompoundObj::OnRelease(int nSocketKind)
{
    return 0;
}

int CCompoundObj::SumChildProperty0()
{
    return ForEachAttachedGetMaxState(&m_listAttachedObjects);
}

int CCompoundObj::SumChildProperty1()
{
    return ForEachAttachedGetMaxState(&m_listAttachedObjects);
}

void CCompoundObj::NotifyAllChildrenDeviceEvent()
{
    ForEachAttachedInvokeSetLightingState(&m_listAttachedObjects, GetActiveModDriverVersion());
}

int CCompoundObj::RebuildResource(char a1, int a2, int a3, int a4)
{
    if (!IsResourceLoaded(a1))
        return 0;
    return ResolveOrBuildResourceImpl(a2, a3, a4, 0);
}

int CCompoundObj::IsCollidable()
{
    return m_generalInfo.GetFlags() & 0x10;
}

unsigned short CCompoundObj::GetTypeTag() const
{
    return static_cast<unsigned short>(m_generalInfo.GetFlags());
}

void CCompoundObj::SetCollisionBounds(const unsigned int *pFlags, float a3)
{
    m_dwCollisionFlag0Copy = pFlags[0];
    m_dwCollisionFlag1Copy = pFlags[1];
    m_dwCollisionFlag2Copy = pFlags[2];
    m_dwCollisionParam = pFlags[3];
    m_dwUnk612 = a3 != 0.0f;
}

int CCompoundObj::LoadModel(int nCharObjHandle, int nResHandle, int a4)
{
    NotifyCharObjLoadBegin(reinterpret_cast<void *>(nCharObjHandle));

    if (!IsResourceLoaded(nResHandle))
    {
        nResHandle = 0;
        IsResourceLoaded(0);
    }

    m_fLODRatio2 = m_fLODRatio;
    NotifyLoadProgress(this, 0.0f, 0);
    NotifyCharObjLoaded(this, reinterpret_cast<int *>(nCharObjHandle));

    int *pCharObj = reinterpret_cast<int *>(nCharObjHandle);
    if (m_dwUnk480)
    {
        int nType = GetGeneralInfoDetailType(this);
        if (GetResFlags(nType) != 0)
            InvalidateSocketMatrixList(pCharObj + 137);
    }
    else
    {
        CopyGeneralInfoBlock(this, pCharObj);
    }

    unsigned int dwKey;
    do
    {
        unsigned int dwPrev = m_nNextLodId++;
        dwKey = m_nNextLodId;
        if (dwPrev == 0xFFFFFFFFu)
        {
            dwKey = 1;
            m_nNextLodId = 1;
        }
    } while (LookupAttachedByKey(static_cast<int>(dwKey)));

    if (pCharObj[124])
        ApplyResLoadFlags(dwKey, pCharObj[124]);
    MarkResUsed(dwKey, nResHandle, a4);

    CRTGeometry *pGeometry = CRTGeometry::AllocateAndConstruct();
    if (!BuildGeometryFromRes(this, dwKey, m_pModDriver ? 1 : 0, nResHandle, a4))
    {
        CRTGeometry::ReleaseToPool(pGeometry);
        return -1;
    }

    m_mapLodGeometry[dwKey] = pGeometry;

    if (!m_fxModManager.BuildFromModelInfo(this, dwKey, reinterpret_cast<void *>(nCharObjHandle)))
        return -1;

    m_mapLodSource[dwKey] = reinterpret_cast<void *>(nCharObjHandle);
    return static_cast<int>(dwKey);
}

std::map<unsigned int, CRTGeometry *> *CCompoundObj::GetLodGeometryMapPtr()
{
    return &m_mapLodGeometry;
}

std::map<unsigned int, void *> *CCompoundObj::GetLodSourceMapPtr()
{
    return &m_mapLodSource;
}

int CCompoundObj::LookupAttachedByKey(int a2)
{
    return LookupAttachedByKeyImpl(&m_listAttachedObjects, a2);
}

int CCompoundObj::FindAttachedByNameKey(int a2, unsigned int *pOut)
{
    return FindAttachedByNameKeyImpl(&m_listAttachedObjects, a2, pOut);
}

int CCompoundObj::GetFxAttachHandleRaw()
{
    return reinterpret_cast<int>(m_pModDriver);
}

int CCompoundObj::InvokeUnloadHook()
{
    return InvokeUnloadHookImpl(this);
}

int CCompoundObj::GetReserved0()
{
    return 0;
}

int CCompoundObj::LoadSocketResources(int a2, int a3, int a4)
{
    return LoadSocketResourcesImpl(this, a2, a3, a4);
}

int CCompoundObj::SerializeState(int a2)
{
    return SerializeStateImpl(this, a2);
}

int CCompoundObj::DetachModDriver(VICompoundObj *pChild)
{
    int *pChildRaw = reinterpret_cast<int *>(pChild);

    if (pChildRaw[124])
    {
        if (!m_pModDriver)
        {
            m_pModDriver = AllocateModDriverHandle();
            if (!SetupModDriverGeometry(this))
            {
                ReleaseModDriverHandle(m_pModDriver);
                m_pModDriver = 0;
                return 0;
            }
        }
    }

    if (!pChildRaw[73])
        return SocketCountIsOne(pChildRaw) != 1 || m_dwDetailFactor != 0 || IsAttachTreeEmpty(this);

    if ((pChildRaw[74] - pChildRaw[73]) <= 0)
        return SocketCountIsOne(pChildRaw) != 1 || m_dwDetailFactor != 0 || IsAttachTreeEmpty(this);

    if (!pChildRaw[124])
        return SocketCountIsOne(pChildRaw) != 1 || m_dwDetailFactor != 0 || IsAttachTreeEmpty(this);

    if (m_pModDriver2)
        return SocketCountIsOne(pChildRaw) != 1 || m_dwDetailFactor != 0 || IsAttachTreeEmpty(this);

    m_pModDriver2 = AllocateModDriverHandle2();
    if (!AttachModDriverToTree(this))
    {
        ReleaseModDriverHandle2(m_pModDriver2);
        m_pModDriver2 = 0;
        return 0;
    }

    return SocketCountIsOne(pChildRaw) != 1 || m_dwDetailFactor != 0 || IsAttachTreeEmpty(this);
}

int CCompoundObj::ReleaseModDriverIfIdle()
{
    if (m_pModDriver && !static_cast<int *>(m_pModDriver)[4])
    {
        ReleaseModDriverHandle(m_pModDriver);
        m_pModDriver = 0;
    }

    if (m_pModDriver2)
    {
        if (!m_pModDriver)
        {
            ReleaseModDriverHandle2(m_pModDriver2);
            m_pModDriver2 = 0;
        }
    }
    return m_pModDriver2 != 0;
}
