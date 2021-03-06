#include "Menu.h"
#include "meta-inf.h"

const std::string Menu::botName{ "bot" };

Menu::Menu(sf::RenderWindow& window, std::string widgetsFile) : menuGui{ window }
{
	menuGui.loadWidgetsFromFile(widgetsFile);
	LoadIcons();
	menuGui.get<tgui::Label>("Version")->setText(VER);
	if (menuGui.get<tgui::RadioButton>("TwoPlayersRB")->isChecked())
		ShowTwoPlayersSettings();
	else if (menuGui.get<tgui::RadioButton>("OnePlayerRB")->isChecked())
		ShowOnePlayerSettings();
	SetConnections();
	needStartGame = false;
	menuSize = menuGui.getView().getSize();
	menuGui.get<tgui::EditBox>("FirstPlayerName")->setInputValidator("[a-zA-Z][a-zA-Z0-9]*");
	menuGui.get<tgui::EditBox>("SecondPlayerName")->setInputValidator("[a-zA-z][a-zA-Z0-9]*");
}

void Menu::Show()
{
	menuGui.draw();
}

InitialData Menu::GetInitialData() const
{
	return initData;
}

void Menu::HandleEvent(sf::Event& event)
{
	menuGui.handleEvent(event);
}

std::string Menu::GetBotName()
{
	return botName;
}

tgui::Vector2f Menu::GetMenuSize() const
{
	return menuSize;
}

