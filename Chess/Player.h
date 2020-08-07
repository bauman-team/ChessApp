#pragma once
#include <string>
#include "Figure.h"
#include <SFML/Window.hpp>

class Figure;
class Pos;
class Map;
enum class Color;

class Player
{
	sf::Clock clock;
	const Color color;
	const std::string name;
	sf::Time remainingTime;
	static Pos* chosenPos;
	int numOfFigures;
	bool timeIsUp;
public:
	Player(Color _color, std::string _name, sf::Time timeLimit);

	int DecreaseNumOfFigures() { return --numOfFigures; }
	void SetChosenPosition(Pos* position) { chosenPos = position; }
	Pos* GetChosenPosition() const { return chosenPos; }
	Color GetColor() const { return color; }

	void StartTimer();
	sf::Time GetRemainingTime();
	bool HasTime() const { return !timeIsUp; }
};