#include <object/movable.h>

void Movable::move(Direction dir) {
	lastDirection = dir;
	switch (dir) {
		case UP:
			if (y - TEXTURE_SIZE / 4 >= bounds.y) {
				setY(y - TEXTURE_SIZE / 4);
			}
			break;
		case DOWN:
			if (y + TEXTURE_SIZE / 4 <= bounds.y + bounds.h) {
				setY(y + TEXTURE_SIZE / 4);
			}
			break;
		case LEFT:
			if (x - TEXTURE_SIZE / 4 >= bounds.x) {
				setX(x - TEXTURE_SIZE / 4);
			}
			break;
		case RIGHT:
			if (x + TEXTURE_SIZE / 4 <= bounds.x + bounds.w) {
				setX(x + TEXTURE_SIZE / 4);
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
	this->x = x;
	this->y = y;
	paint();
}
