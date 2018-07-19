#ifndef GRAPHICS_H
#define GRAPHICS_H

/*
Graphic Class
Holds all information dealing with graphics for the game
*/
#include <SDL.h>
#include <map>
#include <string>

//Structs are used so we dont have to use include SDL
struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
	Graphics();
	~Graphics();

	/*SDL_Surface* loadImage
	loads an image into the _spritesSheets map if it doesn't already exist,
	as a result, each image will only be loaded once
	*/
	SDL_Surface* loadImage(const std::string &filePath);


	/*void blitSurface
	Draws a texture to a certain part of the screen
	function parameters(Which sprite sheet is being used, what part of the sprite sheet is being used, where the part of the sprite sheet will be used)
	*/
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);


	/*void flip
	Renders everthing to the screen
	*/
	void flip();

	/*void clear()
	Clears the screen
	*/
	void clear();

	/*SDL_Renderer* getRenderer
	Returns the renderer
	*/
	SDL_Renderer* getRenderer() const;

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	std::map<std::string, SDL_Surface*> _spriteSheets;
};

#endif