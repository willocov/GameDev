#include "AnimatedTile.h"
#include "graphics.h"
#include "Tile.h"

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPostitions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position) :
	Tile(tileset, size, tilesetPostitions.at(0), position),
	_tilesetPositions(tilesetPostitions),
	_duration(duration),
	_tileToDraw(0)
{}


void AnimatedTile::update(int elapsedtime) {
//timer code
	if (this->_amountOfTime <= 0) {
		if (this->_tileToDraw == this->_tilesetPositions.size() - 1) {
			this->_tileToDraw = 0;
		}
		else {
			this->_tileToDraw++;
		}
		this->_amountOfTime = this->_duration;
	}
	else {
		this->_amountOfTime -= elapsedtime;
	}
	Tile::update(elapsedtime);
}
void AnimatedTile::draw(Graphics &graphics) {
	SDL_Rect destRect = { this->_position.x, this->_position.y, this->_size.x * globals::SPRITE_SCALE, this->_size.y * globals::SPRITE_SCALE };

	SDL_Rect sourceRect = { this->_tilesetPositions.at(this->_tileToDraw).x, this->_tilesetPositions.at(this->_tileToDraw).y, this->_size.x, this->_size.y };

	graphics.blitSurface(this->_tileSet, &sourceRect, &destRect);
}