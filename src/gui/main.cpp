#include <SDL2/SDL.h>
#include <iostream>
#include <thread>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

SDL_Window* init() {
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Failed to initialize SDL! Error: " << SDL_GetError() << "\n";
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

	// This is what we will draw on
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_Event curEvent;

	// so this is our event loop that will update the screen, move the ghosts, handle keyboard events, etc.
	// Hopefully we can make it relatively short and abstract most of the code elsewhere
	bool quit = false;
	while (!quit) {
		// We refill the window every loop because sometimes it can bug out if you move it around to quickly
		SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
		// This is basically like a call to repaint() in java
		SDL_UpdateWindowSurface(window);

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
