#pragma once
#include "Common/SharedDefines.h"
#include "Detour/Detour.h"
#include "Common/Types.h"
#include "Common/Offsets.h"
#include "Common/Timer.h"
#include "Objects/CGPlayer_C.h"

typedef struct SpellRotatorStruct
{
public:
    SpellRotatorStruct(SpellEntryRec p_SpellEntry) : SpellEntry(p_SpellEntry), Enabled(false) {}
    SpellRotatorStruct() {}

    SpellRotatorStruct& operator=(SpellRotatorStruct& p_Right)
    {
        this->SpellEntry = p_Right.SpellEntry;
        this->Enabled = p_Right.Enabled;
        this->HealthStruct = p_Right.HealthStruct;
        this->UseInCombatStruct = p_Right.UseInCombatStruct;
        this->UnionStruct = p_Right.UnionStruct;

        return *this;
    }

public:
    SpellEntryRec SpellEntry;
    bool Enabled;

    void Draw()
    {
        DrawEnabled();
        UseInCombatStruct.Draw();

        if (SpellEntry.m_effect[0] == SPELL_EFFECT_APPLY_AURA)
        {
            UnionStruct.KeepBuffActiveStruct.Draw();
            HealthStruct.Draw();
        }
        else if (SpellEntry.m_effect[0] == SPELL_EFFECT_DISPEL)
            UnionStruct.DispelStruct.Draw();
        else
        {
            UnionStruct.ManaStruct.Draw();
            HealthStruct.Draw();
        }
    }

    void Reset()
    {
        Enabled = false;
        HealthStruct.Reset();
        UseInCombatStruct.Reset();
        UnionStruct = { false };
    }

private:
    void DrawEnabled()
    {
        std::string l_SpellName = SpellEntry.m_name_lang;

        /// Rank
        if (strlen(SpellEntry.m_nameSubtext_lang) > 4)
            l_SpellName = l_SpellName + "(" + SpellEntry.m_nameSubtext_lang + ")";

        ImGui::Text(l_SpellName.c_str());
        ImGui::SameLine(ImGui::GetWindowWidth() - 80.0f);
        ImGui::PushID("Enabled");
        ImGui::Checkbox("Enabled", &Enabled);
        ImGui::PopID();
        ImGui::Separator();
        ImGui::TextWrapped(SpellEntry.m_description_lang);
        ImGui::Spacing(); 
        ImGui::Spacing();
    }

    struct
    {
        bool Enabled;
        int Health;

        void Draw()
        {
            ImGui::TextWrapped("Cast spell at %u%% health", Health);
            ImGui::PushID("Scroll Health");
            ImGui::SliderInt("", &Health, 1, 100);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::PushID("Enable Health");
            ImGui::Checkbox("Enabled", &Enabled);
            ImGui::PopID();
            ImGui::Spacing();
        }

        void Reset()
        {
            Enabled = false;
            Health = 100;
        }
    }HealthStruct = { false, 100 };

    struct
    {
        bool Enabled;

        void Draw()
        {
            ImGui::PushID("Enabled Combat");
            ImGui::Checkbox("Use In Combat", &Enabled);
            ImGui::PopID();
            ImGui::Spacing();
        }

        void Reset()
        {
            Enabled = false;
        }
    }UseInCombatStruct = { false };

    union
    {
        struct
        {
            bool Enabled;
            int Mana;

            void Draw()
            {
                ImGui::TextWrapped("Cast spell at %u%% mana", Mana);
                ImGui::PushID("Scroll Mana");
                ImGui::SliderInt("", &Mana, 1, 100);
                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID("Enabled Mana");
                ImGui::Checkbox("Enabled", &Enabled);
                ImGui::PopID();
                ImGui::Spacing();
            }

        }ManaStruct;

        struct
        {
            bool Enabled;

            void Draw()
            {
                ImGui::PushID("Active Buffer");
                ImGui::Checkbox("Keep Buff Active", &Enabled);
                ImGui::PopID();
                ImGui::Spacing();
            }

        }KeepBuffActiveStruct;

        struct
        {
            bool Enabled;

            void Draw()
            {
                ImGui::PushID("Dispel Aura");
                ImGui::Checkbox("Dispel Aura", &Enabled);
                ImGui::PopID();
                ImGui::Spacing();
            }

        }DispelStruct;
    }UnionStruct = { false };

}SpellRotator;

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

public:
    std::vector<CGPlayer_C*>& GetPulse() { return m_PulsePlayers; }
    std::vector<SpellRotator*> GetLocalSpells() { return m_LocalPlayerSpells; }

private:
    /// Hooks
    static int __fastcall ReadChatMessage(int p_Instance, int EDX_Dummy, int p_A2, int p_A3);
    static int __cdecl AddChatMessage(char *text, int8 type, char *player, int lang, char *channel, char *player2, char *chatTag, uint64 sender, int a9, unsigned __int64 target, int achievement, char isGMChat, int a13);

public:
    /// Visible Objects iterator
    int EnumerateVisibleObjects(int(__cdecl* VisibleObjectsEnumProc)(uint64 p_GUID, void *p_Param), void * p_Stop);
    static int EnumerateVisibleObjectsCallBack(uint64 p_GUID, void* p_Param);

private:
    void LoadLocalPlayerSpells();

private:
    std::map<std::string, Detour*> m_Detours;
    std::vector<CGPlayer_C*> m_PulsePlayers;
    std::vector<SpellRotator*> m_LocalPlayerSpells;
    Timer m_PulseTimer;
};

#define sGodTool GodTool::instance()