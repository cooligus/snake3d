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