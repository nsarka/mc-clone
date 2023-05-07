#pragma once

#include <array>
#include <unordered_map>
#include <string>

#include "texture.h"

extern std::array<float, 42> NZ;
extern std::array<float, 42> PZ;
extern std::array<float, 42> NX;
extern std::array<float, 42> PX;
extern std::array<float, 42> NY;
extern std::array<float, 42> PY;

enum class FaceId {
	NZ = 0,
	PZ,
	NX,
	PX,
	NY,
	PY
};

enum class BlockType {
    BlockType_Air = 0,
    BlockType_Grass,
    BlockType_Sand,
    BlockType_Dirt,
    BlockType_Stone,
	BlockType_Coal,
	BlockType_Cactus,
	BlockType_Shrub,
	BlockType_TallGrass,
	BlockType_Diamond,
	BlockType_Iron,
	BlockType_Leaves,
	BlockType_Log,
	BlockType_Water,
	BlockType_Error,
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

	static int getVertexAO(int side1, int side2, int corner) {
		if (side1 && side2) {
			return 0;
		}
		return 3 - (side1 + side2 + corner);
	}

	static int getTexture(BlockType type, bool is_side) {

		switch (type) {

		case BlockType::BlockType_Grass:
			return is_side ? Texture::str_to_texid["grass_side"] : Texture::str_to_texid["grass"];
			break;
		case BlockType::BlockType_Sand:
			return Texture::str_to_texid["sand"];
			break;
		case BlockType::BlockType_Dirt:
			return Texture::str_to_texid["dirt"];
			break;
		case BlockType::BlockType_Stone:
			return Texture::str_to_texid["stone"];
			break;
		case BlockType::BlockType_Coal:
			return Texture::str_to_texid["coal"];
			break;
		case BlockType::BlockType_Cactus:
			return is_side ? Texture::str_to_texid["common_cactus_side"] : Texture::str_to_texid["common_cactus_top"];
			break;
		case BlockType::BlockType_Shrub:
			return Texture::str_to_texid["common_dead_shrub"];
			break;
		case BlockType::BlockType_TallGrass:
			return Texture::str_to_texid["common_tall_grass"];
			break;
		case BlockType::BlockType_Diamond:
			return Texture::str_to_texid["diamond"];
			break;
		case BlockType::BlockType_Iron:
			return Texture::str_to_texid["iron"];
			break;
		case BlockType::BlockType_Leaves:
			return Texture::str_to_texid["leaves"];
			break;
		case BlockType::BlockType_Log:
			return is_side ? Texture::str_to_texid["log"] : Texture::str_to_texid["logtop"];
			break;
		case BlockType::BlockType_Water:
			return Texture::str_to_texid["water"];
			break;
		case BlockType::BlockType_Error:
			break;


		case BlockType::BlockType_Air:
			[[fallthrough]]
		default:
			assert(0); // should never reach here
		}
	}

//private:
	bool m_active;
	BlockType m_blockType;
};