#ifndef __SKIN_HPP__
#define __SKIN_HPP__

#include <vector>
#include <glm/glm.hpp>
using namespace glm;

typedef struct
{
	/* data */
	std::vector<mat4> inverseBindMatrices;
	std::vector<mat4> globalJointTransforms;
	std::vector<mat4> jointMatrices;
} Skin;


#endif