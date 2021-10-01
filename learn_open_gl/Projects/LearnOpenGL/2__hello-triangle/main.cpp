
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// set OPEN GL VERSION
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// create window 800x600
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// use created window
	glfwMakeContextCurrent(window);
	// special callback for execution on any event with window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// create vertex program
	const char* vertexShaderSource = "#version 330 core\n"
		// input vector from buffer
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		// set output vector
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// create fragment 
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.4f, 0.1f, 1.0f);\n"
		"}\n\0";

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};


	// create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attaching vertex shader to ID 1 and passing source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile vertexShader for future usage
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	// getting info about vertexShader's compilation
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// attaching fragment to ID 1 and passing source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	// getting info about fragmentShader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::fragmentShader::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	unsigned int shaderProgram = glCreateProgram();

	// attach vertex and fragment
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// compile entire shader program
	glLinkProgram(shaderProgram);

	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// delete fragment and vertex shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// store vertex data
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// create buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy data to memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// draw the object
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// configure glClear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// filling window background with the color (0.2f, 0.3f, 0.3f)
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		// glUseProgram(shaderProgram); // useless, we are already using it
		//glBindVertexArray(VAO); // useless, same
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// refresh window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources
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
