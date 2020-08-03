#pragma once
#include <string>
#include "Figure.h"

class Figure;
class Pos;
class Map;
enum class Color;

class Player
{
	const Color color;
	const std::string name;
	double timer;
	static Pos* chosenPos;
	int numOfFigures;
public:
	Player(Color _color, std::string _name, double _timer);

	int DecreaseNumOfFigures() { return --numOfFigures; }
	void SetChosenPosition(Pos* position) { chosenPos = position; }
	Pos* GetChosenPosition() const { return chosenPos; }
	Color GetColor() const { return color; }
	
	void RunFindMoves(Figure* choseFigure);
	void RunMakeMove(Figure* choseFigure, Pos& currentPosition);
	void RunClearPossibleMoves(Map& map);
	//int8_t CheckGameFinal();
};