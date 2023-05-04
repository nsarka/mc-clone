#include "chunk_mesh.h"

#include "chunk.h"

ChunkMesh::ChunkMesh()
    : vertices()
{
    glGenVertexArrays(1, &VAO);
    std::cout << "Generated VAO: " << VAO << " err code: " << glGetError() << std::endl;
    glGenBuffers(1, &VBO);
}

ChunkMesh::~ChunkMesh()
{

}

void ChunkMesh::buildMesh(Block*** blocks)
{
    auto update_face = [](std::array<float, 36>& face, int x, int y, int z, BlockType type) {
        int i = 0;
        while (i < 36) {
            face[i++] += x;
            face[i++] += y;
            face[i++] += z;
            i += 2;
            face[i++] = (float)((int)type - 1);
        }
    };

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {

                if (blocks[x][y][z].m_blockType == BlockType::BlockType_Air)
                    continue;

                // If the other block is outside the chunk, render the face
                // OR, if one face is air and the other is not, draw the face
                // TODO: fix air pockets
                if ((x + 1 >= Chunk::CHUNK_SIZE) || ((x + 1 < Chunk::CHUNK_SIZE) && Block::shouldDrawFace(blocks[x][y][z].m_blockType, blocks[x+1][y][z].m_blockType)))
                {
                    std::array<float, 36> face{ PX };
                    update_face(face, x, y, z, blocks[x][y][z].m_blockType);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((y + 1 >= Chunk::CHUNK_SIZE) || ((y + 1 < Chunk::CHUNK_SIZE) && Block::shouldDrawFace(blocks[x][y][z].m_blockType, blocks[x][y+1][z].m_blockType)))
                {
                    std::array<float, 36> face{ PY };
                    update_face(face, x, y, z, blocks[x][y][z].m_blockType);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((z + 1 >= Chunk::CHUNK_SIZE) || ((z + 1 < Chunk::CHUNK_SIZE) && Block::shouldDrawFace(blocks[x][y][z].m_blockType, blocks[x][y][z+1].m_blockType)))
                {
                    std::array<float, 36> face{ PZ };
                    update_face(face, x, y, z, blocks[x][y][z].m_blockType);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((x - 1 < 0) || (x - 1 >= 0 && Block::shouldDrawFace(blocks[x][y][z].m_blockType, blocks[x-1][y][z].m_blockType)))
                {
                    std::array<float, 36> face{ NX };
                    update_face(face, x, y, z, blocks[x][y][z].m_blockType);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((y - 1 < 0) || (y - 1 >= 0 && Block::shouldDrawFace(blocks[x][y][z].m_blockType, blocks[x][y-1][z].m_blockType)))
                {
                    std::array<float, 36> face{ NY };
                    update_face(face, x, y, z, blocks[x][y][z].m_blockType);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((z - 1 < 0) || (z - 1 >= 0 && Block::shouldDrawFace(blocks[x][y][z].m_blockType, blocks[x][y][z-1].m_blockType)))
                {
                    std::array<float, 36> face{ NZ };
                    update_face(face, x, y, z, blocks[x][y][z].m_blockType);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }
            }
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void ChunkMesh::render(glm::vec3 position, Shader &ourShader)
{
    extern Player player;
    extern Camera camera;
    extern unsigned int texture_array;

    glm::vec3& playerPos = player.getPosition();

    //std::cout << "Binding VAO " << VAO << std::endl;
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    ourShader.use();

    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");

    position.x *= Chunk::CHUNK_SIZE;
    position.y *= Chunk::CHUNK_SIZE;
    position.z *= Chunk::CHUNK_SIZE;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    glm::mat4 view = glm::mat4(1.0f);
    view = camera.getViewMatrix(playerPos);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = projection * view * model;

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}