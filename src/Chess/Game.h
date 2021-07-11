#pragma once
#include "Drawer.h"
#include "Menu.h"
#include <SFML/Window.hpp>

enum class GameMode { TwoPlayers, PlayerWithBot };
enum class GameStatus;
class Menu;

class Game
{
protected:
	Drawer drawer;
	Map map;
	GameStatus status;

	
public:
	Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties, GameMode mode)
		: drawer{ window, resource, properties, mode, &Game::SetExitStatus, &Game::MakeUndoMove, this }, status{ GameStatus::Play } { }
	void virtual StartGame() = 0;
	void virtual Show() = 0;
	void virtual SetPosition(int mouseX, int mouseY) = 0; 
	void virtual MakeUndoMove() = 0;

	GameStatus GetStatus() const { return status; }
	void HandleEvent(sf::Event& event);

	void Save() const;
	void ActivateMenuSettings(Menu& menu);
	void SetExitStatus();
	
	virtual ~Game() { }
};