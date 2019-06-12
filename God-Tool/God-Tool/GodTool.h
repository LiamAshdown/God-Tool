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
            DispelStruct.Draw();
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

    struct
    {
        bool Enabled;
        bool ShowDispelWindow;
        bool ShowAddAuraWindow;
        bool ShowErrorWindow;
        const char* Title;
        const char* Content;
        std::vector<SpellEntryRec> Auras;

        void Draw()
        {
            ImGui::PushID("Dispel Aura");
            ImGui::Checkbox("Dispel Aura", &Enabled);
            ImGui::PopID();
            ImGui::SameLine();

            if (ImGui::Button("Dispel List"))
                ShowDispelWindow = true;
        }

        void DrawDispelWindow(SpellRotatorStruct& p_SpellRotator)
        {
            if (ShowAddAuraWindow)
                DrawAddAuraWindow(p_SpellRotator);

            static bool ShowRemoveAura = false;

            static std::vector<SpellEntryRec>::iterator* s_Itr = nullptr;

            if (ImGui::Begin("Dispel List", &ShowDispelWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Auras to Dispel");
                ImGui::Separator();

                for (auto l_Itr = Auras.begin(); l_Itr != Auras.end(); l_Itr++)
                {
                    std::string l_Name = ((std::string)l_Itr->m_name_lang + "(" + std::to_string(l_Itr->m_ID) + ")");

                    if (ImGui::Selectable(l_Name.c_str()))
                    {
                        s_Itr = &l_Itr;
                        ShowRemoveAura = true;
                    }

                    if (ImGui::IsItemHovered())
                    {
                        if (ImGui::BeginPopupContextWindow((char*)"window_context1", 0, true))
                        {
                            if (ImGui::MenuItem("Remove Aura"))
                                Auras.erase(*s_Itr);
                            ImGui::EndPopup();
                        }
                    }
                }

                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::MenuItem("Add new Aura"))
                    {
                        if (ShowAddAuraWindow)
                            ShowAddAuraWindow = false;
                        else
                            ShowAddAuraWindow = true;
                    }
                    ImGui::EndPopup();
                }

                ImGui::End();
            }
        }

        void DrawAddAuraWindow(SpellRotatorStruct& p_SpellRotator)
        {
            ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Add new aura", &ShowAddAuraWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (ShowErrorWindow)
                    DrawError();

                if (ShowAddAuraWindow)
                {
                    int l_SpellId = 0;
                    ImGui::Text("Enter Aura Id");
                    bool l_Enter = ImGui::InputInt("", &l_SpellId, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);

                    if (l_Enter)
                    {
                        SpellEntryRec l_Entry;
                        sDatabaseMgr->GetDatabaseByIndex(Spell)->GetSpellRow(l_SpellId, l_Entry);
                        if (l_Entry.m_ID == 0)
                        {
                            ShowErrorWindow = true;
                            Title = "Error";
                            Content = "Incorrect Aura ID. Could not find aura with that ID. Please try again.";
                        }
                        else
                        {
                            Auras.push_back(l_Entry);
                            ShowAddAuraWindow = false;
                        }
                    }
                }
                ImGui::End();
            }
        }

        void DrawError()
        {
            ImGui::OpenPopup(Title);
            if (ImGui::BeginPopupModal(Title, &ShowErrorWindow))
            {
                ImGui::Text(Content);
                if (ImGui::Button("Close"))
                {
                    ShowErrorWindow = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }DispelStruct = { false, false, false, false };

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

    }UnionStruct = { false, 100 };

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