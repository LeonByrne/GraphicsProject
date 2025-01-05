#include "Block.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Block::place_horizontal(const vec3 &start, const int length, std::vector<vec3> &offsets)
{
	Building road("../models/Roads/road_straight.glb", offsets);

	for(int i = 0; i < length; i++)
	{
		road.set_pos(vec3(start.x + i, start.y, start.z));
		roads.push_back(road);
	}
}

void Block::place_vertical(const vec3 &start, const int length, std::vector<vec3> &offsets)
{
	Building road("../models/Roads/road_straight.glb", offsets);
	road.set_rotation(vec3(0, 90, 0));

	for(int i = 0; i < length; i++)
	{
		road.set_pos(vec3(start.x, start.y, start.z + i));
		roads.push_back(road);
	}
}

void Block::place_intersection(const vec3 &pos, std::vector<vec3> &offsets)
{
	Building road("../models/Roads/road_crossroad.glb", offsets);
	road.set_pos(pos);

	roads.push_back(road);
}

void Block::place_roundabout(const vec3 &pos, std::vector<vec3> &offsets)
{
	Building road("../models/Roads/road_roundabout.glb", offsets);
	road.set_pos(pos);

	roads.push_back(road);
}

Block::Block(std::vector<vec3> &offsets)
{
	// Building b1("../models/Buildings/large_buildingA.glb");
	// b1.set_pos(vec3(100, 0, 100));
	// b1.set_scale(vec3(10));

	// Building r1("../models/Roads/road_straight.glb");
	// r1.set_pos(vec3(2, 0, 0));

	// Building r2("../models/Roads/road_straight.glb");
	// r2.set_pos(vec3(3, 0, 0));

	// buildings.push_back(b1);

	// roads.push_back(r1);
	// roads.push_back(r2);
	// roads.push_back(r2);
	// roads.push_back(r2);

	Building b1("../models/Buildings/skyscraperC.glb", offsets);
	b1.set_pos(vec3(8, 0, 8));
	b1.set_rotation(vec3(0, 180, 0));
	buildings.push_back(b1);

	Building b2("../models/Buildings/large_buildingA.glb", offsets);
	b2.set_pos(vec3(3.5, 0, 1.75));
	b2.set_rotation(vec3(0, 90, 0));
	buildings.push_back(b2);

	Building b3("../models/Buildings/large_buildingC.glb", offsets);
	b3.set_pos(vec3(1, 0, 4));
	buildings.push_back(b3);

	Building b4("../models/Buildings/large_buildingC.glb", offsets);
	b4.set_pos(vec3(1.75, 0, 4));
	buildings.push_back(b4);

	Building b5("../models/Buildings/large_buildingC.glb", offsets);
	b5.set_pos(vec3(2.5, 0, 4));
	buildings.push_back(b5);

	Building b6("../models/Buildings/large_buildingC.glb", offsets);
	b6.set_pos(vec3(3.25, 0, 4));
	buildings.push_back(b6);

	Building b7("../models/Buildings/large_buildingF.glb", offsets);
	b7.set_pos(vec3(1.5, 0, 1));
	b7.set_rotation(vec3(0, 180, 0));
	buildings.push_back(b7);

	Building b8("../models/Buildings/large_buildingG.glb", offsets);
	b8.set_pos(vec3(3, 0, 7));
	b8.set_rotation(vec3(0, 90, 0));
	buildings.push_back(b8);


	place_horizontal(vec3(1, 0, 0), 4, offsets);
	place_horizontal(vec3(6, 0, 0), 4, offsets);
	place_horizontal(vec3(1, 0, 5), 3, offsets);
	place_horizontal(vec3(7, 0, 5), 3, offsets);

	place_vertical(vec3(0, 0, 1), 4, offsets);
	place_vertical(vec3(0, 0, 6), 4, offsets);
	place_vertical(vec3(5, 0, 1), 3, offsets);
	place_vertical(vec3(5, 0, 7), 3, offsets);

	place_intersection(vec3(0, 0, 0), offsets);
	place_intersection(vec3(5, 0, 0), offsets);
	place_intersection(vec3(0, 0, 5), offsets);

	place_roundabout(vec3(5, 0, 5), offsets);
}

Block::~Block()
{
}

void Block::render(const mat4 &vp, const vec3 &lightPos, const vec3 &lightIntensity)
{
	mat4 transform(1.0f);
	transform = translate(transform, pos);
	transform = glm::scale(transform, scale);

	mat4 mvp = vp * transform;

	for(Building &building : buildings)
	{
		building.render(mvp, lightPos, lightIntensity);
	}

	for(Building &road : roads)
	{
		road.render(mvp, lightPos, lightIntensity);
	}
}

void Block::set_pos(const vec3 &pos)
{
	this->pos = pos;
}

void Block::set_rotation(const vec3 &rotation)
{

}

void Block::set_scale(const vec3 &scale)
{
	this->scale = scale;
}