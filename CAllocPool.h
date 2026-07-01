#ifndef __CALLOCPOOL_H__
#define __CALLOCPOOL_H__

class CAllocPool
{
public:
    CAllocPool(unsigned int dwTypeTag1, unsigned int dwTypeTag2, int nElementSize, int nCommitBatch, int nUnused, bool bAlign16);
    virtual ~CAllocPool();

    void *Alloc();
    void Free(void *pElement);

public:
    typedef void (__thiscall *ScalarDeletingDtorFn)(void *pThis, unsigned int nFlags);

private:
    void *GrowPool();
    void ReleaseAllChunks();
    void ValidateBlockOwnership(void *pElement);
    void ValidateGuardBytes(void *pElement);
    bool IsAddressWithinChunks(void *pElement);

private:
    unsigned int m_dwUnk04;
    unsigned int m_dwUnk08;
    int m_nInUseCount;
    unsigned int m_dwUnk10;
    int m_nElementSize;
    int m_nStride;
    int m_nCommitBatch;
    unsigned int m_dwUnk20;
    int m_nElementSizeDup;
    char m_chTypeTag1[4];
    char m_chTypeTag2[4];
    bool m_bUnk48;
    bool m_bDebugGuardBytes;
    void *m_pChunkListHead;
    void *m_pChunkListTail;
    int m_nCommitCount;
    void *m_pFreeListHead;
    void *m_pFreeListTail;
    int m_nFreeCount;
};

#endif
