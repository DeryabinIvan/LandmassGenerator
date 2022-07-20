#include "Mesh.h"

#include <random>

#include "glm/gtc/noise.hpp"
#include "glm/gtc/matrix_transform.hpp"

Mesh::Mesh() {
	width = 0;
	height = 0;

	modelMTX = glm::identity<glm::mat4>();
}

Mesh::Mesh(int w, int h) {
	generateMesh(w, h);
}

Mesh::Mesh(Point* points, int pCount, int* tris, int tCount) {
	this->pCount = pCount;
	this->tCount = tCount;

	this->points = new Point[pCount];
	this->triangles = new uint32_t[tCount];

	memcpy(this->points, points, sizeof(Point) * pCount);
	memcpy(this->triangles, tris, sizeof(uint32_t) * tCount);
}

const size_t Mesh::getWidth() {
	return width;
}

const size_t Mesh::getHeight() {
	return height;
}

void Mesh::generateMesh(int w, int h) {
	width = w;
	height = h;

	pCount = w * h;
	points = new Point[pCount];

	tCount = (w - 1) * (h - 1) * 6;
	triangles = new uint32_t[tCount];

	float offsetX = (w - 1) / -2.f;
	float offsetZ = (h - 1) / 2.f;

	modelMTX = glm::identity<glm::mat4>();

	Point p{0, 0, 0, 0};

	int vertIndex = 0, trisIndex = 0;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			p.x = (offsetX + x)/2.f;
			p.y = 0;
			p.z = (offsetZ - y)/2.f;
			p.abgr = 0xff0f77f0;

			points[vertIndex] = p;

			if (x < w - 1 && y < h - 1) {
				triangles[trisIndex] = vertIndex;
				triangles[trisIndex+1] = vertIndex + w + 1;
				triangles[trisIndex+2] = vertIndex + w;
				trisIndex += 3;

				triangles[trisIndex] = vertIndex + w + 1;
				triangles[trisIndex+1] = vertIndex;
				triangles[trisIndex+2] = vertIndex + 1;
				trisIndex += 3;
			}

			vertIndex++;
		}
	}
}

void Mesh::createHeightMap() {
	uint32_t index = 0;
	float f1 = 32, f2 = 128, f3 = 8;

	float offsetX = 0, offsetY = 0;

	std::random_device rDev;
	std::mt19937 e2(rDev());
	std::uniform_real dist(-1000.0f, 1000.0f);

	offsetX = dist(e2), offsetY = dist(e2);

	for (uint16_t i = 0; i < width; i++) {
		for (uint16_t j = 0; j < height; j++) {
			float h1 = glm::perlin(glm::vec2((i + offsetX) / f1, (j + offsetY) / f1)),
				  h2 = glm::perlin(glm::vec2((i + f1 - offsetX) / f2, (j - f2 + offsetY) / f2)),
				  h3 = glm::perlin(glm::vec2((i + offsetY) / f3, (j + offsetX) / f3));

			index = i + j * width;
			auto& current = points[index];

			float tmp = (h1 * 10.0f + h2 * 2.0f) * (h3 * 1.25f);

			if (tmp >= -1.0f) {
				current.y = tmp;
			}

			if (tmp < -1.0f) {
				current.abgr = 0xffee0000;
			} else if (tmp < 0.01f) {
				current.abgr = 0xff2ecedb;
			} else if (tmp < 3.5f) {
				current.abgr = 0xff45d449;
			} else if (tmp < 5.0f) {
				current.abgr = 0xff666666;
			} else {
				current.abgr = 0xffe8e8e8;
			}
		}
	}
}

void Mesh::createBuffers() {
	bgfx::VertexLayout layout;
	layout.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	vbh = bgfx::createVertexBuffer(bgfx::makeRef(points, pCount * sizeof(points[0])), layout);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(triangles, tCount * sizeof(triangles[0])), BGFX_BUFFER_INDEX32);
}

void Mesh::bindBuffers() {
	bgfx::setVertexBuffer(0, vbh);
	bgfx::setIndexBuffer(ibh);
}

void Mesh::destroyBuffers() {
	if (bgfx::isValid(vbh)) {
		bgfx::destroy(vbh);
	}

	if (bgfx::isValid(ibh)) {
		bgfx::destroy(ibh);
	}
}

const glm::mat4 Mesh::getModelMatrix() {
	return modelMTX;
}

void Mesh::resetModelMTX() {
	modelMTX = glm::identity<glm::mat4>();
}

void Mesh::translate(float x, float y, float z) {
	modelMTX = glm::translate(modelMTX, glm::vec3(x, y, z));
}

void Mesh::rotateX(float x) {
	modelMTX = glm::rotate(modelMTX, x, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Mesh::rotateY(float y) {
	modelMTX = glm::rotate(modelMTX, y, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Mesh::rotateZ(float z) {
	modelMTX = glm::rotate(modelMTX, z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Mesh::scale(float x, float y, float z) {
	modelMTX = glm::scale(modelMTX, glm::vec3(x, y, z));
}

void Mesh::scale(float s) {
	modelMTX = glm::scale(modelMTX, glm::vec3(s, s, s));
}
