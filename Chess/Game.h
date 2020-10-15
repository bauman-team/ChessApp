#pragma once
#include "Map.h"
#include "Drawer.h"
#include "Menu.h"

enum class GameStatus { Play, Shah, Mat, Pat, TimeIsOver, Exit };
enum class GameMode { TwoPlayers, PlayerAndBot };

class Menu;

class Game
{
protected:
	Drawer drawer;
	Map map;
	GameStatus status;

	tgui::Gui gui;

	GameStatus virtual CheckGameFinal() = 0;
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	void virtual StartGame();
	void virtual Show() = 0;
	void virtual ChangeActivePlayer() = 0;
	bool virtual SetPlayerChosenCell(int, int) = 0;
	void virtual SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0)) = 0;

	GameStatus GetStatus() const { return status; }

	void Save();
	void virtual HandleEvent(sf::Event& event);
	void SetExitStatus() { status = GameStatus::Exit; };
	void ReturnGameToInitialSettings(Menu& menu);
	void UpdateSideMenu();

	virtual ~Game(){}
};