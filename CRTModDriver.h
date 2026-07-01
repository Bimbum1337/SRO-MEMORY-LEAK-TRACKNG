#ifndef CRTMODDRIVER_H
#define CRTMODDRIVER_H

class CRTModSet;
class CRTModSound;
class CFxModManager;

struct SModsetMapNode
{
    SModsetMapNode *m_pLeft;
    SModsetMapNode *m_pParent;
    SModsetMapNode *m_pRight;
    unsigned char m_bIsRed;
    unsigned char m_bIsNil;
    unsigned short m_padding;
    unsigned int m_nKeyHash;
    unsigned int m_nRefCount;
    CRTModSet *m_pModSet;
};

struct SDriverSoundBufferHolder
{
    void *m_pHead;
    void *m_pTail;
    unsigned int m_dwUnk08;
};

class CRTModDriver
{
public:
    CRTModDriver();
    virtual ~CRTModDriver();

    static CRTModDriver *AllocateAndConstruct();
    static void ReleaseToPool(CRTModDriver *pDriver);

    void BindOwnerRegistry(void *pOwner, void *pRegistry);

    bool Dispatch(int nGroupId, int nKey, void *pAttachParams);
    bool Detach(int nGroupId, int nKey);
    bool Attach(int nGroupId, void *pWaveIdPair, int nParamA, int nParamB, void *pTag3, void *pTag3b, int nFlagsA, int nFlagsB);

    bool HasModSet(int nGroupId, int nKey) const;
    bool DetachGroup(int nGroupId);

protected:
    void ClearSoundBufferHolder();
    void ClearWaveIdArray();
    void ClearModSetMap();

protected:
    SDriverSoundBufferHolder m_soundBufferHolder;
    unsigned int m_dwUnk10;
    unsigned int *m_pWaveIdArrayBegin;
    unsigned int *m_pWaveIdArrayEnd;
    unsigned int *m_pWaveIdArrayCapacity;
    unsigned int m_dwModSetMapReserved;
    SModsetMapNode *m_pModSetMapHead;
    unsigned int m_nModSetMapSize;
    void *m_pOwnerRegistry;
    void *m_pRegistryContext;
};

#endif
