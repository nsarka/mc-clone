#include "chunk.h"

Chunk::Chunk()
	: mesh()
	, m_position(0,0,0)
{
	// Create the blocks
	m_pBlocks = new Block * *[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pBlocks[i] = new Block * [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_pBlocks[i][j] = new Block[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() { // Delete the blocks
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_pBlocks[i][j];
		}
		delete[] m_pBlocks[i];
	}
	delete[] m_pBlocks;
}

void Chunk::update()
{
	mesh.buildMesh(*this);
}

void Chunk::render(Shader& ourShader)
{
	mesh.render(m_position, ourShader);
}

const BlockType Chunk::getBlockType(int x, int y, int z) const
{
	if (x < 0 || y < 0 || z < 0)
		return BlockType::BlockType_Air;

	if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
		return BlockType::BlockType_Air;

	return m_pBlocks[x][y][z].m_blockType;
}