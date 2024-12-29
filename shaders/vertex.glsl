#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 joint;
layout(location = 4) in vec4 weight;

out vec3 worldPos;
out vec3 worldNormal;
out vec2 tex;

uniform mat4 mvp;
uniform mat4 joints[100];

void main()
{
  mat4 skin =
    weight.x * joints[int(joint.x)] +
    weight.y * joints[int(joint.y)] +
    weight.z * joints[int(joint.z)] +
    weight.w * joints[int(joint.w)];

	if(skin == mat4(0.0))
	{
		gl_Position = mvp * vec4(pos, 1.0);
	} else 
	{
	  gl_Position = mvp * skin * vec4(pos, 1.0);
	}

  // gl_Position = (MVP * skin * vec4(vertexPosition, 1.0)) * 1000;
  // gl_Position = mvp * skin * vec4(pos, 1.0);

	// gl_Position = mvp * vec4(pos, 1.0);

  worldPos = pos;
  worldNormal = normal;

	tex = uv;
}