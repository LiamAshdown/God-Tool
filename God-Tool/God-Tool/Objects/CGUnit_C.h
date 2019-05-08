#pragma once
#include "CGObject_C.h"
#include "Aura/Aura.h"

class CGUnit_C : public CGObject_C
{
public:
    CGUnit_C() {}

    ~CGUnit_C() {}

    int GetHealth() const { return GetValue<uint64>(UNIT_FIELD_HEALTH); }
    int GetMaxHealth() const { return GetValue<uint64>(UNIT_FIELD_MAXHEALTH); }
    int GetMana() const { return GetValue<uint64>(UNIT_FIELD_BASE_MANA); }
    int GetLevel() const { return GetValue<uint64>(UNIT_FIELD_LEVEL); }
    uint64 GetGUID() const { return GetValue<uint64>(OBJECT_FIELD_GUID); }

    /// Auras
    int GetAuraCount() { return ((int(__thiscall*)(DWORD p_BaseAddress))GetAuraCountAddr)((DWORD)this); }
    int GetAuraSpellId(int p_SpellId) { return ((int(__thiscall*)(DWORD p_BaseAddress, int p_SpellId))GetSpellAuraId)((DWORD)this, p_SpellId); }
    bool HasAuraBySpellId(int p_SpellId) { return ((int(__thiscall*)(DWORD p_BaseAddress, int p_SpellId))HasAuraBySpellIdAddr)((DWORD)this, p_SpellId); }
    int GetAura(int p_SpellId) { return ((int(__thiscall*)(DWORD p_BaseAddress, int p_SpellId))GetAuraAddr)((DWORD)this, p_SpellId); }

    void GetAuras(std::vector<Aura>& p_Auras)
    {
        p_Auras.clear();
        int l_Counter = 0;
        if (GetAuraCount())
        {
            do
            {
                int l_SpellId = GetAuraSpellId(l_Counter);

                if (l_SpellId)
                {
                    Aura l_Aura(*reinterpret_cast<AuraRec*>(GetAura(l_Counter)));
                    p_Auras.push_back(l_Aura);
                }

                l_Counter++;

            } while (l_Counter < GetAuraCount());
        }
    }
};