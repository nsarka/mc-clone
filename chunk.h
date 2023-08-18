#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "block.h"
#include "chunk_mesh.h"


class Chunk {
public:
	Chunk();
	~Chunk();

	void update();
	void render(Shader& ourShader);

	glm::vec3 getPosition() { return m_position; }

	const BlockType getBlockType(int x, int y, int z) const;

	static const int CHUNK_SIZE = 128;

//private:
	glm::vec3 m_position;
	Block*** m_pBlocks;
	ChunkMesh mesh;
};