#ifndef __CCOMPOUNDOBJ_H__
#define __CCOMPOUNDOBJ_H__

#include <string>
#include <list>
#include <map>
#include "VICompoundObj.h"
#include "ObjGeneralInfo.h"
#include "CFxModManager.h"

class CRTGeometry;
struct D3DXMATRIX;

struct SCompoundObjLodEntry
{
    CRTGeometry *m_pGeometry;
    CFxModManager *m_pFxModManager;
};

struct SCompoundObjMeshHolder
{
    std::list<void *> m_listSubMesh0;
    std::list<void *> m_listSubMesh1;
    void *m_pMeshData;
};

struct SCompoundObjEventRecord
{
    int m_nEventType;
    int m_nParam0;
    int m_nParam1;
    int m_nParam2;
    int m_nParam3;
    int m_nParam4;
    float m_fParam0;
    float m_fParam1;
    int m_nParam5;
    int m_nReserved;
};

struct SCompoundObjSocket
{
    void *m_pOwnerObj;
};

class CCompoundObj : public VICompoundObj
{
public:
    CCompoundObj();
    virtual ~CCompoundObj();

    virtual int QueueSocketEvent(int a2, int a3);
    virtual void SetLODRatioEx(float fRatio, int bForce);
    virtual int SetWorldMatrixRaw(const void *pMatrix44);
    virtual void *GetBoundBoxPtr();
    virtual int SetAmbientAlpha(unsigned char byAlpha);
    virtual void *GetWorldMatrixPtr();
    virtual int SetModDriverParam(int a2);
    virtual double GetLODRatio2();
    virtual void SetLODRatio2(float fRatio);
    virtual int *SetWorldMatrixRow3(const int *pRow3);
    virtual void PropagateEvent(int a2);
    virtual int SetYRotation(float fRadian);
    virtual int GetLightingParam();
    virtual double Render(int a2, int a3, float a4);
    virtual int InvokeUpdateHook();
    virtual void SetFlag072Bit0(int bSet);
    virtual int GetModDriverRenderData(int a2);
    virtual int SetCollisionInfo(float *pCenter, unsigned int *pFlags, int dwFlag);
    virtual void ClearDirtyFlag();
    virtual float *GetPositionRaw(float *pOut);
    virtual void SetAmbientColorF(float r, float g, float b);
    virtual void NotifyAttachedStateChange(int a2, int a3);
    virtual int NotifyModDriverA(int a2, int a3);
    virtual int NotifyModDriverB(int a2);
    virtual int SetUnk476(int a2);
    virtual int SetUnk480(int a2);
    virtual int RefreshModDriverFlags();
    virtual int QueryModDriverField(int a2);
    virtual int QueueEventType0(int a2, float a3, int a4, float a5, int a6);
    virtual int QueueEventType1(int a2, int a3, int a4, float a5, int a6);
    virtual int SetSocketLocalOffset(int a2, float a3, float a4, float a5, float a6);
    virtual int ClearSocketLocalOffset(int a2, int a3);
    virtual int PlaySoundEventA(int a1, int a2, int a3, int a4, int a5, int a6);
    virtual int PlaySoundEventB(int a1, int a2, int a3, int a4, int a5, int a6);
    virtual int ClearAllAfterImagePos();
    virtual int ClearAfterImagePos0();
    virtual int ClearAfterImagePos1();
    virtual int AllChildrenLoaded();
    virtual int GetAttachedGeneralInfoField();
    virtual int AnyChildMatches(int a2, int a3, int a4);
    virtual int CopyFromTemplate(int *pTemplate);
    virtual int ResolveOrBuildResource(int a2, int a3, int a4);
    virtual int ResolveResourceHandle(int a2, int a3);
    virtual void QueueEventType2(int a2, int a3);
    virtual void QueueEventType5(int a2, int a3);
    virtual int QueueEventType6(int a2, float a3);
    virtual unsigned int LookupSocketOffsetTable(unsigned int a1, float a2);
    virtual int AttachModDriver(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
    virtual int PlaySoundOnChild(int a2, int a3, int a4, int a5, int a6, float a7, int a8);
    virtual int *ReleaseAllSocketRefs();
    virtual double GetYaw();
    virtual void NotifyAllChildrenUnload();
    virtual int QueueEventType7(int a2, int a3, float a4, int a5, float a6, int a7);
    virtual int GetModDriverExtra();
    virtual char GetVisible();
    virtual char SetVisibleRaw(char bVisible);
    virtual int OnAttach(int nSocketKind);
    virtual int OnDetach(int nSocketKind);
    virtual int OnRelease(int nSocketKind);
    virtual int SumChildProperty0();
    virtual int SumChildProperty1();
    virtual void NotifyAllChildrenDeviceEvent();
    virtual int RebuildResource(char a1, int a2, int a3, int a4);
    virtual int IsCollidable();
    unsigned short GetTypeTag() const;
    virtual void SetCollisionBounds(const unsigned int *pFlags, float a3);

    virtual int LoadModel(int nCharObjHandle, int nResHandle, int a4);
    virtual std::map<unsigned int, CRTGeometry *> *GetLodGeometryMapPtr();
    virtual std::map<unsigned int, void *> *GetLodSourceMapPtr();
    virtual int LookupAttachedByKey(int a2);
    virtual int FindAttachedByNameKey(int a2, unsigned int *pOut);
    virtual int GetFxAttachHandleRaw();
    virtual int ResetState();
    virtual int InvokeUnloadHook();
    virtual int GetReserved0();
    virtual int LoadSocketResources(int a2, int a3, int a4);
    virtual int SerializeState(int a2);
    virtual int DetachModDriver(VICompoundObj *pChild);
    virtual int ReleaseModDriverIfIdle();

protected:
    float m_fAmbientR;
    float m_fAmbientG;
    float m_fAmbientB;
    float m_fAmbientA;
    float m_afUnk052[4];
    unsigned int m_dwFlags072;
    unsigned int m_dwAmbientARGB;
    bool m_bUnk084;
    std::string m_szUnk088;
    unsigned int m_nNextLodId;
    float m_fLODRatio;
    int m_nLODByteOffset;
    float m_fScaleRatio;
    std::map<unsigned int, void *> m_mapLodSource;
    std::map<unsigned int, CRTGeometry *> m_mapLodGeometry;
    float m_fForwardX;
    float m_fForwardY;
    float m_fForwardZ;
    D3DXMATRIX *m_pWorldMatrix;
    float m_v3BBoxMinX;
    float m_v3BBoxMinY;
    float m_v3BBoxMinZ;
    float m_v3BBoxMaxX;
    float m_v3BBoxMaxY;
    float m_v3BBoxMaxZ;
    ObjGeneralInfo m_generalInfo;
    bool m_bUnk212;
    std::string m_szUnk216;
    CFxModManager m_fxModManager;
    std::string m_szMaterialName[4];
    std::list<void *> m_listUnk400;
    std::list<SCompoundObjSocket> m_listSockets;
    float m_fLODRatio2;
    float m_fYaw;
    bool m_bVisible;
    std::list<void *> m_listAttachedObjects;
    SCompoundObjMeshHolder *m_apMeshHolder[2];
    bool m_abAfterImagePos[2];
    unsigned int m_dwUnk476;
    unsigned int m_dwUnk480;
    bool m_bDirty;
    float m_v3CollisionCenterX;
    float m_v3CollisionCenterY;
    float m_v3CollisionCenterZ;
    float m_fCollisionDerivedX;
    float m_fCollisionDerivedY;
    float m_fCollisionDerivedZ;
    unsigned int m_dwCollisionFlag0;
    unsigned int m_dwCollisionFlag1;
    unsigned int m_dwCollisionFlag2;
    unsigned int m_dwCollisionParam;
    unsigned int m_dwUnk588;
    bool m_bCollisionEnabled;
    float m_afUnk524[8];
    float m_fUnk556;
    float m_v3CollisionCenterX2;
    float m_v3CollisionCenterY2;
    float m_v3CollisionCenterZ2;
    unsigned int m_dwCollisionFlag0Copy;
    unsigned int m_dwCollisionFlag1Copy;
    unsigned int m_dwCollisionFlag2Copy;
    void *m_pModDriver;
    void *m_pModDriver2;
    void *m_pFxAttachHandle;
    unsigned int m_dwDetailFactor;
    std::list<SCompoundObjEventRecord> m_listSocketEvents;
    unsigned int m_dwUnk612;
    float m_fUnk636;
    float m_fUnk640;
    float m_fUnk644;
    float m_fUnk648;
    unsigned int m_dwUnk652;
};

#endif
