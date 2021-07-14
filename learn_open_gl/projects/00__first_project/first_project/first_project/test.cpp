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
	
	// простой render loop
	// мы не хотим чтобы приложение сразу закрылось, поэтому выставляем некоторое условие
	while (!glfwWindowShouldClose(window))
	{
		/*
		механизм отрисовки окна double buffer.
		Почему double buffer, потому что если бы у нас был один массив, который бы отрисовывал
		и изменял единый buffer, это повлекло бы за собой фризы на экране. 
		Поэтому есть 2 buffer, один который отрисовывается, другой который обрабатывается
		и готовится к отрисовке.
		*/
		glfwSwapBuffers(window);
		
		// проверяет все events с клавиатуры и экрана
		glfwPollEvents();
	}
	// чистит кеш и корректно завершает работу с glfw
	glfwTerminate();



	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{	
	// callback for resizing window
	glViewport(0, 0, width, height);
}
