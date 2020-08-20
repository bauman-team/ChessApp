#pragma once
#include "Map.h"
#include "Drawer.h"
#include "Pos.h"
#include "MoveInfo.h"

enum class GameStatus { Menu, Play };

class Game
{
protected:
	Drawer drawer;
	Map map;
	int8_t isWin;
	GameStatus status;
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	void virtual Show() = 0;
	void virtual ChangeActivePlayer() = 0;
	void virtual SetPlayerChosenCell(int, int) = 0;
	int8_t virtual CheckGameFinal() = 0;

	int8_t GetIsWin() const { return isWin; }
	GameStatus GetStatus() const { return status; }

	void StartGame();
	void Save();
};