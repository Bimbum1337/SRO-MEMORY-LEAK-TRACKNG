#ifndef __CCOMPSIMPLE_H__
#define __CCOMPSIMPLE_H__

#include "CCompoundObj.h"

class CCompSimple : public CCompoundObj
{
public:
    CCompSimple();
    virtual ~CCompSimple();

    static CCompSimple *AllocateAndConstruct();
    static void ReleaseToPool(CCompSimple *pSimple);
};

#endif
