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

	// ----------------- INPUT VERTICES ----------------------
	// draw triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int VBO;
	// create buffer object by ID stored in VBO
	glGenBuffers(1, &VBO);

	// vertex array object(VAO)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// specifying type of buffer object(GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*
	copying data into buffer object by VBO's id
	there are many types:
	• GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	• GL_STATIC_DRAW: the data is set only once and used many times.
	• GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ----------------- VERTEX SHADER ----------------------
	// vertex shader code
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// create vertex shader object and assign ID
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach source code to vertex shader object ID
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check compile error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::vertexShader::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// ----------------- FRAGMENT SHADER ----------------------
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::fragmentShader::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// ----------------- SHADER PROGRAM ----------------------
	// create shader program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}


	// first argument associate with layout (location = 0) in vertex shader
	// 3 * sizeof(float) - step for access array data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// delete useless shaders 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// simple render loop
	// we don't want close window immediately 
	while (!glfwWindowShouldClose(window))
	{
		// get events
		processInput(window);

		// render commands goes after here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//  activate shader program
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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

