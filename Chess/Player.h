#pragma once
#include <string>
#include "Figure.h"

class Figure;
class Pos;
enum class Color;
class Player
{
	Color color;
	std::string name;
	double timer;
	static Pos* chosenPos;
	int numOfFigures;
public:
	Player(Color _color, std::string _name, double _timer);

	int DecreaseNumOfFigures() { return --numOfFigures; }
	void SetChosenPosition(Pos* position) { chosenPos = position; }
	Pos* GetChosenPosition() { return chosenPos; }
	Color GetColor() { return color; }
	
	void RunFindMoves(Figure* choseFigure);
	void RunMakeMove(Figure* choseFigure, Pos& currentPosition);
	//int8_t CheckGameFinal();
};


