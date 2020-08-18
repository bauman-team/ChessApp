#pragma once
#include "Map.h"
#include "Drawer.h"
#include "Pos.h"
#include "MoveInfo.h"

class Game
{
protected:
	Drawer drawer;
	Map map;
	int8_t isWin;
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	void virtual Show() = 0;
	void virtual ChangeActivePlayer() = 0;
	void virtual SetPlayerChosenCell(int, int) = 0;
	int8_t virtual CheckGameFinal() = 0;

	int8_t GetIsWin() { return isWin; }

	void Save();
};