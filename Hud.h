#ifndef  HUD_H
#define HUD_H

#include "Sprite.h"
#include "Player.h"
class Graphics;


class HUD {
public:
	HUD();
	HUD(Graphics &graphics, Player &player);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
private:
	Player _player;

	//Health Sprites
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;

	//exp sprites
	Sprite _lvWord;
	Sprite _lvNumber;
	Sprite _expBar;

	//weapon info 
	Sprite _slash;
	Sprite _dashes;
};

#endif // ! HUD_H
