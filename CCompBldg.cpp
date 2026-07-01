#include "CCompBldg.h"
#include "CMemPool.h"
#include <new>

static CMemPool<5994344087672848384ULL, CCompBldg, 1000, 1> g_compBldgPool;

CCompBldg::CCompBldg() : CCompoundObj()
{
}

CCompBldg::~CCompBldg()
{
}

CCompBldg *CCompBldg::AllocateAndConstruct()
{
    void *pRaw = g_compBldgPool.Alloc();
    if (!pRaw)
        return 0;
    return new (pRaw) CCompBldg();
}

void CCompBldg::ReleaseToPool(CCompBldg *pBldg)
{
    g_compBldgPool.Free(pBldg);
}
