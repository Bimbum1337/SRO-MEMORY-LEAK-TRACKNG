#ifndef __CCOMPBLDG_H__
#define __CCOMPBLDG_H__

#include "CCompoundObj.h"

class CCompBldg : public CCompoundObj
{
public:
    CCompBldg();
    virtual ~CCompBldg();

    static CCompBldg *AllocateAndConstruct();
    static void ReleaseToPool(CCompBldg *pBldg);
};

#endif
