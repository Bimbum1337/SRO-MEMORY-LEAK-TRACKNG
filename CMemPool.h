#ifndef __CMEMPOOL_H__
#define __CMEMPOOL_H__

#include "CAllocPool.h"

template <unsigned long long ullTypeHash, class T, int nCommitBatch, int nLockPolicy>
class CMemPool : public CAllocPool
{
public:
    CMemPool()
        : CAllocPool(static_cast<unsigned int>(ullTypeHash >> 32), static_cast<unsigned int>(ullTypeHash), sizeof(T), nCommitBatch, 0, false)
    {
    }

    virtual ~CMemPool()
    {
    }

    T *Alloc()
    {
        return static_cast<T *>(CAllocPool::Alloc());
    }

    void Free(T *pElement)
    {
        CAllocPool::Free(pElement);
    }
};

#endif
