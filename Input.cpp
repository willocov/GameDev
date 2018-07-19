#include "Input.h"

/*
Input Class
keeps track of keyboard state
*/

Input::Input()
{
}


Input::~Input()
{
}

//This function is called at the begining of each new frame
//to reset the keys that are no longer relevant
void Input::beginNewFrame() {
	//Clears keys from previous frame
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();
}

//This function is called when a key has been pressed
void Input::keyDownEvent(const SDL_Event& event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;	//heldKey is also called because a key is techincally held down when pressed
}


//This function is called when a key is released
void Input::keyUpEvent(const SDL_Event& event) {
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false; //Set false because the key is no longer being held when it is released
}

//Check if a certain key was pressed during the current frame
bool Input::wasKeyPressed(SDL_Scancode key) {
	return this->_pressedKeys[key];
}

//Check if a ceratin key was released during the current frame
bool Input::wasKeyReleased(SDL_Scancode key) {
	return this->_releasedKeys[key];
}

//Check if a certain key is currently being held
bool Input::isKeyHeld(SDL_Scancode key){
	return this->_heldKeys[key];
}