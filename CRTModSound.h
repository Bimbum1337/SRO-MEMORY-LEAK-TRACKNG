#pragma once

#include "CRTMod.h"

#define MAX_SOUND_TRACK_PER_ANI 15

class CRTModSoundSet;

class CRTModSound : public CRTMod
{
public:
    CRTModSound();
    virtual ~CRTModSound();

    static CRTModSound* AllocateAndConstruct();
    static void ReleaseToPool(CRTModSound* pSound);

    virtual int Init(CFxModManager* pOwnerMgr, CRTModTimeSource* pTimeSource, int nParam2, SRTModInitDesc* pDesc, int nSharedParam);
    virtual void Update(int nElapsed);
    virtual void StopAllTracks();
    virtual int PlayTrack(unsigned int nIndex);
    virtual void NotifyAllTracks();
    virtual void BroadcastTrackParam(int nParam);

protected:
    int PlayTrackVoice(unsigned int nIndex);

protected:
    CRTModSoundSet* m_pSndSet;
    int m_nTrackPlayed[MAX_SOUND_TRACK_PER_ANI];
    int m_hTrackVoice[MAX_SOUND_TRACK_PER_ANI];
};
