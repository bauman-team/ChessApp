#include <SFML/Window.hpp>
#include "Player.h"

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
