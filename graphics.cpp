#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "Globals.h"

/*
Graphic Class
Holds all information dealing with graphics for the game
*/

Graphics::Graphics()
{
	//Parameters = (window size(640, 480), flags(0), pointer to window, pointer to renderer
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);

	//Sets the title for game window
	SDL_SetWindowTitle(this->_window, "William's Game Engine: Test 1.6");
}


Graphics::~Graphics()
{
	//destroys the game window
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

//Loads and returns image
SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	if (this->_spriteSheets.count(filePath) == 0) {		//If the image hasnt been loaded yet
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());		//Loads the image
	}
	return this->_spriteSheets[filePath];
}


void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer()const {
	return this->_renderer;
}