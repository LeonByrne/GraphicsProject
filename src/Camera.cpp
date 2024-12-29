#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

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

	if(key == GLFW_KEY_W)
	{
		pos += front * velocity;
	}

	if(key == GLFW_KEY_S)
	{
		pos -= front * velocity;
	}

	if(key == GLFW_KEY_A)
	{
		pos -= right * velocity;
	}

	if(key == GLFW_KEY_D)
	{
		pos += right * velocity;
	}

	std::cout << "x: " << pos.x << "y: " << pos.y << "z: " << pos.z << std::endl;
}

void Camera::process_mouse(const float xOffset, const float yOffset)
{
	yaw += xOffset * mouseSensitivity;
	pitch += yOffset * mouseSensitivity;

	if(pitch > 89.0f)
	{
		pitch = 89.0f;
	} else if(pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	// std::cout << "yaw: " << yaw << "pitch: " << pitch << std::endl;
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

	// std::cout << front.x << " " << front.y << " " << front.z << std::endl;
}

mat4 Camera::get_view()
{
	return lookAt(pos, pos + front, up);
}