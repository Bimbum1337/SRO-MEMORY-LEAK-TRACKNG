#include "CRTMod.h"

struct SRTModInitDesc
{
    int nDummy0;
    float fWeight;
};

class CRTModTimeSource
{
public:
    int nDummy0[31];
    float fElapsedRatio;
};

CRTMod::CRTMod()
    : m_pDesc(0)
    , m_pTimeSource(0)
    , m_pOwnerMgr(0)
    , m_nParam2(0)
    , m_nSharedParam(0)
    , m_nType(-1)
    , m_fWeight(0.5f)
{
}

CRTMod::~CRTMod()
{
}

int CRTMod::Init(CFxModManager* pOwnerMgr, CRTModTimeSource* pTimeSource, int nParam2, SRTModInitDesc* pDesc, int nSharedParam)
{
    m_pOwnerMgr = pOwnerMgr;
    m_pTimeSource = pTimeSource;
    m_nParam2 = nParam2;
    m_pDesc = pDesc;
    m_nSharedParam = nSharedParam;
    if (pDesc)
        m_fWeight = pDesc->fWeight;
    return 1;
}

void CRTMod::Update(int nElapsed)
{
}

void CRTMod::OnDeviceLost()
{
}

void CRTMod::OnDeviceReset()
{
}

void CRTMod::OnFrameBegin()
{
}

void CRTMod::OnFrameEnd()
{
}

void CRTMod::StopAllTracks()
{
}

int CRTMod::PlayTrack(unsigned int nIndex)
{
    return -1;
}

void CRTMod::NotifyAllTracks()
{
}

void CRTMod::Render()
{
}

void CRTMod::BroadcastTrackParam(int nParam)
{
}

void CRTMod::Pause()
{
}

void CRTMod::Resume()
{
}

bool CRTMod::IsFinished()
{
    return false;
}

bool CRTMod::IsExpired()
{
    return m_fWeight >= m_pTimeSource->fElapsedRatio;
}
