#pragma once
#include "Common/SharedDefines.h"
#include "Detour/Detour.h"
#include "Common/Types.h"
#include "Common/Offsets.h"
#include "Common/Timer.h"
#include "Objects/CGPlayer_C.h"

class GodTool
{
public:
    static GodTool* instance();

public:
    GodTool();
    ~GodTool();

public:
    void Update();

public:
    /// Player
    CGPlayer_C* GetLocalPlayer();
    CGPlayer_C* GetTargetPlayer();
    CGObject_C* GetTargetByGUID(uint64 p_GUID, TypeMask p_Mask);
    uint64 GetLocalPlayerGUID();

private:
    /// Hooks
    static int __fastcall ReadChatMessage(int p_Instance, int EDX_Dummy, int p_A2, int p_A3);
    static int __cdecl AddChatMessage(char *text, int8 type, char *player, int lang, char *channel, char *player2, char *chatTag, uint64 sender, int a9, unsigned __int64 target, int achievement, char isGMChat, int a13);

public:
    /// Visible Objects iterator
    int EnumerateVisibleObjects(int(__cdecl* VisibleObjectsEnumProc)(uint64 p_GUID, void *p_Param), void * p_Stop);
    static int EnumerateVisibleObjectsCallBack(uint64 p_GUID, void* p_Param);

private:
    std::map<std::string, Detour*> m_Detours;
    std::vector<CGPlayer_C*> m_PulsePlayers;
    Timer m_PulseTimer;
};

#define sGodTool GodTool::instance()