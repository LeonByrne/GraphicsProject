#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

#define BLOCK_SIZE 10

Camera::Camera(const vec3 &pos)
{
	this->pos = pos;
	front = vec3(0, 0, -0.1f);

	yaw = -90.0f;
	pitch = 0.0f;

	worldUp = vec3(0.0f, 1.0, 0.0f);

	update_vectors();
}

Camera::~Camera()
{
}

void Camera::process_key(int key, float time)
{
	float velocity = movementSpeed * time;

	if(key == GLFW_KEY_W || key == GLFW_KEY_UP)
	{
		pos += front * velocity;
	}

	if(key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
	{
		pos -= front * velocity;
	}

	if(key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		pos -= right * velocity;
	}

	if(key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		pos += right * velocity;
	}

	if(key == GLFW_KEY_SPACE)
	{
		pos += worldUp * velocity;
	}

	if(key == GLFW_KEY_LEFT_SHIFT)
	{
		pos -= worldUp * velocity;
	}

	// Code so the city loops seemlessly
	if(pos.x > 2000)
	{
		pos.x -= 2000;
	} else if(pos.x < -2000)
	{
		pos.x += 2000;
	}

	if(pos.z > 2000)
	{
		pos.z -= 2000;
	} else if(pos.z < -2000)
	{
		pos.z += 2000;
	}
}

void Camera::process_mouse(const float xOffset, const float yOffset)
{
	yaw += xOffset * mouseSensitivity;
	pitch -= yOffset * mouseSensitivity;

	if(pitch > 89.0f)
	{
		pitch = 89.0f;
	} else if(pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	update_vectors();
}

void Camera::update_vectors()
{
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = normalize(front);

	right = normalize(cross(front, worldUp));
	up = normalize(cross(right, front));
}

mat4 Camera::get_view()
{
	return lookAt(pos, pos + front, up);
}