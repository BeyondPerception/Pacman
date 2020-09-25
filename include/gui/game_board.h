#ifndef PACMAN_GAME_BOARD_H
#define PACMAN_GAME_BOARD_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <object/paintable.h>

struct Point {
	unsigned char x;
	unsigned char y;

	bool operator==(const Point& p) const {
		return x == p.x && y == p.y;
	}
};

struct Chunk {
	std::vector<std::vector<bool> > c;
	int hash;

	Chunk(std::vector<std::vector<bool> > cc) {
		c = std::vector<std::vector<bool> >(3, std::vector<bool>(3));
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				c[i][j] = cc[i][j];
				if (c[i][j]) hash = hash * 10 + i * 9 + j;
			}
		}
	}

	Chunk() {
		c = std::vector<std::vector<bool> >(3, std::vector<bool>(3));
	}

	bool operator==(const Chunk& p) const {
		return hash == p.hash;
	}
};

namespace std {
	template<>
	struct hash<Point> {
		size_t operator()(const Point& p) const {
			return p.x << 0x8 | p.y;
		}
	};

	template<>
	struct hash<Chunk> {
		size_t operator()(const Chunk& p) const {
			return p.hash;
		}
	};
}

class GameBoard {
	unsigned char rows;
	unsigned char cols;

	std::vector<std::vector<Chunk> > pseudo;
	std::unordered_map<Chunk, std::unordered_set<Chunk> > adjUp, adjLeft;
	std::vector<Chunk> h;

	Paintable** mat;
	std::unordered_map<Point, Paintable*> objects;

	SDL_Renderer* renderer;

	void fact(std::vector<bool>& c, int x);

	void precompute1();

	void precompute2();

	inline int conv(int x, int y) const;

	void try_gen(std::vector<std::vector<bool> >& grid);

	bool bad(std::vector<std::vector<bool> >& grid);

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
