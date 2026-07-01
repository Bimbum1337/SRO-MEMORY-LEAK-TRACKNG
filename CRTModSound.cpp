#include "CRTModSound.h"
#include "CMemPool.h"
#include <vector>
#include <new>

static CMemPool<5994344087572316160ULL, CRTModSound, 1000, 1> g_soundPool;

class CRTModSoundTrackListener
{
public:
    virtual void Reserved0();
    virtual void Reserved1();
    virtual void Reserved2();
    virtual void Reserved3();
    virtual void Reserved4();
    virtual void Reserved5();
    virtual void Reserved6();
    virtual int OnTrackParam(int nMode, int nParam, int nUserData);
};

class CRTModSoundTrack
{
public:
    CRTModSoundTrackListener* pListener;
    int nSoundSetRef;
    float afPosition[3];
};

class CRTModVoiceFactory
{
public:
    virtual void Reserved0();
    virtual void Reserved1();
    virtual void Reserved2();
    virtual int CreateVoice(int nMode, float* pPosition, int nFlags);
};

class CRTModSoundSet
{
public:
    int nField[62];
    CRTModVoiceFactory* pVoiceFactory;
    std::vector<CRTModSoundTrack*> Tracks;
};

extern bool RTModSound_IsVoicePlaying(int hVoice);
extern int RTModSound_ReleaseVoice(int hVoice);

CRTModSound::CRTModSound()
    : m_pSndSet(0)
{
    m_nType = 6;
    for (int i = 0; i < MAX_SOUND_TRACK_PER_ANI; ++i)
    {
        m_nTrackPlayed[i] = 0;
        m_hTrackVoice[i] = -1;
    }
}

CRTModSound::~CRTModSound()
{
}

CRTModSound* CRTModSound::AllocateAndConstruct()
{
    void* pRaw = g_soundPool.Alloc();
    if (!pRaw)
        return 0;
    return new (pRaw) CRTModSound();
}

void CRTModSound::ReleaseToPool(CRTModSound* pSound)
{
    g_soundPool.Free(pSound);
}

int CRTModSound::Init(CFxModManager* pOwnerMgr, CRTModTimeSource* pTimeSource, int nParam2, SRTModInitDesc* pDesc, int nSharedParam)
{
    if (!CRTMod::Init(pOwnerMgr, pTimeSource, nParam2, pDesc, nSharedParam))
        return 0;

    if (!((int*)pDesc)[75])
        return 0;

    m_pSndSet = (CRTModSoundSet*)pDesc;
    if (!m_pSndSet)
        return 0;

    return 1;
}

int CRTModSound::PlayTrackVoice(unsigned int nIndex)
{
    CRTModSoundTrack* pTrack = m_pSndSet->Tracks[nIndex];
    int hVoice = -1;

    CRTModVoiceFactory* pFactory = m_pSndSet->pVoiceFactory;
    if (pFactory)
    {
        pFactory->CreateVoice(0, pTrack->afPosition, 0);
        if (pTrack->pListener)
            hVoice = pTrack->pListener->OnTrackParam(1, m_nSharedParam, 0);
    }

    return hVoice;
}

int CRTModSound::PlayTrack(unsigned int nIndex)
{
    if (!m_pSndSet || nIndex >= m_pSndSet->Tracks.size())
        return -1;

    m_hTrackVoice[nIndex] = PlayTrackVoice(nIndex);
    return m_hTrackVoice[nIndex];
}

void CRTModSound::Update(int nElapsed)
{
    if (!IsExpired() || !m_nSharedParam)
        return;

    for (unsigned int i = 0; i < m_pSndSet->Tracks.size(); ++i)
    {
        CRTModSoundTrack* pTrack = m_pSndSet->Tracks[i];
        if (pTrack && pTrack->pListener && !RTModSound_IsVoicePlaying(m_hTrackVoice[i]))
            m_hTrackVoice[i] = PlayTrackVoice(i);
    }
}

void CRTModSound::StopAllTracks()
{
    for (unsigned int i = 0; i < m_pSndSet->Tracks.size(); ++i)
    {
        m_nTrackPlayed[i] = 1;
        if (m_pOwnerMgr)
        {
            if (m_hTrackVoice[i] != -1)
                RTModSound_ReleaseVoice(m_hTrackVoice[i]);
            m_hTrackVoice[i] = -1;
        }
    }
}

void CRTModSound::NotifyAllTracks()
{
    for (unsigned int i = 0; i < m_pSndSet->Tracks.size(); ++i)
    {
    }
}

void CRTModSound::BroadcastTrackParam(int nParam)
{
    for (unsigned int i = 0; i < m_pSndSet->Tracks.size(); ++i)
    {
        CRTModSoundTrack* pTrack = m_pSndSet->Tracks[i];
        if (pTrack->pListener)
            pTrack->pListener->OnTrackParam(0, nParam, 0);
    }
}
