#pragma once

#include <list>
#include <string>

class CFxMod;

class CRTModSet
{
public:
	CRTModSet();
	virtual ~CRTModSet();

	static CRTModSet* AllocateAndConstruct();
	static void ReleaseToPool(CRTModSet* pModSet);

public:
	unsigned int GetId() const { return m_dwId; }
	void SetId(unsigned int dwId) { m_dwId = dwId; }

	bool IsBuilt() const { return m_bBuilt; }
	void SetBuilt(bool bBuilt) { m_bBuilt = bBuilt; }

	bool IsEnabled() const { return m_bEnabled; }
	void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }

	const std::string& GetName() const { return m_strName; }
	void SetName(const std::string& strName) { m_strName = strName; }

	const std::string& GetTag() const { return m_strTag; }
	void SetTag(const std::string& strTag) { m_strTag = strTag; }

	std::list<CFxMod*>& GetTracks() { return m_listTracks; }
	const std::list<CFxMod*>& GetTracks() const { return m_listTracks; }

	void AddTrack(CFxMod* pTrack);
	void ClearTracks();

private:
	unsigned int m_dwId;
	std::list<CFxMod*> m_listTracks;
	unsigned int m_dwFlags;
	bool m_bBuilt;
	std::string m_strName;
	std::string m_strTag;
	bool m_bEnabled;
};
