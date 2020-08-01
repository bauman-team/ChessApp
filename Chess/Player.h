#pragma once
#include <string>
#include "Figure.h"

class Player
{
	std::string name;
	int numOfFigures;
	double timer;
	//Figure* chosenFigure;
public:
	Player(std::string _name, double _timer) : name(_name), timer(_timer) { numOfFigures = 16; } // if need add SAVE MOD replace numOfFigures to params
	int DecreaseNumOfFigures() { return --numOfFigures; }
	//void SetChosenFigure()
	//int8_t CheckGameFinal();
};
