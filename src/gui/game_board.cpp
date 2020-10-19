//#pragma GCC optimize ("Ofast")
//#pragma GCC target ("sse4")

#include <gui/game_board.h>
#include <numeric>
#include <queue>
#include <thread>

GameBoard::GameBoard(SDL_Renderer* renderer, unsigned char rows, unsigned char cols) {
	this->renderer = renderer;
	this->rows = rows;
	this->cols = cols;
	this->points = 0;

	grid2 = std::vector<std::vector<bool>>(cols, std::vector<bool>(rows, false));
	cheese = std::vector<std::vector<bool>>(cols, std::vector<bool>(rows, false));

	mat = new Paintable* [rows * cols];
	pacman = new Movable(renderer, "../assets/pacman_small.png", TEXTURE_SIZE, TEXTURE_SIZE,
						 SDL_Rect{16, 16, 464, 464});

	for (int i = 0; i < 2; i++) {
		ghosts.push_back(
				new Movable(renderer, "../assets/ghost" + std::to_string(i) + ".png",
							(i == 0) ? (cols - 2) * TEXTURE_SIZE : TEXTURE_SIZE,
							(i == 0) ? TEXTURE_SIZE : (rows - 2) * TEXTURE_SIZE, SDL_Rect{16, 16, 464, 464}));
	}

	std::fill(mat, mat + rows * cols, nullptr);
}

void GameBoard::try_gen(std::vector<std::vector<bool> >& grid) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			grid[i][j] = false;
		}
	}
	pseudo = std::vector<std::vector<Chunk> >(rows / 3, std::vector<Chunk>(cols / 3));
	for (int i = 0; i < rows; i += 3) {
		for (int j = 0; j < cols; j += 3) {
			while (true) {
				int cur = std::rand() % (int) h.size();
				if ((int) adjUp[h[cur]].size() == 0 || (int) adjLeft[h[cur]].size() == 0) {
					continue;
				}
				if ((j == 0 ||
					 adjLeft[pseudo[i / 3][j / 3 - 1]].find(h[cur]) != adjLeft[pseudo[i / 3][j / 3 - 1]].end()) &&
					(i == 0 || adjUp[pseudo[i / 3 - 1][j / 3]].find(h[cur]) != adjUp[pseudo[i / 3 - 1][j / 3]].end())) {
					for (int x = i; x < i + 3; ++x) {
						for (int y = j; y < j + 3; ++y) {
							grid[x][y] = h[cur].c[x - i][y - j];
						}
					}
					pseudo[i / 3][j / 3] = h[cur];
					break;
				}
			}
		}
	}
	for (int i = 0; i < rows; ++i) {
		placeWall(i, 0);
		placeWall(i, cols - 1);
	}
	for (int i = 1; i < cols - 1; ++i) {
		placeWall(0, i);
		placeWall(rows - 1, i);
	}
}

static std::vector<std::vector<bool> > unflatten(std::vector<bool>& c) {
	std::vector<std::vector<bool> > op(3, std::vector<bool>(3));
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			op[i][j] = c[i * 3 + j];
		}
	}
	return op;
}

void GameBoard::fact(std::vector<bool>& c, int x) {
	if (x == 9) {
		Chunk cur(unflatten(c));
		h.push_back(cur);
	} else {
		if (x != 4) {
			c[x] = true;
			fact(c, x + 1);
		}
		c[x] = false;
		fact(c, x + 1);
	}
}

void GameBoard::precompute1() {
	h = std::vector<Chunk>();
	std::vector<bool> base(9, false);
	fact(base, 0);
}

static bool left(Chunk a, Chunk b) {
	if (a.c[1][2] == b.c[1][0] && !a.c[1][2]) return true;
	return false;
}

static bool up(Chunk a, Chunk b) {
	if (a.c[2][1] == b.c[0][1] && !a.c[2][1]) return true;
	return false;
}

void GameBoard::precompute2() {
	adjUp = std::unordered_map<Chunk, std::unordered_set<Chunk> >();
	adjLeft = std::unordered_map<Chunk, std::unordered_set<Chunk> >();
	for (const Chunk& i : h) {
		for (const Chunk& j : h) {
			if (left(i, j)) {
				adjLeft[i].insert(j);
			}
			if (up(i, j)) {
				adjUp[i].insert(j);
			}
		}
	}
}

struct DSU {
	int SZ, numsets;
	std::vector<int> par, siz;

