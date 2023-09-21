#pragma once
#include <iostream>
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "VertexArrayObject.h"

enum class DirectionOfMoving
{
	REST = 0,
	RIGHT = 1,
	LEFT = 2,
	FORWARD = 3,
	BACK = 4
};

struct XZ
{
	float x;
	float z;
	XZ(float value_x, float value_z) : x(value_x), z(value_z) {}
	XZ() : x(0.0f), z(0.0f) {};
};
struct SnakeWallXZ
{
	XZ vertex1;
	XZ vertex2;
	SnakeWallXZ(XZ first_vertex, XZ second_vertex) : vertex1(first_vertex),  vertex2(second_vertex) {}
	SnakeWallXZ() : vertex1(XZ()), vertex2(XZ()) {}
};

struct SnakeSegment
{
	XZ back_left_corner;
	XZ front_right_corner;
	DirectionOfMoving direction;

	SnakeSegment(XZ back_left_vertex = XZ(0,0), XZ front_right_vertex = XZ(0,0), DirectionOfMoving direction_of_moving = DirectionOfMoving::REST, bool mix_vertices = true);
};



class Point
{
public:

	Point(XZ position = XZ(), float incisor_x = 0.5f, float incisor_z = 0.5f, float floor_height = -0.5f, float ceiling_height = 0.5f);

	void draw();
	void setMatrix(glm::mat4 matrix);
	SnakeSegment getPosition();
	glm::mat4 getMatix();

private:

	float m_incisor_x;
	float m_incisor_z;
	SnakeSegment m_corners;
	VertexArrayObject* m_vao;
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	glm::mat4 m_matrix;
};

class Snake
{
public:

	Snake(float lenght = 10.0f, int floor_width = 22, int floor_length = 22, float floor_height = -0.99f, float ceiling_height = 1.0f, float incisor_x = 0.05f, float incisor_z = 0.05f, float adder = 5.0f);

	void go(bool deaths = true, bool paths = true, bool walls = true);
	
	void setDirectionOfMoving(DirectionOfMoving direction);
	DirectionOfMoving getDirectionOfMoving();

	void draw();
	void setMatrix(glm::mat4 matrix);
	glm::mat4 getMatix();

	static Snake* current_snake;

private:
	bool isDead();
	bool isPointInSnake();
	bool areSegmentsOverlap(SnakeSegment first, SnakeSegment second);
	float getSnakeLenght();
	void setPositionOfPoint(bool paths = true);
	void updateVAO();

	Point m_point;

	glm::vec4 m_color[8];
	SnakeSegment m_start_segment;
	std::vector <SnakeSegment> m_segments;
	float m_segment_size_x;
	float m_segment_size_z;

	float m_adder;
	float m_incisor_x; 
	float m_incisor_z; 
	float m_floor_height;
	int m_floor_width;
	int m_floor_length;
	float m_ceiling_height;

	float m_speed;
	float m_start_lenght;
	float m_lenght;
	DirectionOfMoving m_actually_direction;
	DirectionOfMoving m_next_direction;

	VertexArrayObject* m_vao;
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	glm::mat4 m_matrix;
};

class Camera 
{
public:
	Camera(float position_x = 0.0f, float position_y = 0.0f, float position_z = 0.0f, float rotation_x = 0.0f, float rotation_y = 0.0f, float rotation_z = 0.0f);
	glm::mat4 getView(bool first_rotating = false);
	glm::mat4 getProjection();

	void setRotationX(float rotation);
	void setRotationY(float rotation);
	void setRotationZ(float rotation);
	void setPositionX(float position);
	void setPositionY(float position);
	void setPositionZ(float position);
	void resetPerspective(float zoom = 30.0f, float aspect = 1.3f, float zNear = 0.1f, float zFar = 100.0f);
	void reset();
	void setUpMatrices(ShaderProgram& shader, const char* projection_matrix_name, const char* view_matrix_name, bool first_rotating = false);

	float* getPosition();
	float* getRotation();
	float getZFar();
	float getZNear();
	float getAspect();
	float getZoom();

	static Camera* current_camera;
private:
	void resetView(bool first_rotating = false);


	float m_position[3];
	float m_rotation[3];
	float m_zoom;
	float m_aspect;
	float m_zNear;
	float m_zFar;

	float m_deafult_position[3];
	float m_deafult_rotation[3];
	float m_deafult_zoom;
	float m_deafult_aspect;
	float m_deafult_zNear;
	float m_deafult_zFar;

	glm::mat4 m_view;
	glm::mat4 m_projection;
};	