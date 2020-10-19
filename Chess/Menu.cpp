#include "Menu.h"

const std::string Menu::botName = "bot";

Menu::Menu(sf::RenderWindow& window, std::string widgetsFile) : menuGui(window)
{
	menuGui.loadWidgetsFromFile(widgetsFile);
	LoadIcons();
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

InputValues Menu::GetInputValues() const
{
	return inputValues;
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
	std::string errorMessage = "";
	if (menuGui.get<tgui::RadioButton>("TwoPlayersRB")->isChecked())
	{
		std::string firstName = menuGui.get<tgui::EditBox>("FirstPlayerName")->getText();
		std::string secondName = menuGui.get<tgui::EditBox>("SecondPlayerName")->getText();
		if (firstName.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			errorMessage = "Enter the name of the first player!";
		else if (secondName.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			errorMessage = "Enter the name of the second player!";
		else if (firstName == secondName)
			errorMessage = "Enter different names for players!";
		else
		{
			inputValues.mode = GameMode::TwoPlayers;
			inputValues.firstName = firstName;
			inputValues.secondName = secondName;
			inputValues.time = sf::seconds(0);
			if (menuGui.get<tgui::CheckBox>("TimerCheckBox")->isChecked())
			{
				inputValues.time = sf::seconds(menuGui.get<tgui::Slider>("TimeSlider")->getValue() * 60);
			}
		}

	}
	else if (menuGui.get<tgui::RadioButton>("OnePlayerRB")->isChecked())
	{
		std::string name = menuGui.get<tgui::EditBox>("FirstPlayerName")->getText();
		if (name.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			errorMessage = "Enter the name!";
		else
		{
			inputValues.mode = GameMode::PlayerAndBot;
			if (menuGui.get<tgui::Tabs>("ColorsTab")->getSelected() == "White")
			{
				inputValues.firstName = name;
				inputValues.secondName = botName;
			}
			else
			{
				inputValues.firstName = botName;
				inputValues.secondName = name;
			}
			inputValues.time = sf::seconds(0);
			if (menuGui.get<tgui::CheckBox>("TimerCheckBox")->isChecked())
			{
				inputValues.time = sf::seconds(menuGui.get<tgui::Slider>("TimeSlider")->getValue() * 60);
			}
		}
	}
	else
		errorMessage = "Select mode of the game!";

	menuGui.get<tgui::Label>("MessageLabel")->setText(errorMessage);
	needStartGame = (errorMessage == "") ? true : false;
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
	tgui::Widget::Ptr timerCheckBox = menuGui.get("TimerCheckBox");
	timerCheckBox->connect(tgui::Signals::CheckBox::Checked, &Menu::ShowTimerSettings, this);
	timerCheckBox->connect(tgui::Signals::CheckBox::Unchecked, &Menu::HideTimerSettings, this);

	tgui::Widget::Ptr timeSlider = menuGui.get("TimeSlider");
	timeSlider->connect(tgui::Signals::Slider::ValueChanged, &Menu::ChangeChoosenTime, this);

	tgui::RadioButton::Ptr twoPlayersRB = menuGui.get<tgui::RadioButton>("TwoPlayersRB");
	twoPlayersRB->connect(tgui::Signals::RadioButton::Checked, &Menu::ShowTwoPlayersSettings, this);

	tgui::RadioButton::Ptr onePlayerRB = menuGui.get<tgui::RadioButton>("OnePlayerRB");
	onePlayerRB->connect(tgui::Signals::RadioButton::Checked, &Menu::ShowOnePlayerSettings, this);

	tgui::Tabs::Ptr colorsTab = menuGui.get<tgui::Tabs>("ColorsTab");
	colorsTab->connect(tgui::Signals::Tabs::TabSelected, &Menu::ShowChoosenColor, this);

	tgui::Button::Ptr startButton = menuGui.get<tgui::Button>("StartButton");
	startButton->connect(tgui::Signals::Button::Pressed, &Menu::CanStartGame, this);
}

void Menu::LoadIcons()
{
	tgui::Picture::Ptr firstPlayerIcon = menuGui.get<tgui::Picture>("FirstPlayerIcon");
	tgui::Texture firstIconTexture("images/Pawn_white.png");
	firstPlayerIcon->getRenderer()->setTexture(firstIconTexture);

	tgui::Picture::Ptr secondPlayerIcon = menuGui.get<tgui::Picture>("SecondPlayerIcon");
	tgui::Texture secondIconTexture("images/Pawn_black.png");
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
	tgui::Texture firstIconTexture("images/Pawn_white.png");
	menuGui.get<tgui::Picture>("FirstPlayerIcon")->getRenderer()->setTexture(firstIconTexture);

	menuGui.get("FirstPlayerIcon")->setVisible(true);
	menuGui.get("FirstPlayerName")->setVisible(true);
	menuGui.get("SecondPlayerIcon")->setVisible(true);
	menuGui.get("SecondPlayerName")->setVisible(true);
	menuGui.get("ColorsTab")->setVisible(false);
}

void Menu::ChangeChoosenTime()
{
	menuGui.get<tgui::Label>("TimeLabel")->setText("Minutes: " + std::to_string((int)menuGui.get<tgui::Slider>("TimeSlider")->getValue()));
}

void Menu::ShowChoosenColor()
{
	std::string texturePath = (menuGui.get<tgui::Tabs>("ColorsTab")->getSelected() == "White") ? "images/Pawn_white.png" : "images/Pawn_black.png";
	tgui::Texture firstIconTexture(texturePath);
	tgui::Picture::Ptr firstPlayerIcon = menuGui.get<tgui::Picture>("FirstPlayerIcon");
	firstPlayerIcon->getRenderer()->setTexture(firstIconTexture);
}
