#include "Skybox.hpp"
  
Skybox::Skybox(const vec3 &p1, const vec3 &p2, const char *texture_path)
{
  // planes[0] = Plane(p1, vec3(p2.x, p1.y, p1.z), vec3(p2.x, p2.y, p1.z), texture_path);
  planes[0] = Plane(p1, vec3(p2.x, p1.y, p1.z), vec3(p2.x, p2.y, p1.z), vec2(0.25,  2.0 / 3.0), vec2(0.0, 1.0 / 3.0), texture_path);
  planes[1] = Plane(vec3(p1.x, p1.y, p2.z), p1, vec3(p1.x, p2.y, p1.z), vec2(0.5, 2.0 / 3.0), vec2(0.25,  1.0 / 3.0), texture_path);
  planes[2] = Plane(p1, vec3(p1.x, p1.y, p2.z), vec3(p2.x, p1.y, p2.z), vec2(0.25, 1.0),       vec2(0.5,  2.0 / 3.0), texture_path); // Bottom

  planes[3] = Plane(vec3(p1.x, p2.y, p2.z), p2, vec3(p2.x, p1.y, p2.z), vec2(0.5, 1.0 / 3.0), vec2(0.75, 2.0 / 3.0), texture_path);
  planes[5] = Plane(p2, vec3(p2.x, p2.y, p1.z), vec3(p2.x, p1.y, p1.z), vec2(0.75, 1.0 / 3.0), vec2(1.0, 2.0 / 3.0), texture_path);
  planes[4] = Plane(vec3(p2.x, p2.y, p1.z), p2, vec3(p1.x, p2.y, p2.z), vec2(1.0 / 4.0, 0.0), vec2(2.0 / 4.0, 1.0 / 3.0), texture_path); // Top
}

Skybox::Skybox(/* args */)
{
}

Skybox::~Skybox()
{
}

void Skybox::render(const mat4 &vp)
{
  for(int i = 0; i < 6; i++)
  {
    planes[i].render(vp);
  }
}