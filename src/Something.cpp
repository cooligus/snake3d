#include "Something.h"
#include "pca.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include <glm/gtc/matrix_transform.hpp>


//SnakeSegment
SnakeSegment::SnakeSegment(XZ back_left_vertex, XZ front_right_vertex, DirectionOfMoving direction_of_moving, bool mix_vertices)
	: back_left_corner(back_left_vertex), front_right_corner(front_right_vertex), direction(direction_of_moving)
{
	if (mix_vertices)
	{
		XZ first_vertex;
		XZ second_vertex;

		if (back_left_vertex.x > front_right_vertex.x)
		{
			first_vertex.x = front_right_vertex.x;
			second_vertex.x = back_left_vertex.x;
		}
		else
		{
			first_vertex.x = back_left_vertex.x;
			second_vertex.x = front_right_vertex.x;
		}
		
		if (back_left_vertex.z < front_right_vertex.z)
		{
			first_vertex.z = front_right_vertex.z;
			second_vertex.z = back_left_vertex.z;
		}
		else
		{
			first_vertex.z = back_left_vertex.z;
			second_vertex.z = front_right_vertex.z;
		}

		back_left_corner = first_vertex;
		front_right_corner = second_vertex;
	}
}

//Point
Point::Point(XZ position, float incisor_x, float incisor_z, float floor_height, float ceiling_height)
	: m_incisor_x(incisor_x), m_incisor_z(incisor_z), m_corners(), m_matrix(1.0f)
{
	glm::vec3 vertices[2] = { glm::vec3(position.x + incisor_x, floor_height, position.z + incisor_z), glm::vec3(position.x - incisor_x, ceiling_height, position.z - incisor_z) };
	glm::vec4 color[8] = {
		glm::vec4(0.1f,0.1f,0.5f,1.0f), glm::vec4(0.1f,0.1f,0.5f,1.0f), //floor
		glm::vec4(0.1f,0.1f,0.5f,1.0f), glm::vec4(0.1f,0.1f,0.5f,1.0f), //floor
		glm::vec4(0.3f,0.3f,0.7f,1.0f), glm::vec4(0.3f,0.3f,0.7f,1.0f), //ceiling
		glm::vec4(0.3f,0.3f,0.7f,1.0f), glm::vec4(0.3f,0.3f,0.7f,1.0f)  //ceiling
	};
	m_corners = SnakeSegment(XZ(vertices[0].x, vertices[0].z), XZ(vertices[1].x, vertices[1].z), DirectionOfMoving::REST);

	pca::RectangularPrism prism(vertices, color);

	m_vertices = prism.getVectoredandVertexedVertices();
	m_indices = vector<GLuint>(prism.indices, end(prism.indices));

	m_vao = new VertexArrayObject(new std::vector<Vertex>(m_vertices), new std::vector<GLuint>(m_indices));
}

void Point::draw()
{
	m_vao->draw(-1);
}

void Point::setMatrix(glm::mat4 matrix)
{
	m_matrix = matrix;
}

SnakeSegment Point::getPosition()
{
	return m_corners;
}

glm::mat4 Point::getMatix()
{
	return m_matrix;
}

//Snake
Snake* Snake::current_snake = nullptr;

