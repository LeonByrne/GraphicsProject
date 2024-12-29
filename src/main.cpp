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

void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void process_key(GLFWwindow *window, float time);

// Camera
Camera camera(vec3(0, 100.0f, 800.0f));

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

	// Set callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// Model bot("../models/bot.gltf");
	// Model bot2("../models/bot.gltf");

	// bot2.set_pos(vec3(100, 0, 0));

	// Model bot3("../models/CesiumMan.gltf");
	// bot3.set_pos(vec3(0, 0, 100));
	// bot3.set_scale(vec3(100));
	// model bot("../models/Box.gltf");

	// model bot("../models/AnimatedTriangle.gltf");

	Model binary("../models/man/CesiumMan.glb");
	binary.set_pos(vec3(0, 0, 100));
	binary.set_scale(vec3(100));	
	
	// Model camera("../models/AntiqueCamera.glb");
	// camera.set_pos(vec3(0, 0, 100));
	// camera.set_scale(vec3(100));

	mat4 projection = perspective(
		radians(FoV),
		(float) 800 / 600,
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

			binary.update(time);

			// bot.update(time);
			// bot2.update(time);
			// bot3.update(time);
		}

		// mat4 view = lookAt(eye_center, lookat, up);
		mat4 view = camera.get_view();
		mat4 vp = projection * view;

		// bot.render(modelLoc);
		// bot.render(vp, lightPosition, lightIntensity);
		// bot2.render(vp, lightPosition, lightIntensity);
		// bot3.render(vp, lightPosition, lightIntensity);

		binary.render(vp, lightPosition, lightIntensity);
		// camera.render(vp, lightPosition, lightIntensity);

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

void process_key(GLFWwindow *window, float time)
{
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_W, time);
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_A, time);
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_S, time);
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.process_key(GLFW_KEY_D, time);
	}
}

// TODO add camera movements
// TODO add textures