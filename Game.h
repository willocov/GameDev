#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Level.h"
#include "Audio.h"
#include "Hud.h"
#include <string>
class Graphics;

class Game
{
public:
	Game(std::string filename);
	~Game();


private:
	void gameLoop(std::string filename);
	void draw(Graphics &graphics);
	void update(float elapsedTime);
	Player _player ;
	Level _level;
	HUD _hud;
	Audio music;
};

#endif