Snake::Snake(float lenght, int floor_width, int floor_length, float floor_height, float ceiling_height, float incisor_x, float incisor_z, float adder)
	:m_point(), m_speed(0.05f), m_incisor_x(incisor_x), m_incisor_z(incisor_z), m_floor_width(floor_width), m_floor_length(floor_length), m_lenght(lenght), m_start_lenght(lenght), m_adder(adder), m_matrix(1.0f), m_floor_height(floor_height), m_ceiling_height(ceiling_height), m_actually_direction(DirectionOfMoving::REST), m_next_direction(DirectionOfMoving::REST), m_color{ glm::vec4(0.1f,0.1f,0.1f,1.0f), glm::vec4(0.1f,0.1f,0.1f,1.0f), glm::vec4(0.1f,0.1f,0.1f,1.0f), glm::vec4(0.1f,0.1f,0.1f,1.0f),glm::vec4(0.3f,0.3f,0.3f,1.0f), glm::vec4(0.3f,0.3f,0.3f,1.0f), glm::vec4(0.3f,0.3f,0.3f,1.0f), glm::vec4(0.3f,0.3f,0.3f,1.0f) }
{
	glm::vec3 vertices[2] = { glm::vec3 (-1.0f + incisor_x, m_floor_height, -1.0f + incisor_z), glm::vec3(1.0f - incisor_x, m_ceiling_height, 1.0f - incisor_z) };

	SnakeSegment segment(XZ(vertices[0].x, vertices[0].z), XZ(vertices[1].x, vertices[1].z), m_actually_direction);
	m_segments.push_back(segment);
	m_start_segment = m_segments.back();

	pca::RectangularPrism prism(vertices, m_color);

	m_vertices = prism.getVectoredandVertexedVertices();
	m_indices = vector<GLuint>(prism.indices, end(prism.indices));

	m_vao = new VertexArrayObject(new std::vector<Vertex>(m_vertices), new std::vector<GLuint>(m_indices));

	m_segment_size_x = 2.0f - m_incisor_x * 2.0f;
	m_segment_size_z = 2.0f - m_incisor_z * 2.0f;

	setPositionOfPoint(true);

	current_snake = this;
}

void Snake::draw()
{
	m_vao->draw(-1);
	m_point.draw();
}

