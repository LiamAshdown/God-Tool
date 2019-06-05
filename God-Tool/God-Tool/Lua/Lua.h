#pragma once

#include "../Common/SharedDefines.h"

inline int FramescriptExecute(const char* text)
{
    return ((int(__cdecl*)(const char*, const char*, void*))0x00819210)(text, "GodTool.lua", nullptr);
}

inline auto FramescriptRegister(const char* command, const int func_pointer)
{
    return ((bool(__cdecl*)(const char*, int))0x00817F90)(command, func_pointer);
}

inline auto FramescriptUnregister(const char* command)
{
    return ((bool(__cdecl*)(const char*))0x00817FD0)(command);
}

inline auto FramescriptIsString(int state, int n_param)
{
    return ((bool(__cdecl*)(int, int))0x00817FD0)(state, n_param);
}

inline auto FramescriptToLstring(int state, int n_param, int zero = 0)
{
    return ((char*(__cdecl*)(int, int, int))0x0084E0E0)(state, n_param, zero);
}

inline auto FrameScriptPushNumber(int state, double number)
{
    return ((int(__cdecl*)(int, double))0x0084E2A0)(state, number);
}

inline auto FrameScriptPushNil(int state)
{
    return ((int(__cdecl*)(int))0x0084E280)(state);
}

inline auto FrameScriptDisplayError(int state, char* err, ...)
{
    return ((int(__cdecl*)(int, char*, ...))0x0084F280)(state, err);
}

inline auto GetPtrFromUnitId(const char* unit_id)
{
    return ((int(__cdecl*)(const char*))0x0060C1F0)(unit_id);
}