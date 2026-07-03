#include "CRTModFactory.h"
#include "CRTMod.h"
#include "CRTModSound.h"

class CRTModMtrl;
class CRTModTexAnim;
class CRTModMultiTex;
class CRTModMultiTexRev;
class CRTModParticle;
class CRTModEnvMap;
class CRTModBumpEnv;
class CRTModDyVertex;
class CRTModDyJoint;
class CRTModDyLattice;
class CRTModProgEquipPow;

extern CRTModMtrl* ConstructCRTModMtrl();
extern CRTModTexAnim* ConstructCRTModTexAnim();
extern CRTModMultiTex* ConstructCRTModMultiTex();
extern CRTModMultiTexRev* ConstructCRTModMultiTexRev(SRTModInitDesc* pDesc);
extern CRTModParticle* ConstructCRTModParticle();
extern CRTModEnvMap* ConstructCRTModEnvMap();
extern CRTModBumpEnv* ConstructCRTModBumpEnv();
extern CRTModDyVertex* ConstructCRTModDyVertex();
extern CRTModDyJoint* ConstructCRTModDyJoint();
extern CRTModDyLattice* ConstructCRTModDyLattice();
extern CRTModProgEquipPow* ConstructCRTModProgEquipPow();

extern void DestroyCRTModMtrl(CRTMod* pMod);
extern void DestroyCRTModTexAnim(CRTMod* pMod);
extern void DestroyCRTModMultiTex(CRTMod* pMod);
extern void DestroyCRTModMultiTexRev(CRTMod* pMod);
extern void DestroyCRTModParticle(CRTMod* pMod);
extern void DestroyCRTModEnvMap(CRTMod* pMod);
extern void DestroyCRTGeometry(CRTMod* pMod);
extern void DestroyCRTModBumpEnv(CRTMod* pMod);
extern void DestroyCRTModDyJoint(CRTMod* pMod);
extern void DestroyCRTModDyVertex(CRTMod* pMod);
extern void DestroyCRTModProgEquipPow(CRTMod* pMod);
extern void DestroyCRTModDyLattice(CRTMod* pMod);

CRTMod* CreateRTModByTypeId(int nPackedTypeId, SRTModInitDesc* pDesc)
{
    short nOuter = (short)(nPackedTypeId >> 16);
    short nInner = (short)(nPackedTypeId & 0xFFFF);

    switch (nOuter)
    {
    case 0:
        if (nInner == 0)
            return (CRTMod*)ConstructCRTModMtrl();
        return 0;

    case 1:
        if (nInner == 0)
            return (CRTMod*)ConstructCRTModTexAnim();
        if (nInner == 1)
            return (CRTMod*)ConstructCRTModMultiTex();
        if (nInner == 2)
            return (CRTMod*)ConstructCRTModMultiTexRev(pDesc);
        return 0;

    case 2:
        return 0;

    case 3:
        if (nInner == 0)
            return (CRTMod*)ConstructCRTModParticle();
        return 0;

    case 4:
        if (nInner == 0)
            return (CRTMod*)ConstructCRTModEnvMap();
        if (nInner == 1)
            return (CRTMod*)ConstructCRTModBumpEnv();
        return 0;

    case 5:
        if (nInner == 0)
            return (CRTMod*)CRTModSound::AllocateAndConstruct();
        return 0;

    case 6:
        if (nInner == 0)
            return (CRTMod*)ConstructCRTModDyVertex();
        if (nInner == 1)
            return (CRTMod*)ConstructCRTModDyJoint();
        if (nInner == 2)
            return (CRTMod*)ConstructCRTModDyLattice();
        return 0;

    case 7:
        if (nInner == 0)
            return (CRTMod*)ConstructCRTModProgEquipPow();
        return 0;

    default:
        return 0;
    }
}

void DestroyRTModByTag(CRTMod* pMod)
{
    if (!pMod)
        return;

    switch (pMod->GetType())
    {
    case 0:
        DestroyCRTModMtrl(pMod);
        break;
    case 1:
        DestroyCRTModTexAnim(pMod);
        break;
    case 2:
        DestroyCRTModMultiTex(pMod);
        break;
    case 3:
        DestroyCRTModMultiTexRev(pMod);
        break;
    case 4:
        DestroyCRTModParticle(pMod);
        break;
    case 5:
        DestroyCRTGeometry(pMod);
        break;
    case 6:
        pMod->~CRTMod();
        CRTModSound::ReleaseToPool((CRTModSound*)pMod);
        break;
    case 7:
        DestroyCRTModDyJoint(pMod);
        break;
    case 8:
        DestroyCRTModBumpEnv(pMod);
        break;
    case 9:
        DestroyCRTModDyVertex(pMod);
        break;
    case 0xA:
        DestroyCRTModProgEquipPow(pMod);
        break;
    case 0xB:
        DestroyCRTModDyLattice(pMod);
        break;
    default:
        break;
    }
}