void Snake::go(bool deaths, bool paths, bool walls)
{
	if (m_segments.size() == 1 && m_actually_direction == DirectionOfMoving::REST && m_next_direction != DirectionOfMoving::REST)
	{
		if (m_next_direction == DirectionOfMoving::BACK) m_segments[0].front_right_corner.z += m_speed;
		else if (m_next_direction == DirectionOfMoving::LEFT)m_segments[0].back_left_corner.x -= m_speed;
		else if (m_next_direction == DirectionOfMoving::FORWARD) m_segments[0].back_left_corner.z -= m_speed;
		else if (m_next_direction == DirectionOfMoving::RIGHT)m_segments[0].front_right_corner.x += m_speed;

		m_segments[0].direction = m_next_direction;
		m_actually_direction = m_next_direction;
	}
	
	if (m_actually_direction != m_next_direction && m_segments.size() > 0)
	{
		if (m_actually_direction == DirectionOfMoving::FORWARD)
		{
			XZ corner_position = m_segments.back().front_right_corner;
			
			float position2 = corner_position.z - 1;

			float position1 = position2 + m_speed;
			float position3 = position2 - m_speed;
			position2 = nearbyint(position2);

			if (paths == true && (int)position2 % 2 == 0 && position1 > (int)position2 && position3 < (int)position2)
			{
				if (m_next_direction == DirectionOfMoving::RIGHT || m_next_direction == DirectionOfMoving::LEFT) m_segments.push_back(SnakeSegment(corner_position, XZ(corner_position.x - m_segment_size_x, corner_position.z + m_segment_size_z), m_next_direction));
				m_actually_direction = m_next_direction;
			}
			else if (paths == false)
			{
				if (m_next_direction == DirectionOfMoving::RIGHT || m_next_direction == DirectionOfMoving::LEFT) m_segments.push_back(SnakeSegment(corner_position, XZ(corner_position.x - m_segment_size_x, corner_position.z + m_segment_size_z), m_next_direction));
				m_actually_direction = m_next_direction;
			}
		}
		else if (m_actually_direction == DirectionOfMoving::BACK)
		{
			XZ corner_position = m_segments.back().back_left_corner;
			
			float position2 = corner_position.z + 1;

			float position1 = position2 - m_speed;
			float position3 = position2 + m_speed;
			position2 = nearbyint(position2);

			if (paths == true && (int)position2 % 2 == 0 && position1 < (int)position2 && position3 > (int)position2)
			{
				if (m_next_direction == DirectionOfMoving::RIGHT || m_next_direction == DirectionOfMoving::LEFT) m_segments.push_back(SnakeSegment(corner_position, XZ(corner_position.x + m_segment_size_x, corner_position.z - m_segment_size_z), m_next_direction));
				m_actually_direction = m_next_direction;
			}
			else if (paths == false)
			{
				if (m_next_direction == DirectionOfMoving::RIGHT || m_next_direction == DirectionOfMoving::LEFT) m_segments.push_back(SnakeSegment(corner_position, XZ(corner_position.x + m_segment_size_x, corner_position.z - m_segment_size_z), m_next_direction));
				m_actually_direction = m_next_direction;
			}
		}
		else if (m_actually_direction == DirectionOfMoving::LEFT)
		{
			XZ corner_position = m_segments.back().back_left_corner;
			
			float position2 = corner_position.x - 1;

			float position1 = position2 + m_speed;
			float position3 = position2 - m_speed;
			position2 = nearbyint(position2);

			if (paths == true && (int)position2 % 2 == 0 && position1 > (int)position2 && position3 < (int)position2)
			{
				if (m_next_direction == DirectionOfMoving::BACK || m_next_direction == DirectionOfMoving::FORWARD) m_segments.push_back(SnakeSegment(corner_position, XZ(corner_position.x + m_segment_size_x, corner_position.z - m_segment_size_z), m_next_direction));
				m_actually_direction = m_next_direction;
			}
			else if (paths == false)
			{
				if (m_next_direction == DirectionOfMoving::BACK || m_next_direction == DirectionOfMoving::FORWARD) m_segments.push_back(SnakeSegment(corner_position, XZ(corner_position.x + m_segment_size_x, corner_position.z - m_segment_size_z), m_next_direction));
				m_actually_direction = m_next_direction;
			}
		}
		else if (m_actually_direction == DirectionOfMoving::RIGHT)
		{
			XZ corner_position = m_segments.back().front_right_corner;

			float position2 = corner_position.x + 1;

			float position1 = position2 - m_speed;
			float position3 = position2 + m_speed;
			position2 = nearbyint(position2);

			if (paths == true && (int)position2 % 2 == 0 && position1 < (int)position2 && position3 >(int)position2)
			{
				if (m_next_direction == DirectionOfMoving::BACK || m_next_direction == DirectionOfMoving::FORWARD) m_segments.push_back(SnakeSegment(XZ(corner_position.x - m_segment_size_x, corner_position.z+m_segment_size_z), corner_position, m_next_direction));
				m_actually_direction = m_next_direction;
			}
			else if (paths == false)
			{
				if (m_next_direction == DirectionOfMoving::BACK || m_next_direction == DirectionOfMoving::FORWARD) m_segments.push_back(SnakeSegment(XZ(corner_position.x - m_segment_size_x, corner_position.z + m_segment_size_z), corner_position, m_next_direction));
				m_actually_direction = m_next_direction;
			}
		}
	}
	if (m_actually_direction != DirectionOfMoving::REST && m_segments.size() > 0)
	{
		if (m_lenght <= getSnakeLenght())
		{
			if (m_segments[0].direction == DirectionOfMoving::FORWARD)
			{
				m_segments[0].back_left_corner.z -= m_speed;

				if(m_segments.size() > 1 && m_segments[0].back_left_corner.z <= m_segments[0].front_right_corner.z + m_segment_size_z && m_segments[1].direction != DirectionOfMoving::FORWARD)m_segments.erase(m_segments.begin());
				else if(m_segments.size() > 1 && m_segments[0].back_left_corner.z <= m_segments[0].front_right_corner.z)m_segments.erase(m_segments.begin());
			}
			else if (m_segments[0].direction == DirectionOfMoving::RIGHT)
			{
				m_segments[0].back_left_corner.x += m_speed;

				if (m_segments.size() > 1 && m_segments[0].back_left_corner.x >= m_segments[0].front_right_corner.x - m_segment_size_x && m_segments[1].direction != DirectionOfMoving::RIGHT)m_segments.erase(m_segments.begin());
				else if (m_segments.size() > 1 && m_segments[0].back_left_corner.x >= m_segments[0].front_right_corner.x)m_segments.erase(m_segments.begin());
			}
			else if (m_segments[0].direction == DirectionOfMoving::BACK)
			{
				m_segments[0].front_right_corner.z += m_speed;

				if (m_segments.size() > 1 && m_segments[0].front_right_corner.z >= m_segments[0].back_left_corner.z - m_segment_size_z && m_segments[1].direction != DirectionOfMoving::BACK)m_segments.erase(m_segments.begin());
				else if (m_segments.size() > 1 && m_segments[0].front_right_corner.z >= m_segments[0].back_left_corner.z)m_segments.erase(m_segments.begin());
			}
			else if (m_segments[0].direction == DirectionOfMoving::LEFT)
			{
				m_segments[0].front_right_corner.x -= m_speed;

				if (m_segments.size() > 1 && m_segments[0].front_right_corner.x <= m_segments[0].back_left_corner.x + m_segment_size_x && m_segments[1].direction != DirectionOfMoving::LEFT)m_segments.erase(m_segments.begin());
				else if (m_segments.size() > 1 && m_segments[0].front_right_corner.x <= m_segments[0].back_left_corner.x)m_segments.erase(m_segments.begin());
			}
		}
		
		if (m_actually_direction == DirectionOfMoving::BACK && m_segments.back().back_left_corner.z + m_speed >= m_floor_width / 2)m_segments.push_back(SnakeSegment(XZ(m_segments.back().back_left_corner.x, m_segments.back().back_left_corner.z*-1), XZ(m_segments.back().front_right_corner.x, m_segments.back().back_left_corner.z * -1), m_actually_direction));
		else if (m_actually_direction == DirectionOfMoving::LEFT && m_segments.back().back_left_corner.x - m_speed <= m_floor_length / 2 * -1)m_segments.push_back(SnakeSegment(XZ(m_segments.back().back_left_corner.x * -1, m_segments.back().back_left_corner.z), XZ(m_segments.back().back_left_corner.x * -1, m_segments.back().front_right_corner.z), m_actually_direction));
		else if (m_actually_direction == DirectionOfMoving::FORWARD && m_segments.back().front_right_corner.z - m_speed <= m_floor_width / 2 * -1)m_segments.push_back(SnakeSegment(XZ(m_segments.back().back_left_corner.x, m_segments.back().front_right_corner.z * -1), XZ(m_segments.back().front_right_corner.x, m_segments.back().front_right_corner.z * -1), m_actually_direction));
		else if (m_actually_direction == DirectionOfMoving::RIGHT && m_segments.back().front_right_corner.x + m_speed >= m_floor_width / 2)m_segments.push_back(SnakeSegment(XZ(m_segments.back().front_right_corner.x * -1, m_segments.back().back_left_corner.z), XZ(m_segments.back().front_right_corner.x * -1, m_segments.back().front_right_corner.z), m_actually_direction));

		if (m_actually_direction == DirectionOfMoving::BACK) m_segments.back().back_left_corner.z += m_speed;
		else if (m_actually_direction == DirectionOfMoving::LEFT)m_segments.back().back_left_corner.x -= m_speed;
		else if (m_actually_direction == DirectionOfMoving::FORWARD) m_segments.back().front_right_corner.z -= m_speed;
		else if (m_actually_direction == DirectionOfMoving::RIGHT)m_segments.back().front_right_corner.x += m_speed;
		

		if (areSegmentsOverlap(m_segments.back(), m_point.getPosition()))
		{
			setPositionOfPoint();
			m_lenght += m_adder;
		}
	}
	if (deaths && isDead()) 
	{ 
		m_segments.clear(); 
		m_segments.push_back(m_start_segment);
		m_lenght = m_start_lenght;
		setPositionOfPoint();

		m_actually_direction = DirectionOfMoving::REST;
		m_next_direction = DirectionOfMoving::REST;
	}
	updateVAO();
}

