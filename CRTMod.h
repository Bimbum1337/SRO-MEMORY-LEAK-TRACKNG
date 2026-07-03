#pragma once

#include <string>

struct SRTModInitDesc;
class CRTModSet;
class CRTModTimeSource;

class CRTMod
{
public:
    CRTMod();
    virtual ~CRTMod();

    virtual int Init(CRTModSet* pOwnerMgr, CRTModTimeSource* pTimeSource, int nParam2, SRTModInitDesc* pDesc, int nSharedParam);
    virtual void Update(int nElapsed);
    virtual void OnDeviceLost();
    virtual void OnDeviceReset();
    virtual void OnFrameBegin();
    virtual void OnFrameEnd();
    virtual void StopAllTracks();
    virtual int PlayTrack(unsigned int nIndex);
    virtual void NotifyAllTracks();
    virtual void Render();
    virtual void BroadcastTrackParam(int nParam);
    virtual void Pause();
    virtual void Resume();
    virtual bool IsFinished();

    int GetType() const { return m_nType; }

protected:
    bool IsExpired();

protected:
    SRTModInitDesc* m_pDesc;
    CRTModTimeSource* m_pTimeSource;
    CRTModSet* m_pOwnerMgr;
    int m_nParam2;
    int m_nSharedParam;
    int m_nType;
    int m_nReserved028;
    std::string m_strName;
    int m_nReserved056;
    float m_fWeight;
};
