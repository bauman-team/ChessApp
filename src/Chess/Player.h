#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <Pos.h>

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

	bool isBot;
	
public:
	Player(Color _color, std::string _name, sf::Time timeLimit, bool _isBot = false)
		: color{ _color }, name{ _name }, remainingTime{ timeLimit }, timeIsUp{ false }, isBot{ _isBot }, numOfFigures{ 16 }, chosenPos{ } { }

	int DecreaseNumOfFigures() { return --numOfFigures; }
	void SetChosenPosition(Pos position) { chosenPos = position; }

	Pos GetChosenPosition() const { return chosenPos; }
	Color GetColor() const { return color; }
	std::string GetName() const { return name; }
	bool GetIsBot() const { return isBot; }
	sf::Time GetRemainingTime();

	void StartTimer();

	// if time isn't limited, return true
	bool HasTime() const { return !timeIsUp; } 
};