#pragma once

#include <vector>
#include <map>
#include <string>

class CFxMod;

class CRTModPalette
{
public:
    CRTModPalette();
    virtual ~CRTModPalette();

    static CRTModPalette* AllocateAndConstruct();
    static void ReleaseToPool(CRTModPalette* pPalette);

    CFxMod* AddTrack(CFxMod* pTrack);
    CFxMod* AddNamedTrack(CFxMod* pTrack);
    void InsertKeyedTrack(int nKey, CFxMod* pTrack);
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

    std::vector<CFxMod*> m_tracks;
    std::vector<CFxMod*> m_namedTracks;
    std::map<int, CFxMod*> m_keyedTracks;

    std::vector<int> m_reservedIds;
};
