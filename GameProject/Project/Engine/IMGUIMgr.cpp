#include "pch.h"
#include "IMGUIMgr.h"

#include"GameFramework.h"
#include "Device.h"


#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "UserStruct.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ResMgr.h"
#include "Texture.h"



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

    ImFont* mdFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
    
    // create descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 2;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (FAILED(DEVICE.Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pHeap))))
        assert(nullptr);

    // Setup Platform/Renderer backends
    HWND hWnd = CGameFramework::GetInst()->GetMainHwnd();
    m_tResloution = CGameFramework::GetInst()->Resolution();


    bool bSuccess = ImGui_ImplWin32_Init(hWnd);
    bSuccess = ImGui_ImplDX12_Init(DEVICE.Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM
        , m_pHeap, m_pHeap->GetCPUDescriptorHandleForHeapStart()
        , m_pHeap->GetGPUDescriptorHandleForHeapStart());
    m_pTexture = new CTexture;

    UINT handleIncrement = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    int descriptor_index = 1;
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
    cpu_handle.ptr += (handleIncrement * descriptor_index);
    GPU_Handle = m_pHeap->GetGPUDescriptorHandleForHeapStart();
    GPU_Handle.ptr += (handleIncrement * descriptor_index);

    m_pTexture->Load(L"Texture\\asd.png", cpu_handle);


}
void CIMGUIMgr::load_styles()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    {
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);  //윈도우 배경 색깔

        colors[ImGuiCol_FrameBg] = ImColor(11, 11, 11, 255);    //Frame 배경 색깔
        colors[ImGuiCol_FrameBgHovered] = ImColor(11, 11, 11, 255);

        colors[ImGuiCol_Button] = ImColor(46, 0, 255, UserInformations.button_opacity);                    //버튼 색깔 
        colors[ImGuiCol_ButtonActive] = ImColor(46, 40, 255, UserInformations.button_opacity);              //버튼 색깔 
        colors[ImGuiCol_ButtonHovered] = ImColor(46, 60, 255, UserInformations.button_opacity);             //버튼 색깔 


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
    if (SCENE_TYPE::INGAME == CSceneMgr::GetInst()->GetCurScene()->GetCurScene()) {
        ImGuiIO& io = ImGui::GetIO();
        
    }
}

void CIMGUIMgr::Progress()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    if (SCENE_TYPE::LOGIN == CSceneMgr::GetInst()->GetCurScene()->GetCurScene())
    {

        if (!doOnce)
        {
            load_styles();
            doOnce = true;
        }

        ImGui::SetNextWindowSize(ImVec2(m_tResloution.fWidth, m_tResloution.fHeight));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        bool bOpen = true;
        ImGui::Begin("Login Scene", &bOpen, iw.window_flags);
        {
            

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.054, 0.054, 0.054, 255));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.082, 0.078, 0.078, 255));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
            {
                ImGui::SetCursorPos(ImVec2(0, 0));
                ImGui::BeginChild("##MainPanel", ImVec2(m_tResloution.fWidth, m_tResloution.fHeight), true);
                {
                    if (show_login)
                    {

                        ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth / 2 - 170, m_tResloution.fHeight / 2 - 200));//35
                        ImGui::Text("Log into your account");

                        ImGui::PushItemWidth(260.f);
                        {
                            ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth / 2 - 225, m_tResloution.fHeight / 2 - 148));
                            ImGui::TextDisabled("Username");

                            ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth / 2 - 223, m_tResloution.fHeight / 2 - 128));//95
                            ImGui::InputText("##Username", UserInformations.m_cUserName, IM_ARRAYSIZE(UserInformations.m_cUserName));
                        }
                        ImGui::PopItemWidth();

                        ImGui::PushItemWidth(260.f);
                        {
                            ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth / 2 - 225, m_tResloution.fHeight / 2 - 77));//130
                            ImGui::TextDisabled("Password");


                            ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth / 2 - 223, m_tResloution.fHeight / 2 - 57));
                            //ImGui::SetCursorPos(ImVec2(20, 146));
                            ImGui::InputText("##Passowrd", UserInformations.m_cUserPassWord, IM_ARRAYSIZE(UserInformations.m_cUserPassWord));
                        }
                        ImGui::PopItemWidth();
                        ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth / 2 - 225, m_tResloution.fHeight / 2 + 10));//130
               //         ImGui::SetCursorPos(ImVec2(22, 190));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                        if (ImGui::Button("Login", ImVec2(260.f, 30.f)))
                        {
                            //로그인 버튼  클릭시


                        }
                        ImGui::PopStyleVar();
                        ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth / 2 - 225, m_tResloution.fHeight / 2 + 50));
                        //ImGui::SetCursorPos(ImVec2(22, 220));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                        if (ImGui::Button("Register", ImVec2(260.f, 30.f)))
                        {
                            m_bOpen = true;

                        }
                        if (m_bOpen) {
                            SUserInformation user;
                            ImGui::Begin("Register", &m_bOpen, ImGuiWindowFlags_AlwaysAutoResize);
                            //   ImGui::Separator();
                         
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
                                    ImGui::InputText("##Password", InitInformations.m_cUserPassWord, IM_ARRAYSIZE(InitInformations.m_cUserPassWord));
                                }

                                ImGui::PopItemWidth();
                                ImGui::SetCursorPos(ImVec2(22, 190));
                                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                                if (ImGui::Button("Register", ImVec2(260.f, 30.f)))
                                {
                                    m_bOpen = false;

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

            ImGui::SetCursorPos(ImVec2(m_tResloution.fWidth - 200, m_tResloution.fHeight - 100));
            ImGui::TextDisabled("Check Version 1.00");
        }


        ImGui::End();
    }
    else if (SCENE_TYPE::INGAME == CSceneMgr::GetInst()->GetCurScene()->GetCurScene()) {
        if (!doOnce)
        {
            load_styles();
            doOnce = true;
        }
        ImGui::SetNextWindowSize(ImVec2(m_tResloution.fWidth/3-200, m_tResloution.fHeight/3+10));
        ImGui::SetNextWindowPos(ImVec2(50, m_tResloution.fHeight/2+100));

        if (ImGui::Begin("Chat")) {
            ImGui::BeginChild("Chat", ImVec2(300, 300));
  // 채팅치면 위에 그려주는듯?          ImGui::Text(chatItems);
          //  Ptr<CTexture> Images = CResMgr::GetInst()->FindRes<CTexture>(L"TestTex");
            ImGui::Image((ImTextureID)GPU_Handle.ptr, ImVec2(m_pTexture->Width(), m_pTexture->Height()));
            ImGui::EndChild();
            ImGui::InputText("", buffer, sizeof(buffer));
            ImGui::SameLine();
            if (ImGui::Button("Send")) {

            }
        }
        ImGui::End();

    }


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
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}