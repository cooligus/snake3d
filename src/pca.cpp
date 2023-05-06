#include "pca.h"
#include <iostream>
#include <algorithm>
#include <GLM/gtc/matrix_transform.hpp>

//functions uses to creating figures

std::vector<GLuint> pca::getGLsquareIndices(GLuint bottom_left, GLuint bottom_right, GLuint top_left, GLuint top_right)
{
	GLuint indices[6] = {
		top_left, bottom_left, bottom_right,
		top_left, top_right, bottom_right
	};

	std::vector<GLuint> to_return;

	for (int i = 0; i < 6; i++)
	{
		to_return.push_back(indices[i]);
	}

	return to_return;
}
/*
std::vector<GLuint> pca::getGLfigureIndices(vector<GLuint>indices)
{
	vector<GLuint>to_return;
	if (indices.size() > 2);
	{
		if (indices.size() == 3)
		{
			to_return.push_back(indices[0]);
			to_return.push_back(indices[1]);
			to_return.push_back(indices[2]);
			return to_return;
		}

		int number = indices.size() - 1;
		unsigned int element = 0;

		while (number > 1)
		{
			element++;
			number--;
			for (int i = 0; i < 3; i++)
			{
				to_return.push_back(indices[0]);
				to_return.push_back(indices[element]);
				to_return.push_back(indices[element + 1]);
			}
		}
		return to_return;
	}
	std::cout << "Propably you made a mistake using function getGLfigureIndices() because size of vector<GLuint>indices is smaller then 3.\n";
	to_return.push_back(0);
	return to_return;
}
*/

//functions uses in sorting functions
/*
int pca::getWhichElemFromVecIsVal(std::vector<float> to_sort, float value)
{
	for (int i = 0; i < to_sort.size(); i++)
	{
		if (value == to_sort[i])return i;
	}
	return -1;
}
int pca::getWhichElementFromVecIsIndVal(std::vector<pca::IndexedValue> to_sort, pca::IndexedValue value)
{
	for (int i = 0; i < to_sort.size(); i++)
	{
		if (value.value == to_sort[i].value && value.index == to_sort[i].index)return i;
	}
	return -1;
}
float pca::getBiggestElementFromVector(std::vector<float> to_sort)
{
	float to_return = to_sort[0];

	for (int i = 0; i < to_sort.size(); i++)
	{
		if (to_return < to_sort[i]) to_return = to_sort[i];
	}

	return to_return;
}
float pca::getSmallestElementFromVector(std::vector<float> to_sort)
{
	float to_return = to_sort[0];

	for (int i = 0; i < to_sort.size(); i++)
	{
		if (to_return > to_sort[i]) to_return = to_sort[i];
	}

	return to_return;
}
pca::IndexedValue pca::getSmallestIndValFromVector(std::vector<pca::IndexedValue> to_sort)
{
	pca::IndexedValue to_return;
	float value = to_sort[0].value;
	unsigned int index = to_sort[0].index;

	vector<float> values;
	for (int i = 0; i < to_sort.size(); i++)
	{
		values.push_back(to_sort[i].value);
	}

	value = pca::getSmallestElementFromVector(values);
	for (int i = 0; i < to_sort.size(); i++)
	{
		if (to_sort[i].value == value)
		{
			index = to_sort[i].index;
			break;
		}
	}
	

	to_return.value = value;
	to_return.index = index;

	return to_return;
}
pca::IndexedValue pca::getBiggestIndValFromVector(std::vector<pca::IndexedValue> to_sort)
{
	pca::IndexedValue to_return;
	float value = to_sort[0].value;
	unsigned int index = to_sort[0].index;

	vector<float> values;
	for (int i = 0; i < to_sort.size(); i++)
	{
		values.push_back(to_sort[i].value);
	}

	value = pca::getBiggestElementFromVector(values);
	for (int i = 0; i < to_sort.size(); i++)
	{
		if (to_sort[i].value == value)
		{
			index = to_sort[i].index;
			break;
		}
	}


	to_return.value = value;
	to_return.index = index;

	return to_return;
}
*/

