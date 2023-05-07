#include "texture.h"

unsigned Texture::texture_array;
std::unordered_map<std::string, unsigned int> Texture::str_to_texid;

void Texture::setup()
{
    IMG_Init(IMG_INIT_PNG);
    const int textureSize = 16;
    const int numTextures = 17;
    int texid = 0;

    glGenTextures(1, &texture_array);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureSize, textureSize,
        numTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    SDL_Surface* surface = IMG_Load("texture_packs/default/voxels/grass.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["grass"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/grass_side.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["grass_side"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/sand.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels); // even though sand.png is a png, it's GL_RGB instead of GL_RGBA
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["sand"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/dirt.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["dirt"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/stone.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels); // even though stone.png is a png, it's GL_RGB instead of GL_RGBA
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["stone"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/logtop.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["logtop"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/log.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["log"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/water.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["water"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/iron.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["iron"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/coal.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["coal"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/common_cactus_top.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["common_cactus_top"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/common_cactus_side.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["common_cactus_side"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/common_dead_shrub.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["common_dead_shrub"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/common_tall_grass.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["common_tall_grass"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/diamond.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["diamond"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/error.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["error"] = texid++;
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/leaves.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texid, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    str_to_texid["leaves"] = texid++;
    SDL_FreeSurface(surface);
}