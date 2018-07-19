#include "Enemy.h"
#include "Player.h"
//Base enemy class
Enemy::Enemy(){}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(0),
	_currentHealth(0)
{}

void Enemy::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

//Bat Classs
Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "Content/Sprites/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	this->setupAnimation();
	this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player &player) {
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

	//Logic to move up or down
	this->_x += this->_shouldMoveUp ? -.02 : .02;	//SPEED OF THE BAT
	if (this->_x > (this->_startingX + 30) || this->_x < this->_startingX - 30) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}

	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation) {

}

void Bat::setupAnimation() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Bat::touchPlayer(Player* player) {
	player->gainHealth(-1);
}

//Walker Class
Walker::Walker() {}

Walker::Walker(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "Content/Sprites/NpcCemet.png", 32, 32, 24, 24, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveLeft(true)
{
	this->setupAnimation();
	this->playAnimation("WalkRight");
}

void Walker::update(int elapsedTime, Player &player) {
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "WalkRight" : "WalkLeft");

	//Logic to move up or down
	this->_x += this->_shouldMoveLeft ? -.05 : .05;	//SPEED OF THE BAT
	if (this->_x > (this->_startingX + 100) || this->_x < this->_startingX - 100) {
		this->_shouldMoveLeft = !this->_shouldMoveLeft;
	}

	Enemy::update(elapsedTime, player);
}

void Walker::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Walker::animationDone(std::string currentAnimation) {

}

void Walker::setupAnimation() {
	this->addAnimation(3, 1, 62, "WalkLeft",16, 24, Vector2(0, 0));
	this->addAnimation(3, 1, 96, "WalkRight", 16, 24, Vector2(0, 0));
}

void Walker::touchPlayer(Player* player) {
	player->gainHealth(-1);
}

