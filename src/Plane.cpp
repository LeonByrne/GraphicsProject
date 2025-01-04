#include "Plane.hpp"

Plane::Plane(const vec3 &p1, const vec3 &p2, const vec3 &p3, const char *texture_path) : Plane(p1, p2, p3, vec2(0, 0), vec2(1, 1), texture_path)
{
  
}

Plane::Plane(const vec3 &p1, const vec3 &p2, const vec3 &p3, const vec2 uv1, const vec2 uv2, const char *texture_path)
{
  const vec3 p4 = p1 + (p3 - p2);

  float vertices[] = {
    p1.x, p1.y, p1.z,   uv1.x, uv1.y,
    p2.x, p2.y, p2.z,   uv2.x, uv1.y,
    p3.x, p3.y, p3.z,   uv2.x, uv2.y,
    p4.x, p4.y, p4.z,   uv1.x, uv2.y
  };

  // float texCoords[] = {
  //   uv1.x, uv1.y,
  //   uv2.x, uv2.y,
  //   uv2.x, uv1.y,
  //   uv1.x, uv2.y
  // };

  programID = create_program("plane_vert.glsl", "plane_frag.glsl");
  mvpID = glGetUniformLocation(programID, "mvp");
  textureID = create_texture(texture_path);

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void Plane::render(const mat4 &mvp)
{
  glUseProgram(programID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);
}

Plane::~Plane()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &textureID);
  glDeleteProgram(programID);
}