float Snake::getSnakeLenght()
{
	float lenght = 0;
	for (int i = 0; i < m_segments.size(); i++)
	{
		float segment_lenght = 0;
		if (m_segments[i].direction == DirectionOfMoving::BACK || m_segments[i].direction == DirectionOfMoving::FORWARD)
		{
			if (m_segments[i].back_left_corner.z > m_segments[i].front_right_corner.z)
				segment_lenght = std::abs(m_segments[i].back_left_corner.z - m_segments[i].front_right_corner.z);
			else
				segment_lenght = std::abs(m_segments[i].front_right_corner.z - m_segments[i].back_left_corner.z);
		}
		
		else if (m_segments[i].direction == DirectionOfMoving::RIGHT || m_segments[i].direction == DirectionOfMoving::LEFT)
		{
			if (m_segments[i].back_left_corner.x > m_segments[i].front_right_corner.x)
				segment_lenght = std::abs(m_segments[i].back_left_corner.x - m_segments[i].front_right_corner.x);
			else
				segment_lenght = std::abs(m_segments[i].front_right_corner.x - m_segments[i].back_left_corner.x);
		}

		lenght += segment_lenght;
	}
	return lenght;
}

bool Snake::areSegmentsOverlap(SnakeSegment first, SnakeSegment second)
{
	if (second.back_left_corner.x <= first.front_right_corner.x && second.back_left_corner.z >= first.front_right_corner.z && first.back_left_corner.x <= second.front_right_corner.x && first.back_left_corner.z >= second.front_right_corner.z)return true;
	
	if (second.back_left_corner.x <= first.back_left_corner.x && first.back_left_corner.x <= second.front_right_corner.x)
		if (second.back_left_corner.z <= first.back_left_corner.z && first.back_left_corner.z <= second.front_right_corner.z)return true;
		else if (second.back_left_corner.z >= first.back_left_corner.z && first.back_left_corner.z >= second.front_right_corner.z)return true;
		else if (first.back_left_corner.z <= second.back_left_corner.z && second.back_left_corner.z <= first.front_right_corner.z)return true;
		else if (first.back_left_corner.z >= second.back_left_corner.z && second.back_left_corner.z >= first.front_right_corner.z)return true;

	else if (second.back_left_corner.x >= first.back_left_corner.x && first.back_left_corner.x >= second.front_right_corner.x)
		if (second.back_left_corner.z <= first.back_left_corner.z && first.back_left_corner.z <= second.front_right_corner.z)return true;
		else if (second.back_left_corner.z >= first.back_left_corner.z && first.back_left_corner.z >= second.front_right_corner.z)return true;
		else if (first.back_left_corner.z <= second.back_left_corner.z && second.back_left_corner.z <= first.front_right_corner.z)return true;
		else if (first.back_left_corner.z >= second.back_left_corner.z && second.back_left_corner.z >= first.front_right_corner.z)return true;

	else if (first.back_left_corner.x <= second.front_right_corner.x && second.front_right_corner.x <= first.front_right_corner.x)
		if (second.back_left_corner.z <= first.back_left_corner.z && first.back_left_corner.z <= second.front_right_corner.z)return true;
		else if (second.back_left_corner.z >= first.back_left_corner.z && first.back_left_corner.z >= second.front_right_corner.z)return true;
		else if (first.back_left_corner.z <= second.back_left_corner.z && second.back_left_corner.z <= first.front_right_corner.z)return true;
		else if (first.back_left_corner.z >= second.back_left_corner.z && second.back_left_corner.z >= first.front_right_corner.z)return true;

	else if (first.back_left_corner.x >= second.front_right_corner.x && second.front_right_corner.x >= first.front_right_corner.x)
		if (second.back_left_corner.z <= first.back_left_corner.z && first.back_left_corner.z <= second.front_right_corner.z)return true;
		else if (second.back_left_corner.z >= first.back_left_corner.z && first.back_left_corner.z >= second.front_right_corner.z)return true;
		else if (first.back_left_corner.z <= second.back_left_corner.z && second.back_left_corner.z <= first.front_right_corner.z)return true;
		else if (first.back_left_corner.z >= second.back_left_corner.z && second.back_left_corner.z >= first.front_right_corner.z)return true;
	
	
	return false;
}