bool Menu::CanStartGame()
{
	std::string errorMessage{ };
	if (menuGui.get<tgui::RadioButton>("TwoPlayersRB")->isChecked())
	{
		auto firstName{ static_cast<std::string>(menuGui.get<tgui::EditBox>("FirstPlayerName")->getText()) };
		auto secondName{ static_cast<std::string>(menuGui.get<tgui::EditBox>("SecondPlayerName")->getText()) };
		if (firstName.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			errorMessage = "Enter the name of the first player!";
		else if (secondName.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			errorMessage = "Enter the name of the second player!";
		else if (firstName == secondName)
			errorMessage = "Enter different names for players!";
		else
		{
			initData.mode = GameMode::TwoPlayers;
			initData.firstName = firstName;
			initData.secondName = secondName;
			initData.time = sf::seconds(0);
			if (menuGui.get<tgui::CheckBox>("TimerCheckBox")->isChecked())
			{
				initData.time = sf::seconds(menuGui.get<tgui::Slider>("TimeSlider")->getValue() * 60);
			}
		}

	}
	else if (menuGui.get<tgui::RadioButton>("OnePlayerRB")->isChecked())
	{
		auto name{ static_cast<std::string>(menuGui.get<tgui::EditBox>("FirstPlayerName")->getText()) };
		if (name.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			errorMessage = "Enter the name!";
		else
		{
			initData.mode = GameMode::PlayerWithBot;
			if (menuGui.get<tgui::Tabs>("ColorsTab")->getSelected() == "White")
			{
				initData.firstName = name;
				initData.secondName = botName;
			}
			else
			{
				initData.firstName = botName;
				initData.secondName = name;
			}
			initData.time = sf::seconds(0);
			if (menuGui.get<tgui::CheckBox>("TimerCheckBox")->isChecked())
			{
				initData.time = sf::seconds(menuGui.get<tgui::Slider>("TimeSlider")->getValue() * 60);
			}
		}
	}
	else
		errorMessage = "Select mode of the game!";

	menuGui.get<tgui::Label>("MessageLabel")->setText(errorMessage);
	needStartGame = errorMessage == "";
	return needStartGame;
}

bool Menu::NeedStartGame() const
{
	return needStartGame;
}

void Menu::ActivateStartMenu()
{
	needStartGame = false;
}

void Menu::SetConnections()
{
	auto timerCheckBox{ menuGui.get("TimerCheckBox") };
	timerCheckBox->connect(tgui::Signals::CheckBox::Checked, &Menu::ShowTimerSettings, this);
	timerCheckBox->connect(tgui::Signals::CheckBox::Unchecked, &Menu::HideTimerSettings, this);

	auto timeSlider{ menuGui.get("TimeSlider") };
	timeSlider->connect(tgui::Signals::Slider::ValueChanged, &Menu::ChangeChoosenTime, this);

	auto twoPlayersRB{ menuGui.get<tgui::RadioButton>("TwoPlayersRB") };
	twoPlayersRB->connect(tgui::Signals::RadioButton::Checked, &Menu::ShowTwoPlayersSettings, this);

	auto onePlayerRB{ menuGui.get<tgui::RadioButton>("OnePlayerRB") };
	onePlayerRB->connect(tgui::Signals::RadioButton::Checked, &Menu::ShowOnePlayerSettings, this);

	auto colorsTab{ menuGui.get<tgui::Tabs>("ColorsTab") };
	colorsTab->connect(tgui::Signals::Tabs::TabSelected, &Menu::ShowChoosenColor, this);

	auto startButton{ menuGui.get<tgui::Button>("StartButton") };
	startButton->connect(tgui::Signals::Button::Pressed, &Menu::CanStartGame, this);
}

void Menu::LoadIcons()
{
	auto firstPlayerIcon{ menuGui.get<tgui::Picture>("FirstPlayerIcon") };
	tgui::Texture firstIconTexture{ "../res/images/Pawn_white.png" };
	firstPlayerIcon->getRenderer()->setTexture(firstIconTexture);

	auto secondPlayerIcon{ menuGui.get<tgui::Picture>("SecondPlayerIcon") };
	tgui::Texture secondIconTexture{ "../res/images/Pawn_black.png" };
	secondPlayerIcon->getRenderer()->setTexture(secondIconTexture);
}

void Menu::ShowTimerSettings()
{
	menuGui.get("TimeLabel")->setVisible(true);
	menuGui.get("TimeSlider")->setVisible(true);
}

void Menu::HideTimerSettings()
{
	menuGui.get("TimeLabel")->setVisible(false);
	menuGui.get("TimeSlider")->setVisible(false);
}

void Menu::ShowOnePlayerSettings()
{
	menuGui.get("FirstPlayerIcon")->setVisible(true);
	menuGui.get("FirstPlayerName")->setVisible(true);
	menuGui.get("SecondPlayerIcon")->setVisible(false);
	menuGui.get("SecondPlayerName")->setVisible(false);
	menuGui.get("ColorsTab")->setVisible(true);
	ShowChoosenColor();
}

void Menu::ShowTwoPlayersSettings()
{
	auto firstIconTexture{ tgui::Texture{ "../res/images/Pawn_white.png" } };
	menuGui.get<tgui::Picture>("FirstPlayerIcon")->getRenderer()->setTexture(firstIconTexture);

	menuGui.get("FirstPlayerIcon")->setVisible(true);
	menuGui.get("FirstPlayerName")->setVisible(true);
	menuGui.get("SecondPlayerIcon")->setVisible(true);
	menuGui.get("SecondPlayerName")->setVisible(true);
	menuGui.get("ColorsTab")->setVisible(false);
}

void Menu::ChangeChoosenTime()
{
	menuGui.get<tgui::Label>("TimeLabel")->setText("Minutes: " + std::to_string(static_cast<int>(menuGui.get<tgui::Slider>("TimeSlider")->getValue())));
}

void Menu::ShowChoosenColor()
{
	std::string texturePath{ (menuGui.get<tgui::Tabs>("ColorsTab")->getSelected() == "White") ? "../res/images/Pawn_white.png" : "../res/images/Pawn_black.png" };
	tgui::Texture firstIconTexture{ texturePath };
	auto firstPlayerIcon{ menuGui.get<tgui::Picture>("FirstPlayerIcon") };
	firstPlayerIcon->getRenderer()->setTexture(firstIconTexture);
}
