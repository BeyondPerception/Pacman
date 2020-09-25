#include <object/paintable.h>

Paintable::Paintable(SDL_Renderer* renderer, const std::string& path, int x, int y, bool visible) {
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
	this->visible = visible;

	if (visible) {
		paint();
	}
}

void Paintable::paint() {
	SDL_Rect pos = {.x=x, .y=y, .w=w, .h=h};
	SDL_RenderCopy(renderer, texture, nullptr, &pos);
	visible = true;
}

void Paintable::unpaint() {
	SDL_Rect pos = {.x=x, .y=y, .w=w, .h=h};
	SDL_RenderFillRect(renderer, &pos);
	visible = false;
}

int Paintable::getX() const {
	return x;
}

int Paintable::getY() const {
	return y;
}

bool Paintable::intersects(Paintable o) {
	if (!visible || !o.isVisible()) {
		return false;
	}

	return containsPoint(o.getX(), o.getY()) || containsPoint(o.getX() + TEXTURE_SIZE, o.getY()) ||
		   containsPoint(o.getX(), o.getY() + TEXTURE_SIZE) ||
		   containsPoint(o.getX() + TEXTURE_SIZE, o.getY() + TEXTURE_SIZE);
}

inline bool Paintable::containsPoint(int x1, int y1) const {
	return x <= x1 && x + TEXTURE_SIZE >= x1 && y <= y1 && y + TEXTURE_SIZE >= y1;
}

bool Paintable::isVisible() const {
	return visible;
}
