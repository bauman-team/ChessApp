#pragma once
#include "Map.h"
#include "Drawer.h"
#include "Pos.h"

class Game
{
protected:
	Drawer drawer;
	Map map;
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
		: drawer(window, resource, properties)
	{
		Figure::SetMapPtr(&map);
	};

	void virtual StartGame() = 0;
	void virtual ChangeActivePlayer() = 0;
	void virtual SetPlayerChosenCell(int, int) = 0;
};