#include "model.hpp"

#include <iostream>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "files.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Helper functions
glm::mat4 getNodeTransform(const tinygltf::Node& node) {
	glm::mat4 transform(1.0f); 

	if (node.matrix.size() == 16) {
		transform = glm::make_mat4(node.matrix.data());
	} else {
		if (node.translation.size() == 3) {
			transform = glm::translate(transform, glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
		}

		if (node.rotation.size() == 4) {
			glm::quat q(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
			transform *= glm::mat4_cast(q);
		}

		if (node.scale.size() == 3) {
			transform = glm::scale(transform, glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
		}
	}

	return transform;
}

void computeLocalNodeTransform(const tinygltf::Model& model, 
	int nodeIndex, 
	std::vector<glm::mat4> &localTransforms)
{
	const tinygltf::Node &node = model.nodes[nodeIndex];
	localTransforms[nodeIndex] = getNodeTransform(node);

	for(const int child : node.children)
	{
		computeLocalNodeTransform(model, child, localTransforms);
	}
}

void computeGlobalNodeTransform(const tinygltf::Model& model, 
	const std::vector<glm::mat4> &localTransforms,
	int nodeIndex, const glm::mat4& parentTransform, 
	std::vector<glm::mat4> &globalTransforms)
{
	globalTransforms[nodeIndex] = parentTransform * localTransforms[nodeIndex];

	const tinygltf::Node &node = model.nodes[nodeIndex];
	for(const int child : node.children)
	{
		computeGlobalNodeTransform(model, localTransforms, child, globalTransforms[nodeIndex], globalTransforms);
	}
}

int findKeyframeIndex(const std::vector<float>& times, float animationTime) 
{
	int left = 0;
	int right = times.size() - 1;

	while (left <= right) {
		int mid = (left + right) / 2;

		if (mid + 1 < times.size() && times[mid] <= animationTime && animationTime < times[mid + 1]) {
			return mid;
		}	else if (times[mid] > animationTime) {
			right = mid - 1;
		} else { // animationTime >= times[mid + 1]
			left = mid + 1;
		}
	}

	// Target not found
	return times.size() - 2;
}


// Functions of the class

void Model::bind_model()
{
	const tinygltf::Scene &scene = model.scenes[model.defaultScene];

	for(const int node : scene.nodes)
	{
		assert(node >= 0 && node < model.nodes.size());
		bind_nodes(model.nodes[node]);
	}
}

void Model::bind_nodes(const tinygltf::Node &node)
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

void Model::bind_mesh(const tinygltf::Mesh &mesh)
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

void Model::prepare_skinning()
{
	for(const tinygltf::Skin &skin : model.skins)
	{
		Skin preparedSkin;

		// Read inverse bind matrices
		const tinygltf::Accessor &accessor = model.accessors[skin.inverseBindMatrices];
		assert(accessor.type == TINYGLTF_TYPE_MAT4);
		const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
		const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
		const float *ptr = (float *) (buffer.data.data() + accessor.byteOffset + bufferView.byteOffset);

		preparedSkin.inverseBindMatrices.resize(accessor.count);
		for(int i = 0; i < accessor.count; i++)
		{
			float m[16];
			memcpy(m, ptr + i * 16, 16 * sizeof(float));
			preparedSkin.inverseBindMatrices[i] = make_mat4(m);
		}

		assert(skin.joints.size() == accessor.count);
		preparedSkin.globalJointTransforms.resize(skin.joints.size());
		preparedSkin.jointMatrices.resize(skin.joints.size());

		std::vector<mat4> localTransforms(model.nodes.size());
		std::vector<mat4> globalTransforms(model.nodes.size(), mat4(1.0f));

		computeLocalNodeTransform(model, skin.joints[0], localTransforms);
		computeGlobalNodeTransform(model, localTransforms, skin.joints[0], mat4(1.0f), globalTransforms);

		// Compute joint matrices
		for(int i = 0; i < skin.joints.size(); i++)
		{
			int jointIndex = skin.joints[i];
			preparedSkin.jointMatrices[i] = globalTransforms[jointIndex] * preparedSkin.inverseBindMatrices[i];
		}

		skins.push_back(preparedSkin);
	}
}

void Model::prepare_animation()
{
	for(const tinygltf::Animation &animation : model.animations)
	{
		Animation preparedAnimation;

		for(const auto &sampler : animation.samplers)
		{
			Sampler preparedSampler;

			const tinygltf::Accessor &inputAccessor = model.accessors[sampler.input];
			const tinygltf::BufferView &inputBufferView = model.bufferViews[inputAccessor.bufferView];
			const tinygltf::Buffer &inputBuffer = model.buffers[inputBufferView.buffer];

			assert(inputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(inputAccessor.type == TINYGLTF_TYPE_SCALAR);
		
			// Input times
			preparedSampler.input.resize(inputAccessor.count);

			const float *inputPtr = (float *) (&inputBuffer.data[inputBufferView.byteOffset + inputAccessor.byteOffset]);
			const int inputStride = inputAccessor.ByteStride(inputBufferView);
			for(int i = 0; i < inputAccessor.count; i++)
			{
				preparedSampler.input[i] = *(inputPtr + i * inputStride);
			}

			const tinygltf::Accessor &outputAccessor = model.accessors[sampler.output];
			const tinygltf::BufferView &outputBufferView = model.bufferViews[outputAccessor.bufferView];
			const tinygltf::Buffer &outputBuffer = model.buffers[outputBufferView.buffer];

			assert(outputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

			// Output values
			preparedSampler.output.resize(outputAccessor.count);

			// TODO Could stride be used here?
			const float *outputPtr = (float *) (&outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset]);
			const int outputStride = outputAccessor.ByteStride(outputBufferView);
			for(int i = 0; i < outputAccessor.count; i++)
			{
				if(outputAccessor.type == TINYGLTF_TYPE_VEC3)
				{
					memcpy(&preparedSampler.output[i], outputPtr + i * 3 * sizeof(float), 3 * sizeof(float));
				} else if(outputAccessor.type == TINYGLTF_TYPE_VEC4)
				{
					memcpy(&preparedSampler.output[i], outputPtr + i * 4 * sizeof(float), 4 * sizeof(float));
				} else
				{
					std::cout << "Unsupported type" << std::endl;
				}
			}

			preparedAnimation.samplers.push_back(preparedSampler);
		}

		animations.push_back(preparedAnimation);
	}
}

void Model::draw_model()
{
	// TODO what if there's more than one scene?
	const tinygltf::Scene &scene = model.scenes[model.defaultScene];
	for(const int node : scene.nodes)
	{
		draw_nodes(model.nodes[node]);
	}
}

void Model::draw_nodes(const tinygltf::Node &node)
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

void Model::draw_mesh(const tinygltf::Mesh &mesh)
{
	for(int i = 0; i < mesh.primitives.size(); i++)
	{
		GLuint vao = primitives[i].vao;
		std::map<int, GLuint> vbos = primitives[i].vbos; // TODO could be a reference?

		glBindVertexArray(vao);

		const tinygltf::Primitive &primitive = mesh.primitives[i];
		const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

		glDrawElements(
			primitive.mode,
			indexAccessor.count,
			indexAccessor.componentType,
			BUFFER_OFFSET(indexAccessor.byteOffset)
		);

		glBindVertexArray(0);
	}
}

void Model::update_skinning(const std::vector<mat4> &nodeTransforms)
{
	for(int i = 0; i < model.skins.size(); i++)
	{
		Skin &skin = skins[i];

		for(int j = 0; j < skin.jointMatrices.size(); j++)
		{
			int jointIndex = model.skins[i].joints[j];
			skin.jointMatrices[j] = nodeTransforms[jointIndex] * skin.inverseBindMatrices[j];
		}
	}

	std::cout << nodeTransforms[0][0][0] << std::endl;
}

void Model::update_animation(const tinygltf::Animation &anim, const Animation &animation, const float time, std::vector<mat4> &nodeTransforms)
{
	for(const auto &channel : anim.channels)
	{
		int targetNode = channel.target_node;
		const auto &sampler = anim.samplers[channel.sampler];

		// Get output data for channel
		const tinygltf::Accessor &outputAccessor = model.accessors[sampler.output];
		const tinygltf::BufferView &outputBufferView = model.bufferViews[outputAccessor.bufferView];
		const tinygltf::Buffer &outputBuffer = model.buffers[outputBufferView.buffer];

		// Calculate current animation time (wrap if necessary)
		const std::vector<float> &times = animation.samplers[channel.sampler].input;
		float animationTime = fmod(time, times.back());

		// Get keyFrame
		int keyframeIndex = findKeyframeIndex(times, animationTime);

		const float *outputPtr = (float *) (&outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset]);

		// TODO: Add interpolation for smooth animation
		if (channel.target_path == "translation") {
			glm::vec3 translation0, translation1;
			memcpy(&translation0, outputPtr + keyframeIndex * 3 * sizeof(float), 3 * sizeof(float));

			glm::vec3 translation = translation0;
			nodeTransforms[targetNode] = translate(nodeTransforms[targetNode], translation);
		} else if (channel.target_path == "rotation") {
			glm::quat rotation0, rotation1;
			memcpy(&rotation0, outputPtr + keyframeIndex * 4 * sizeof(float), 4 * sizeof(float));

			glm::quat rotation = rotation0;
			nodeTransforms[targetNode] *= mat4_cast(rotation);
		} else if (channel.target_path == "scale") {
			glm::vec3 scale0, scale1;
			memcpy(&scale0, outputPtr + keyframeIndex * 3 * sizeof(float), 3 * sizeof(float));

			glm::vec3 scale = scale0;
			nodeTransforms[targetNode] = glm::scale(nodeTransforms[targetNode], scale);
		}
	}
}

Model::Model(const std::string &filepath)
{
	pos = vec3(0.0f);
	rotation = vec3(0.0f);
	scale = vec3(1.0f);

	assert(load_model(filepath.c_str(), model));

	bind_model();
	prepare_skinning();
	prepare_animation();

	programID = create_program("../shaders/vertex.glsl", "../shaders/fragment.glsl");

	mvpID = glGetUniformLocation(programID, "mvp");
	jointsID = glGetUniformLocation(programID, "joints");
	lightPosID = glGetUniformLocation(programID, "lightPos");
	lightIntensityID = glGetUniformLocation(programID, "lightIntensity");
}

Model::~Model()
{
}

void Model::update(const float time)
{
	std::cout << time << std::endl;

	std::vector<mat4> nodeTransforms(model.nodes.size(), mat4(1.0f));
	update_animation(model.animations[0], animations[0], time, nodeTransforms);

	std::vector<mat4> globalTransforms(model.nodes.size(), mat4(1.0f));
	computeGlobalNodeTransform(model, nodeTransforms, model.skins[0].joints[0], mat4(1.0f), globalTransforms);

	update_skinning(globalTransforms);
}

void Model::render(const mat4 &vp, const vec3 &lightPos, const vec3 &lightStrength)
{
	glUseProgram(programID);

	mat4 transform(1.0f);

	transform = translate(transform, pos);
	// transform = rotate(transform, ) // TODO add rotations in
	transform = glm::scale(transform, scale);

	mat4 mvp = vp * transform;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
	// glUniformMatrix4fv(mvpID, 1, GL_FALSE, &vp[0][0]);

	// TODO move to elsewhere? Maybe where should this go for more complex models?
	// Send joint matices
	// std::cout << skins[0].jointMatrices.size() << std::endl;
	glUniformMatrix4fv(jointsID, skins[0].jointMatrices.size(), GL_FALSE, &skins[0].jointMatrices[0][0][0]);

	// Light data
	glUniform3fv(lightPosID, 1, &lightPos[0]);
	glUniform3fv(lightIntensityID, 1, &lightStrength[0]);

	// Draw model
	draw_model();
}

void Model::set_pos(const vec3 &pos)
{
	this->pos = pos;
}

void Model::set_rotation(const vec3 &rotation)
{
	this->rotation = rotation;
}

void Model::set_scale(const vec3 &scale)
{
	this->scale = scale;
}