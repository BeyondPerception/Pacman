#include <gui/game_board.h>
#include <numeric>

GameBoard::GameBoard(SDL_Renderer* renderer, unsigned char rows, unsigned char cols) {
	this->renderer = renderer;
	this->rows = rows;
	this->cols = cols;

	mat = new Paintable* [rows * cols];
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
                if ((j == 0 || adjLeft[pseudo[i/3][j/3-1]].find(h[cur]) != adjLeft[pseudo[i/3][j/3-1]].end()) && (i == 0 || adjUp[pseudo[i/3-1][j/3]].find(h[cur]) != adjUp[pseudo[i/3-1][j/3]].end())) {
                    for (int x = i; x < i + 3; ++x) {
                        for (int y = j; y < j + 3; ++y) {
                            grid[x][y] = h[cur].c[x-i][y-j];
                        }
                    }
                    pseudo[i/3][j/3] = h[cur];
                    break;
                }
            }
        }
    }
}

std::vector<std::vector<bool> > GameBoard::unflatten(std::vector<bool>& c) {
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
    }
    else {
        if (std::rand() % 3 == 0) {
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

bool GameBoard::left(Chunk a, Chunk b) {
    for (int i = 0; i < 3; ++i) {
        if (a.c[i][2] == b.c[i][0] && !a.c[i][2]) return true;
    }
    return false;
}

bool GameBoard::up(Chunk a, Chunk b) {
    for (int i = 0; i < 3; ++i) {
        if (a.c[2][i] == b.c[0][i] && !a.c[2][i]) return true;
    }
    return false;
}

void GameBoard::precompute2() {
    adjUp = std::unordered_map<Chunk, std::unordered_set<Chunk> >();
    adjLeft = std::unordered_map<Chunk, std::unordered_set<Chunk> >();
    for (Chunk i : h) {
        for (Chunk j : h) {
            if (left(i, j)) {
                adjLeft[i].insert(j);
                adjLeft[j].insert(i);
            }
            if (up(i, j)) {
                adjUp[i].insert(j);
                adjUp[j].insert(i);
            }
        }
    }
}

struct DSU {
    int SZ, numsets;
    std::vector<int> par, siz;

    DSU(int n) {
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
        }
        else {
            siz[x] += siz[y];
            par[y] = x;
        }
    }
};

int GameBoard::conv(int x, int y) {
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
    std::cout << d.numsets << std::endl;
    return d.numsets > 1;
}

void GameBoard::generate() {
    precompute1();
    precompute2();
    std::vector<std::vector<bool> > grid(rows, std::vector<bool>(cols, false)); // grid[x][y] = true IFF there is a wall at (x, y)

    try_gen(grid);
    bad(grid);
    do {
        try_gen(grid);
    } while(bad(grid));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j]) {
                placeWall(i, j);
            }
        }
    }
}

void GameBoard::placeWall(unsigned char row, unsigned char col) {
	auto* wall = new Paintable(renderer, "../assets/wall.png", row * TEXTURE_SIZE, col * TEXTURE_SIZE, true);
	objects[{row, col}] = wall;
	mat[row * cols + col] = wall;
}

void GameBoard::clearWall(unsigned char row, unsigned char col) {
	Paintable* wall = objects[{row, col}];
	wall->unpaint();
	objects.erase({row, col});
	mat[row * cols + col] = nullptr;
	delete wall;
}