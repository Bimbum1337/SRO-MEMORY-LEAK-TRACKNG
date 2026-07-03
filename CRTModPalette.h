#pragma once

#include <vector>
#include <map>
#include <string>

class CRTMod;

class CRTModPalette
{
public:
    CRTModPalette();
    virtual ~CRTModPalette();

    static CRTModPalette* AllocateAndConstruct();
    static void ReleaseToPool(CRTModPalette* pPalette);

    CRTMod* AddTrack(CRTMod* pTrack);
    CRTMod* AddNamedTrack(CRTMod* pTrack);
    void InsertKeyedTrack(int nKey, CRTMod* pTrack);
    void Clear();

    unsigned int GetTrackCount() const;
    unsigned int GetNamedTrackCount() const;

protected:
    struct SPositionKey
    {
        int nKeyLo;
        int nKeyHi;
    };

    bool m_bReady;
    void* m_pListSentinelPrev;
    void* m_pListSentinelNext;
    int m_nReserved0;
    std::vector<SPositionKey> m_positionKeys;
    bool m_bLocked0;
    bool m_bLocked1;

    std::vector<CRTMod*> m_tracks;
    std::vector<CRTMod*> m_namedTracks;
    std::map<int, CRTMod*> m_keyedTracks;

    std::vector<int> m_reservedIds;
};
