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

	int numOfFigures;

	bool isBot;
	
public:
	Player(Color _color, std::string _name, sf::Time timeLimit, bool _isBot = false)
		: color{ _color }, name{ _name }, remainingTime{ timeLimit }, timeIsUp{ false }, isBot{ _isBot }, numOfFigures{ 16 } { }

	int DecreaseNumOfFigures() { return --numOfFigures; } // TODO: don't use?

	Color GetColor() const { return color; }
	std::string GetName() const { return name; }
	bool GetIsBot() const { return isBot; }
	sf::Time GetRemainingTime();

	void StartTimer();

	// if time isn't limited, return true
	bool HasTime() const { return !timeIsUp; } 
};