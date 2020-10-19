#include "Player.h"

Player::Player(Color _color, std::string _name, sf::Time timeLimit, bool _isBot) : color(_color), name(_name), remainingTime(timeLimit), timeIsUp(false), isBot(_isBot)
{
	 numOfFigures = 16;
	 chosenPos = Pos::NULL_POS;
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
		return sf::seconds(0);
	}
}
