#include "GodTool.h"
#include "GUI/Graphics.h"

typedef HRESULT(_stdcall * f_EndScene)(IDirect3DDevice9 * pDevice);
f_EndScene oEndScene;

static bool l_Initialize = false;

HRESULT _stdcall Hooked_EndScene(IDirect3DDevice9* p_Device)
{
    if (!l_Initialize)
    {
        sGraphics->InitializeGraphics(p_Device);
        sDatabaseMgr->InitializeTables();
        l_Initialize = true;
    }

    sGodTool->Update();
    sGraphics->Update();

    return oEndScene(p_Device);
}

DWORD WINAPI HookThread(LPVOID hModule)
{
    void * d3d9Device[119];
    if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
        oEndScene = (f_EndScene)DetourFunction((PBYTE)d3d9Device[42], (PBYTE)Hooked_EndScene);

    return false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(0, 0, HookThread, hModule, 0, 0);
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

