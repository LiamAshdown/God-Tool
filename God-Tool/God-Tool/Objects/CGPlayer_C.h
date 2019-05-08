#pragma once
#include "../Common/SharedDefines.h"
#include "../Common/Offsets.h"
#include "CGUnit_C.h"
#include "../ClientService/ClientServices.h"

class CGPlayer_C : public CGUnit_C
{
public:
    CGPlayer_C() {}

    ~CGPlayer_C() {}

public:
    bool __thiscall IsInPartyOrRaid() { return ((bool(__thiscall*)(DWORD p_BaseAddress))IsInRaidOrPartyAddr)((DWORD)this); }
    int __thiscall GetCoinage() { return ((int(__thiscall*)(DWORD p_BaseAddress))GetCoinageAddr)((DWORD)this); }

    bool IntersectsTarget(CGPlayer_C* p_Target)
    {
        if (p_Target)
        {
            V3Position* l_PlayerPos = new V3Position;
            V3Position* l_TargetPos = new V3Position;
            V3Position* l_ResultPos = new V3Position;

            GetPosition(*l_PlayerPos);
            p_Target->GetPosition(*l_TargetPos);
            float l_Unk = 1.0f;
            char test = WorldIntersect(l_PlayerPos, l_TargetPos, l_ResultPos, &l_Unk, 0x100111, 0);
        }
    }

    void SendChatMessage(std::string const p_Message, uint8 const p_Type)
    {
        CDataStore data;
        data.PutInt32(CMSG_MESSAGECHAT);
        data.PutInt32(p_Type);
        data.PutInt32(LANG_COMMON);
        data.PutString(p_Message.c_str());
        data.Finalize();
        ClientServices::SendPacket(&data);
    }

private:
    char __cdecl WorldIntersect(V3Position * a1, V3Position * a2, V3Position * a3, float * a4, int a5, int a6)
    {
        return ((char(__cdecl*)(V3Position*, V3Position*, V3Position*, float*, int, int))IntersectAddr)(a1, a2, a3, a4, a5, a6);
    }
};
