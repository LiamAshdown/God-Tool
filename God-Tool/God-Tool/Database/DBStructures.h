#pragma once

#include "../Common/SharedDefines.h"
#include "../Common/Types.h"

typedef struct
{
    DWORD m_ID = 0;
    DWORD m_category;
    DWORD m_dispelType;
    DWORD m_mechanic;
    DWORD m_attributes;
    DWORD m_attributesEx;
    DWORD m_attributesExB;
    DWORD m_attributesExC;
    DWORD m_attributesExD;
    DWORD m_attributesExE;
    DWORD m_attributesExF;
    DWORD m_attributesExG;
    DWORD m_shapeshiftMask[2];
    DWORD m_shapeshiftExclude[2];
    DWORD m_targets;
    DWORD m_targetCreatureType;
    DWORD m_requiresSpellFocus;
    DWORD m_facingCasterFlags;
    DWORD m_casterAuraState;
    DWORD m_targetAuraState;
    DWORD m_excludeCasterAuraState;
    DWORD m_excludeTargetAuraState;
    DWORD m_casterAuraSpell;
    DWORD m_targetAuraSpell;
    DWORD m_excludeCasterAuraSpell;
    DWORD m_excludeTargetAuraSpell;
    DWORD m_castingTimeIndex;
    DWORD m_recoveryTime;
    DWORD m_categoryRecoveryTime;
    DWORD m_interruptFlags;
    DWORD m_auraInterruptFlags;
    DWORD m_channelInterruptFlags;
    DWORD m_procTypeMask;
    DWORD m_procChance;
    DWORD m_procCharges;
    DWORD m_maxLevel;
    DWORD m_baseLevel;
    DWORD m_spellLevel;
    DWORD m_durationIndex;
    DWORD m_powerType;
    DWORD m_manaCost;
    DWORD m_manaCostPerLevel;
    DWORD m_manaPerSecond;
    DWORD m_manaPerSecondPerLevel;
    DWORD m_rangeIndex;
    float m_speed;
    DWORD m_modalNextSpell;
    DWORD m_cumulativeAura;
    DWORD m_totem[2];
    DWORD m_reagent[8];
    DWORD m_reagentCount[8];
    DWORD m_equippedItemClass;
    DWORD m_equippedItemSubclass;
    DWORD m_equippedItemInvTypes;
    DWORD m_effect[3];                      ///< Spell Effect Type      
    DWORD m_effectDieSides[3];
    float m_effectRealPointsPerLevel[3];
    DWORD m_effectBasePoints[3];
    DWORD m_effectMechanic[3];
    DWORD m_implicitTargetA[3];
    DWORD m_implicitTargetB[3];
    DWORD m_effectRadiusIndex[3];
    DWORD m_effectAura[3];                  ///< Spell Aura Type
    DWORD m_effectAuraPeriod[3];
    float m_effectAmplitude[3];
    DWORD m_effectChainTargets[3];
    DWORD m_effectItemType[3];
    DWORD m_effectMiscValue[3];
    DWORD m_effectMiscValueB[3];
    DWORD m_effectTriggerSpell[3];
    float m_effectPointsPerCombo[3];
    DWORD m_effectSpellClassMaskA[3];
    DWORD m_effectSpellClassMaskB[3];
    DWORD m_effectSpellClassMaskC[3];
    DWORD m_spellVisualID[2];
    DWORD m_spellIconID;
    DWORD m_activeIconID;
    DWORD m_spellPriority;
    char *m_name_lang;					
    char *m_nameSubtext_lang;
    char *m_description_lang;
    char *m_auraDescription_lang;
    DWORD m_manaCostPct;
    DWORD m_startRecoveryCategory;
    DWORD m_startRecoveryTime;
    DWORD m_maxTargetLevel;
    DWORD m_spellClassSet;
    DWORD m_spellClassMask[3];
    DWORD m_maxTargets;
    DWORD m_defenseType;
    DWORD m_preventionType;
    DWORD m_stanceBarOrder;
    float m_effectChainAmplitude[3];
    DWORD m_minFactionID;
    DWORD m_minReputation;
    DWORD m_requiredAuraVision;
    DWORD m_requiredTotemCategoryID[2];
    DWORD m_requiredAreasID;
    DWORD m_schoolMask;
    DWORD m_runeCostID;
    DWORD m_spellMissileID;
    DWORD m_powerDisplayID;
    float m_effectBonusCoefficient[3];
    DWORD m_descriptionVariablesID;
    DWORD m_difficulty;
}SpellEntryRec;

typedef struct
{
    DWORD       ID;
    char*       Directory;
    DWORD       InstanceType;
    DWORD       Flags;
    DWORD       PVP;
    char*       MapName_Lang[16];
    DWORD       AreaTableID;
    char*       HordeIntro[16];                                                       
    char*       AllianceIntro[16];                                                                            
    DWORD       LoadingScreenID;
    float       MinimapIconScale;
    DWORD       CorpseMapID;
    float       CorpseX;
    float       CorpseY;
    DWORD       TimeOfDayOverride;
    DWORD       ExpansionID;
    DWORD       RaidOffset;
    DWORD       MaxPlayers;                  
}MapEntry;

typedef struct
{
    uint64 CreatorGuid;
    uint32 SpellId;
    AuraFlags Flags;
    uint8 Level;
    uint16 StackCount;
    int Duration;
    int EndTime;
}AuraRec;

typedef struct
{
    DWORD m_ID;
    DWORD m_ContinentID;
    DWORD m_ParentAreaID;
    DWORD m_AreaBit;
    DWORD m_flags;
    DWORD m_SoundProviderPref;
    DWORD m_SoundProviderPrefUnderwater;
    DWORD m_AmbienceID;
    DWORD m_ZoneMusic;
    DWORD m_IntroSound;
    DWORD m_ExplorationLevel;
    char *m_AreaName_lang;
    DWORD m_factionGroupMask;
    DWORD m_liquidTypeID[4];
    float m_minElevation;
    float m_ambient_multiplier;
    DWORD m_lightid;
}AreaTableEntry; // 241

typedef struct WoWClientDBStruct
{
    void *funcTable;
    int isLoaded;
    int numRows;
    int maxIndex;
    int minIndex;
    int stringTable;
    void* funcTable2;
    int* FirstRow;
    int* Rows;
}WoWClientDBRec;

typedef struct WMOAreaTableStruct
{
    DWORD m_ID;
    DWORD m_WMOID;
    DWORD m_NameSetID;
    DWORD m_WMOGroupID;
    DWORD m_SoundProviderPref;
    DWORD m_SoundProviderPrefUnderwater;
    DWORD m_AmbienceID;
    DWORD m_ZoneMusic;
    DWORD m_IntroSound;
    DWORD m_flags;
    DWORD m_AreaTableID;
    char *m_AreaName_lang;
}WMOAreaTableRec;