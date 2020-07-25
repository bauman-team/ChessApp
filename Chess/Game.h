#pragma once
#include "Map.h"
#include "Drawer.h"

class Game
{
protected:
	Drawer drawer;
	Map map;
public:
	//Game() {};
	Game(sf::RenderWindow* window, const Resources& resource) : drawer(window, resource) {};
	//Game(const Resources* resource) : drawer(resource) {};

	//void SetResources(const Resources& resource) { drawer.SetResources(resource); }

	void virtual StartGame() = 0;
	void virtual ChangeActivePlayer() = 0;
};