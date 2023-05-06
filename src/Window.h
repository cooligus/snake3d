#pragma once
#include "InputHandler.h"

class Window
{
public:
	Window();
	~Window();
	void enableBlending();
	void pollEvents();
	void swapBuffers();
	void clear(float red = 0.1, float green = 0.1, float blue = 0.1);
	bool init(const unsigned int window_size_x, const unsigned int window_size_y, const char *window_name);
	bool windowShouldClose()const;
	GLFWwindow* getWindow();
	
private:

	GLFWwindow *m_window;
	InputHandler m_input_handler;
	int size_x, size_y;
};