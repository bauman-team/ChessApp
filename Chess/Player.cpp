#include "Player.h"

Pos* Player::chosenPos = nullptr;

Player::Player(Color _color, std::string _name, sf::Time timeLimit) : color(_color), name(_name), remainingTime(timeLimit), timeIsUp(false)
{
	 numOfFigures = 16;
}

void Player::StartTimer()
{
	clock.restart();
}

sf::Time Player::GetRemainingTime()
{
	remainingTime -= clock.restart();
	if (remainingTime.asSeconds() > 0)
		return remainingTime;
	else
	{
		timeIsUp = true;
		sf::seconds(0);
	}
}
