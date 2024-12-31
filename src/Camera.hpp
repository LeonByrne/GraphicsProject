#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>
using namespace glm;

class Camera
{
private:
	/* data */
	vec3 pos;
	vec3 front;
	vec3 up;
	vec3 right;

	vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed = 500.0f;
	float mouseSensitivity = 0.1f;

public:
	Camera(const vec3 &pos);
	~Camera();

	void process_key(int key, float time);
	void process_mouse(const float xOffset, const float yOffset);
	void update_vectors();
	mat4 get_view();
};


#endif