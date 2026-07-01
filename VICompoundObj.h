#ifndef __VICOMPOUNDOBJ_H__
#define __VICOMPOUNDOBJ_H__

class CRTGeometry;
class CFxModManager;

class VICompoundObj
{
public:
    virtual ~VICompoundObj();
    virtual int QueueSocketEvent(int a2, int a3) = 0;
    virtual void SetLODRatioEx(float fRatio, int bForce) = 0;
    virtual int SetWorldMatrixRaw(const void *pMatrix44) = 0;
    virtual void *GetBoundBoxPtr() = 0;
    virtual int SetAmbientAlpha(unsigned char byAlpha) = 0;
    virtual void *GetWorldMatrixPtr() = 0;
    virtual int SetModDriverParam(int a2) = 0;
    virtual double GetLODRatio2() = 0;
    virtual void SetLODRatio2(float fRatio) = 0;
    virtual int *SetWorldMatrixRow3(const int *pRow3) = 0;
    virtual void PropagateEvent(int a2) = 0;
    virtual int SetYRotation(float fRadian) = 0;
    virtual int GetLightingParam() = 0;
    virtual double Render(int a2, int a3, float a4) = 0;
    virtual int InvokeUpdateHook() = 0;
    virtual void SetFlag072Bit0(int bSet) = 0;
    virtual int GetModDriverRenderData(int a2) = 0;
    virtual int SetCollisionInfo(float *pCenter, unsigned int *pFlags, int dwFlag) = 0;
    virtual void ClearDirtyFlag() = 0;
    virtual float *GetPositionRaw(float *pOut) = 0;
    virtual void SetAmbientColorF(float r, float g, float b) = 0;
    virtual void NotifyAttachedStateChange(int a2, int a3) = 0;
    virtual int NotifyModDriverA(int a2, int a3) = 0;
    virtual int NotifyModDriverB(int a2) = 0;
    virtual int SetUnk476(int a2) = 0;
    virtual int SetUnk480(int a2) = 0;
    virtual int RefreshModDriverFlags() = 0;
    virtual int QueryModDriverField(int a2) = 0;
    virtual int QueueEventType0(int a2, float a3, int a4, float a5, int a6) = 0;
    virtual int QueueEventType1(int a2, int a3, int a4, float a5, int a6) = 0;
    virtual int SetSocketLocalOffset(int a2, float a3, float a4, float a5, float a6) = 0;
    virtual int ClearSocketLocalOffset(int a2, int a3) = 0;
    virtual int PlaySoundEventA(int a1, int a2, int a3, int a4, int a5, int a6) = 0;
    virtual int PlaySoundEventB(int a1, int a2, int a3, int a4, int a5, int a6) = 0;
    virtual int ClearAllAfterImagePos() = 0;
    virtual int ClearAfterImagePos0() = 0;
    virtual int ClearAfterImagePos1() = 0;
    virtual int AllChildrenLoaded() = 0;
    virtual int GetAttachedGeneralInfoField() = 0;
    virtual int AnyChildMatches(int a2, int a3, int a4) = 0;
    virtual int CopyFromTemplate(int *pTemplate) = 0;
    virtual int ResolveOrBuildResource(int a2, int a3, int a4) = 0;
    virtual int ResolveResourceHandle(int a2, int a3) = 0;
    virtual void QueueEventType2(int a2, int a3) = 0;
    virtual void QueueEventType5(int a2, int a3) = 0;
    virtual int QueueEventType6(int a2, float a3) = 0;
    virtual unsigned int LookupSocketOffsetTable(unsigned int a1, float a2) = 0;
    virtual int AttachModDriver(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) = 0;
    virtual int PlaySoundOnChild(int a2, int a3, int a4, int a5, int a6, float a7, int a8) = 0;
    virtual int *ReleaseAllSocketRefs() = 0;
    virtual double GetYaw() = 0;
    virtual void NotifyAllChildrenUnload() = 0;
    virtual int QueueEventType7(int a2, int a3, float a4, int a5, float a6, int a7) = 0;
    virtual int GetModDriverExtra() = 0;
    virtual char GetVisible() = 0;
    virtual char SetVisibleRaw(char bVisible) = 0;

    virtual int OnAttach(int nSocketKind);
    virtual int OnDetach(int nSocketKind);
    virtual int OnRelease(int nSocketKind);

    virtual int SumChildProperty0() = 0;
    virtual int SumChildProperty1() = 0;
    virtual void NotifyAllChildrenDeviceEvent() = 0;
    virtual int RebuildResource(char a1, int a2, int a3, int a4) = 0;
    virtual int IsCollidable() = 0;
    virtual void SetCollisionBounds(const unsigned int *pFlags, float a3) = 0;
};

#endif
