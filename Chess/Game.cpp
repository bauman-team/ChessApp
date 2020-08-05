#include "Game.h"

Game::Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: drawer(window, resource, properties)
{
	Figure::SetMapPtr(&map);
	isWin = 0;
};
