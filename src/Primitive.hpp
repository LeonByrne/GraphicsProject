#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include <map>

#include <GL/glew.h>

struct Primitive
{
	/* data */
	GLuint vao;
	std::map<int, GLuint> vbos;

	// Do some have indices?
};


#endif