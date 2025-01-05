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
#include "Camera.hpp"
#include "Building.hpp"
#include "Block.hpp"
#include "Plane.hpp"
#include "Skybox.hpp"

void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_key(GLFWwindow *window, float time);

// Camera
Camera camera(vec3(0.0f));

// Project matrix stuff
int screenWidth = 800;
int screenHeight = 600;
static float FoV = 45.0f;
static float zNear = 0.1f;
static float zFar = 2000.0f; 

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

	// Set callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Turn off for debugging

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

	std::vector<vec3> offsets;
	for(int x = -400; x <= 400; x += 10)
	{
		for(int z = -400; z <= 400; z += 10)
		{
			offsets.push_back(vec3(x, 0, z));
		}
	}

	// Don't need as many bots as buildings
	std::vector<vec3> offsets1;
	for(int x = -100; x <= 100; x += 10)
	{
		for(int z = -100; z <= 100; z += 10)
		{
			offsets1.push_back(vec3(x, 0, z));
		}
	}

	Block block(offsets);
	block.set_scale(vec3(1));
	block.set_pos(vec3(0));

	Skybox sky(vec3(-1000), vec3(1000), "../textures/DaylightBox.png");

	Plane ground(vec3(-2000, -0.01, -2000), vec3(-2000, -0.01, 2000), vec3(2000, -0.01, 2000), vec2(-2000, -2000), vec2(2000, 2000), "../textures/smooth-gray-stucco-wall.jpg");

	Model person("../models/People/walking_man_2.glb", offsets);
	person.set_scale(vec3(0.0001));

	Model bot("../models/Bot/bot.gltf", offsets1);
	bot.set_pos(vec3(1.5, -0.15, 2));
	bot.set_scale(vec3(0.005f));

	// Building building("../models/Buildings/skyscraperC.glb", offsets);

	mat4 projection = perspective(
		radians(FoV),
		(float) screenWidth / screenHeight,
		zNear,
		zFar
	);

	double lastTime = glfwGetTime();
	float time = 0.0f;			// Animation time 
	float fTime = 0.0f;			// Time for measuring fps
	unsigned long frames = 0;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	do
	{
		glClearColor(0.3f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		float deltaTime = (float) (currentTime - lastTime);
		lastTime = currentTime;

		process_key(window, deltaTime);

		if(playAnimation)
		{
			time += deltaTime * playbackSpeed;

			bot.update(time);
		}

		mat4 view = camera.get_view();
		mat4 vp = projection * view;

		block.render(vp, lightPosition, lightIntensity);
		ground.render(vp);
		sky.render(vp);

		bot.render(vp, lightPosition, lightIntensity);

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
	} while(!(glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE)));

	return 0;
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
	static double lastX = xPos;
	static double lastY = yPos;

	float xOffset = xPos - lastX;
	float yOffset = yPos - lastY;

	lastX = xPos;
	lastY = yPos;

	camera.process_mouse(xOffset, yOffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // Adjust the viewport to the new window size
  glViewport(0, 0, width, height);

	screenWidth = width;
	screenHeight = height;
}

void process_key(GLFWwindow *window, float time)
{
	// Forward
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_W, time);
	} else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_UP, time);
	}

	// Left
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_A, time);
	} else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_LEFT, time);
	}

	// Backward
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_S, time);
	} else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_DOWN, time);
	}

	// Right
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_D, time);
	} else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_RIGHT, time);
	}

	// Up
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_SPACE, time);
	}

	// Down
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_LEFT_SHIFT, time);
	}
}