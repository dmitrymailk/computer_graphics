#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


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
	// set callback
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{	
	// callback for resizing window
	glViewport(0, 0, width, height);
}
