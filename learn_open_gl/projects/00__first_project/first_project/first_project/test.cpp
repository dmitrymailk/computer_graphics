#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	// ��������� 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ������� render loop
	// �� �� ����� ����� ���������� ����� ���������, ������� ���������� ��������� �������
	while (!glfwWindowShouldClose(window))
	{
		/*
		�������� ��������� ���� double buffer.
		������ double buffer, ������ ��� ���� �� � ��� ��� ���� ������, ������� �� �����������
		� ������� ������ buffer, ��� �������� �� �� ����� ����� �� ������.
		������� ���� 2 buffer, ���� ������� ��������������, ������ ������� ��������������
		� ��������� � ���������.
		*/
		glfwSwapBuffers(window);

		// ��������� ��� events � ���������� � ������
		glfwPollEvents();
	}
	// ������ ��� � ��������� ��������� ������ � glfw
	glfwTerminate();



	return 0;
}

// callback ��� ��������� ������� ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ���������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

