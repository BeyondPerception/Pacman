#ifndef PACMAN_MOVEABLE_H
#define PACMAN_MOVEABLE_H

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Moveable {
public:
	virtual void move(Direction dir) = 0;
};

#endif //PACMAN_MOVEABLE_H
