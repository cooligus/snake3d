#include "InputHandler.h"
#include "something.h"


InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyInputCallBack);
}

void InputHandler::keyInputCallBack(GLFWwindow* window, int key, int scancode, int state, int mods)
{
	if (key == GLFW_KEY_ESCAPE && state == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_F5 && state == GLFW_PRESS)
	{
		ShaderProgram::current_shader->refresh();
	}

	if (key == GLFW_KEY_W && state == GLFW_PRESS)
	{
		Snake::current_snake->setDirectionOfMoving(DirectionOfMoving::FORWARD);
	}
	if (key == GLFW_KEY_D && state == GLFW_PRESS)
	{
		Snake::current_snake->setDirectionOfMoving(DirectionOfMoving::RIGHT);
	}
	if (key == GLFW_KEY_A && state == GLFW_PRESS)
	{
		Snake::current_snake->setDirectionOfMoving(DirectionOfMoving::LEFT);
	}
	if (key == GLFW_KEY_S && state == GLFW_PRESS)
	{
		Snake::current_snake->setDirectionOfMoving(DirectionOfMoving::BACK);
	}

	if (key == GLFW_KEY_LEFT && state == GLFW_PRESS)
	{
		if(Camera::current_camera->getRotation()[1] < 60.0f)
		Camera::current_camera->setRotationY(Camera::current_camera->getRotation()[1] + 2.5f);
	}
	if (key == GLFW_KEY_RIGHT && state == GLFW_PRESS)
	{
		if (Camera::current_camera->getRotation()[1] > -60.0f)
		Camera::current_camera->setRotationY(Camera::current_camera->getRotation()[1] - 2.5f);
	}
	if (key == GLFW_KEY_UP && state == GLFW_PRESS)
	{
		if (Camera::current_camera->getRotation()[0] < 90.0f)
		Camera::current_camera->setRotationX(Camera::current_camera->getRotation()[0] + 2.5f);
	}
	if (key == GLFW_KEY_DOWN && state == GLFW_PRESS)
	{
		if (Camera::current_camera->getRotation()[0] > 0.0f)
		Camera::current_camera->setRotationX(Camera::current_camera->getRotation()[0] - 2.5f);
	}


	if (key == GLFW_KEY_LEFT && state == GLFW_REPEAT)
	{
		if (Camera::current_camera->getRotation()[1] < 60.0f)
		Camera::current_camera->setRotationY(Camera::current_camera->getRotation()[1] + 2.5f);
	}
	if (key == GLFW_KEY_RIGHT && state == GLFW_REPEAT)
	{
		if (Camera::current_camera->getRotation()[1] > -60.0f)
		Camera::current_camera->setRotationY(Camera::current_camera->getRotation()[1] - 2.5f);
	}
	if (key == GLFW_KEY_UP && state == GLFW_REPEAT)
	{
		if (Camera::current_camera->getRotation()[0] < 90.0f)
		Camera::current_camera->setRotationX(Camera::current_camera->getRotation()[0] + 2.5f);
	}
	if (key == GLFW_KEY_DOWN && state == GLFW_REPEAT)
	{
		if (Camera::current_camera->getRotation()[0] > 0.0f)
		Camera::current_camera->setRotationX(Camera::current_camera->getRotation()[0] - 2.5f);
	}
}
