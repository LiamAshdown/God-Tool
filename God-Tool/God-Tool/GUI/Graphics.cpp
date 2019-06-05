#include "Graphics.h"
#include "../GodTool.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT m_Msg, WPARAM wParam, LPARAM lParam);

Graphics * Graphics::instance()
{
    static Graphics instance;
    return &instance;
}

Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

void Graphics::InitializeGraphics(IDirect3DDevice9* p_Device)
{
    m_Device = p_Device;

    m_ImGUIHwnd = FindWindowA(NULL, "World of Warcraft");
    m_HWNDOriginal = (WNDPROC)GetWindowLongPtr(m_ImGUIHwnd, GWL_WNDPROC);
    SetWindowLongPtr(m_ImGUIHwnd, GWL_WNDPROC, (LONG_PTR)&WndProc);

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, (WNDPROC)m_ImGUIHwnd, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);

    ::ShowWindow(m_ImGUIHwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_ImGUIHwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(m_ImGUIHwnd);
    ImGui_ImplDX9_Init(m_Device);

    m_ShowChatLog = false;
    m_ShowVisibleEnumatorWindow = false;
    m_MenuOpen = true;
    m_ShowRotatorCreator = false;
    m_WindowFlags |= ImGuiWindowFlags_MenuBar;
}

void Graphics::Update()
{
    DrawGUI();
}

V3Position Graphics::Forward()
{
    V3Position l_Res;

    if (GetActiveCamera())
    {
        int l_FunctionAddress = GetVirtualFuncAddr((int)GetActiveCamera(), 1);
        ((V3Position*(__thiscall*)(int, V3Position*))(l_FunctionAddress))((int)GetActiveCamera(), &l_Res);
    }
    return l_Res;
}

void Graphics::ProjectionMatrix(D3DXMATRIX* p_Projector)
{
    CGCamera* l_Camera = GetActiveCamera();
    D3DXMatrixPerspectiveFovRH(p_Projector, l_Camera->FieldOfView * 0.6f, l_Camera->Aspect, l_Camera->NearPlane, l_Camera->FarPlane);
}

void Graphics::ViewMatrix(D3DXMATRIX* p_View)
{
    CGCamera* l_Camera = GetActiveCamera();
    D3DXVECTOR3 vEyePt(l_Camera->Position.x, l_Camera->Position.y, l_Camera->Position.z);
    D3DXVECTOR3 vLookatPt(l_Camera->Position.x + Forward().x, l_Camera->Position.y + Forward().y, l_Camera->Position.z + Forward().z);
    D3DXVECTOR3 vUpVec(0.0f, 0.0f, 1.0f);
    D3DXMatrixLookAtRH(p_View, &vEyePt, &vLookatPt, &vUpVec);
}

bool Graphics::WorldToScreen(D3DXVECTOR3& p_From, D3DXVECTOR3& p_To)
{
    m_Device->GetViewport(&m_ViewPort);

    D3DXMATRIX Proj, View, World;
    ProjectionMatrix(&Proj);
    ViewMatrix(&View);
    D3DXMatrixIdentity(&World);

    D3DXVec3Project(&p_To, &p_From, &m_ViewPort, &Proj, &View, &World);

    return (p_To.z < 1.f);
}

void Graphics::Render(D3DXVECTOR3& p_From)
{
    m_Device->GetViewport(&m_ViewPort);

    D3DXMATRIX Proj, View, World;
    D3DXMatrixIdentity(&World);
    D3DXMatrixTranslation(&World, p_From.x, p_From.y, p_From.z);
    ProjectionMatrix(&Proj);
    ViewMatrix(&View);

    m_Device->SetTransform(D3DTS_WORLD, &World);
    m_Device->SetTransform(D3DTS_VIEW, &View);
    m_Device->SetTransform(D3DTS_PROJECTION, &Proj);
}

void Graphics::DrawGUI()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("GodTool", false, m_WindowFlags);

    if (m_ShowChatLog)
        ConsoleChatLog();

    if (m_ShowVisibleEnumatorWindow)
        VisibleEnumatorWindow(&m_ShowVisibleEnumatorWindow);

    if (m_ShowRotatorCreator)
        ShowRotatorCreator(&m_ShowRotatorCreator);

    bool test = true;
    ImGui::ShowDemoWindow(&test);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Chat History", NULL, &m_ShowChatLog);
            ImGui::MenuItem("Visible Players", NULL, &m_ShowVisibleEnumatorWindow);
            ImGui::MenuItem("Rotation Creator", NULL, &m_ShowRotatorCreator);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Graphics::ConsoleChatLog()
{
    while (!m_ConsoleChatLog.ChatLog.empty())
    {
        m_ConsoleChatLog.AddLog(m_ConsoleChatLog.ChatLog.front().c_str());
        m_ConsoleChatLog.ChatLog.pop_front();
    }
    m_ConsoleChatLog.Draw("Chat History");
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT m_Msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Graphics::WndProc(HWND p_Hwnd, UINT p_Msg, WPARAM p_WParam, LPARAM p_LParam)
{
    if (p_Msg == WM_KEYDOWN)
    {
        if (p_WParam == VK_INSERT)
        {
            if (sGraphics->IsMenuOpen())
                sGraphics->SetMenuOpen(false);
            else
                sGraphics->SetMenuOpen(true);
        }
    }

    if (sGraphics->IsMenuOpen())
    {
        ImGui_ImplWin32_WndProcHandler(p_Hwnd, p_Msg, p_WParam, p_LParam);
        return true;
    }
    else
        return CallWindowProc(sGraphics->m_HWNDOriginal, p_Hwnd, p_Msg, p_WParam, p_LParam);
}