bool Snake::isDead()
{
	if (m_segments.size() > 3)
		for (int i = 0; i < m_segments.size() - 2; i++)
		{
			if(areSegmentsOverlap(m_segments.back(), m_segments[i]))return true;
	}
		
	if (m_segments.size() == 2)
		if(m_segments[0].direction == m_segments[1].direction)
			if (areSegmentsOverlap(m_segments[0], m_segments[1]))return true;

	return false;
}

void Snake::setDirectionOfMoving(DirectionOfMoving direction)
{
	if (direction == DirectionOfMoving::REST)return;
	if (m_actually_direction == direction || m_next_direction == direction)return;
	if (m_actually_direction == DirectionOfMoving::BACK && direction == DirectionOfMoving::FORWARD)return;
	else if (m_actually_direction == DirectionOfMoving::FORWARD && direction == DirectionOfMoving::BACK)return;
	else if (m_actually_direction == DirectionOfMoving::RIGHT && direction == DirectionOfMoving::LEFT)return;
	else if (m_actually_direction == DirectionOfMoving::LEFT && direction == DirectionOfMoving::RIGHT)return;

	m_next_direction = direction;
	current_snake = this;
}

bool Snake::isPointInSnake()
{
	for (int i = 0; i < m_segments.size(); i++)
	{
		if (areSegmentsOverlap(m_point.getPosition(), m_segments[i]))return true;
	}

	return false;
}