//sorting functions
/*
std::vector<glm::vec3> pca::sortVertices(std::vector<glm::vec3> to_sort)
{
	std::vector<pca::IndexedVertex> to_finish;

	std::vector<pca::IndexedValue> x;
	std::vector<pca::IndexedValue> y;
	std::vector<pca::IndexedValue> z;

	//setting up x, y, z
	for (int i = 0; i < to_sort.size(); i++)
	{
		x.push_back(pca::IndexedValue(to_sort[i].x, i));
		y.push_back(pca::IndexedValue(to_sort[i].y, i));
		z.push_back(pca::IndexedValue(to_sort[i].z, i));
	}

	//x sorting 
	for (int i = 0; i < x.size(); i++)
	{
		x = pca::sortIndexedValues(x);
	}

	//setting x to to_finish
	for (int i = 0; i < x.size(); i++)
	{
		to_finish.push_back(
			pca::IndexedVertex(glm::vec3(x[i].value, 0.0f, 0.0f), x[i].index)
		);
	}

	//setting y and z to to_finish
	for (int i = 0; i < to_finish.size(); i++)
	{
		int element = 0;
		while (to_finish[i].index != z[element].index)
		{
			element++;
			if (element >= z.size()) break;
		}

		to_finish[i].position.z = z[element].value;
		to_finish[i].position.y = y[element].value;
		z.erase(z.begin() + element);
		y.erase(y.begin() + element);
	}

	//setting to_finish to to_return
	std::vector<glm::vec3> to_return;
	for (int i = 0; i < to_finish.size(); i++)
	{
		to_return.push_back(to_finish[i].position);
	}

	//returning to_return
	return to_return;
}
std::vector<vector<glm::vec3>> pca::sortVerticesBasedOnHeight(std::vector<glm::vec3> to_sort)
{
	vector<vector<glm::vec3>> to_return;
	if (to_sort.size() <= 0) { return to_return; cout << "vector which pca::sortVerticesBasedOnHeight() function got probably is empty!\n"; }

	to_sort = pca::sortVertices(to_sort);

	vector<pca::IndexedValue> heights;
	for (int i = 0; i < to_sort.size(); i++)
	{
		heights.push_back(pca::IndexedValue(to_sort[i].y, i));
	}
	heights = pca::sortIndexedValues(heights);

	int y = 1;
	for (int i = 0; i < heights.size()-1; i++)
	{
		if (heights[i].value != heights[i+1].value)
		{
			y++;
		}
	}

	for (; heights.size() > 0 ;)
	{
		vector<glm::vec3> actually;
		for (int start_y = y; start_y == y;)
		{
			if (heights.size() <= 1)y--;
			if (y != 0 && heights[0].value != heights[1].value)y--;
			
			actually.push_back(glm::vec3(to_sort[heights[0].index].x, heights[0].value, to_sort[heights[0].index].z));
			heights.erase(heights.begin());
		}

		to_return.push_back(actually);
	}

	return to_return;
}

std::vector<pca::IndexedValue> pca::sortIndexedValues(std::vector<IndexedValue> to_sort)
{
	vector<IndexedValue> to_return;
	IndexedValue actually;


	while (to_sort.size() > 0)
	{
		actually = pca::getSmallestIndValFromVector(to_sort);
		to_return.push_back(actually);

		to_sort.erase(to_sort.begin() + pca::getWhichElementFromVecIsIndVal(to_sort, actually));
	}


	return to_return;
}
std::vector<pca::IndexedVertex> pca::sortIndexedVertices(std::vector<IndexedVertex> to_sort)
{
	std::vector<pca::IndexedVertex> to_return;
	std::vector<pca::IndexedValue> x;
	
	for (int i = 0; i < to_sort.size(); i++)
	{
		x.push_back(pca::IndexedValue(to_sort[i].position.x, to_sort[i].index));
	}
	x = pca::sortIndexedValues(x);
	
	while (to_sort.size() > 0)
	{
		int i = 0;
		while (i < to_sort.size());

		to_sort.erase(to_sort.begin() + i);
	}


	return to_return;
}
std::vector<vector<pca::IndexedVertex>> pca::sortIndexedVerticesBasedOnHeight(std::vector<IndexedVertex> to_sort)
{
	std::vector<vector<pca::IndexedVertex>> to_return;
	return to_return;
}
*/

//RectangularPrism
pca::RectangularPrism::RectangularPrism()
	: vertices{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f),glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
	colors{ glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f) },
	indices{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, }
{
}

