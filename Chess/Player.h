#pragma once
#include <string>
#include "Figure.h"

class Player
{
	Color color;
	std::string name;
	double timer;
	int numOfFigures;
public:
	Player(Color _color, std::string _name, double _timer);

	int DecreaseNumOfFigures() { return --numOfFigures; }
	//int8_t CheckGameFinal();
};
