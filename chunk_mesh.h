#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "block.h"
#include "camera.h"
#include "shader.h"


class ChunkMesh
{
public:

	ChunkMesh();
	~ChunkMesh();

	void buildMesh(Block*** blocks);
	void render(glm::vec3 position, Shader& ourShader);

//private:
    unsigned int VBO, VAO;
	std::vector<float> vertices;
};