	explicit DSU(int n) {
		SZ = n;
		numsets = n;
		par = std::vector<int>(n);
		std::iota(par.begin(), par.end(), 0);
		siz = std::vector<int>(n, 1);
	}

	int find(int x) {
		return par[x] == x ? x : par[x] = find(par[x]);
	}

	bool same(int x, int y) {
		return find(x) == find(y);
	}

	void merge(int x, int y) {
		x = find(x), y = find(y);
		if (x == y) return;
		--numsets;
		if (siz[x] < siz[y]) {
			siz[y] += siz[x];
			par[x] = y;
		} else {
			siz[x] += siz[y];
			par[y] = x;
		}
	}
};

inline int GameBoard::conv(int x, int y) const {
	return x * cols + y;
}

bool GameBoard::bad(std::vector<std::vector<bool> >& grid) {
	DSU d((int) rows * cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (grid[i][j]) {
				--d.numsets;
				continue;
			}
			if (i - 1 >= 0 && !grid[i - 1][j]) d.merge(conv(i, j), conv(i - 1, j));
			if (i + 1 < rows && !grid[i + 1][j]) d.merge(conv(i, j), conv(i + 1, j));
			if (j - 1 >= 0 && !grid[i][j - 1]) d.merge(conv(i, j), conv(i, j - 1));
			if (j + 1 < cols && !grid[i][j + 1]) d.merge(conv(i, j), conv(i, j + 1));
		}
	}
	return d.numsets > 10;
}

void GameBoard::generate() {
	precompute1();
	precompute2();
	std::vector<std::vector<bool> > grid(rows, std::vector<bool>(cols,
																 false)); // grid[x][y] = true IFF there is a wall at (x, y)
	do {
		try_gen(grid);
	} while (bad(grid));

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
				grid[i][j] = true;
			}
		}
	}

	grid2 = grid;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (grid[i][j]) {
				if (!(i == 1 && j == cols - 2) && !(i == rows - 2 && j == 1) && !(i == 1 && j == 1)) {
					placeWall(i, j);
				}
			} else {
				if (!(i == 1 && j == cols - 2) && !(i == rows - 2 && j == 1) && !(i == 1 && j == 1)) {
					cheese[i][j] = true;
					placeCheese(i, j);
				}
			}
		}
	}

	calc_cost();
}

void GameBoard::placeCheese(unsigned char row, unsigned char col) {
	auto* wall = new Paintable(renderer, "../assets/cheese.png", col * TEXTURE_SIZE, row * TEXTURE_SIZE, true);
	objects[{row, col}] = wall;
	mat[row * cols + col] = wall;
}

void GameBoard::placeWall(unsigned char row, unsigned char col) {
	auto* wall = new Paintable(renderer, "../assets/wall.png", col * TEXTURE_SIZE, row * TEXTURE_SIZE, true);
	objects[{row, col}] = wall;
	mat[row * cols + col] = wall;
}

void GameBoard::clearCheese(unsigned char row, unsigned char col) {
	Paintable* obj = objects[{row, col}];
	obj->unpaint();
	objects.erase({row, col});
	mat[row * cols + col] = nullptr;
	delete obj;

	cheese[row][col] = false;
}

void GameBoard::clearWall(unsigned char row, unsigned char col) {
	Paintable* wall = objects[{row, col}];
	wall->unpaint();
	objects.erase({row, col});
	mat[row * cols + col] = nullptr;
	delete wall;
}

Point GameBoard::getPos(unsigned int pxRow, unsigned int pxCol) {
	return {static_cast<unsigned char>(std::floor(pxRow / TEXTURE_SIZE)),
			static_cast<unsigned char>(std::floor(pxCol / TEXTURE_SIZE))};
}

void GameBoard::move_ghost(Movable* ghost, int r, int c) {
	int r1, c1, r2, c2;
	r1 = ghost->getY() / TEXTURE_SIZE;
	c1 = ghost->getX() / TEXTURE_SIZE;
	r2 = r;
	c2 = c;

	int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
	int cur = cost[r1][c1][r2][c2];
	int dir = -1;
	for (int i = 0; i < 4; ++i) {
		int r3 = r1 + dx[i];
		int c3 = c1 + dy[i];
		if (r3 < 0 || r3 >= rows || c3 < 0 || c3 >= cols || grid2[r3][c3]) continue;
//		std::cout << cost[r3][c3][r2][c2] << " " << cur << " " << dir << " " << r1 << " " << c1 << " " << r3 << " " << c3 << "\n";
		if (cost[r3][c3][r2][c2] < cur) {
			cur = cost[r3][c3][r2][c2];
			dir = i;
		}
	}

	if (dir != -1) {
		switch (dir) {
			case 0:
				ghost->setDirection(UP);
				break;
			case 1:
				ghost->setDirection(DOWN);
				break;
			case 2:
				ghost->setDirection(LEFT);
				break;
			case 3:
				ghost->setDirection(RIGHT);
				break;
			default:
				break;
		}
	}

	int dx1[4] = {-1, 1, 0, 0};
	int dy1[4] = {0, 0, -1, 1};
	int nr = r1 + dx1[ghost->getLastDirection()];
	int nc = c1 + dy1[ghost->getLastDirection()];
	bool bad = nr < 0 || nc < 0 || nr >= rows - 1 || nc >= cols - 1 || grid2[nr][nc];
	if (!bad) {
		ghost->move(ghost->getLastDirection());
	}
}

