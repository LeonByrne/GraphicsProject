#include <iostream>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "files.hpp"
#include "model.hpp"

// Camera
static glm::vec3 eye_center(0.0f, 100.0f, 800.0f);
static glm::vec3 lookat(0.0f, 0.0f, 0.0f);
static glm::vec3 up(0.0f, 1.0f, 0.0f);
static float FoV = 45.0f;
static float zNear = 100.0f;
static float zFar = 1500.0f; 

// Lighting  
static glm::vec3 lightIntensity(5e6f, 5e6f, 5e6f);
static glm::vec3 lightPosition(-275.0f, 500.0f, 800.0f);

// Animation 
static bool playAnimation = true;
static float playbackSpeed = 2.0f;

void GLAPIENTRY OpenGLDebugCallback(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, 
	const GLchar* message, const void* userParam)
{
	// Print the error message
  std::cerr << "OpenGL Debug Message:\n";
  std::cerr << "Source: " << source << "\n";
  std::cerr << "Type: " << type << "\n";
  std::cerr << "ID: " << id << "\n";
  std::cerr << "Severity: " << severity << "\n";
  std::cerr << "Message: " << message << "\n\n";
}

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

	// Enable OpenGL debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // To make the callback synchronous

	// Register the debug callback
	// glDebugMessageCallback(OpenGLDebugCallback, nullptr);

	std::cout << "Hello world!" << std::endl;

	// GLuint programID = create_program("../shaders/vertex.glsl", "../shaders/fragment.glsl");

	Model bot("../models/bot.gltf");
	Model bot2("../models/bot.gltf");

	bot2.set_pos(vec3(100, 0, 0));

	Model bot3("../models/CesiumMan.gltf");
	bot3.set_pos(vec3(0, 0, 100));
	bot3.set_scale(vec3(100));
	// model bot("../models/Box.gltf");

	// model bot("../models/AnimatedTriangle.gltf");

	mat4 projection = perspective(
		radians(FoV),
		(float) 800 / 600,
		zNear,
		zFar
	);

	// GLuint modelLoc = glGetUniformLocation(programID, "model");
	// GLuint viewLoc = glGetUniformLocation(programID, "view");
	// GLuint projectionLoc = glGetUniformLocation(programID, "projection");

	// glUseProgram(programID);

	// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mat4(1.0f)[0][0]);
	// glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	// glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

	double lastTime = glfwGetTime();
	float time = 0.0f;			// Animation time 
	float fTime = 0.0f;			// Time for measuring fps
	unsigned long frames = 0;

	do
	{
		glClearColor(0.3f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		float deltaTime = (float) (currentTime - lastTime);
		lastTime = currentTime;

		if(playAnimation)
		{
			time += deltaTime * playbackSpeed;
			bot.update(time);
			bot2.update(time);
			// bot3.update(time);
		}

		mat4 view = lookAt(eye_center, lookat, up);
		mat4 vp = projection * view;

		// bot.render(modelLoc);
		bot.render(vp, lightPosition, lightIntensity);
		bot2.render(vp, lightPosition, lightIntensity);
		bot3.render(vp, lightPosition, lightIntensity);

		frames++;
		fTime += deltaTime;
		if (fTime > 2.0f) {		
			float fps = frames / fTime;
			frames = 0;
			fTime = 0;
			
			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << "Lab 4 | Frames per second (FPS): " << fps;
			glfwSetWindowTitle(window, stream.str().c_str());
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	} while(!glfwWindowShouldClose(window));

	return 0;
}