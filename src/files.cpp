#include "files.hpp"

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

/**
 * @brief Attempts to crete a program using shader files.
 * 
 * @param vertex_path 
 * @param fragment_path 
 * @return GLuint -1 on failure.
 */
GLuint create_program(const char *vertex_path, const char *fragment_path)
{
	// Load vertex shader
	std::string vertexCode;
	std::ifstream vertexFile(vertex_path);
	if(vertexFile.is_open())
	{
		std::stringstream sstr;
		sstr << vertexFile.rdbuf();
		vertexCode = sstr.str();
		vertexFile.close();
	} else
	{
		std::cerr << "Failed to open: " << vertex_path << "." << std::endl;
		return -1;
	}

	// Load fragment shader
	std::string fragmentCode;
	std::ifstream fragmentFile(fragment_path);
	if(fragmentFile.is_open())
	{
		std::stringstream sstr;
		sstr << fragmentFile.rdbuf();
		fragmentCode = sstr.str();
		fragmentFile.close();
	} else
	{
		std::cerr << "Failed to open: " << fragment_path << "." << std::endl;
		return -1;
	}

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile vertex shader
	char const *vertexCodePtr = vertexCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexCodePtr, NULL);
	glCompileShader(vertexShaderID);

	// Check vertex shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if(infoLogLength > 0)
	{
		std::vector<char> log(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &log[0]);
		std::cerr << log[0] << std::endl;
	}

	// Compile fragment shader
	char const *fragmentCodePtr = fragmentCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentCodePtr, NULL);
	glCompileShader(fragmentShaderID);

	// Check fragment shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if(infoLogLength > 0)
	{
		std::vector<char> log(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &log[0]);
		std::cerr << log[0] << std::endl;
	}

	// Compile program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if(infoLogLength > 0)
	{
		std::vector<char> log(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &log[0]);
		std::cout << log[0] << std::endl;
	}

	// glDetachShader(programID, vertexShaderID);
	// glDetachShader(programID, vertexShaderID);

	// glDeleteShader(vertexShaderID);
	// glDeleteShader(fragmentShaderID);

	return programID;
}

bool load_model(const char *model_path, tinygltf::Model &model)
{
	tinygltf::TinyGLTF loader;
	std::string err, warn;

	bool success = loader.LoadASCIIFromFile(&model, &err, &warn, model_path);

	if(!success)
		std::cerr << "Failed to load model: " << model_path << std::endl;

	if(!err.empty())
		std::cerr << err << std::endl;

	if(!warn.empty())
		std::cerr << warn << std::endl;
	
	return success;
}