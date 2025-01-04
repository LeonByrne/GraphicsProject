#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;

#include "files.hpp"

class Plane
{
private:
  /* data */
  GLuint programID;
  GLuint textureID;
  GLuint mvpID;
  GLuint vao, vbo;

public:
  Plane(const vec3 &p1, const vec3 &p2, const vec3 &p3, const char *texture_path);
  Plane(const vec3 &p1, const vec3 &p2, const vec3 &p3, const vec2 uv1, const vec2 uv2, const char *texture_path);
  Plane(/* args */);
  ~Plane();

  void render(const mat4 &mvp);
};