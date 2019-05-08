#pragma once
#include "SharedDefines.h"

#define ClntObjMgrGetActivePlayerGuidAddr 0x004D3790
#define GetObjectPtrAddr 0x004D4DB0
#define EnumVisibleObjectsAddr 0x004D4B30
#define ReadChatAddr 0x00966580
#define AddChatAddr 0x00509DD0
#define IsInRaidOrPartyAddr 0x0071C570
#define GetCoinageAddr 0x0051A2B0
#define IntersectAddr 0x0077F310
#define ProcessPacketsAddr 0x00631FE0

#define GetAuraCountAddr 0x004F8850
#define GetSpellAuraId 0x004F8870
#define HasAuraBySpellIdAddr 0x007282A0
#define GetAuraAddr 0x00556E10

#define WindowProcAddr 0x006A0360

#define ConsoleWriteAddr 0x00765360
#define ConsoleRegisterCommandAddr 0x00769100
#define ConsoleUnregisterCommandAddr 0x007689E0

#define SIZE_OF_CODE_ADDR 0x00D415B8
#define ENABLE_CONSOLE_ADDR 0x00CABCC4

enum TypeMask
{
    TYPEMASK_OBJECT             = 0x1,
    TYPEMASK_ITEM               = 0x2,
    TYPEMASK_CONTAINER          = 0x4,
    TYPEMASK_UNIT               = 0x8,
    TYPEMASK_PLAYER             = 0x10,
    TYPEMASK_GAMEOBJECT         = 0x20,
    TYPEMASK_DYNAMICOBJECT      = 0x40,
    TYPEMASK_CORPSE             = 0x80,
};

enum ObjMgrOffset : uint32
{
	ObjectMgrOffset				= 0x2ED0,   // 3.3.5
	LocalPlayerOffset			= 0xC0,		// 3.3.5
	ClntObjectGetActivePlr      = 0x2C,	    // 3.3.5
	FirstObjectOffset			= 0xAC,		// 3.3.5
	NextOjbectOffset			= 0x3C,		// 3.3.5
	UnitFieldAddressOffset      = 0x8,		// 3.3.5
	XposOffset                  = 0x798,    // 3.3.5
	YposOffset                  = 0x79C,    // 3.3.5
	ZposOffset                  = 0x7A0,	// 3.3.5	
	OposOffset                  = 0x7A8,    // 3.3.5
	CurrentHealthOffset         = 0x18 * 4, // 3.3.5
	CurrentManaOffset           = 0x19 * 4,	// 3.3.5	
	MaxHealthOffset             = 0x20 * 4,	// 3.3.5
	MaxManaOffset               = 0x5C,		// 3.3.5
	LevelOffset                 = 0x36 * 4,	// 3.3.5
	TypeOffset                  = 0x14		// 3.3.5
};

inline int ClientConnection()
{
    return *(int*)0x00C79CE0;
}

inline int ObjManager()
{
    return ClientConnection() ? *(int*)(ClientConnection() + ObjectMgrOffset) : 0;
}

inline DWORD GetLocalPlayerGuid()
{
    if (ObjManager())
        return *(uint64*)(ObjManager() + LocalPlayerOffset);
    return 0;
}

inline DWORD GetAddrByGUID()
{
    return ((int(__cdecl*)(DWORD))0x004D4DB0)(GetLocalPlayerGuid());
}

inline int PerformanceCount() 
{ 
    return ((int(__cdecl*)())0x0086AE20)(); 
}

inline int GetVirtualFuncAddr(int addr, int offset) 
{ 
    return addr ? *(int*)(*(int*)addr + 4 * offset) : 0;
}