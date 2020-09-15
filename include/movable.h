#ifndef PACMAN_MOVEABLE_H
#define PACMAN_MOVEABLE_H

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Movable {
	int x;
	int y;

public:
	Movable(int x, int y) {
		this.x = x;
		this.y = y;
	}

	virtual void move(Direction dir) = 0;

	[[nodiscard]] int getX() const {
		return x;
	}

	[[nodiscard]] int getY() const {
		return y;
	}

};

#endif //PACMAN_MOVEABLE_H
