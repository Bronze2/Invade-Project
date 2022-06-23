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
#include "KeyMgr.h"
#include <string.h>
#include <stdint.h>
 int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
  
    return CIMGUIMgr::GetInst()->TextEditCallback(data);
}
int CIMGUIMgr::TextEditCallback(ImGuiInputTextCallbackData* data)
{
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackCompletion:
    {
        // Example of TEXT COMPLETION

        // Locate beginning of current word
        const char* word_end = data->Buf + data->CursorPos;
        const char* word_start = word_end;
        while (word_start > data->Buf)
        {
            const char c = word_start[-1];
            if (c == ' ' || c == '\t' || c == ',' || c == ';')
                break;
            word_start--;
        }

        // Build a list of candidates
        ImVector<const char*> candidates;
        for (int i = 0; i < Commands.Size; i++)
            if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                candidates.push_back(Commands[i]);

        if (candidates.Size == 0)
        {
            // No match
            AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
        }
        else if (candidates.Size == 1)
        {
            // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
            data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
            data->InsertChars(data->CursorPos, candidates[0]);
            data->InsertChars(data->CursorPos, " ");
        }
        else
        {
            // Multiple matches. Complete as much as we can..
            // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
            int match_len = (int)(word_end - word_start);
            for (;;)
            {
                int c = 0;
                bool all_candidates_matches = true;
                for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                    if (i == 0)
                        c = toupper(candidates[i][match_len]);
                    else if (c == 0 || c != toupper(candidates[i][match_len]))
                        all_candidates_matches = false;
                if (!all_candidates_matches)
                    break;
                match_len++;
            }

            if (match_len > 0)
            {
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
            }

            // List matches
            AddLog("Possible matches:\n");
            for (int i = 0; i < candidates.Size; i++)
                AddLog("- %s\n", candidates[i]);
        }

        break;
    }
    case ImGuiInputTextFlags_CallbackHistory:
    {
        // Example of HISTORY
        const int prev_history_pos = HistoryPos;
        if (data->EventKey == ImGuiKey_UpArrow)
        {
            if (HistoryPos == -1)
                HistoryPos = History.Size - 1;
            else if (HistoryPos > 0)
                HistoryPos--;
        }
        else if (data->EventKey == ImGuiKey_DownArrow)
        {
            if (HistoryPos != -1)
                if (++HistoryPos >= History.Size)
                    HistoryPos = -1;
        }

        // A better implementation would preserve the data on the current input line along with cursor position.
        if (prev_history_pos != HistoryPos)
        {
            const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, history_str);
        }
    }
    }
    return 0;
}

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

    ImFont* mdFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\NaviUpdaterKia-Regular.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
    
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

    m_pTexture->Load(L"Texture\\BackGround.png", cpu_handle);

    // 로그인 씬 택스처
    // 


}
void CIMGUIMgr::load_styles()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    {
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f,0.50f);  //윈도우 배경 색깔

        colors[ImGuiCol_FrameBg] = ImColor(11, 11, 11, 123);    //Frame 배경 색깔
        colors[ImGuiCol_FrameBgHovered] = ImColor(11, 11, 11, 123);

        colors[ImGuiCol_Button] = ImColor(46, 0, 255, UserInformations.button_opacity);                    //버튼 색깔 
        colors[ImGuiCol_ButtonActive] = ImColor(46, 40, 255, UserInformations.button_opacity);              //버튼 색깔 
        colors[ImGuiCol_ButtonHovered] = ImColor(46, 60, 255, UserInformations.button_opacity);             //버튼 색깔 


        //    colors[ImGuiCol_Button] = ImColor(255, 0, 46, SUserInformaions.button_opacity);
        //    colors[ImGuiCol_ButtonActive] = ImColor(255, 0, 46, SUserInformaions.button_opacity);
        //    colors[ImGuiCol_ButtonHovered] = ImColor(255, 0, 46, SUserInformaions.button_opacity);
        colors[ImGuiCol_TextDisabled] = ImVec4(1.f, 1.f, 1.f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.f, 0.f, 0.f, 1.00f);
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
    if (SCENE_TYPE::LOGIN
        == CSceneMgr::GetInst()->GetCurScene()->GetCurScene())
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
                    ImGui::Image((ImTextureID)GPU_Handle.ptr, ImVec2(m_pTexture->Width(), m_pTexture->Height()));
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
                            //등록 버튼 클릭시

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
        bool bOpen = true;
        if (ImGui::Begin("Chat", &bOpen, iw.window_flags)) {
            ImGui::BeginChild("Chat", ImVec2(300, 300));
  // 채팅치면 위에 그려주는듯?          ImGui::Text(chatItems);
          //  Ptr<CTexture> Images = CResMgr::GetInst()->FindRes<CTexture>(L"TestTex");
           // ImGui::Image((ImTextureID)GPU_Handle.ptr, ImVec2(m_pTexture->Width(), m_pTexture->Height()));


            ImGui::EndChild();
            bool reclaim_focus = false;
            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
            if (KEY_TAB(KEY_TYPE::KEY_ENTER)) {
              
                if (!bChat) {
                    if (!ImGui::IsAnyItemActive())
                        ImGui::SetKeyboardFocusHere(0);
                    bChat = true;
                  
                }
                else {
                    bChat = false;
                    ImGui::SetItemDefaultFocus();
                  
                    ImGui::SetKeyboardFocusHere(-1);
                }
               
            }

            if (bChat) {
                if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer)))
                {
                
                }
           //     ImGui::SetKeyboardFocusHere(-1);
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
    delete m_pTexture;
    m_pTexture = nullptr;
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

}