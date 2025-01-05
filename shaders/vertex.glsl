#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 joint;
layout(location = 4) in vec4 weight;
layout(location = 5) in vec3 offset;

out vec3 worldPos;
out vec3 worldNormal;
out vec2 tex;

uniform mat4 mvp;
uniform mat4 joints[100];

void main()
{
  if(weight.x + weight.y + weight.z + weight.w < 0.001)
  {
    gl_Position = mvp * vec4(pos + offset * 200, 1.0);
  } else
  {
    mat4 skin =
      weight.x * joints[int(joint.x)] +
      weight.y * joints[int(joint.y)] +
      weight.z * joints[int(joint.z)] +
      weight.w * joints[int(joint.w)];

    gl_Position = mvp * vec4((skin * vec4(pos, 1.0)).xyz + offset * 200, 1.0);
  }

  worldPos = pos;
  worldNormal = normal;

	tex = uv;
}