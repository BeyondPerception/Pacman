#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <thread>
#include <gui/colors.h>
#include <object/movable.h>
#include <gui/game_board.h>

const int SCREEN_WIDTH = 1056;
const int SCREEN_HEIGHT = 720;

SDL_Window* init() {
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Failed to initialize SDL! Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cout << "Failed to initialize SDL_image! Error: " << IMG_GetError() << "\n";
		exit(1);
	}

	// Create a new window object
	SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
										  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "Could not create window! Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	return window;
}

void cleanup(SDL_Window* window) {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main() {
	// For nice time suffixes :)
	using namespace std::chrono_literals;

	SDL_Window* window = init();

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << "Could not create renderer! Error: " << SDL_GetError() << "\n";
		exit(1);
	}
	SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND >> 16 & 0xFF, COLOR_BACKGROUND >> 8 & 0xFF,
						   COLOR_BACKGROUND & 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, nullptr);
	// This creates pacman

	GameBoard gameBoard(renderer, SCREEN_HEIGHT / TEXTURE_SIZE, SCREEN_WIDTH / TEXTURE_SIZE);
	gameBoard.generate();

	SDL_Event curEvent;

	// so this is our event loop that will update the screen, move the ghosts, handle keyboard events, etc.
	// Hopefully we can make it relatively short and abstract most of the code elsewhere
	bool quit = false;
	while (!quit) {
		SDL_RenderPresent(renderer);

		SDL_Keycode* keycode = nullptr;

		// PollEvent puts the next event into curEvent
		while (SDL_PollEvent(&curEvent) != 0) {
			// Handle quit events so we can actually close the window without SIGTERM/SIGABRT
			if (curEvent.type == SDL_QUIT) {
				quit = true;
				continue;
			} else if (curEvent.type == SDL_KEYDOWN) {
				// Handle keyboard input
				keycode = &curEvent.key.keysym.sym;
			}
		}

		gameBoard.update(keycode);

//		std::cout << (int) p.r << " " << (int) p.c << "\n";

//		for (int i = 0; i < gameBoard.getRows(); i++) {
//			for (int j = 0; j < gameBoard.getCols(); j++) {
//				if (p.r == i && p.c == j) {
//					std::cout << "O";
//				} else if (gameBoard[i][j] == nullptr) {
//					std::cout << ".";
//				} else {
//					std::cout << "#";
//				}
//			}
//			std::cout << "\n";
//		}
//		std::cout << "\n";

		// slow the loop speed to something like 30fps (idk i'm bad at math, you should probably check this)
		std::this_thread::sleep_for(33ms);
	}

	// Cleanup
	cleanup(window);
	return 0;
}