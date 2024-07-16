#include "ChunkManager.h"

ChunkManager::ChunkManager(int drawDistance, int chunkSize) {
	distance = drawDistance;
	size = chunkSize;

	addNewMesh( 0, 0, 0xffff0000);
	addNewMesh(-1, -1, 0xff00ff00);
	addNewMesh( 0, -1, 0xff0000ff);
	addNewMesh( 1, -1, 0xfff0f0f0);
}

void ChunkManager::addNewMesh(int offsetX, int offsetY, uint32_t color) {
	Mesh* m = new Mesh(size, color);
	m->translate(offsetX * (size - 1), 0, offsetY * (size - 1));
	m->createBuffers();

	chunkList.push_back(Chunk{offsetX, offsetY, m});
}

void ChunkManager::update(const glm::vec3& position, const glm::vec3& direction) {

}

void ChunkManager::draw(bgfx::ViewId view, bgfx::ProgramHandle program) {
	for (auto& chunk : chunkList) {
		chunk.mesh->bindBuffers();

		glm::mat4 model = chunk.mesh->getModelMatrix();
		bgfx::setTransform(&model[0][0]);

		bgfx::submit(view, program);
	}
}
