#pragma once
#include "../Common/SharedDefines.h"

class Detour
{
public:
    Detour(int p_HookAddress, int m_HookFunction) : m_HookAddress{ (byte*)p_HookAddress }
        , m_HookFunction(m_HookFunction)
    {
        m_OriginalBytes.resize(6);
        CopyOriginal();
    }

    void Hook()
    {
        VirtualProtect(m_HookAddress, 6, PAGE_EXECUTE_READWRITE, &m_OldProtection);

        // Set the current bytes to NOP (no operation)
        memset(m_HookAddress, 0x90, 6);

        DWORD offSetAddr = ((DWORD)m_HookFunction - (DWORD)m_HookAddress) - 5; // Minus 5 as that's our jump instruction

        *(BYTE*)m_HookAddress = 0xE9; // jump instruction
        *(DWORD*)((DWORD)m_HookAddress + 1) = offSetAddr;

        // How it looks in asm
        // jmp [EndSceneDetour]

        VirtualProtect(m_HookAddress, 6, m_OldProtection, (DWORD*)0);
    }

    void Restore()
    {
        VirtualProtect(m_HookAddress, 6, PAGE_EXECUTE_READWRITE, &m_OldProtection);
        memcpy(m_HookAddress, m_OriginalBytes.data(), 6);
        VirtualProtect(m_HookAddress, 6, m_OldProtection, 0);
    }

    void Apply()
    {
        Hook();
    }

    void CopyOriginal()
    {
        VirtualProtect(m_OriginalBytes.data(), 6, PAGE_EXECUTE_READWRITE, &m_OldProtection);
        memcpy(m_OriginalBytes.data(), m_HookAddress, 6);
        VirtualProtect(m_OriginalBytes.data(), 6, m_OldProtection, 0);
    }

public:
    int m_HookFunction;
    std::vector<byte> m_OriginalBytes{};
    byte* m_HookAddress{};
    DWORD m_OldProtection;
};