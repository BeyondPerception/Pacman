#ifndef PACMAN_MOVABLE_H
#define PACMAN_MOVABLE_H

#include <object/paintable.h>

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Movable : public Paintable {
	Direction lastDirection;
	SDL_Rect bounds;

public:
	Movable(SDL_Renderer* renderer, const std::string& path, int x, int y, SDL_Rect bounds) : Paintable(renderer, path,
																										x, y) {
		this->bounds = bounds;
		lastDirection = RIGHT;
	}

	void move(Direction dir);

	void setX(int x);

	void setY(int y);

	void setXY(int x, int y);
};

#endif //PACMAN_MOVABLE_H
