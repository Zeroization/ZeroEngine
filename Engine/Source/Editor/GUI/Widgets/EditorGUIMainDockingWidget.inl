#pragma once

namespace ZeroEngine
{
    static bool bIsRedoEnableForMainDockingWidget = false;
    ZERO_EXPLICIT_STATIC void MainDockingWidget()
    {
        // 主菜单栏================================================
        if (ImGui::BeginMainMenuBar())
        {
            // 文件 File==========================================
            if (ImGui::BeginMenu("文件##MainDockingWidget"))
            {
                if (ImGui::MenuItem("TODO1##MainDockingWidget"))
                {
                }
                ImGui::EndMenu();
            }

            // 编辑 Edit==========================================
            if (ImGui::BeginMenu("编辑##MainDockingWidget"))
            {
                ImGui::SeparatorText("历史操作##MainDockingWidget");
                // TODO inputmanger里绑定输入快捷键
                if (ImGui::MenuItem("撤销##MainDockingWidget", "CTRL+Z"))
                {
                    LOG_DEBUG("UNDO");
                }
                if (ImGui::MenuItem("重做##MainDockingWidget", "CTRL+Y", false, bIsRedoEnableForMainDockingWidget))
                {
                } // Disabled item
                ImGui::SeparatorText("编辑##MainDockingWidget");
                if (ImGui::MenuItem("剪贴##MainDockingWidget", "CTRL+X"))
                {
                }
                if (ImGui::MenuItem("复制##MainDockingWidget", "CTRL+C"))
                {
                }
                if (ImGui::MenuItem("粘贴##MainDockingWidget", "CTRL+V"))
                {
                }
                if (ImGui::MenuItem("创建副本##MainDockingWidget", "CTRL+D"))
                {
                }
                if (ImGui::MenuItem("删除##MainDockingWidget", "DELETE"))
                {
                }
                ImGui::SeparatorText("配置##MainDockingWidget");
                if (ImGui::MenuItem("编辑器设置##MainDockingWidget"))
                {
                }
                if (ImGui::MenuItem("项目设置##MainDockingWidget"))
                {
                }
                ImGui::EndMenu();
            }

            // 窗口 Window========================================
            if (ImGui::BeginMenu("窗口##MainDockingWidget"))
            {
                if (ImGui::MenuItem("TODO2##MainDockingWidget"))
                {
                }
                ImGui::EndMenu();
            }

            // 工具 Tools========================================
            if (ImGui::BeginMenu("工具##MainDockingWidget"))
            {
                if (ImGui::MenuItem("TODO3##MainDockingWidget"))
                {
                }
                ImGui::EndMenu();
            }

            // 帮助 Window========================================
            if (ImGui::BeginMenu("帮助##MainDockingWidget"))
            {
                if (ImGui::MenuItem("TODO4##MainDockingWidget"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    }
}
