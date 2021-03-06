#ifndef PACMAN_GAME_BOARD_H
#define PACMAN_GAME_BOARD_H

#pragma GCC optimize ("Ofast")
#pragma GCC target ("sse4")

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <object/paintable.h>
#include <object/movable.h>
#include <queue>

struct Point {
	unsigned char r;
	unsigned char c;

	bool operator==(const Point& p) const {
		return r == p.r && c == p.c;
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
			return p.r << 0x8 | p.c;
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
	std::vector<std::vector<bool>> grid2;
	int cost[30][30][30][30];

	std::vector<std::vector<bool>> cheese;
	int points;

	SDL_Renderer* renderer;

	std::vector<Movable*> ghosts;

	SDL_Keycode lastKeyPressed;

	void move_ghost(Movable* ghost, int r, int c);

	void fact(std::vector<bool>& c, int x);

	void precompute1();

	void precompute2();

	void calc_cost();

	void game_over();

	inline int conv(int x, int y) const;

	void try_gen(std::vector<std::vector<bool> >& grid);

	bool bad(std::vector<std::vector<bool> >& grid);

public:
	Movable* pacman;

	GameBoard(SDL_Renderer* renderer, unsigned char rows, unsigned char cols);

	void generate();

	void placeWall(unsigned char row, unsigned char col);

	void placeCheese(unsigned char row, unsigned char col);

	void clearWall(unsigned char row, unsigned char col);

	void clearCheese(unsigned char row, unsigned char col);

	void update(SDL_Keycode code);

	Point getPos(unsigned int pxRow, unsigned int pxCol);

	unsigned char getRows() const {
		return rows;
	}

	unsigned char getCols() const {
		return cols;
	}

	Paintable** operator[](int index) {
		return &mat[index * cols];
	}
};

#endif //PACMAN_GAME_BOARD_H
