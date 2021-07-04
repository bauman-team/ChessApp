#pragma once
#include <string>

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
		: color{ _color }, name{ _name }, remainingTime{ timeLimit }, timeIsUp{ false }, isBot{ _isBot } { }

	Color GetColor() const { return color; }
	std::string GetName() const { return name; }
	bool GetIsBot() const { return isBot; }
	sf::Time GetRemainingTime();

	void StartTimer();

	// if time isn't limited, return true
	bool HasTime() const { return !timeIsUp; } 
};