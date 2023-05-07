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

void ChunkMesh::buildMesh(Chunk& chunk)
{
    auto update_face = [chunk](std::array<float, 42>& face, int x, int y, int z, BlockType type, bool is_side, FaceId faceid) {
        size_t i = 0;
        while (i < 42) {
            int lx = face[i + 0] > 0 ? 1 : -1;                      // get whether this vertex's local x value is positive so we can use it to grab the next/prev block in the x direction in the chunk
            int ly = face[i + 1] > 0 ? 1 : -1;                      // get whether this vertex's local y value is positive so we can use it to grab the next/prev block in the y direction in the chunk
            int lz = face[i + 2] > 0 ? 1 : -1;                      // get whether this vertex's local z value is positive so we can use it to grab the next/prev block in the z direction in the chunk

            face[i + 0] += x;                                       // update local coords x
            face[i + 1] += y;                                       // update local coords y
            face[i + 2] += z;                                       // update local coords z
            face[i + 3] += 0;                                       // skip texture coords x
            face[i + 4] += 0;                                       // skip texture coords y
            face[i + 5] = (float)Block::getTexture(type, is_side);  // update texture index

            // update vertex ambient occlusion
            int side1 = 0, side2 = 0, corner = 0;
            switch (faceid) {
            case FaceId::PX:
            {
                side1 = chunk.getBlockType(x + 1, y, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                side2 = chunk.getBlockType(x + 1, y + ly , z) != BlockType::BlockType_Air ? 1 : 0;
                corner = chunk.getBlockType(x + 1, y + ly, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                break;
            }
            case FaceId::PY:
            {
                side1 = chunk.getBlockType(x, y + 1, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                side2 = chunk.getBlockType(x + lx, y + 1, z) != BlockType::BlockType_Air ? 1 : 0;
                corner = chunk.getBlockType(x + lx, y + 1, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                break;
            }
            case FaceId::PZ:
            {
                side1 = chunk.getBlockType(x, y + ly, z + 1) != BlockType::BlockType_Air ? 1 : 0;
                side2 = chunk.getBlockType(x + lx, y, z + 1) != BlockType::BlockType_Air ? 1 : 0;
                corner = chunk.getBlockType(x + lx, y + ly, z + 1) != BlockType::BlockType_Air ? 1 : 0;
                break;
            }
            case FaceId::NX:
            {
                side1 = chunk.getBlockType(x - 1, y + ly, z) != BlockType::BlockType_Air ? 1 : 0;
                side2 = chunk.getBlockType(x - 1, y, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                corner = chunk.getBlockType(x - 1, y + ly, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                break;
            }
            case FaceId::NY:
            {
                side1 = chunk.getBlockType(x, y - 1, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                side2 = chunk.getBlockType(x + lx, y - 1, z) != BlockType::BlockType_Air ? 1 : 0;
                corner = chunk.getBlockType(x + lx, y - 1, z + lz) != BlockType::BlockType_Air ? 1 : 0;
                break;
            }
            case FaceId::NZ:
            {
                side1 = chunk.getBlockType(x, y + ly, z - 1) != BlockType::BlockType_Air ? 1 : 0;
                side2 = chunk.getBlockType(x + lx, y, z - 1) != BlockType::BlockType_Air ? 1 : 0;
                corner = chunk.getBlockType(x + lx, y + ly, z - 1) != BlockType::BlockType_Air ? 1 : 0;
                break;
            }
            }

            face[i + 6] = (float)Block::getVertexAO(side1, side2, corner);

            i += 7;
        }
    };

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {

                if (chunk.getBlockType(x, y, z) == BlockType::BlockType_Air)
                    continue;

                // If the other block is outside the chunk, render the face
                // OR, if one face is air and the other is not, draw the face
                // TODO: fix air pockets
                if ((x + 1 >= Chunk::CHUNK_SIZE) || ((x + 1 < Chunk::CHUNK_SIZE) && Block::shouldDrawFace(chunk.getBlockType(x, y, z), chunk.getBlockType(x + 1, y, z))))
                {
                    std::array<float, 42> face{ PX };
                    update_face(face, x, y, z, chunk.getBlockType(x, y, z), true, FaceId::PX);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((y + 1 >= Chunk::CHUNK_SIZE) || ((y + 1 < Chunk::CHUNK_SIZE) && Block::shouldDrawFace(chunk.getBlockType(x, y, z), chunk.getBlockType(x, y + 1, z))))
                {
                    std::array<float, 42> face{ PY };
                    update_face(face, x, y, z, chunk.getBlockType(x, y, z), false, FaceId::PY);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((z + 1 >= Chunk::CHUNK_SIZE) || ((z + 1 < Chunk::CHUNK_SIZE) && Block::shouldDrawFace(chunk.getBlockType(x, y, z), chunk.getBlockType(x, y, z + 1))))
                {
                    std::array<float, 42> face{ PZ };
                    update_face(face, x, y, z, chunk.getBlockType(x, y, z), true, FaceId::PZ);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((x - 1 < 0) || (x - 1 >= 0 && Block::shouldDrawFace(chunk.getBlockType(x, y, z), chunk.getBlockType(x - 1, y, z))))
                {
                    std::array<float, 42> face{ NX };
                    update_face(face, x, y, z, chunk.getBlockType(x, y, z), true, FaceId::NX);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((y - 1 < 0) || (y - 1 >= 0 && Block::shouldDrawFace(chunk.getBlockType(x, y, z), chunk.getBlockType(x, y - 1, z))))
                {
                    std::array<float, 42> face{ NY };
                    update_face(face, x, y, z, chunk.getBlockType(x, y, z), false, FaceId::NY);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }

                if ((z - 1 < 0) || (z - 1 >= 0 && Block::shouldDrawFace(chunk.getBlockType(x, y, z), chunk.getBlockType(x, y, z - 1))))
                {
                    std::array<float, 42> face{ NZ };
                    update_face(face, x, y, z, chunk.getBlockType(x, y, z), true, FaceId::NZ);
                    vertices.insert(vertices.end(), face.begin(), face.end());
                }
            }
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
}

void ChunkMesh::render(glm::vec3 position, Shader &ourShader)
{
    extern Player player;
    extern Camera camera;

    glm::vec3& playerPos = player.getPosition();

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, Texture::texture_array);
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
    projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 200.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = projection * view * model;

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}