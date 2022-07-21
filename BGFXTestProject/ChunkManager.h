#pragma once

#include <vector>
#include <bgfx/bgfx.h>

#include "Mesh.h"

class ChunkManager {
	private:
	int size;
	int distance;

	struct Chunk {
		int x, y;
		Mesh* mesh;
	};

	std::vector<Chunk> chunkList;

	void addNewMesh(int offsetX, int offsetY, uint32_t color);

	public:
	ChunkManager(int drawDistance, int chunkSize);

	void update(const glm::vec3& position, const glm::vec3& direction);

	void draw(bgfx::ViewId view, bgfx::ProgramHandle program);
};

