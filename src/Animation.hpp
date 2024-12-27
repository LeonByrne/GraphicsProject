#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include <vector>
#include <string>

#include <glm/glm.hpp>
using namespace glm;

typedef struct 
{
	std::vector<float> input;
	std::vector<vec4> output;
	int interpolation;
} Sampler;

typedef struct
{
	int sampler;
	std::string targetPath;
	int targetNode;
} Channel;

typedef struct
{
	/* data */
	std::vector<Sampler> samplers;
} Animation;
	

#endif