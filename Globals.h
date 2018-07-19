#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals {

	//Resolution should be twice the size of the actual map size
	//because it's scaled up by 2
	const int SCREEN_WIDTH =1280;
	const int SCREEN_HEIGHT = 1024;

	const float SPRITE_SCALE = 2.0f;

}

namespace sides {
	enum Side{TOP, BOTTOM, LEFT, RIGHT, NONE};
	inline Side getOppositeSide(Side side)  {
		return
			side == TOP ? BOTTOM :	//if(side == top) return bottom
			side == BOTTOM ? TOP :	//else if(side == bottom) return top
			side == LEFT ? RIGHT :	//...
			side == RIGHT ? LEFT :	//...
			NONE;					//Else return none
	}

}

enum Direction{LEFT, RIGHT, UP, DOWN};


struct Vector2 {
	int x, y;
	Vector2():
	x(0), y(0){}
	Vector2(int x, int y) :
		x(x), y(y) {}
	Vector2 zero() {
		return Vector2(0, 0);
	}
};


#endif // !GLOBALS_H