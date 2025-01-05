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
	Building b1("../models/Buildings/skyscraperC.glb", offsets);
	b1.set_pos(vec3(7, 0, 7));
	b1.set_rotation(vec3(0, 0, 0));
	buildings.push_back(b1);

	Building b2("../models/Buildings/large_buildingA.glb", offsets);
	b2.set_pos(vec3(3.5, 0, 1.5));
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

	Building b9("../models/Buildings/large_buildingE.glb", offsets);
	b9.set_pos(vec3(1.35, 0, 8.75));
	buildings.push_back(b9);

	Building b10("../models/Buildings/large_buildingE.glb", offsets);
	b10.set_pos(vec3(2.5, 0, 8.75));
	buildings.push_back(b10);

	Building b11("../models/Buildings/large_buildingE.glb", offsets);
	b11.set_pos(vec3(3.65, 0, 8.75));
	buildings.push_back(b11);

	Building b12("../models/Buildings/large_buildingD.glb", offsets);
	b12.set_pos(vec3(1.25, 0, 6.75));
	b12.set_rotation(vec3(0, -90, 0));
	buildings.push_back(b12);

	Building b13("../models/Buildings/large_buildingD.glb", offsets);
	b13.set_pos(vec3(6.25, 0, 3.25));
	b13.set_rotation(vec3(0, -90, 0));
	buildings.push_back(b13);

	Building b14("../models/Buildings/large_buildingA.glb", offsets);
	b14.set_pos(vec3(6.25, 0, 1.5));
	b14.set_rotation(vec3(0, -90, 0));
	buildings.push_back(b14);

	Building b15("../models/Buildings/large_buildingB.glb", offsets);
	b15.set_pos(vec3(9, 0, 1));
	b15.set_rotation(vec3(0, 90, 0));
	buildings.push_back(b15);

	Building b16("../models/Buildings/large_buildingB.glb", offsets);
	b16.set_pos(vec3(9, 0, 1.75));
	b16.set_rotation(vec3(0, 90, 0));
	buildings.push_back(b16);

	Building b17("../models/Buildings/large_buildingB.glb", offsets);
	b17.set_pos(vec3(9, 0, 2.5));
	b17.set_rotation(vec3(0, 90, 0));
	buildings.push_back(b17);

	Building b18("../models/Buildings/large_buildingB.glb", offsets);
	b18.set_pos(vec3(9, 0, 3.25));
	b18.set_rotation(vec3(0, 90, 0));
	buildings.push_back(b18);

	Building b19("../models/Buildings/large_buildingB.glb", offsets);
	b19.set_pos(vec3(9, 0, 4));
	b19.set_rotation(vec3(0, 90, 0));
	buildings.push_back(b19);


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