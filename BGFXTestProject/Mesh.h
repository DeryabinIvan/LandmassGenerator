#pragma once

#include <vector>

#include "glm/glm.hpp"
#include <bgfx/bgfx.h>

struct Point {
	float x, y, z;
	uint32_t abgr;
};

class Mesh {
	private:
	uint16_t width, height;
	
	size_t pCount;
	Point* points;
	
	size_t tCount;
	uint32_t* triangles;
	
	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	glm::mat4 modelMTX;

	public:
	Mesh();
	Mesh(int w, int h);
	Mesh(Point* points, int pCount, int* tris, int tCount);

	const size_t getWidth();
	const size_t getHeight();

	void generateMesh(int w, int h);
	void createHeightMap();

	void createBuffers();
	void bindBuffers();

	void destroyBuffers();

	const glm::mat4 getModelMatrix();
	void resetModelMTX();

	void translate(float x, float y, float z);

	void rotateX(float x);
	void rotateY(float y);
	void rotateZ(float z);

	void scale(float x, float y, float z);
	void scale(float s);
};

