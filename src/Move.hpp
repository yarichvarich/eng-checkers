#ifndef MOVE_HPP
#define MOVE_HPP

#include <iostream>

struct mMove {
    int px;
	int py;
	int x;
	int y;

	mMove(int px1, int py1, int x1, int y1) : px(px1), py(py1), x(x1), y(y1) {};
};

#endif