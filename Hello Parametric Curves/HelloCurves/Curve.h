#pragma once


//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector> 

#include "Shader.h"

using namespace std;

class Curve
{
public:
	Curve() {}
	inline void setControlPoints(vector <glm::vec3> controlPoints) { this->controlPoints = controlPoints; }
	void setShader(Shader* shader);
	void generateCurve(int pointsPerSegment);
	void drawCurve(glm::vec4 color);
protected:
	vector <glm::vec3> controlPoints;
	vector <glm::vec3> curvePoints;
	glm::mat4 M; //Matriz de geometria
	GLuint VAO;
	Shader* shader;
};

