#pragma once
#include <string>
#include <SFML/Window.hpp>
#include "Pos.h"

enum class Color;

class Player
{
	sf::Clock clock;
	sf::Time remainingTime;
	bool timeIsUp;

	const Color color;
	std::string name;

	Pos chosenPos;
	int numOfFigures;
	
public:
	Player(Color _color, std::string _name, sf::Time timeLimit);

	int DecreaseNumOfFigures() { return --numOfFigures; }
	void SetChosenPosition(Pos position) { chosenPos = position; }

	const Pos& GetChosenPosition() const { return chosenPos; }
	Color GetColor() const { return color; }
	std::string GetName() const { return name; }

	void StartTimer();
	sf::Time GetRemainingTime();
	bool HasTime() const { return !timeIsUp; } // if time isn't limited, return true
};