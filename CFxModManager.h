#pragma once

#include <map>
#include <vector>

class CRTModSet;

class CFxModManager
{
public:
	typedef std::map<unsigned int, CRTModSet*> ModSetMap;
	typedef std::vector<CRTModSet*> ModSetArray;

public:
	CFxModManager();
	virtual ~CFxModManager();

public:
	void* GetOwner() const { return m_pOwner; }
	void SetOwner(void* pOwner) { m_pOwner = pOwner; }

	bool BuildFromModelInfo(void* pOwner, unsigned int dwModelId, void* pModelInfo);

	ModSetMap& GetModSetMap() { return m_mapModSet; }
	const ModSetMap& GetModSetMap() const { return m_mapModSet; }

	ModSetArray& GetPendingModSets() { return m_vecPendingModSets; }
	ModSetArray& GetModSets() { return m_vecModSets; }

	CRTModSet* FindModSet(unsigned int dwKey) const;
	CRTModSet* AddModSet(unsigned int dwKey);

	void ClearPendingModSets();
	void ClearModSets();
	void ClearModSetMap();

	bool IsBuilt() const { return m_bBuilt; }

private:
	void* m_pOwner;
	ModSetMap m_mapModSet;
	ModSetArray m_vecPendingModSets;
	ModSetArray m_vecModSets;
	CRTModSet* m_pLastModSet;
	bool m_bBuilt;
};
