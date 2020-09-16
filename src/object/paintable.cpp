#include <object/paintable.h>

Paintable::Paintable(SDL_Renderer* renderer, const std::string& path, int x, int y) {
	texture = IMG_LoadTexture(renderer, path.c_str());
	if (texture == nullptr) {
		std::cout << "Failed to load image path: " << path << "! Error: " << SDL_GetError() << "\n";
		exit(1);
	}
	this->renderer = renderer;
	this->x = x;
	this->y = y;
}


void Paintable::paint() {
	SDL_Rect pos = {.x=x, .y=y};
	SDL_QueryTexture(texture, nullptr, nullptr, &pos.w, &pos.h);
	SDL_RenderCopy(renderer, texture, nullptr, &pos);
}

int Paintable::getX() const {
	return x;
}

int Paintable::getY() const {
	return y;
}