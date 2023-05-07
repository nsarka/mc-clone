#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>

#include <assert.h>
#include <unordered_map>
#include <string>


class Texture
{
public:
	static unsigned int texture_array;
	static std::unordered_map<std::string, unsigned int> str_to_texid;

public:
	static void setup();
};