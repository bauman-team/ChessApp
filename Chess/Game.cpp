#include "Game.h"
#include <fstream>

void Game::StartGame()
{
	status = GameStatus::Play;
}

Game::Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: drawer(window, resource, properties), gui(*window)
{
	tgui::Button::Ptr exitButton = tgui::Button::create();
	exitButton->setWidgetName("ExitButton");
	exitButton->setText("Exit");
	exitButton->setEnabled(true);
	exitButton->setSize(drawer.mapProperties.GetSideMenuWidth() * 0.5, 50);
	exitButton->setPosition((drawer.mapProperties.GetGameWindowWidth() - drawer.mapProperties.GetSideMenuWidth() / 2 - exitButton->getSize().x / 2),
		(drawer.mapProperties.GetGameWindowHeight() - exitButton->getSize().y - 5));
	exitButton->connect(tgui::Signals::Button::Clicked, &Game::SetExitStatus, this);
	gui.add(exitButton);
}

void Game::Save()
{
	std::vector<MoveInfo> movesHistory = map.GetMovesHistory();
	std::ofstream out("LastGame.txt");
	Pos posBefore;
	Pos posAfter;
	out << "   \n";
	int numberOfMoves = movesHistory.size();
	for (int i = 0; i < numberOfMoves; ++i)
	{
		posBefore = movesHistory[i].GetPosBeforeMove();
		posAfter = movesHistory[i].GetPosAfterMove();
		if ((i == 0) || (Figure::GetFigureTypeColor(movesHistory[i].GetTypeActiveFigure()) != Figure::GetFigureTypeColor(movesHistory[i - 1].GetTypeActiveFigure())))
			out << (int)posBefore.GetX() << " " << (int)posBefore.GetY() << " -> " << (int)posAfter.GetX() << " " << (int)posAfter.GetY() << "\n";
		else
			--numberOfMoves;
	}
	out.seekp(0, std::ios::beg);
	out << numberOfMoves;
	out.close();
}

void Game::HandleEvent(sf::Event& event)
{
	gui.handleEvent(event);
}

void Game::ReturnGameToInitialSettings(Menu& menu)
{
	drawer.ResizeWindowForGame(menu.GetMenuSize());
	menu.ActivateStartMenu();
}
