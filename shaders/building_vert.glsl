#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 joint;
layout(location = 4) in vec4 weight;
layout(location = 5) in vec3 offset;

uniform mat4 mvp;

out vec3 worldPos;
out vec3 worldNormal;

void main()
{
	worldPos = pos;
	worldNormal = normal;

	gl_Position = mvp * vec4(pos + offset, 1.0);
}