#ifndef PACMAN_PAINTABLE_H
#define PACMAN_PAINTABLE_H

#include <string>
#include <SDL_render.h>
#include <SDL_image.h>
#include <iostream>

class Paintable {
	SDL_Texture* texture;
	SDL_Renderer* renderer;

	int x;
	int y;
public:
	Paintable(SDL_Renderer* renderer, const std::string& path, int x, int y);

	void paint();
};

#endif //PACMAN_PAINTABLE_H
