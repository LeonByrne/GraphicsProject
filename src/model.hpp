#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <tiny_gltf.h>
#include <GL/glew.h>
#include <vector>

#include "Primitive.hpp"
#include "Animation.hpp"
#include "Skin.hpp"

class Model
{
private:
	/* data */
	GLuint mvpID;
	GLuint jointsID;
	GLuint lightPosID;
	GLuint lightIntensityID;
	GLuint programID;

	tinygltf::Model model;

	std::vector<Primitive> primitives;
	std::vector<Skin> skins;
	std::vector<Animation> animations;
	std::vector<GLuint> textures;

	// TODO use these for instancing
	// The model can be moved about
	vec3 pos;
	vec3 scale;
	vec3 rotation;

	int nInstances;

	void bind_model(std::vector<vec3> &offsets);
	void bind_nodes(const tinygltf::Node &node, std::vector<vec3> &offsets);
	void bind_mesh(const tinygltf::Mesh &mesh, std::vector<vec3> &offsets);
	void bind_textures();
	void prepare_skinning();
	void prepare_animation();

	void draw_model();
	void draw_nodes(const tinygltf::Node &node);
	void draw_mesh(const tinygltf::Mesh &mesh);
	void bind_material_textures(const tinygltf::Material &material);
	void update_skinning(const std::vector<mat4> &nodeTransforms);
	void update_animation(const tinygltf::Animation &anim, const Animation &animation, const float time, std::vector<mat4> &nodeTransforms);

public:
	Model(const std::string &filepath, std::vector<vec3> &offsets);
	~Model();

	void update(const float time);
	void render(const mat4 &vp, const vec3 &lightPos, const vec3 &lightStrength);

	void set_pos(const vec3 &pos);
	void set_rotation(const vec3 &rotation);
	void set_scale(const vec3 &scale);
};



#endif