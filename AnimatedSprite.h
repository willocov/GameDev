#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <map>
#include <string>
#include "Sprite.h"
#include "Globals.h"
#include <vector>
class Graphics;

/*
AnimatedSprite Class
Holds logic for animating sprites
*/

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate);
	
	//void playAnimation
	//Plays the animation provided if it's not already playing
	void playAnimation(std::string animation, bool once = false);

	//void update
	//Updates the animated sprite(timer)
	void update(int elapsedTime);

	//void draw
	//Draws the sprite to the screen
	void draw(Graphics &graphics, int x, int y);

	

protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;  

	//void addAnimation
	//Adds an animation to the map of animations for the sprite
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);
	
	//void resetAnimation
	//resets all current animations associated with the sprite
	void resetAnimations();

	//void stopAnimation
	//stops current animation
	void stopAnimation();

	//void setVisible
	//changes the visibility of the animated sprite
	void setVisible(bool visible);

	//void animationDone
	//Logic that happens when an animation ends
	virtual void animationDone(std::string currentAnimation) = 0;
	
	//virtual void setupAnimation
	//A required function that sets up all animations for a sprite
	virtual void setupAnimation() = 0;


private:
	std::map<std::string, std::vector<SDL_Rect> > _animations;
	std::map<std::string, Vector2> _offsets;

	int _franeIndex;
	double _timeElapsed;
	bool _visible;

};
#endif // !ANIMATED_SPRITE_H