void Snake::setPositionOfPoint(bool paths)
{
	srand(time(NULL));
	do
	{
		int position_x = rand() % (m_floor_length / 2);
		if (rand() % 2 == 0)position_x *= -1;
		XZ position = XZ(position_x, 0);

		if(paths && position_x % 2 != 0)
			position_x++;

		int position_z = rand() % (m_floor_width / 2);
		if (rand() % 2 == 0)position_z *= -1;
		
		if (paths && position_z % 2 != 0)
			position_z++;

		position = XZ(position_x, position_z);
		m_point = Point(position);
	} while (isPointInSnake());	
}

DirectionOfMoving Snake::getDirectionOfMoving()
{
	return m_actually_direction;
}

void Snake::setMatrix(glm::mat4 matrix)
{
	m_matrix = matrix;
	current_snake = this;
}

glm::mat4 Snake::getMatix()
{
	return m_matrix;
}

void Snake::updateVAO()
{
	m_vertices.clear();
	m_indices.clear();

	pca::RectangularPrism prism;
	glm::vec3 vertices[2];
	vector<Vertex>new_veretices;
	vector<GLuint>new_indices;
	
	for(int i=0;i < m_segments.size();i++)
	{ 
		vertices[0] = glm::vec3(m_segments[i].back_left_corner.x, m_floor_height, m_segments[i].back_left_corner.z);
		vertices[1] = glm::vec3(m_segments[i].front_right_corner.x, m_ceiling_height, m_segments[i].front_right_corner.z);
		prism = pca::RectangularPrism(vertices, m_color, i*8);

		new_veretices = prism.getVectoredandVertexedVertices();
		new_indices = vector<GLuint>(prism.indices, end(prism.indices));

		for (int j = 0; j < new_veretices.size(); j++)
		{
			m_vertices.push_back(new_veretices[j]);
		}
		for (int j = 0; j < new_indices.size(); j++)
		{
			m_indices.push_back(new_indices[j]);
		}
	}
	
	m_vao = new VertexArrayObject(new std::vector<Vertex>(m_vertices), new std::vector<GLuint>(m_indices));

	current_snake = this;
}

