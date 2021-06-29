#pragma once
#include "Game.h"

class Game;
enum class GameStatus;
enum class GameMode;

struct InitialData
{
	GameMode mode;
	sf::Time time;
	std::string firstName;
	std::string secondName;
};

class Menu
{
	tgui::Gui menuGui;
	Game* game;
	InitialData initData;
	tgui::Vector2f menuSize;

	const static std::string botName;
	bool needStartGame; // true if the start button was pressed and the game can be started

	void SetConnections();
	void LoadIcons();

	void ShowTimerSettings();
	void HideTimerSettings();

	void ShowOnePlayerSettings();
	void ShowTwoPlayersSettings();

	void ChangeChoosenTime();
	void ShowChoosenColor();
public:
	Menu(sf::RenderWindow& window, std::string widgetsFile);

	void Show();
	InitialData GetInitialData() const;
	bool CanStartGame(); // returns true if game can be started
	bool NeedStartGame() const;
	void ActivateStartMenu();
	void HandleEvent(sf::Event& event);
	static std::string GetBotName();
	tgui::Vector2f GetMenuSize() const;
};