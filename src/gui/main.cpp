#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <thread>
#include <gui/colors.h>
#include <object/movable.h>

const int SCREEN_WIDTH = 1024;
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

	Movable pacman(renderer, "../assets/pacman_small.png", 0, 10, SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

	SDL_Event curEvent;

	std::this_thread::sleep_for(5s);

	// so this is our event loop that will update the screen, move the ghosts, handle keyboard events, etc.
	// Hopefully we can make it relatively short and abstract most of the code elsewhere
	bool quit = false;
	while (!quit) {
		SDL_RenderPresent(renderer);

		if (pacman.isAtXBounds()) {
			if (pacman.getLastDirection() == LEFT) {
				pacman.move(RIGHT);
			} else {
				pacman.move(LEFT);
			}
		} else {
			pacman.move(pacman.getLastDirection());
		}
		printf("x: %d y: %d\n", pacman.getX(), pacman.getY());

		// PollEvent puts the next event into curEvent
		while (SDL_PollEvent(&curEvent) != 0) {
			// Handle quit events so we can actually close the window without SIGTERM/SIGABRT
			if (curEvent.type == SDL_QUIT) {
				quit = true;
				continue;
			}
		}

		// slow the loop speed to something like 30fps (idk i'm bad at math, you should probably check this)
		std::this_thread::sleep_for(33ms);
	}

	// Cleanup
	cleanup(window);
	return 0;
}
