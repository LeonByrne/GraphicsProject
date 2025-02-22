#pragma once

#include <vector>

#include <glm/glm.hpp>
using namespace glm;

#include "Building.hpp"

class Block
{
private:
	/* data */
	vec3 pos;
	vec3 scale;

	std::vector<Building> buildings;
	std::vector<Building> roads;

	void place_horizontal(const vec3 &start, const int length, std::vector<vec3> &offsets);
	void place_vertical(const vec3 &start, const int length, std::vector<vec3> &offsets);
	void place_intersection(const vec3 &pos, std::vector<vec3> &offsets);
	void place_roundabout(const vec3 &pos, std::vector<vec3> &offsets);

public:
	Block(std::vector<vec3> &offsets);
	~Block();

	void render(const mat4 &vp, const vec3 &lightPos, const vec3 &lightIntensity);

	void set_pos(const vec3 &pos);
	void set_rotation(const vec3 &rotation);
	void set_scale(const vec3 &scale);
};
