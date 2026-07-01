#include "ObjGeneralInfo.h"

struct SSerializeBuffer
{
    void *m_pCursor;
    void *m_pEnd;
};

extern void SerializeBufferWriteDword(SSerializeBuffer *pBuf, unsigned int dwValue);
extern void SerializeBufferWriteString(SSerializeBuffer *pBuf, const std::string &strValue);
extern unsigned int SerializeBufferReadDword(SSerializeBuffer *pBuf);
extern void SerializeBufferReadString(SSerializeBuffer *pBuf, std::string &strOut);

ObjGeneralInfo::ObjGeneralInfo()
    : m_dwFlags(0x2FFFF), m_nId1(-1), m_nId2(-1)
{
}

unsigned int ObjGeneralInfo::GetFlags() const
{
    return m_dwFlags;
}

void ObjGeneralInfo::SetFlags(unsigned int dwFlags)
{
    m_dwFlags = dwFlags;
}

void ObjGeneralInfo::Serialize(SSerializeBuffer *pBuf)
{
    SerializeBufferWriteDword(pBuf, m_dwFlags);
    SerializeBufferWriteString(pBuf, m_strName);
    SerializeBufferWriteDword(pBuf, static_cast<unsigned int>(m_nId1));
    SerializeBufferWriteDword(pBuf, static_cast<unsigned int>(m_nId2));
}

void ObjGeneralInfo::Deserialize(SSerializeBuffer *pBuf)
{
    m_dwFlags = SerializeBufferReadDword(pBuf);
    SerializeBufferReadString(pBuf, m_strName);
    m_nId1 = static_cast<int>(SerializeBufferReadDword(pBuf));
    m_nId2 = static_cast<int>(SerializeBufferReadDword(pBuf));
}
