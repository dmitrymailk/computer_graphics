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
	// set window size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// simple render loop
	// we don't want close window immediately 
	while (!glfwWindowShouldClose(window))
	{
		// get events
		processInput(window);
		
		// render commands goes after here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	
		/*
		Double buffer When an application draws in a single buffer the resulting image may display flickering issues. 
		first buffer store image on the screen
		second buffer process 
		*/
		glfwSwapBuffers(window);
		// check all events from keyboard and screen
		glfwPollEvents();
	}
	// clean cache and close program correctly
	glfwTerminate();



	return 0;
}

// callback for size changing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// callback for keyboard input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

