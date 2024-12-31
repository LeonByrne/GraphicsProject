#include "Building.hpp"

#include <iostream>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "files.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Functions of the class

void Building::bind_model()
{
	const tinygltf::Scene &scene = model.scenes[model.defaultScene];

	for(const int node : scene.nodes)
	{
		assert(node >= 0 && node < model.nodes.size());
		bind_nodes(model.nodes[node]);
	}
}

void Building::bind_nodes(const tinygltf::Node &node)
{
	// bind this nodes mesh
	if(node.mesh >= 0 && node.mesh < model.meshes.size())
	{
		bind_mesh(model.meshes[node.mesh]);
	}

	// Bind children and their meshes
	for(const int child : node.children)
	{
		assert(child >= 0 && child < model.nodes.size());
		bind_nodes(model.nodes[child]);
	}
}

void Building::bind_mesh(const tinygltf::Mesh &mesh)
{
	std::map<int, GLuint> vbos;
	for(int i = 0; i < model.bufferViews.size(); i++)
	{
		const tinygltf::BufferView &bufferView = model.bufferViews[i];
		int target = bufferView.target;

		// A bufferView of target 0 is for skinning weights
		if(target == 0)
		{
			continue;
		}

		const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(target, vbo);
		glBufferData(
			target,
			bufferView.byteLength,
			&buffer.data.at(0) + bufferView.byteOffset,
			GL_STATIC_DRAW
		);

		vbos[i] = vbo;
	}

	for(const tinygltf::Primitive &primitive : mesh.primitives)
	{
		tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		for(auto &attrib : primitive.attributes)
		{
			tinygltf::Accessor accessor = model.accessors[attrib.second];
			int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
			glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

			int size = 1;
			if(accessor.type != TINYGLTF_TYPE_SCALAR)
			{
				size = accessor.type;
			}

			int vaa = -1;
			if (attrib.first.compare("POSITION") == 0) vaa = 0;
			if (attrib.first.compare("NORMAL") == 0) vaa = 1;
			if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
			if (attrib.first.compare("JOINTS_0") == 0) vaa = 3;
			if (attrib.first.compare("WEIGHTS_0") == 0) vaa = 4;

			if(vaa > -1)
			{
				glEnableVertexAttribArray(vaa);
				glVertexAttribPointer(
					vaa,
					size,
					accessor.componentType,
					accessor.normalized ? GL_TRUE : GL_FALSE,
					byteStride,
					BUFFER_OFFSET(accessor.byteOffset)
				);
			} else
			{
				std::cout << "vaa missing, not sure what to do here :|" << std::endl;
			}
		}

		// Record vao
		Primitive prim;
		prim.vao = vao;
		prim.vbos = vbos;
		primitives.push_back(prim);

		glBindVertexArray(0);
	}
}

void Building::draw_model()
{
	// TODO what if there's more than one scene?
	const tinygltf::Scene &scene = model.scenes[model.defaultScene];
	for(const int node : scene.nodes)
	{
		draw_nodes(model.nodes[node]);
	}
}

void Building::draw_nodes(const tinygltf::Node &node)
{
	if(node.mesh >= 0 && node.mesh < model.meshes.size())
	{
		draw_mesh(model.meshes[node.mesh]);
	}

	for(const int child : node.children)
	{
		draw_nodes(model.nodes[child]);
	}
}

void Building::draw_mesh(const tinygltf::Mesh &mesh)
{
	for(int i = 0; i < mesh.primitives.size(); i++)
	{
		GLuint vao = primitives[i].vao;
		std::map<int, GLuint> vbos = primitives[i].vbos; // TODO could be a reference?

		glBindVertexArray(vao);

		const tinygltf::Primitive &primitive = mesh.primitives[i];
		const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

		// Bind textures
		const auto &material = model.materials[mesh.primitives[i].material];
		bind_material_colour(material);

		glDrawElements(
			primitive.mode,
			indexAccessor.count,
			indexAccessor.componentType,
			BUFFER_OFFSET(indexAccessor.byteOffset)
		);

		glBindVertexArray(0);
	}
}

void Building::bind_material_colour(const tinygltf::Material &material)
{
	vec4 baseColour(0.0f);
	if(!material.pbrMetallicRoughness.baseColorFactor.empty())
	{
		const std::vector<double> &base = material.pbrMetallicRoughness.baseColorFactor;
		baseColour = vec4(base[0], base[1], base[2], base[3]);
	}

	glUniform4fv(baseColourID, 1, &baseColour[0]);
}

Building::Building(const std::string &filepath)
{
	pos = vec3(0.0f);
	rotation = vec3(0.0f);
	scale = vec3(1.0f);

	assert(load_model(filepath.c_str(), model));

	bind_model();

	programID = create_program("../shaders/building_vert.glsl", "../shaders/building_frag.glsl");

	mvpID = glGetUniformLocation(programID, "mvp");
	lightPosID = glGetUniformLocation(programID, "lightPos");
	lightIntensityID = glGetUniformLocation(programID, "lightIntensity");
	baseColourID = glGetUniformLocation(programID, "baseColourFactor");
}

Building::~Building()
{
}

void Building::update(const float time)
{
}

void Building::render(const mat4 &vp, const vec3 &lightPos, const vec3 &lightStrength)
{
	glUseProgram(programID);

	mat4 transform(1.0f);

	transform = translate(transform, pos);
	transform = rotate(transform, rotation.x, vec3(1, 0, 0));
	transform = rotate(transform, rotation.y, vec3(0, 1, 0));
	transform = rotate(transform, rotation.z, vec3(0, 0, 1));
	transform = glm::scale(transform, scale);

	mat4 mvp = vp * transform;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	// Light data
	glUniform3fv(lightPosID, 1, &lightPos[0]);
	glUniform3fv(lightIntensityID, 1, &lightStrength[0]);

	// Draw model
	draw_model();
}

void Building::set_pos(const vec3 &pos)
{
	this->pos = pos;
}

void Building::set_rotation(const vec3 &rotation)
{
	this->rotation = vec3(radians(rotation.x), radians(rotation.y), radians(rotation.z));
}

void Building::set_scale(const vec3 &scale)
{
	this->scale = scale;
}