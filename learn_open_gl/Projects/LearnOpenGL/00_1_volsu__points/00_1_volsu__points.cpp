//Компьютерная графика.Лабораторная работа №0.
//Рисование точек, заданных пользователем.
//Язык реализации : С, С++ или C#.
//Задача.
//Реализовать оконное приложение, имеющее следующий функционал :
//1. По нажатию левой клавиши мыши :
//В позиции указателя рисуется пиксель черного цвета.
//2. По нажатию правой кнопки мыши :
//Все точки меняют цвет на случайный.


//Компьютерная графика.Лабораторная работа №1.
//Рисование замкнутой ломаной по точкам, заданным пользователем, используя алгоритм Брезенхэма.
//
//Задача.
//Реализовать функцию, которая отображает отрезок попиксельно, используя алгоритм Брезенхэма. 
//Использовать её, чтобы создать оконное приложение, имеющее следующий функционал :
//1. По нажатию левой клавиши мыши :
//В позиции указателя рисуется пиксель черного цвета.
//2. По нажатию правой кнопки мыши :
//Если ранее уже была отрисована ломаная, она стирается.Поставленные ранее точки соединяются 
//линией красного цвета в порядке их нажатия.То есть первая со второй, вторая с третьей и т.д. 
//Последняя точка соединяется с первой.

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <time.h>

#include <vector>


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// generate number between 0.0 and 1.0
float generate_rand_num()
{
	return (float)rand() / RAND_MAX;
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool firstMouse = true;

double mousePosX = 0.0;
double mousePosY = 0.0;


vector<float> vertices = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // top 
};

unsigned int VBO, VAO;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_PROGRAM_POINT_SIZE);
	srand((unsigned)time(NULL));
	glLineWidth(5.0);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("shader.vert", "shader.frag");


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	ourShader.use();
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// ENABLE FOR LAB 00
		glDrawArrays(GL_POINTS, 0, vertices.size() / 6);
		//glEnableClient
		// ENABLE FOR LAB 01
		//glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	/*cout
		<< "xpos " << ((double)(xpos / (SCR_WIDTH)) - 0.5f) * 2 << " "
		<< "ypos " << ((double)((SCR_HEIGHT - ypos) / (SCR_HEIGHT)) - 0.5f) * 2 << "\n";*/
	mousePosX = ((double)(xpos / (SCR_WIDTH)) - 0.5f) * 2;
	mousePosY = ((double)((SCR_HEIGHT - ypos) / (SCR_HEIGHT)) - 0.5f) * 2;
}

void copy_vertices_to_memory()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		std::cout << "GLFW_MOUSE_BUTTON_RIGHT \n";
		std::cout << vertices.size() / 6 << "\n";
		for (int i = 0; i < vertices.size() / 6; i++)
		{
			vertices[i * 6 + 3] = generate_rand_num();
			vertices[i * 6 + 4] = generate_rand_num();
			vertices[i * 6 + 5] = generate_rand_num();
			//for (int j = i; j < i + 6; j++)
			//	cout << vertices[j] << " ";
			//cout << "\n";
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "GLFW_MOUSE_BUTTON_LEFT\n";
		double xPos = mousePosX;
		double yPos = mousePosY;
		vertices.push_back(xPos);
		vertices.push_back(yPos);
		vertices.push_back(0.0);

		vertices.push_back(0.0);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		/*vertices.push_back(xPos);
		vertices.push_back(yPos);
		vertices.push_back(0.0);

		vertices.push_back(0.0);
		vertices.push_back(0.0);
		vertices.push_back(0.0);*/

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	}

}