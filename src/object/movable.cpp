#include <object/movable.h>

Movable::Movable(SDL_Renderer* renderer, const std::string& path, int x, int y, SDL_Rect bounds) : Paintable(renderer,
																											 path,
																											 x, y) {
	this->bounds = bounds;
	lastDirection = RIGHT;
	x_bounds = false;
	y_bounds = false;
}


void Movable::move(Direction dir) {
	lastDirection = dir;
	switch (dir) {
		case UP:
			if (y - TEXTURE_SIZE / 4 >= bounds.y) {
				setY(y - TEXTURE_SIZE / 4);
				y_bounds = false;
			} else {
				y_bounds = true;
			}
			break;
		case DOWN:
			if (y + TEXTURE_SIZE / 4 <= bounds.y + bounds.h) {
				setY(y + TEXTURE_SIZE / 4);
				y_bounds = false;
			} else {
				y_bounds = true;
			}
			break;
		case LEFT:
			if (x - TEXTURE_SIZE / 4 >= bounds.x) {
				setX(x - TEXTURE_SIZE / 4);
				x_bounds = false;
			} else {
				x_bounds = true;
			}
			break;
		case RIGHT:
			if (x + TEXTURE_SIZE / 4 <= bounds.x + bounds.w) {
				setX(x + TEXTURE_SIZE / 4);
				x_bounds = false;
			} else {
				x_bounds = true;
			}
			break;
	}
}

void Movable::setX(int x) {
	setXY(x, y);
}

void Movable::setY(int y) {
	setXY(x, y);
}

void Movable::setXY(int x, int y) {
	unpaint();
	this->x = x;
	this->y = y;
	paint();
}

bool Movable::isAtXBounds() const {
	return x_bounds;
}

bool Movable::isAtYBounds() const {
	return y_bounds;
}

Direction Movable::getLastDirection() {
	return lastDirection;
}