//camera
Camera* Camera::current_camera = nullptr;

Camera::Camera(float position_x, float position_y, float position_z, float rotation_x, float rotation_y, float rotation_z)
	:m_aspect(1.3f),m_zNear(0.1f), m_zFar(100.0f), m_position{ position_x,position_y,position_z }, m_rotation{ rotation_x,rotation_y,rotation_z }, m_zoom(30), m_deafult_aspect(1.3f), m_deafult_zNear(0.1f), m_deafult_zFar(100.0f), m_deafult_position{ m_position[0], m_position[1], m_position[2] }, m_deafult_rotation{ m_rotation[0], m_rotation[1], m_rotation[2] }, m_deafult_zoom(m_zoom), m_view(1.0f), m_projection(1.0f)
{
	resetPerspective();
	current_camera = this;
}

void Camera::setRotationX(float rotation)
{
	m_rotation[0] = rotation;
	current_camera = this;
}

void Camera::setRotationY(float rotation)
{
	m_rotation[1] = rotation;
	current_camera = this;
}

void Camera::setRotationZ(float rotation)
{
	m_rotation[2] = rotation;
	current_camera = this;
}

void Camera::setPositionX(float position)
{
	m_position[0] = position;
	current_camera = this;
}

void Camera::setPositionY(float position)
{
	m_position[1] = position;
	current_camera = this;
}

void Camera::setPositionZ(float position)
{
	m_position[2] = position;
	current_camera = this;
}

void Camera::resetView(bool first_rotating)
{
	m_view = glm::mat4(1.0f);

	if (first_rotating)
	{
		m_view = glm::rotate(m_view, glm::radians(m_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		m_view = glm::rotate(m_view, glm::radians(m_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		m_view = glm::rotate(m_view, glm::radians(m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		m_view = glm::translate(m_view, glm::vec3(m_position[0], m_position[1], m_position[2]));
	}
	else
	{
		m_view = glm::translate(m_view, glm::vec3(m_position[0], m_position[1], m_position[2]));

		m_view = glm::rotate(m_view, glm::radians(m_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		m_view = glm::rotate(m_view, glm::radians(m_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		m_view = glm::rotate(m_view, glm::radians(m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void Camera::resetPerspective(float zoom, float aspect, float zNear, float zFar)
{
	m_zoom = zoom;
	m_aspect = aspect;
	m_zNear = zNear;
	m_zFar = zFar;

	m_projection = glm::perspective(glm::radians(m_zoom), m_aspect, m_zNear, m_zFar);
	current_camera = this;
}

void Camera::reset()
{
	m_position[0] = m_deafult_position[0];
	m_position[1] = m_deafult_position[1];
	m_position[2] = m_deafult_position[2];

	m_rotation[0] = m_deafult_position[0];
	m_rotation[1] = m_deafult_position[1];
	m_rotation[2] = m_deafult_position[2];

	m_zoom = m_deafult_zoom;
	m_aspect = m_deafult_aspect;
	m_zNear = m_deafult_zNear;
	m_zFar = m_deafult_zFar;

	resetPerspective();
}

void Camera::setUpMatrices(ShaderProgram& shader, const char* projection_matrix_name, const char* view_matrix_name, bool first_rotating)
{
	resetView();

	shader.bind();
	shader.setUniformMarix4fv(projection_matrix_name, getProjection());
	shader.setUniformMarix4fv(view_matrix_name, getView());
}

float* Camera::getPosition()
{
	return m_position;
}

float* Camera::getRotation()
{
	return m_rotation;
}

float Camera::getZoom()
{
	return m_zoom;
}

float Camera::getZFar()
{
	return m_zFar;
}

float Camera::getZNear()
{
	return m_zNear;
}

float Camera::getAspect()
{
	return m_aspect;
}

glm::mat4 Camera::getView(bool first_rotating)
{
	resetView(first_rotating);

	return m_view;
}

glm::mat4 Camera::getProjection()
{
	return m_projection;
}