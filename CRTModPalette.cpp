#include "CRTModPalette.h"
#include "CMemPool.h"
#include <new>

static CMemPool<5994344087588765696ULL, CRTModPalette, 1000, 1> g_palettePool;

CRTModPalette::CRTModPalette()
    : m_bReady(false)
    , m_pListSentinelPrev(0)
    , m_pListSentinelNext(0)
    , m_nReserved0(0)
    , m_bLocked0(true)
    , m_bLocked1(true)
{
    m_positionKeys.reserve(9);
}

CRTModPalette::~CRTModPalette()
{
    Clear();
}

CRTModPalette* CRTModPalette::AllocateAndConstruct()
{
    void* pRaw = g_palettePool.Alloc();
    if (!pRaw)
        return 0;
    return new (pRaw) CRTModPalette();
}

void CRTModPalette::ReleaseToPool(CRTModPalette* pPalette)
{
    g_palettePool.Free(pPalette);
}

CRTMod* CRTModPalette::AddTrack(CRTMod* pTrack)
{
    m_tracks.push_back(pTrack);
    return pTrack;
}

CRTMod* CRTModPalette::AddNamedTrack(CRTMod* pTrack)
{
    m_namedTracks.push_back(pTrack);
    return pTrack;
}

void CRTModPalette::InsertKeyedTrack(int nKey, CRTMod* pTrack)
{
    m_keyedTracks[nKey] = pTrack;
}

void CRTModPalette::Clear()
{
    m_tracks.clear();
    m_namedTracks.clear();
    m_keyedTracks.clear();
    m_reservedIds.clear();
}

unsigned int CRTModPalette::GetTrackCount() const
{
    return (unsigned int)m_tracks.size();
}

unsigned int CRTModPalette::GetNamedTrackCount() const
{
    return (unsigned int)m_namedTracks.size();
}
