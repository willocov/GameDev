#ifndef PLAYER_H
#define PLAYER_H
#include "AnimatedSprite.h"
#include "Globals.h"
#include "Slope.h"
#include "Enemy.h"
class Graphics;

class Player : public AnimatedSprite
{
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	~Player();
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	void moveLeft();	//Moves played left by -dx
	void moveRight();	//Moves player right by dx
	void stopMoving();	//Stop moving and resets direction
	void jump();		//Starts jump
	
	void lookUp();			//Player looks up
	void stopLookingUp();	//Player stops looking up
	void lookDown();		//Player looks down
	void stopLookingDown();	//Player stops looking down


	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimation();

	void handleTileCollision(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleEnemyCollisions(std::vector<Enemy*> &others);

	const float getX() const;
	const float getY() const;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	void gainHealth(int amount);

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
	bool _lookingUp;
	bool _lookingDown;

	int _maxHealth;
	int _currentHealth;
};


#endif // !PLAYER_H


