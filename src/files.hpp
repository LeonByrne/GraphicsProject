#ifndef __FILES_HPP__
#define __FILES_HPP__

#include <GL/glew.h>
#include <tiny_gltf.h>

GLuint create_program(const char *vertex_path, const char *fragment_path);

bool load_model(const char *model_path, tinygltf::Model &model);

#endif