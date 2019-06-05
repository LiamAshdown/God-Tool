#pragma once
#include "../Common/SharedDefines.h"
#include "../Common/Offsets.h"
#include "DearGUI/imgui_structures.h"

struct V3Position : D3DXVECTOR3
{
    V3Position() : D3DXVECTOR3(0, 0, 0) {}

    V3Position(D3DXVECTOR3& l_V) :D3DXVECTOR3(l_V) {}

    V3Position(float l_X, float l_Y, float l_Z) :D3DXVECTOR3(l_X, l_Y, l_Z) {}

    bool Invalid() const
    {
        return (!x && !y && !z);
    }

    float DistanceTo(const V3Position& p_Position) const
    {
        return sqrt(pow(x - p_Position.x, 2) + pow(y - p_Position.y, 2) + pow(z - p_Position.z, 2));
    }

    float Distance2D(const V3Position& p_Position) const
    {
        return sqrt(pow(x - p_Position.x, 2) + pow(y - p_Position.y, 2));
    }

    float Length() const
    {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    float Angle() const
    {
        return atan2(y, x);
    }

};

struct CGCamera
{
    int vtable;
    int field2;
    V3Position Position;
    float fViewMat[9];
    float NearPlane;
    float FarPlane;
    float FieldOfView;
    float Aspect;
};

class Graphics
{
public:
    static Graphics* instance();

public:
    Graphics();
    ~Graphics();

public:
    void InitializeGraphics(IDirect3DDevice9* p_Device);
    void Update();

public:
    /// IMGUI
    void DrawGUI();

public:
    void ProjectionMatrix(D3DXMATRIX *p_Projector);
    void ViewMatrix(D3DXMATRIX* p_View);
    bool WorldToScreen(D3DXVECTOR3& p_From, D3DXVECTOR3& p_To);
    void Render(D3DXVECTOR3& p_From);
    V3Position Forward();

public:
    ConsolePacketLog m_ConsoleChatLog;

private:
    void ConsoleChatLog();

private:
    static LRESULT __stdcall WndProc(HWND p_Hwnd, UINT p_Msg, WPARAM p_WParam, LPARAM p_LParam);
    CGCamera* __thiscall GetActiveCamera() { return ((CGCamera*(__thiscall*)())0x004F5960)(); }

    bool IsMenuOpen() const { return m_MenuOpen; }
    void SetMenuOpen(bool const p_Visible) { m_MenuOpen = p_Visible; }

private:
    HWND m_ImGUIHwnd;
    WNDPROC m_HWNDOriginal;
    IDirect3DDevice9* m_Device;
    D3DVIEWPORT9 m_ViewPort;

private:
    ImGuiWindowFlags m_WindowFlags;
    bool m_ShowChatLog;
    bool m_ShowVisibleEnumatorWindow;
    bool m_ShowRotatorCreator;
    bool m_MenuOpen;
};

#define sGraphics Graphics::instance()
