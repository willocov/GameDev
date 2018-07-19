#ifndef ANIMATEDTILE_H
#define ANIMATEDTILE_H

#include <vector>
#include "Tile.h"
#include "Globals.h"

class AnimatedTile : public Tile {
public:
	AnimatedTile(std::vector<Vector2> tilesetPostitions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position);
	void update(int elapsedtime);
	void draw(Graphics &graphics);

protected:
	int _amountOfTime = 0;
	bool _notDone = false;

private:
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;
};

struct AnimatedTileInfo {
public:
	int TilesetsFirstGid;
	int StartTileId;
	std::vector<int> TileIds;
	int Duration;
};

#endif // !1