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

	bool x_bounds;
	bool y_bounds;
public:
	Movable(SDL_Renderer* renderer, const std::string& path, int x, int y, SDL_Rect bounds);

	void move(Direction dir);

	void setX(int x);

	void setY(int y);

	void setXY(int x, int y);

	bool isAtXBounds() const;

	bool isAtYBounds() const;

	void setDirection(Direction dir);

	Direction getLastDirection();
};

#endif //PACMAN_MOVABLE_H
