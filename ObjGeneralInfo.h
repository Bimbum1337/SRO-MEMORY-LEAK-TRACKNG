#ifndef __OBJGENERALINFO_H__
#define __OBJGENERALINFO_H__

#include <string>

struct SSerializeBuffer;

class ObjGeneralInfo
{
public:
    ObjGeneralInfo();

    virtual void Serialize(SSerializeBuffer *pBuf);
    virtual void Deserialize(SSerializeBuffer *pBuf);

    unsigned int GetFlags() const;
    void SetFlags(unsigned int dwFlags);

private:
    unsigned int m_dwFlags;
    std::string m_strName;
    int m_nId1;
    int m_nId2;
};

#endif
