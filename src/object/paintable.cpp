#include <object/paintable.h>

Paintable::Paintable(SDL_Renderer* renderer, const std::string& path, int x, int y) {
	texture = IMG_LoadTexture(renderer, path.c_str());
	if (texture == nullptr) {
		std::cout << "Failed to load image path: " << path << "! Error: " << SDL_GetError() << "\n";
		exit(1);
	}
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

	if (w != TEXTURE_SIZE && h != TEXTURE_SIZE) {
		std::cout << "Loaded texture with invalid size. Must be " << TEXTURE_SIZE << "x" << TEXTURE_SIZE << "\n";
		exit(1);
	}

	this->renderer = renderer;
	this->x = x;
	this->y = y;

	paint();
}

void Paintable::paint() {
	SDL_Rect pos = {.x=x, .y=y, .w=w, .h=h};
	SDL_RenderCopy(renderer, texture, nullptr, &pos);
}

void Paintable::unpaint() {
	SDL_Rect pos = {.x=x, .y=y, .w=w, .h=h};
	SDL_RenderFillRect(renderer, &pos);
}

int Paintable::getX() const {
	return x;
}

int Paintable::getY() const {
	return y;
}
