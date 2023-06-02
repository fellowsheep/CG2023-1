/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 02/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STL
#include <vector>

#include <random>
#include <algorithm>

//Classes utilit�rias
#include "Shader.h"

#include "Hermite.h"
#include "Bezier.h"
#include "CatmullRom.h"

using namespace std;

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
vector <glm::vec3> generateControlPointsSet(int nPoints);
vector <glm::vec3> generateControlPointsSet();
GLuint generateControlPointsBuffer(vector <glm::vec3> controlPoints);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 600, HEIGHT = 600;


bool rotateX = false, rotateY = false, rotateZ = false;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Curvas Parametricas!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	
	// Compilando e buildando o programa de shader
	Shader shader = Shader("../shaders/hello.vs", "../shaders/hello.fs");

	
	glUseProgram(shader.ID);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Neste exemplo, a cor da curva � passada como uma vari�vel uniform
	shader.setVec4("finalColor", 0, 0, 0, 1);

	//Conjunto de pontos de controle

	std::vector<glm::vec3> controlPoints = generateControlPointsSet();
	GLuint VAO = generateControlPointsBuffer(controlPoints);

	Hermite hermite;
	hermite.setControlPoints(controlPoints);
	hermite.setShader(&shader);
	hermite.generateCurve(5);

	Bezier bezier;
	bezier.setControlPoints(controlPoints);
	bezier.setShader(&shader);
	bezier.generateCurve(200);

	CatmullRom catmull;
	catmull.setControlPoints(controlPoints);
	catmull.setShader(&shader);
	catmull.generateCurve(100);

	int nbCurvePoints = bezier.getNbCurvePoints();
	int i = 0;

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		
		//glBindVertexArray(VAO);

		//shader.setVec4("finalColor", 0, 0, 0, 1);
		// Chamada de desenho - drawcall
		// CONTORNO e PONTOS - GL_LINE_LOOP e GL_POINTS
		//glDrawArrays(GL_POINTS, 0, controlPoints.size());
		//glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());

		//glBindVertexArray(0);

		//hermite.drawCurve(glm::vec4(1, 0, 0, 1));
		bezier.drawCurve(glm::vec4(0, 1, 0, 1));
		//catmull.drawCurve(glm::vec4(1, 0, 1, 1));

		glm::vec3 pointOnCurve = bezier.getPointOnCurve(i);
		vector <glm::vec3> aux;
		aux.push_back(pointOnCurve);
		GLuint VAOPoint = generateControlPointsBuffer(aux);

		glBindVertexArray(VAOPoint);

		shader.setVec4("finalColor", 0, 0, 0, 1);
		// Chamada de desenho - drawcall
		// CONTORNO e PONTOS - GL_LINE_LOOP e GL_POINTS
		glDrawArrays(GL_POINTS, 0, aux.size());
		//glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
		glBindVertexArray(0);

		i = (i + 1) % nbCurvePoints;

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<glm::vec3> generateControlPointsSet(int nPoints) {
	std::vector<glm::vec3> controlPoints;

	// Gerar n�meros aleat�rios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distribution(-0.9f, 0.9f);  // Intervalo aberto (-0.9, 0.9)

	for (int i = 0; i < nPoints; i++) {
		glm::vec3 point;
		do {
			// Gerar coordenadas x e y aleat�rias
			point.x = distribution(gen);
			point.y = distribution(gen);
		} while (std::find(controlPoints.begin(), controlPoints.end(), point) != controlPoints.end());

		// Definir coordenada z como 0.0
		point.z = 0.0f;

		controlPoints.push_back(point);
	}

	return controlPoints;
}

vector<glm::vec3> generateControlPointsSet()
{
	vector <glm::vec3> controlPoints;

	controlPoints.push_back(glm::vec3(-0.6, -0.4, 0.0));
	controlPoints.push_back(glm::vec3(-0.4, -0.6, 0.0));
	controlPoints.push_back(glm::vec3(-0.2, -0.2, 0.0));
	controlPoints.push_back(glm::vec3(0.0, 0.0, 0.0));
	controlPoints.push_back(glm::vec3(0.2, 0.2, 0.0));
	controlPoints.push_back(glm::vec3(0.4, 0.6, 0.0));
	controlPoints.push_back(glm::vec3(0.6, 0.4, 0.0));

	return controlPoints;
}

GLuint generateControlPointsBuffer(vector <glm::vec3> controlPoints)
{
	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(GLfloat)* 3, controlPoints.data(), GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

