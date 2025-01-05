#pragma once

#include <vector>

#include <tiny_gltf.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;

#include "Primitive.hpp"

class Building
{
private:
	/* data */
	GLuint mvpID;
	GLuint lightPosID;
	GLuint lightIntensityID;
	GLuint baseColourID;
	GLuint programID;

	tinygltf::Model model;

	std::vector<Primitive> primitives;

	// The model can be moved about
	vec3 pos;
	vec3 scale;
	vec3 rotation;

	int nInstances;

	void bind_model(std::vector<vec3> &offsets);
	void bind_nodes(const tinygltf::Node &node, std::vector<vec3> &offsets);
	void bind_mesh(const tinygltf::Mesh &mesh, std::vector<vec3> &offsets);

	void draw_model();
	void draw_nodes(const tinygltf::Node &node);
	void draw_mesh(const tinygltf::Mesh &mesh);
	void bind_material_colour(const tinygltf::Material &material);

public:
	Building(const std::string &filepath, std::vector<vec3> &offsets);
	~Building();

	void update(const float time);
	void render(const mat4 &vp, const vec3 &lightPos, const vec3 &lightStrength);

	void set_pos(const vec3 &pos);
	void set_rotation(const vec3 &rotation);
	void set_scale(const vec3 &scale);
};
