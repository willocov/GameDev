#include "Game.h"
#include <SDL.h>
#include "graphics.h"
#include "Input.h"
#include <algorithm>


/*
Game Class
This holds all the information for our main game loop
*/

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game(std::string filename)
{
	//Initializes all SDL functions for the program
	SDL_Init(SDL_INIT_EVERYTHING);

	//Plays music (Doesnt work)
	this->music.load("peppershake.wav");
	this->music.play();

	//Starts the game loop
	this->gameLoop(filename);

}

Game::~Game(){}

void Game::gameLoop(std::string filename) {

	Graphics graphics;	//Creates a graphic object for every frame
	Input input;		//Checks for input 
	SDL_Event event;	//Holds whatever event happens during that frame
	
	this->_level = Level(filename, Vector2(100, 100), graphics);
	
	this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	this->_hud = HUD(graphics, this->_player);

	int LAST_UPDATE_TIME = SDL_GetTicks();	//Gets the number of milliseconds since SDL was initialized
	//Start Loop to check for events
	while (true)
	{
		input.beginNewFrame();
		if (SDL_PollEvent(&event))//Poll for pending events
		{
			if (event.type == SDL_KEYDOWN) {	//If a key has been pressed down
				if (event.key.repeat == 0) {	
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}
		//checks if user wants to quit program
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
			this->_player.moveLeft();
		}

		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
			this->_player.moveRight();
		}

		if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
			this->_player.lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
			this->_player.lookDown();
		}

		if (input.wasKeyReleased(SDL_SCANCODE_UP) == true) {
			this->_player.stopLookingUp();
		}
		
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN) == true) {
			this->_player.stopLookingDown();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_UP) == true) {
			this->_player.jump();
		}

		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.stopMoving();
		}


		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);

	}
	
}
void Game::draw(Graphics &graphics) {
	graphics.clear();
	
	this->_level.draw(graphics);
	this->_player.draw(graphics);
	this->_hud.draw(graphics);

	graphics.flip();

}
void Game::update(float elapsedTime) {
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);
	//check collisions
	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		//PLayer collided with a tile, handle it
		this->_player.handleTileCollision(others);
	}

	//checking slopes
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleSlopeCollisions(otherSlopes);
	}

	//check enemies
	std::vector<Enemy*> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleEnemyCollisions(otherEnemies);
		return;
	}
}
