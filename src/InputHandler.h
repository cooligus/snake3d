#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void init(GLFWwindow* window);
	static void keyInputCallBack(GLFWwindow* window, int key, int scancode, int state, int mods);
};

