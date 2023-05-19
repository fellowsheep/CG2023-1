#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


class Mesh
{
public:
	Mesh() {}
	~Mesh() {}
	void initialize(GLuint VAO, int nVertices, Shader* shader, GLuint texID);
	void update();
	void draw();

protected:
	GLuint VAO; //Identificador do Vertex Array Object - V�rtices e seus atributos
	int nVertices;

	//Refer�ncia (endere�o) do shader
	Shader* shader;
	GLuint texID; //id da textura
};

