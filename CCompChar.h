#ifndef __CCOMPCHAR_H__
#define __CCOMPCHAR_H__

#include "CCompoundObj.h"

class CCompChar : public CCompoundObj
{
public:
    CCompChar();
    virtual ~CCompChar();

    static CCompChar *AllocateAndConstruct();
    static void ReleaseToPool(CCompChar *pChar);

    virtual int InvokeUpdateHook();
    virtual int RefreshModDriverFlags();

    virtual int OnAttach(int nSocketKind);
    virtual int OnDetach(int nSocketKind);
    virtual int OnRelease(int nSocketKind);

    virtual void NotifyAllChildrenDeviceEvent();

    virtual int LoadModel(int nCharObjHandle, int nResHandle, int a4);
};

#endif
