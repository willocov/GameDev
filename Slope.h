#ifndef  SLOPE_H
#define SLOPE_H
#include "Globals.h"
#include <math.h>
#include "Rectangle.h"
#include <cmath>
class Slope {
public:
	Slope() {}
	Slope(Vector2 p1, Vector2 p2) :
		_p1(p1),
		_p2(p2)
	{
		if (this->_p2.x - this->_p1.x != 0) {
			this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x)) ;
		}
	}

	const inline float getSlope() const {
		return this->_slope;
	}

	const bool collidesWith(const Rectangle &other) const {
		return
			//Each set determines if it's colliding with the slope object in the four possible ways
			//Top: increasing/decreasing
			//Bottom: increasing/decreasing
			//whichever set is true will be the way the player collides with the object
			(other.getRight() >= this->_p2.x &&
				other.getLeft() <= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y)
			
			|| (other.getRight() >= this->_p1.x &&
				other.getLeft() <= this->_p2.x &&
				other.getTop() <= this->_p1.y &&
				other.getBottom() >= this->_p2.y)
			
			|| (other.getLeft() <= this->_p1.x &&
				other.getRight() >= this->_p2.x &&
				other.getTop() <= this->_p1.y &&
				other.getBottom() >= this->_p2.y)
			
			|| (other.getLeft() <= this->_p2.x &&
				other.getRight() >= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y);
	}

	const inline Vector2 getp1() const { return this->_p1; }
	const inline Vector2 getp2() const { return this->_p2; }
private:
	Vector2 _p1;
	Vector2 _p2;
	float _slope;
};
#endif // ! SLOPE_H
