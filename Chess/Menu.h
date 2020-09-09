#pragma once
#include <TGUI/TGUI.hpp>
#include "Game.h"

struct InputValues
{
	GameMode mode;
	sf::Time time;
	std::string firstName;
	std::string secondName;
};

class Menu
{
	tgui::Gui gui;
	Game* game;
	InputValues inputValues;

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
	InputValues GetInputValues() const;
	bool CanStartGame(); // returns true if game can be started
	bool NeedStartGame() const;
	void HandleEvent(sf::Event& event);
	static std::string GetBotName();
};