#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "Plane.hpp"

class Skybox
{
private:
  /* data */
  Plane planes[6];

public:
  Skybox(const vec3 &p1, const vec3 &p2, const char *texture_path);
  Skybox(/* args */);
  ~Skybox();

  void render(const mat4 &vp);
};
