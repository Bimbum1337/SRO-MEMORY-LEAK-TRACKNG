#include "CRTModSet.h"
#include "CMemPool.h"
#include <new>

static CMemPool<5994344087689428992ULL, CRTModSet, 100, 1> g_modSetPool;

class CFxMod
{
public:
	virtual ~CFxMod() {}
};

CRTModSet::CRTModSet()
	: m_dwId(0)
	, m_dwFlags(0)
	, m_bBuilt(false)
	, m_strName()
	, m_strTag()
	, m_bEnabled(false)
{
}

CRTModSet::~CRTModSet()
{
	ClearTracks();
}

CRTModSet* CRTModSet::AllocateAndConstruct()
{
	void* pRaw = g_modSetPool.Alloc();
	if (!pRaw)
		return 0;
	return new (pRaw) CRTModSet();
}

void CRTModSet::ReleaseToPool(CRTModSet* pModSet)
{
	g_modSetPool.Free(pModSet);
}

void CRTModSet::AddTrack(CFxMod* pTrack)
{
	m_listTracks.push_back(pTrack);
}

void CRTModSet::ClearTracks()
{
	std::list<CFxMod*>::iterator it = m_listTracks.begin();
	while (it != m_listTracks.end())
	{
		delete *it;
		++it;
	}
	m_listTracks.clear();
}
