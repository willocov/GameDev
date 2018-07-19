#include "Player.h"
#include "graphics.h"

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;
	const float GRAVITY = 0.001f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player(){}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "Content/Sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_lookingUp(false), 
	_lookingDown(false), 
	_maxHealth(1),
	_currentHealth(1)
{
	graphics.loadImage("Content/Sprites/MyChar.png");
	this->setupAnimation();
	this->playAnimation("RunRight");
}

Player::~Player(){}

void Player::setupAnimation() {
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0,0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0,0));
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0,0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0,0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0,0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0,0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0,0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0,0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0,0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0,0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

void Player::moveLeft() {
	if (this->_lookingDown == true && this->_grounded == true) {
		return;
	}
	this->_dx = -player_constants::WALK_SPEED;
	if (this->_lookingUp == false) {
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight() {
	if (this->_lookingDown == true && this->_grounded == true) {
		return;
	}
	this->_dx = player_constants::WALK_SPEED;
	if (this->_lookingDown == false) {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	if (this->_lookingUp == false && this->_lookingDown == false) {
	this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::lookUp() {
	this->_lookingUp = true;
	if (this->_dx == 0) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp(){
	this->_lookingUp = false;
}

void Player::lookDown() {
	this->_lookingDown = true;
	if (this->_grounded == true) {
		//we need to interact (turn backwards)
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "LookingDownRight" : "LookingDownLeft");
	}
	
}

void Player::stopLookingDown() {
	this->_lookingDown = false;
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

//handleTileCollisions
//handles all collisions the player is colliding with
void Player::handleTileCollision(std::vector<Rectangle> &others) {
	//figure out which side colluision happened and move player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded) {
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
				}
			
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() -1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}
		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	for (int i = 0; i < others.size(); i++) {
		//calculate where on the slope the players bottom center is tounching
		// and use y = mx + b to figure out the y position to place him at
		//First calculcate "b" (slope intercept) using one of the points (b= y - mx)
		int b = (others.at(i).getp1().y - (others.at(i).getSlope() * fabs(others.at(i).getp1().x)));

		//now get the players center x
		int centerX = this->_boundingBox.getCenterX();

		//now pass that x into the equation uy = mx + b
		int newY = (others.at(i).getSlope() * centerX) + b - 8;	//8 is a temporary fix for a problem

		//Reposition the player to the correct y
		if (this->_grounded) {
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*> &others) {
	for (int i = 0; i < others.size(); i++) {
		others.at(i)->touchPlayer(this);
	}
}

void Player::gainHealth(int amount) {
	this->_currentHealth += amount;
}

void Player::update(float elapsedTime) {
	//Aplpy gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}
	//Move by dx
	this->_x += this->_dx * elapsedTime;
	//move by dy
	this->_y += this->_dy * elapsedTime;

	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}