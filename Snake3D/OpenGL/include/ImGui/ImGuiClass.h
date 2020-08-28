#pragma once
#include <GL/glfw3.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui.cpp"
#include "imgui_demo.cpp"
#include "imgui_draw.cpp"
#include "imgui_impl_glfw.cpp"
#include "imgui_impl_opengl3.cpp"
#include "imgui_widgets.cpp"

class ImGuiClass
{
public:
	//constructor and destructor

	ImGuiClass(GLFWwindow* window = nullptr)
	{
		if (window != nullptr)createContext(window);
	}
	~ImGuiClass()
	{
		if (!m_destoyed)destroyContext();
	}

	//create and destroy context

	void createContext(GLFWwindow* window) 
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();
		ImGui_ImplOpenGL3_Init("#version 130");
		m_destoyed = false;
	}
	void destroyContext()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		m_destoyed = true;
	}

	//rendering functions

	void newFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	//helping functions

	void createFPScounter()
	{
		ImGui::Begin("FPS");
		ImGui::Text("%.1f", ImGui::GetIO().Framerate);
		ImGui::End();
	}

	

private:
	bool m_destoyed = true;
};

