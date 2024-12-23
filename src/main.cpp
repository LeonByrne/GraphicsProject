#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "files.hpp"

int main()
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return -1;
	}

	GLFWwindow *window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
	if(!window)
	{
		std::cerr << "Failed to create GLFWwindow" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	std::cout << "Hello world!" << std::endl;

	GLuint programID = create_program("../shaders/vertex.glsl", "../shaders/fragment.glsl");

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.3f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	return 0;
}