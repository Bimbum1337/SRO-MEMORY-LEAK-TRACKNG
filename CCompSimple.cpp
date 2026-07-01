#include "CCompSimple.h"
#include "CMemPool.h"
#include <new>

static CMemPool<5994344087672913920ULL, CCompSimple, 1000, 1> g_compSimplePool;

CCompSimple::CCompSimple() : CCompoundObj()
{
}

CCompSimple::~CCompSimple()
{
}

CCompSimple *CCompSimple::AllocateAndConstruct()
{
    void *pRaw = g_compSimplePool.Alloc();
    if (!pRaw)
        return 0;
    return new (pRaw) CCompSimple();
}

void CCompSimple::ReleaseToPool(CCompSimple *pSimple)
{
    g_compSimplePool.Free(pSimple);
}
