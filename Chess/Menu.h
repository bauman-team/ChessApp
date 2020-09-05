#pragma once
#include <TGUI/TGUI.hpp>
#include "Game.h"

class Menu
{
	tgui::Gui gui;
	Game* game;

	void SetConnections();
	void LoadIcons();

	void ShowTimerSettings();
	void HideTimerSettings();

	void ShowOnePlayerSettings();
	void ShowTwoPlayersSettings();

	void ChangeChoosenTime();
	void ShowChoosenColor();
public:
	Menu(sf::RenderWindow& window, Game* _game, std::string widgetsFile);

	void Show();
	void PrestartChecks();
	void HandleEvent(sf::Event& event);
};
