#include <gui/game_board.h>

GameBoard::GameBoard(SDL_Renderer* renderer, unsigned char rows, unsigned char cols) {
	this->renderer = renderer;
	rows = rows;
	cols = cols;

	mat = new Paintable* [rows * cols];
}

void GameBoard::generate() {

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
