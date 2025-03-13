#include "EditorGUIManager_OpenGLImpl.h"
#include "Function/Render/Window/WindowManager.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace ZeroEngine
{
	EditorGUIManager_OpenGLImpl::EditorGUIManager_OpenGLImpl()
	{
		// 初始化渲染后端
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(WindowManager::GetInstance()->GetWindowPtr()), true);
		ImGui_ImplOpenGL3_Init("#version 460"); // TODO: 读配置
	}

	EditorGUIManager_OpenGLImpl::~EditorGUIManager_OpenGLImpl()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorGUIManager_OpenGLImpl::BeforeGUIRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EditorGUIManager_OpenGLImpl::AfterGUIRender()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
} // ZeroEngine
