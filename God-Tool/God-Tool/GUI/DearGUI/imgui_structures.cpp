#include "imgui_structures.h"
#include "../../GodTool.h"
#include "../../Lua/Lua.h"

static void ShowPlayerInfo(bool* p_open, CGPlayer_C* p_Player)
{
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player Info", p_open, ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("Player Info");
        ImGui::Text("Player Name: %s", p_Player->GetObjectName());
        V3Position l_Position;
        p_Player->GetPosition(l_Position);
        ImGui::Text("Health: %u", p_Player->GetHealth());
        ImGui::Text("Level: %u", p_Player->GetLevel());
        ImGui::Text("GUID: %u", p_Player->GetGUID());
        ImGui::Text("Friendly: %s", sGodTool->GetLocalPlayer()->GetUnitReaction(p_Player) >= REP_FRIENDLY ? "true" : "false");
        ImGui::Text("X: %f", l_Position.x);
        ImGui::Text("Y: %f", l_Position.y);
        ImGui::Text("Z: %f", l_Position.z);
        ImGui::Text("In LoS: %s", sGodTool->GetLocalPlayer()->IsInLoS(p_Player) ? "false" : "true");
        ImGui::Separator();
        ImGui::Text("Auras");
        std::vector<Aura> l_Auras;
        p_Player->GetAuras(l_Auras);

        for (auto l_Itr : l_Auras)
        {
            ImGui::Text("Aura Name: %s", l_Itr.GetSpell().m_name_lang);
        }

        ImGui::Separator();

        AreaTableEntry* l_AreaTable = (AreaTableEntry*)sDatabaseMgr->GetDatabaseByIndex(AreaTable)->GetClientDB()->Rows[GetAreaId() -
            sDatabaseMgr->GetDatabaseByIndex(AreaTable)->GetClientDB()->minIndex];

        ImGui::Text("Zone");
        ImGui::Text("Id: %u", GetAreaId());
        ImGui::Text("Continent: %u", l_AreaTable->m_ContinentID);
        ImGui::Text("Name: %s", l_AreaTable->m_AreaName_lang);

        ImGui::End();
    }
}

void VisibleEnumatorWindow(bool* p_open)
{
    static CGPlayer_C* l_Player = nullptr;
    static bool l_ShowPlayerInfo = false;

    if (l_ShowPlayerInfo && l_Player)
        ShowPlayerInfo(&l_ShowPlayerInfo, l_Player);

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(850, 1600), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

    if (ImGui::Begin("Visible Unit Enumerator", p_open))
    {
        ImGui::Text("Visible Units : %u", sGodTool->GetPulse().size() ? sGodTool->GetPulse().size() : 0);
        ImGui::Separator();

        for (auto l_Itr : sGodTool->GetPulse())
        {
            if (ImGui::Selectable(l_Itr->GetObjectName()))
            {
                l_Player = l_Itr;
                l_ShowPlayerInfo = true;
            }
        }

        ImGui::End();
    }
}

void ShowSpellList(bool* p_open)
{
    static SpellRotator* s_SpellRotator = nullptr;
    static SpellRotator s_TempSpellRotator;
    static bool l_ShowRightSide = false;

    if (s_TempSpellRotator.DispelStruct.ShowDispelWindow)
        s_TempSpellRotator.DispelStruct.DrawDispelWindow(s_TempSpellRotator);

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_Always);
    if (ImGui::Begin("Local Spell List", p_open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::BeginChild("Spell List", ImVec2(180, 0), true);
        for (auto l_Itr : sGodTool->GetLocalSpells())
        {
            SpellRotator* l_SpellRotator = l_Itr;
            std::string l_SpellName = l_SpellRotator->SpellEntry.m_name_lang;

            /// Rank
            if (strlen(l_SpellRotator->SpellEntry.m_nameSubtext_lang) > 4)
                l_SpellName = l_SpellName + "(" + l_SpellRotator->SpellEntry.m_nameSubtext_lang + ")";

            if (ImGui::Selectable(l_SpellName.c_str()))
            {
                s_SpellRotator = l_Itr;
                l_ShowRightSide = true;
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        if (l_ShowRightSide)
        {
            if (s_TempSpellRotator.SpellEntry.m_ID != s_SpellRotator->SpellEntry.m_ID)
                s_TempSpellRotator = *s_SpellRotator;

            /// Right
            ImGui::BeginGroup();
            ImGui::BeginChild("Spell", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

            s_TempSpellRotator.Draw();

            ImGui::EndChild();
            if (ImGui::Button("Reset")) 
            {
                s_TempSpellRotator.Reset();
            }
            ImGui::SameLine();
            if (ImGui::Button("Save")) 
            {
                *s_SpellRotator = s_TempSpellRotator;
            }
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}

void ShowRotatorCreator(bool* p_open)
{
    static bool l_ShowSpellList = false;

    if (l_ShowSpellList)
        ShowSpellList(&l_ShowSpellList);

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Rotator Creator", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Creator"))
            {
                ImGui::MenuItem("Rotator", NULL, &l_ShowSpellList);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}