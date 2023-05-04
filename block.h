#pragma once

#include <array>

extern std::array<float, 30> NZ;
extern std::array<float, 30> PZ;
extern std::array<float, 30> NX;
extern std::array<float, 30> PX;
extern std::array<float, 30> NY;
extern std::array<float, 30> PY;

enum class BlockType {
    BlockType_Air = 0,
    BlockType_Grass,
    BlockType_Sand,
    BlockType_Dirt,
    BlockType_Stone,
    BlockType_NumTypes,
};


class Block {
public:
	Block()
		: m_active(true)
		, m_blockType(BlockType::BlockType_Air)
	{}

	~Block() {}

	bool IsActive() { return m_active; }
	void SetActive(bool active) { m_active = active; }

    // This still has the problem of drawing air pockets in solid ground. i.e., one air block in a sea of dirt blocks will make the surrounding dirt blocks have a face rendered
    static bool shouldDrawFace(BlockType x, BlockType y) { return (x == BlockType::BlockType_Air && y != BlockType::BlockType_Air) || (x != BlockType::BlockType_Air && y == BlockType::BlockType_Air); }

//private:
	bool m_active;
	BlockType m_blockType;
};