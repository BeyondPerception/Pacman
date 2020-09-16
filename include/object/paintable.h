#ifndef PACMAN_PAINTABLE_H
#define PACMAN_PAINTABLE_H

#include <string>
#include <SDL_render.h>
#include <SDL_image.h>
#include <iostream>

#define TEXTURE_SIZE 16

class Paintable {
	SDL_Texture* texture;
	SDL_Renderer* renderer;

	int w{};
	int h{};
protected:
	// DO NOT CHANGE THESE VALUES MANUALLY.
	int x;
	int y;

	inline bool containsPoint(int x1, int y1) const;

public:
	Paintable(SDL_Renderer* renderer, const std::string& path, int x, int y);

	void paint();

	void unpaint();

	bool intersects(Paintable o);

	[[nodiscard]] int getX() const;

	[[nodiscard]] int getY() const;
};

#endif //PACMAN_PAINTABLE_H
