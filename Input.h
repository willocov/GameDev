#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL.h>

class Input
{
public:
	Input();
	~Input();
	void beginNewFrame();
	void keyUpEvent(const SDL_Event& event);
	void keyDownEvent(const SDL_Event& event);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);
private:
	std::map<SDL_Scancode, bool> _heldKeys;		//Stores a key that is being held down
	std::map<SDL_Scancode, bool> _pressedKeys;	//Stores a kay that has been pressed
	std::map<SDL_Scancode, bool> _releasedKeys;	//Stores a key that has been released
};

#endif
