#include <GL/glew.h>
#include <GL/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Something.h"
#include <ImGui/ImGuiClass.h>
#include "pca.h"
using namespace std;


int main()
{
	//window
	int window_size[2] = {1000, 800};

	Window window;
	if (!window.init(window_size[0], window_size[1], "Snake3D")) return -1;
	window.enableBlending();

	float floor_size = 11.0f;

	//floor
	Vertex floor_vertices[] = {
		glm::vec3(-1 * floor_size, -1.0, -1 * floor_size), glm::vec4(0.3, 0.3, 0.3, 1.0),
		glm::vec3(floor_size, -1.0, -1 * floor_size), glm::vec4(0.3, 0.3, 0.3, 1.0),
		glm::vec3(-1 * floor_size, -1.0,    floor_size), glm::vec4(0.2, 0.2, 0.2, 1.0),
		glm::vec3(floor_size, -1.0,    floor_size), glm::vec4(0.2, 0.2, 0.2, 1.0)
	};
	GLuint floor_indices[] = {
		0, 1, 2,
		2, 3, 1
	};
	glm::mat4 floor_matrix = glm::mat4(1.0f);

	VertexArrayObject floor(new vector<Vertex>(floor_vertices, end(floor_vertices)), new vector<GLuint>(floor_indices, end(floor_indices)));
	Snake snake;

	
	//camera	
	Camera camera(0.0f, 0.0f, -50.0f, 50.0f, 0.0f, 0.0f);

	float position[3] = { camera.getPosition()[0], camera.getPosition()[1], camera.getPosition()[2] };
	float rotation[3] = { camera.getRotation()[0], camera.getRotation()[1], camera.getRotation()[2] };
	float zoom = camera.getZoom();
	float aspect = camera.getAspect();
	
	//shader
	shared_ptr<Shader> vs(new Shader(
	R"(
		#version 330 core
		
		layout(location = 0) in vec3 position;
		layout(location = 1) in vec4 input_color;
		layout(location = 2) in vec2 input_texture_coordinates;
		layout(location = 3) in vec3 normal;
		
		out vec4 vertex_color;
		
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		
		void main()
		{
			gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
			vertex_color = input_color;
		}
	)",
	VERTEX_SHADER, true));

	shared_ptr<Shader> fs(new Shader(
	R"(
		#version 330 core

		in vec4 vertex_color;
		out vec4 color;

		void main()
		{
			color = vertex_color;
		}
    )", 
	FRAGMENT_SHADER, true));

	ShaderProgram shader(vs, fs);


	//Point direction;
	//ImGuiClass imgui(window.getWindow());


	floor.unBind();
	shader.unBind();
	while (!window.windowShouldClose())
	{
		window.pollEvents();
		window.clear();

		//imgui.newFrame();
		//imgui.createFPScounter();
		
		
		//camera
		{
			position[0] = camera.getPosition()[0];
			position[1] = camera.getPosition()[1];
			position[2] = camera.getPosition()[2];

			rotation[0] = camera.getRotation()[0];
			rotation[1] = camera.getRotation()[1];
			rotation[2] = camera.getRotation()[2];

			zoom = camera.getZoom();

			//ImGui::Begin("Camera");
			//ImGui::SliderFloat3("Position", position , -100.0f, 100.0f);
			//ImGui::SliderFloat3("Rotation", rotation, -180.0f, 180.0f);
			//ImGui::SliderFloat ("Zoom", &zoom, 0.0f, 100.0f);
			//ImGui::SliderFloat("Aspect", &aspect, -10.0f, 10.0f);
			//if (ImGui::Button("Reset"))
			//{
			//	camera.reset();

			//	position[0] = camera.getPosition()[0];
			//	position[1] = camera.getPosition()[1];
			//	position[2] = camera.getPosition()[2];

			//	rotation[0] = camera.getRotation()[0];
			//	rotation[1] = camera.getRotation()[1];
			//	rotation[2] = camera.getRotation()[2];

			//	zoom = camera.getZoom();
			//	aspect = camera.getAspect();
			//}
			//ImGui::End();

			//camera.reset();

			//camera.setPositionX(position[0]);
			//camera.setPositionY(position[1]);
			//camera.setPositionZ(position[2]);
			//camera.setRotationX(rotation[0]);
			//camera.setRotationY(rotation[1]);
			//camera.setRotationZ(rotation[2]);

			camera.resetPerspective();
			camera.setUpMatrices(shader,"projection", "view");
		}
		

		shader.setUniformMarix4fv("model", floor_matrix);
		floor.draw(-1);


		shader.setUniformMarix4fv("model", snake.getMatix());
		snake.go();
		snake.draw();


		//imgui.render();
		window.swapBuffers();
	}
	//imgui.destroyContext();


	return 0;
}