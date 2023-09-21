#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "VertexArrayObject.h"

namespace pca
{
	//===STRUCTS===//
	/*
	//This struct have int value and index.
	struct IndexedValue
	{
		float value = 0;
		GLuint index = 0;

		IndexedValue() {}
		IndexedValue(float val, GLuint ind) { value = val; index = ind; }
	};

	//This struct have position in glm::vec3 and index.
	struct IndexedVertex
	{
		glm::vec3 position = glm::vec3(0.0);
		GLuint index = 0;

		IndexedVertex() {}
		IndexedVertex(glm::vec3 pos, GLuint ind) { position = pos; index = ind; }
	};
	*/

	//This struct is the best for creating cubes but different Rectangluar prisms can be made good too.
	struct RectangularPrism
	{
		glm::vec3 vertices[8];

		glm::vec4 colors[8];

		GLuint indices[36];
		
		RectangularPrism();
		RectangularPrism(glm::vec3 bottom_left_vertex, glm::vec3 top_right_vertex, glm::vec4 color, unsigned int number_of_vertices_in_vao = 0);
		RectangularPrism(glm::vec3 vertices[2], glm::vec4 color[8], unsigned int number_of_vertices_in_vao = 0);
		vector<Vertex> getVectoredandVertexedVertices();
	};

	//===FUNCTIONS===//
	/*
	//It returns which element from vector is value which this function got.
	int getWhichElemFromVecIsVal(std::vector<float> to_sort, float value);

	//It returns which indexed value from vector is value which this function got.
	int getWhichElementFromVecIsIndVal(std::vector<IndexedValue> to_sort, IndexedValue value);

	//It returns biggest element from vector which this function got.
	float getBiggestElementFromVector(std::vector<float> to_sort);

	//It returns smallest element from vector which this function got.
	float getSmallestElementFromVector(std::vector<float> to_sort);

	//It returns smallest element from vector which this function got.
	IndexedValue getSmallestIndValFromVector(std::vector<IndexedValue> to_sort);

	//It returns biggest element from vector which this function got.
	IndexedValue getBiggestIndValFromVector(std::vector<IndexedValue> to_sort);

	//This function returns vector built from sorted from smallest to biggest.
	//elements from vector which this function got
	std::vector<IndexedValue> sortIndexedValues(std::vector<IndexedValue> to_sort);

	//This function returns vector built from sorted from smallest to biggest and counterclockwise.
	//elements from vector which this function got.
	std::vector<glm::vec3> sortVertices(std::vector<glm::vec3> to_sort);

	//This function returns vector built from sorted vector from smallest to biggest and counterclockwise 
	//elements from vector which this function got.
	std::vector<vector<glm::vec3>> sortVerticesBasedOnHeight(std::vector<glm::vec3> to_sort);

	//This function returns vector built from sorted from smallest to biggest and counterclockwise. 
	//elements from vector which this function got
	//======DO NOT FINISHED======//
	std::vector<IndexedVertex> sortIndexedVertices(std::vector<IndexedVertex> to_sort);

	//This function returns vector built from sorted vector from smallest to biggest and counterclockwise 
	//elements from vector which this function got.
	//======DO NOT FINISHED======//
	std::vector<vector<IndexedVertex>> sortIndexedVerticesBasedOnHeight(std::vector<IndexedVertex> to_sort);

	//It returns the easiest combination of indices but WARNING!!! You have to set Vertices in vector one by one counterclockwise.
	std::vector<GLuint> getGLfigureIndices(vector<GLuint>indices);
	*/

	//It returns the easiest combination of indices.
	std::vector<GLuint> getGLsquareIndices(GLuint bottom_left, GLuint bottom_right, GLuint top_left, GLuint top_right);
}