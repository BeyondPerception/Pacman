#ifndef PACMAN_GAME_BOARD_H
#define PACMAN_GAME_BOARD_H

#include <vector>
#include <unordered_map>

#include <object/paintable.h>

struct Point {
	unsigned char x;
	unsigned char y;

	bool operator==(const Point& p) const {
		return x == p.x && y == p.y;
	}
};

namespace std {
	template<>
	struct hash<Point> {
		size_t operator()(const Point& p) const {
			return p.x << 0x8 | p.y;
		}
	};
}

class GameBoard {
	unsigned char rows;
	unsigned char cols;

	Paintable** mat;
	std::unordered_map<Point, Paintable*> objects;

	SDL_Renderer* renderer;
public:
	GameBoard(SDL_Renderer* renderer, unsigned char rows, unsigned char cols);

	void generate();

	void placeWall(unsigned char row, unsigned char col);

	void clearWall(unsigned char row, unsigned char col);

	Paintable** operator[](int index) {
		return &mat[index * cols];
	}
};

#endif //PACMAN_GAME_BOARD_H
