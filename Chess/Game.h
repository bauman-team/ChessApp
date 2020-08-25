#pragma once
#include "Map.h"
#include "Drawer.h"
#include "Pos.h"
#include "MoveInfo.h"

enum class GameStatus { Menu, Play, Shah, Mat, Pat, TimeIsOver };

class Game
{
protected:
	Drawer drawer;
	Map map;
	GameStatus status;

	GameStatus virtual CheckGameFinal() = 0;
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	void virtual StartGame();
	void virtual Show() = 0;
	void virtual ChangeActivePlayer() = 0;
	void virtual SetPlayerChosenCell(int, int) = 0;

	GameStatus GetStatus() const { return status; }

	void Save();

	virtual ~Game(){}
};