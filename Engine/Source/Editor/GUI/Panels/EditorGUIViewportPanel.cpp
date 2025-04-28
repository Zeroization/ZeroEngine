#include "EditorGUIViewportPanel.h"

#include "imgui.h"
#include "Core/GlobalDataManager.h"
#include "Function/Render/FrameBuffer/FBOManager.h"
#include "Function/Render/RHI/RHI.h"
#include "Function/Render/Texture/Texture.h"
#include "GUI/GUITexture/EditorGUITexManager.h"

namespace ZeroEngine
{
    EditorGUIViewportPanel::EditorGUIViewportPanel()
    {
        mPanelTagName = "视口##EditorGUIViewportPanel";
        mpIdentifier = std::make_unique<Identifier>(mPanelTagName);
    }

    void EditorGUIViewportPanel::ImGuiRender()
    {
        ImGui::Begin(mPanelTagName.c_str());
        mbIsWindowFocused = ImGui::IsWindowFocused();

        CheckManualResize();
        float yMax = ImGui::GetCursorPosY();

        if (ImGui::BeginTabBar("ViewSwitchBar##EditorGUIViewportPanel", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Game##EditorGUIViewportPanel"))
            {
                GlobalDataManager::GetInstance().GetGlobalDataRef()->IsGameView = true;
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Scene##EditorGUIViewportPanel"))
            {
                GlobalDataManager::GetInstance().GetGlobalDataRef()->IsGameView = false;
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        auto& texMgr = EditorGUITexManager::GetInstance();
        auto buffer = FBOManager::GetInstance().GetFBOByName(ZERO_BUILTIN_FBO_OUTPUT);
        // TODO: 完成整个渲染管线(包括后处理后使用)
        // uint32_t id = RHI::GetInstance().GetRenderBufferTexture(buffer->mColorAttachment0ID);
        // if (!texMgr.CheckExistenceByEngineID(id))
        // {
        //     texMgr.CreateTextureFromEngineID(id);
        // }

#ifdef ZERO_GRAPHIC_OPENGL
        // TODO: 完成整个渲染管线(包括后处理后使用)
        // ImGui::Image(texMgr.GetImTextureIDByEngineID(id),
        //              ImVec2(static_cast<float>(buffer->mWidth),
        //                     static_cast<float>(buffer->mHeight)),
        //              ImVec2(0, 1), ImVec2(1, 0));
#else
        // ImGui::Image(tex.GetID(), ImVec2(static_cast<float>(tex.mWidth), static_cast<float>(tex.mHeight))));
#endif

        ImGui::End();
    }

    void EditorGUIViewportPanel::OnEvent(Event& e)
    {
    }

    void EditorGUIViewportPanel::BeforePanelSizeChange()
    {
        mLastWidth = GlobalDataManager::GetInstance().GetGlobalDataRef()->engineCfg.mGameViewWidth;
        mLastHeight = GlobalDataManager::GetInstance().GetGlobalDataRef()->engineCfg.mGameViewHeight;
    }

    void EditorGUIViewportPanel::OnPanelSizeChange(const glm::vec2& size, uint8_t flags)
    {
        EditorGUIBasePanel::OnPanelSizeChange(size, flags);
    }

    void EditorGUIViewportPanel::AfterPanelSizeChange(const glm::vec2& size)
    {
        EditorGUIBasePanel::AfterPanelSizeChange(size);
    }
} // ZeroEngine
