#include "CFxModManager.h"
#include "CRTModSet.h"

CFxModManager::CFxModManager()
	: m_pOwner(0)
	, m_mapModSet()
	, m_vecPendingModSets()
	, m_vecModSets()
	, m_pLastModSet(0)
	, m_bBuilt(false)
{
}

CFxModManager::~CFxModManager()
{
	ClearPendingModSets();
	ClearModSetMap();
	m_vecPendingModSets.clear();
	m_vecModSets.clear();
}

void CFxModManager::ClearPendingModSets()
{
	ModSetArray::iterator it = m_vecPendingModSets.begin();
	for (; it != m_vecPendingModSets.end(); ++it)
		delete *it;
	m_vecPendingModSets.clear();
}

void CFxModManager::ClearModSets()
{
	ModSetArray::iterator it = m_vecModSets.begin();
	for (; it != m_vecModSets.end(); ++it)
		delete *it;
	m_vecModSets.clear();
}

void CFxModManager::ClearModSetMap()
{
	ClearModSets();
	m_mapModSet.clear();
}

CRTModSet* CFxModManager::FindModSet(unsigned int dwKey) const
{
	ModSetMap::const_iterator it = m_mapModSet.find(dwKey);
	if (it == m_mapModSet.end())
		return 0;
	return it->second;
}

CRTModSet* CFxModManager::AddModSet(unsigned int dwKey)
{
	CRTModSet*& rpModSet = m_mapModSet[dwKey];
	if (rpModSet == 0)
	{
		rpModSet = new CRTModSet;
		rpModSet->SetId(dwKey);
	}
	return rpModSet;
}

bool CFxModManager::BuildFromModelInfo(void* pOwner, unsigned int dwModelId, void* pModelInfo)
{
	m_pOwner = pOwner;

	if (pModelInfo == 0)
		return true;

	ClearModSets();

	CRTModSet* pModSet = AddModSet(dwModelId);
	pModSet->SetBuilt(true);
	m_pLastModSet = pModSet;
	m_vecModSets.push_back(pModSet);

	m_bBuilt = true;
	return true;
}
