#ifndef RECTANGLES_H
#define RECTANGLES_H

#include "Globals.h"

class Rectangle
{
public:
	Rectangle() {}
	Rectangle(int x, int y, int width, int height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height)
	{}

	const int getCenterX() const { return this->_x + this->_width / 2; }
	const int getCenterY() const { return this->_y + this->_height / 2; }

	const int getLeft()const { return this->_x; }
	const int getRight() const { return this->_x + this->_width; }
	const int getTop() const { return this->_y; }
	const int getBottom() const { return this->_y + this->_height; }
	
	const int getWidth() const { return this->_width; }
	const int getHeight() const { return this->_height; }

	const int getSide(const sides::Side side) const {
		return
			side == sides::LEFT ? this->getLeft() :		//if (sides == sides::LEFT) return getLeft()
			side == sides::RIGHT ? this->getRight() :	//else id(sides...)return getRight()
			side == sides::TOP ? this->getTop() :		//else if..
			side == sides::BOTTOM ? this->getBottom() :	//else if..
			sides::NONE;								//else

	}

	//collision function
	//Takes in another rectangle and determines if the two are colliding
	const bool collidesWith(const Rectangle &other) const {
		return
			this->getRight() >= other.getLeft() &&	//if this rectangles right side is greater than or equal to the other rectangles left side
			this->getLeft() <= other.getRight() &&	//and if this rectangles left side is less than or equal to the other rectangles right side
			this->getTop() <= other.getBottom() &&		//...	
			this->getBottom() >= other.getTop();	//if all all true, then object is colliding, return true, otherwise false
	}

	//check if the rectangle is valid
	const bool isValidRectangle() const {
		return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

private:
	int _x, _y, _width, _height;
};

#endif // !RECTANGLES_H
