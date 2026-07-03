#include "CRTModSet.h"
#include "CRTMod.h"
#include "CRTModFactory.h"
#include "CMemPool.h"
#include <new>

static CMemPool<5994344087689428992ULL, CRTModSet, 100, 1> g_modSetPool;

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

CRTMod* CRTModSet::CreateTrack(int nPackedTypeId, CRTModTimeSource* pTimeSource, int nParam2, SRTModInitDesc* pDesc, int nSharedParam)
{
	CRTMod* pTrack = CreateRTModByTypeId(nPackedTypeId, pDesc);
	if (!pTrack)
		return 0;

	if (!pTrack->Init(this, pTimeSource, nParam2, pDesc, nSharedParam))
	{
		DestroyRTModByTag(pTrack);
		return 0;
	}

	m_listTracks.push_back(pTrack);
	return pTrack;
}

void CRTModSet::ClearTracks()
{
	std::list<CRTMod*>::iterator it = m_listTracks.begin();
	while (it != m_listTracks.end())
	{
		DestroyRTModByTag(*it);
		++it;
	}
	m_listTracks.clear();
}
