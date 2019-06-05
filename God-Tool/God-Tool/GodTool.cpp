#include "GodTool.h"
#include "Lua/Lua.h"
#include "GUI/Graphics.h"

GodTool * GodTool::instance()
{
    static GodTool instance;
    return &instance;
}

GodTool::GodTool()
{
    m_Detours["ReadChat"] = new Detour(ReadChatAddr, (int)&GodTool::ReadChatMessage);
    m_Detours["AddChatMessage"] = new Detour(AddChatAddr, (int)&GodTool::AddChatMessage);
    m_PulseTimer.Reset();

    FramescriptExecute("DEFAULT_CHAT_FRAME:AddMessage(\"|c00D358F7GodTool|r Loaded GodTool successfully. By Quadral\")");

    LoadLocalPlayerSpells();
}

GodTool::~GodTool()
{
}

void GodTool::Update()
{
    for (auto const& l_Itr : m_Detours)
        l_Itr.second->Apply();

    //FramescriptExecute("CastSpellByName(\"Demon Skin\")");

    if (m_PulseTimer.Elasped() > 10000)
    {
        m_PulsePlayers.clear();
        EnumerateVisibleObjects(EnumerateVisibleObjectsCallBack, NULL);
        m_PulseTimer.Reset();
    }
}

void GodTool::LoadLocalPlayerSpells()
{
    for (int v19 = 0; v19 < GetTotalSpells(); ++v19)
    {
        int* rowId = (int*)(0x00BE6D88);
        int v20 = rowId[v19];

        SpellEntryRec l_Entry;
        sDatabaseMgr->GetDatabaseByIndex(Spell)->GetSpellRow(v20, l_Entry);

        SpellRotator* l_SpellRotator = new SpellRotator(std::move(l_Entry));

        m_LocalPlayerSpells.push_back(l_SpellRotator);
    }
}

CGPlayer_C* GodTool::GetLocalPlayer()
{
    return ((CGPlayer_C*(__cdecl*)(uint64 p_Guid, TypeMask p_Mask))GetObjectPtrAddr)(GetLocalPlayerGUID(), TYPEMASK_PLAYER);
}

CGPlayer_C* GodTool::GetTargetPlayer()
{
    return ((CGPlayer_C*(__cdecl*)(uint64 p_Guid, TypeMask p_Mask))GetObjectPtrAddr)(GetLocalPlayer()->GetValue<uint64>(UNIT_FIELD_TARGET), TYPEMASK_PLAYER);
}

CGObject_C * GodTool::GetTargetByGUID(uint64 p_GUID, TypeMask p_Mask)
{
    return ((CGObject_C*(__cdecl*)(uint64 p_Guid, TypeMask p_Mask))GetObjectPtrAddr)(p_GUID, p_Mask);
}

uint64 GodTool::GetLocalPlayerGUID()
{
    return ((uint64(__cdecl*)())ClntObjMgrGetActivePlayerGuidAddr)();
}

int __fastcall GodTool::ReadChatMessage(int p_Instance, int EDX_Dummy, int p_A2, int p_A3)
{
    int l_Ptr = *(int*)(p_Instance + 0x2B4);

    if (l_Ptr)
    {
        std::string l_Message = (char*)l_Ptr;
        /*if (l_Message.length() > 1)
            std::cout << l_Message << std::endl;*/
    }

    auto l_Itr = sGodTool->m_Detours["ReadChat"];
    l_Itr->Restore();

    return ((int(__thiscall*)(int, int, int))l_Itr->m_HookAddress)(p_Instance, p_A2, p_A3);
}

int __cdecl GodTool::AddChatMessage(char* p_Text, int8 p_Type, char* p_Player, int p_Language, char* p_Channel, char * p_Player2, char * p_ChatTag, uint64 p_SenderGUID, int a9, unsigned __int64 p_Target, int p_Achievement, char p_IsGMChat, int a13)
{
    if (p_Text && p_Language == LANG_UNIVERSAL && p_Achievement == 0)
    {
        std::string l_Channel;
        switch (p_Type)
        {
        case CHAT_TYPE_SAY:
            l_Channel = "[SAY] ";
            break;
        case CHAT_TYPE_WHISPER:
            l_Channel = "[WHISPER] ";
            break;
        case CHAT_TYPE_YELL:
            l_Channel = "[YELL] ";
            break;
        default:
            l_Channel = "[CHANNEL] ";
            break;
        }

        /// Get our sender
        CGPlayer_C* l_Sender = reinterpret_cast<CGPlayer_C*>(sGodTool->GetTargetByGUID(p_SenderGUID, TYPEMASK_UNIT));

        if (l_Sender)
            sGraphics->m_ConsoleChatLog.ChatLog.push_back(l_Channel + l_Sender->GetObjectName() + ": " + p_Text + "\n");
    }

    auto l_Itr = sGodTool->m_Detours["AddChatMessage"];
    l_Itr->Restore();

    return ((int(__cdecl*)(char*, int8, char*, int, char*, char*, char*, uint64, int, unsigned __int64, int, char, int))l_Itr->m_HookAddress)(p_Text, p_Type, p_Player, p_Language, p_Channel, p_Player2, p_ChatTag, p_SenderGUID, a9, p_Target, p_Achievement, p_IsGMChat, a13);
}

int GodTool::EnumerateVisibleObjects(int(__cdecl* VisibleObjectsEnumProc)(uint64 p_GUID, void* p_Param), void* p_Stop)
{
    return ((int(__cdecl*)(int(__cdecl* VisibleObjectsEnumProc)(uint64 p_GUID, void *p_Param), void * p_Stop))EnumVisibleObjectsAddr)(VisibleObjectsEnumProc, p_Stop);
}

int GodTool::EnumerateVisibleObjectsCallBack(uint64 p_GUID, void* p_Param)
{
    CGObject_C* l_Object = sGodTool->GetTargetByGUID(p_GUID, TYPEMASK_UNIT);

    if (!l_Object)
        return true;

    switch (l_Object->GetTypeID())
    {
    case TYPEID_PLAYER:
    {
        sGodTool->m_PulsePlayers.push_back(reinterpret_cast<CGPlayer_C*>(l_Object));
    }
    break;
    default:
        break;
    }

    return true;
}