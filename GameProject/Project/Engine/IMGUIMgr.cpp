#include "pch.h"
#include "IMGUIMgr.h"
#include <Engine/GameFramework.h>
#include <Engine/Device.h>

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "UserStruct.h"


class initWindow {
public:
    const char* window_title = "Loader";
    ImVec2 window_size{ 740, 460 };

    DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
} iw;
void CIMGUIMgr::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags != ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    // create descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (FAILED(DEVICE.Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pHeap))))
        assert(nullptr);

    // Setup Platform/Renderer backends
    HWND hWnd = CGameFramework::GetInst()->GetMainHwnd();

    bool bSuccess = ImGui_ImplWin32_Init(hWnd);
    bSuccess = ImGui_ImplDX12_Init(DEVICE.Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM
        , m_pHeap, m_pHeap->GetCPUDescriptorHandleForHeapStart()
        , m_pHeap->GetGPUDescriptorHandleForHeapStart());
}
void CIMGUIMgr::load_styles()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    {
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);

        colors[ImGuiCol_FrameBg] = ImColor(11, 11, 11, 255);
        colors[ImGuiCol_FrameBgHovered] = ImColor(11, 11, 11, 255);

        colors[ImGuiCol_Button] = ImColor(255, 0, 46, UserInformations.button_opacity);
        colors[ImGuiCol_ButtonActive] = ImColor(255, 0, 46, UserInformations.button_opacity);
        colors[ImGuiCol_ButtonHovered] = ImColor(255, 0, 46, UserInformations.button_opacity);


        //    colors[ImGuiCol_Button] = ImColor(255, 0, 46, SUserInformaions.button_opacity);
        //    colors[ImGuiCol_ButtonActive] = ImColor(255, 0, 46, SUserInformaions.button_opacity);
        //    colors[ImGuiCol_ButtonHovered] = ImColor(255, 0, 46, SUserInformaions.button_opacity);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
    }

    ImGuiStyle* style = &ImGui::GetStyle();
    {
        style->WindowPadding = ImVec2(4, 4);
        style->WindowBorderSize = 0.f;

        style->FramePadding = ImVec2(8, 6);
        style->FrameRounding = 3.f;
        style->FrameBorderSize = 1.f;
    }
}

void CIMGUIMgr::Progress()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();



    if (!doOnce)
    {
        load_styles();
        doOnce = true;
    }

    ImGui::SetNextWindowSize(ImVec2(1920, 1080));
    bool bOpen = true;
    ImGui::Begin("Login Scene", &bOpen, iw.window_flags);
    {


        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.054, 0.054, 0.054, 255));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.082, 0.078, 0.078, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
        {
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::BeginChild("##MainPanel", ImVec2(600, 276), true);
            {
                if (show_login)
                {
                    ImGui::SetCursorPos(ImVec2(118, 20));
                    ImGui::TextDisabled("Welcome Back");

                    ImGui::SetCursorPos(ImVec2(97, 35));
                    ImGui::Text("Log into your account");

                    ImGui::PushItemWidth(260.f);
                    {
                        ImGui::SetCursorPos(ImVec2(22, 79));
                        ImGui::TextDisabled("Username");

                        ImGui::SetCursorPos(ImVec2(20, 95));
                        ImGui::InputText("##Username", UserInformations.m_cUserName, IM_ARRAYSIZE(UserInformations.m_cUserName));
                    }
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(260.f);
                    {
                        ImGui::SetCursorPos(ImVec2(22, 130));
                        ImGui::TextDisabled("Password");

                        ImGui::SetCursorPos(ImVec2(188, 130));
                        ImGui::TextDisabled("Forgot password?");

                        ImGui::SetCursorPos(ImVec2(20, 146));
                        ImGui::InputText("##Passowrd", UserInformations.m_cUserPassWord, IM_ARRAYSIZE(UserInformations.m_cUserPassWord));
                    }
                    ImGui::PopItemWidth();

                    ImGui::SetCursorPos(ImVec2(22, 190));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                    if (ImGui::Button("Login", ImVec2(260.f, 30.f)))
                    {
                        //로그인 버튼  클릭시


                    }
                    ImGui::PopStyleVar();
                    ImGui::SetCursorPos(ImVec2(22, 220));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                    if (ImGui::Button("Register", ImVec2(260.f, 30.f)))
                    {
                        m_bOpen = true;

                    }
                    if (m_bOpen) {
                        SUserInformation user;
                        ImGui::Begin("Register", &m_bOpen, ImGuiWindowFlags_AlwaysAutoResize);
                        ImGui::Separator();
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.054, 0.054, 0.054, 255));
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.082, 0.078, 0.078, 255));
                        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);

                        {

                            ImGui::SetCursorPos(ImVec2(0, 0));
                            ImGui::BeginChild("##ChildPanel", ImVec2(600, 276), true);
                            ImGui::PushItemWidth(260.f);
                            {
                                ImGui::SetCursorPos(ImVec2(22, 79));
                                ImGui::TextDisabled("Set Your Username");

                                ImGui::SetCursorPos(ImVec2(20, 95));
                                ImGui::InputText("##Username", InitInformations.m_cUserName, IM_ARRAYSIZE(InitInformations.m_cUserName));
                            }
                            ImGui::PopItemWidth();
                            ImGui::PushItemWidth(260.f);
                            {
                                ImGui::SetCursorPos(ImVec2(22, 130));
                                ImGui::TextDisabled("Set Your Password");


                                ImGui::SetCursorPos(ImVec2(20, 146));
                                ImGui::InputText("##Passowrd", InitInformations.m_cUserPassWord, IM_ARRAYSIZE(InitInformations.m_cUserPassWord));
                            }

                            ImGui::PopItemWidth();
                            ImGui::SetCursorPos(ImVec2(22, 190));
                            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                            if (ImGui::Button("Register", ImVec2(260.f, 30.f)))
                            {
                                InitInformations;
                              
                            }
                            ImGui::PopStyleVar();

                            ImGui::EndChild();
                        }
                        ImGui::PopStyleColor(2);
                        ImGui::PopStyleVar(1);

                        ImGui::End();
                    }



                    ImGui::PopStyleVar();

                }


            }
            ImGui::EndChild();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(1);

        ImGui::SetCursorPos(ImVec2(5, 445));
        ImGui::TextDisabled("Loader base, made with <3 by booget#0001");
    }
   
    
    ImGui::End();

    CMDLIST->SetDescriptorHeaps(1, &m_pHeap);
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CMDLIST.Get());


    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
CIMGUIMgr::CIMGUIMgr() {

}

CIMGUIMgr::~CIMGUIMgr() {
	m_pHeap->Release();

}