void GameBoard::game_over() {
	std::cout << "Points Earned: " << points << "\n";

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (mat[i * rows + j] != nullptr) {
				clearWall(i, j);
			}
		}
	}
32
//	SDL_RenderFillRect(renderer, nullptr);
	Paintable game_over_screen(renderer, "../assets/game_over.png", 0, 0, true);

//	using namespace std::chrono_literals;
//	std::this_thread::sleep_for(10s);
	exit(0);
}

void GameBoard::update(SDL_Keycode code) {
	if (code != -1) {
		lastKeyPressed = code;
	}

	for (auto& ghost : ghosts) {
		move_ghost(ghost, pacman->getY() / TEXTURE_SIZE,
				   pacman->getX() / TEXTURE_SIZE);
		if (ghost->getX() / TEXTURE_SIZE == pacman->getX() / TEXTURE_SIZE &&
			ghost->getY() / TEXTURE_SIZE == pacman->getY() / TEXTURE_SIZE) {
			std::cout << "GAME OVER!!!\n";
			game_over();
		}
	}

	if (pacman->getX() % TEXTURE_SIZE == 0 && pacman->getY() % TEXTURE_SIZE == 0 && lastKeyPressed != -1) {
		switch (lastKeyPressed) {
			case SDLK_UP:
				pacman->setDirection(UP);
				std::cout << "Changing Dir to UP\n";
				break;
			case SDLK_DOWN:
				pacman->setDirection(DOWN);
				std::cout << "Changing Dir to DOWN\n";
				break;
			case SDLK_LEFT:
				pacman->setDirection(LEFT);
				std::cout << "Changing Dir to LEFT\n";
				break;
			case SDLK_RIGHT:
				pacman->setDirection(RIGHT);
				std::cout << "Changing Dir to RIGHT\n";
				break;
			default:
				break;
		}
		lastKeyPressed = -1;
	}

	int r = pacman->getY() / TEXTURE_SIZE;
	int c = pacman->getX() / TEXTURE_SIZE;

	if (cheese[r][c]) {
		points++;
		clearCheese(r, c);
	}

	int dx[4] = {-1, 1, 0, 0};
	int dy[4] = {0, 0, -1, 1};
	int nr = r + dx[pacman->getLastDirection()];
	int nc = c + dy[pacman->getLastDirection()];
	bool bad = nr < 0 || nc < 0 || nr >= rows - 1 || nc >= cols - 1 || grid2[nr][nc];

	if (!bad) {
		pacman->move(pacman->getLastDirection());
	}
}

void GameBoard::calc_cost() {
	int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, 1, -1};
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			for (int k = 0; k < rows; ++k) {
				for (int l = 0; l < cols; ++l) {
					cost[i][j][k][l] = 212212212;
				}
			}
		}
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			cost[i][j][i][j] = 0;
		}
	}
	for (int i = 1; i < rows - 1; ++i) {
		for (int j = 1; j < cols - 1; ++j) {
			if (grid2[i][j]) continue;
			for (int d = 0; d < 4; ++d) {
				int k = i + dx[d];
				int l = j + dy[d];
				if (k < 0 || l < 0 || k >= rows || l >= cols || grid2[i][j]) continue;
				cost[i][j][k][l] = cost[k][l][i][j] = 1;
			}
		}
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			for (int k = 0; k < rows; ++k) {
				for (int l = 0; l < cols; ++l) {
					for (int m = 0; m < rows; ++m) {
						for (int n = 0; n < cols; ++n) {
							cost[i][j][m][n] = std::min(cost[i][j][m][n], cost[i][j][k][l] + cost[k][l][m][n]);
						}
					}
				}
			}
		}
	}
}
