#include "Menu.h"
#include "TwoPlayersGame.h"

const std::string Menu::botName = "bot";

Menu::Menu(sf::RenderWindow& window, std::string widgetsFile) : gui(window)
{
	gui.loadWidgetsFromFile(widgetsFile);
	SetConnections();
	LoadIcons();
}

void Menu::Show()
{
	gui.draw();
}

InputValues Menu::GetInputValues() const
{
	return inputValues;
}

void Menu::HandleEvent(sf::Event& event)
{
	gui.handleEvent(event);
}

std::string Menu::GetBotName()
{
	return botName;
}

bool Menu::CanStartGame()
{
	std::string errorMessage = "";
	if (gui.get<tgui::RadioButton>("TwoPlayersRB")->isChecked())
	{
		std::string firstName = gui.get<tgui::EditBox>("FirstPlayerName")->getText();
		std::string secondName = gui.get<tgui::EditBox>("SecondPlayerName")->getText();
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
			if (gui.get<tgui::CheckBox>("TimerCheckBox")->isChecked())
			{
				inputValues.time = sf::seconds(gui.get<tgui::Slider>("TimeSlider")->getValue() * 60);
				//static_cast<TwoPlayersGame*>(game)->SetPlayers(firstName, secondName, sf::seconds(timeLimit));
			}
			/*else
			{
				static_cast<TwoPlayersGame*>(game)->SetPlayers(firstName, secondName);
			}
			game->StartGame();*/
		}

	}
	else if (gui.get<tgui::RadioButton>("OnePlayerRB")->isChecked())
	{
		std::string name = gui.get<tgui::EditBox>("FirstPlayerName")->getText();
		if (name.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			errorMessage = "Enter the name!";
		else
		{
			inputValues.mode = GameMode::PlayerAndBot;
			if (gui.get<tgui::Tabs>("ColorsTab")->getSelected() == "White")
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
			if (gui.get<tgui::CheckBox>("TimerCheckBox")->isChecked())
			{
				inputValues.time = sf::seconds(gui.get<tgui::Slider>("TimeSlider")->getValue() * 60);
			}
		}
	}
	else
		errorMessage = "Select mode of the game!";

	gui.get<tgui::Label>("MessageLabel")->setText(errorMessage);
	return (errorMessage == "") ? true : false;
}

void Menu::SetConnections()
{
	tgui::Widget::Ptr timerCheckBox = gui.get("TimerCheckBox");
	timerCheckBox->connect(tgui::Signals::CheckBox::Checked, &Menu::ShowTimerSettings, this);
	timerCheckBox->connect(tgui::Signals::CheckBox::Unchecked, &Menu::HideTimerSettings, this);

	tgui::Widget::Ptr timeSlider = gui.get("TimeSlider");
	timeSlider->connect(tgui::Signals::Slider::ValueChanged, &Menu::ChangeChoosenTime, this);

	tgui::RadioButton::Ptr twoPlayersRB = gui.get<tgui::RadioButton>("TwoPlayersRB");
	twoPlayersRB->connect(tgui::Signals::RadioButton::Checked, &Menu::ShowTwoPlayersSettings, this);

	tgui::RadioButton::Ptr onePlayerRB = gui.get<tgui::RadioButton>("OnePlayerRB");
	onePlayerRB->connect(tgui::Signals::RadioButton::Checked, &Menu::ShowOnePlayerSettings, this);

	tgui::Tabs::Ptr colorsTab = gui.get<tgui::Tabs>("ColorsTab");
	colorsTab->connect(tgui::Signals::Tabs::TabSelected, &Menu::ShowChoosenColor, this);

	tgui::Button::Ptr startButton = gui.get<tgui::Button>("StartButton");
	startButton->connect(tgui::Signals::Button::Pressed, &Menu::CanStartGame, this);
}

void Menu::LoadIcons()
{
	tgui::Picture::Ptr firstPlayerIcon = gui.get<tgui::Picture>("FirstPlayerIcon");
	tgui::Texture firstIconTexture("images/Pawn_white.png");
	firstPlayerIcon->getRenderer()->setTexture(firstIconTexture);

	tgui::Picture::Ptr secondPlayerIcon = gui.get<tgui::Picture>("SecondPlayerIcon");
	tgui::Texture secondIconTexture("images/Pawn_black.png");
	secondPlayerIcon->getRenderer()->setTexture(secondIconTexture);
}

void Menu::ShowTimerSettings()
{
	gui.get("TimeLabel")->setVisible(true);
	gui.get("TimeSlider")->setVisible(true);
}

void Menu::HideTimerSettings()
{
	gui.get("TimeLabel")->setVisible(false);
	gui.get("TimeSlider")->setVisible(false);
}

void Menu::ShowOnePlayerSettings()
{
	gui.get("FirstPlayerIcon")->setVisible(true);
	gui.get("FirstPlayerName")->setVisible(true);
	gui.get("SecondPlayerIcon")->setVisible(false);
	gui.get("SecondPlayerName")->setVisible(false);
	gui.get("ColorsTab")->setVisible(true);
	ShowChoosenColor();
}

void Menu::ShowTwoPlayersSettings()
{
	tgui::Texture firstIconTexture("images/Pawn_white.png");
	gui.get<tgui::Picture>("FirstPlayerIcon")->getRenderer()->setTexture(firstIconTexture);

	gui.get("FirstPlayerIcon")->setVisible(true);
	gui.get("FirstPlayerName")->setVisible(true);
	gui.get("SecondPlayerIcon")->setVisible(true);
	gui.get("SecondPlayerName")->setVisible(true);
	gui.get("ColorsTab")->setVisible(false);
}

void Menu::ChangeChoosenTime()
{
	gui.get<tgui::Label>("TimeLabel")->setText("Minutes: " + std::to_string((int)gui.get<tgui::Slider>("TimeSlider")->getValue()));
}

void Menu::ShowChoosenColor()
{
	std::string texturePath = (gui.get<tgui::Tabs>("ColorsTab")->getSelected() == "White") ? "images/Pawn_white.png" : "images/Pawn_black.png";
	tgui::Texture firstIconTexture(texturePath);
	tgui::Picture::Ptr firstPlayerIcon = gui.get<tgui::Picture>("FirstPlayerIcon");
	firstPlayerIcon->getRenderer()->setTexture(firstIconTexture);
}
