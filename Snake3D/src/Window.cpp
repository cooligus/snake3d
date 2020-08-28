#include "Window.h"
#include <iostream>

using namespace std;

Window::Window()
	:m_window(0), size_x(0), size_y(0)
{
}
Window::~Window()
{
	glfwTerminate();
}
void Window::enableBlending()
{
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
void Window::pollEvents()
{
	glCheck(glfwPollEvents());
}
void Window::swapBuffers()
{
	glCheck(glfwSwapBuffers(m_window));
}
void Window::clear(float red, float green, float blue)
{
	glCheck(glClearColor(red, green, blue, 1.0));
	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
bool Window::init(const unsigned int window_size_x, const unsigned int window_size_y, const char* window_name)
{
	if (!glfwInit())
	{
		cout << "Failed to initialile glfw\n";
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(window_size_x, window_size_y, window_name, nullptr, nullptr);

	if (m_window == nullptr)
	{
		cout << "Window doesn't exist\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialile glew\n";
		glfwTerminate();
		return false;
	}

	int size_x, size_y;

	glfwGetFramebufferSize(m_window, &size_x, &size_y);
	glViewport(0, 0, size_x, size_y);

	glEnable(GL_DEPTH_TEST);
	m_input_handler.init(m_window);

	return true;
}

bool Window::windowShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}
GLFWwindow* Window::getWindow()
{
	return m_window;
}
