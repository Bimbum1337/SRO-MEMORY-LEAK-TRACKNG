#pragma once

class CRTMod;
struct SRTModInitDesc;

CRTMod* CreateRTModByTypeId(int nPackedTypeId, SRTModInitDesc* pDesc);
void DestroyRTModByTag(CRTMod* pMod);
