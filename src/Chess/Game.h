#pragma once
#include "Menu.h"
#include "Drawer.h"
#include <SFML/Window.hpp>

enum class GameMode { TwoPlayers, PlayerAndBot };
enum class GameStatus;
class Menu;

class Game
{
protected:
	Drawer drawer;
	Map map;
	GameStatus status;

	
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
		: drawer{ window, resource, properties, &Game::SetExitStatus, this }, status{ GameStatus::Play } { }
	void virtual StartGame() = 0;
	void virtual Show() = 0;
	void virtual ChangeActivePlayer() = 0;
	void virtual SetPosition(int mouseX, int mouseY) = 0; 
	void virtual SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0)) = 0;

	volatile GameStatus GetStatus() const { return status; }
	void HandleEvent(sf::Event& event);

	void Save() const;
	void ActivateMenuSettings(Menu& menu);
	void SetExitStatus();
	
	virtual ~Game() { }
};