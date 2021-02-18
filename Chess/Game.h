#pragma once
#include "Map.h"
#include "Drawer.h"
#include "Menu.h"
#include <mutex>

enum class GameMode { TwoPlayers, PlayerAndBot };
enum class GameStatus;
class Menu;

class Game
{
protected:
	Drawer drawer;
	Map map;
	GameStatus status;
	tgui::Gui gameGui;

	void UpdateSideMenu();
	void SetExitStatus(); // private?
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	void virtual StartGame() = 0;
	void virtual Show() = 0;
	void virtual ChangeActivePlayer() = 0;
	void virtual SetPosition(int mouseX, int mouseY) = 0; 
	void virtual SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0)) = 0;

	volatile GameStatus GetStatus() const;

	void Save() const;
	void HandleEvent(sf::Event& event);
	void ActivateMenuSettings(Menu& menu);

	virtual ~Game(){}
};