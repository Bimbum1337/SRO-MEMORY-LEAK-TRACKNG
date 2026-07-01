#include "CCompChar.h"
#include "CCompBldg.h"
#include "CCompSimple.h"

CCompoundObj *CreateCompoundObjInstance(int nTypeId)
{
    if (static_cast<unsigned short>(static_cast<unsigned int>(nTypeId) >> 16) == 2)
    {
        switch (static_cast<short>(nTypeId))
        {
        case 0:
            return CCompChar::AllocateAndConstruct();
        case 2:
            return CCompBldg::AllocateAndConstruct();
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
            return CCompSimple::AllocateAndConstruct();
        default:
            return 0;
        }
    }
    else if (static_cast<unsigned short>(static_cast<unsigned int>(nTypeId) >> 16) == 3)
    {
        switch (static_cast<short>(nTypeId))
        {
        case 0:
            return CCompChar::AllocateAndConstruct();
        case 2:
            return CCompBldg::AllocateAndConstruct();
        default:
            return CCompSimple::AllocateAndConstruct();
        }
    }

    return 0;
}

void DestroyCompoundObjInstance(CCompoundObj *pObj)
{
    switch (pObj->GetTypeTag())
    {
    case 0:
        CCompChar::ReleaseToPool(static_cast<CCompChar *>(pObj));
        break;
    case 2:
        CCompBldg::ReleaseToPool(static_cast<CCompBldg *>(pObj));
        break;
    default:
        CCompSimple::ReleaseToPool(static_cast<CCompSimple *>(pObj));
        break;
    }
}
