#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main() {
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Failed to initialize SDL! Error: " << SDL_GetError() << "\n";
	}

	// Create a new window object
	SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
										  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "Could not create window! Error: " << SDL_GetError() << "\n";
	}

	// Cleanup
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