pca::RectangularPrism::RectangularPrism(glm::vec3 bottom_left_vertex, glm::vec3 top_right_vertex, glm::vec4 color, unsigned int number_of_vertices_in_vao)
	: vertices{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f),glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
	colors{ glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f) },
	indices{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, }
{
	//bottom
	vertices[0] = bottom_left_vertex;  														   //0 - - -
	vertices[1] = glm::vec3(bottom_left_vertex.x, bottom_left_vertex.y, top_right_vertex.z);   //1 - - +
	vertices[2] = glm::vec3(top_right_vertex.x, bottom_left_vertex.y, bottom_left_vertex.z);   //2 + - -
	vertices[3] = glm::vec3(top_right_vertex.x, bottom_left_vertex.y, top_right_vertex.z);	   //3 + - +
	//top																					   
	vertices[4] = glm::vec3(bottom_left_vertex.x, top_right_vertex.y, bottom_left_vertex.z);   //4 - + -
	vertices[5] = glm::vec3(bottom_left_vertex.x, top_right_vertex.y, top_right_vertex.z);	   //5 - + +
	vertices[6] = glm::vec3(top_right_vertex.x, top_right_vertex.y, bottom_left_vertex.z);	   //6 + + -
	vertices[7] = top_right_vertex;															   //7 + + +


	vector<GLuint> f_indices;
	vector<GLuint> f_timeable;

	//1
	f_timeable = getGLsquareIndices(0, 1, 2, 3);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//2
	f_timeable = getGLsquareIndices(0, 2, 4, 6);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//3
	f_timeable = getGLsquareIndices(1, 3, 5, 7);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//4
	f_timeable = getGLsquareIndices(0, 1, 4, 5);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//5
	f_timeable = getGLsquareIndices(2, 3, 6, 7);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//6
	f_timeable = getGLsquareIndices(4, 5, 6, 7);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//finishing
	for (int i = 0; i < 36; i++)
	{
		indices[i] = f_indices[i] + number_of_vertices_in_vao;
	}


	//color
	for (int i = 0; i < 8; i++)
	{
		colors[i] = color;
	}
}
pca::RectangularPrism::RectangularPrism(glm::vec3 verticees[2], glm::vec4 color[8], unsigned int number_of_vertices_in_vao)
	: vertices{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f),glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
	colors{ glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f) },
	indices{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, }
{
	//bottom
	vertices[0] = verticees[0];  											    //0 - - -
	vertices[1] = glm::vec3(verticees[0].x, verticees[0].y, verticees[1].z);    //1 - - +
	vertices[2] = glm::vec3(verticees[1].x, verticees[0].y, verticees[0].z);    //2 + - -
	vertices[3] = glm::vec3(verticees[1].x, verticees[0].y, verticees[1].z);	//3 + - +
	//top																					   
	vertices[4] = glm::vec3(verticees[0].x, verticees[1].y, verticees[0].z);    //4 - + -
	vertices[5] = glm::vec3(verticees[0].x, verticees[1].y, verticees[1].z);	//5 - + +
	vertices[6] = glm::vec3(verticees[1].x, verticees[1].y, verticees[0].z);	//6 + + -
	vertices[7] = verticees[1];												    //7 + + +

	vector<GLuint> f_indices;
	vector<GLuint> f_timeable;

	//1
	f_timeable = getGLsquareIndices(0, 1, 2, 3);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//2
	f_timeable = getGLsquareIndices(0, 2, 4, 6);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//3
	f_timeable = getGLsquareIndices(1, 3, 5, 7);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//4
	f_timeable = getGLsquareIndices(0, 1, 4, 5);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//5
	f_timeable = getGLsquareIndices(2, 3, 6, 7);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//6
	f_timeable = getGLsquareIndices(4, 5, 6, 7);
	for (int i = 0; i < 6; i++)
	{
		f_indices.push_back(f_timeable[i]);
	}

	//finishing
	for (int i = 0; i < 36; i++)
	{
		indices[i] = f_indices[i] + number_of_vertices_in_vao;
	}

	//color
	for (int i = 0; i < 8; i++)
	{
		colors[i] = color[i];
	}
}
vector<Vertex> pca::RectangularPrism::getVectoredandVertexedVertices()
{
	vector<Vertex> to_return;

	Vertex* timeable = new Vertex;
	for (int i = 0; i < 8; i++)
	{
		timeable->position = vertices[i];
		timeable->color = colors[i];

		to_return.push_back(*timeable);
	}
	return to_return;
}