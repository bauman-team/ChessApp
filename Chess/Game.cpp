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
	exitButton->setText("Exit");
	exitButton->setEnabled(true);
	exitButton->setSize(drawer.mapProperties.GetSideMenuWidth() * 0.5, 50);
	exitButton->setPosition((drawer.mapProperties.GetGameWindowWidth() - drawer.mapProperties.GetSideMenuWidth() / 2 - exitButton->getSize().x / 2),
		(drawer.mapProperties.GetGameWindowHeight() - exitButton->getSize().y - 5));
	exitButton->connect(tgui::Signals::Button::Clicked, &Game::SetExitStatus, this);
	gui.add(exitButton);
	exitButton->setWidgetName("ExitButton");
	
	tgui::ScrollablePanel::Ptr panel = tgui::ScrollablePanel::create();
	tgui::Label::Ptr t1 = tgui::Label::create(), t2 = tgui::Label::create(), t3 = tgui::Label::create();
	panel->setPosition((drawer.mapProperties.GetGameWindowWidth() - drawer.mapProperties.GetSideMenuWidth() / 2 - exitButton->getSize().x / 2), 0);
	panel->setSize(200, 600);
	gui.add(panel);
	panel->tgui::Widget::setWidgetName("ScrollablePanel");
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

void Game::UpdateSideMenu()
{
	tgui::ScrollablePanel::Ptr panel = gui.get<tgui::ScrollablePanel>("ScrollablePanel");
	if (map.GetMovesCount() > 0 && !panel->get(std::to_string(map.GetMovesCount())))
	{
		tgui::Label::Ptr label = tgui::Label::create();
		if (map.GetMovesCount() > 1 && !panel->get(std::to_string(map.GetMovesCount() - 1)))
		{
			label->setText(std::to_string(map.GetMovesCount()) + ") " + map.GetMovesHistory().at(map.GetMovesCount() - 2).GetPosBeforeMove().ToString() + " --> " + map.GetMovesHistory().at(map.GetMovesCount() - 2).GetPosAfterMove().ToString());
			label->setPosition(0, gui.get<tgui::ScrollablePanel>("ScrollablePanel")->get(std::to_string(map.GetMovesCount() - 2))->getPosition().y + 20);
		}
		else
		{
			label->setText(map.GetLastMoveInfo()->GetPosBeforeMove().ToString() + " --> " + map.GetLastMoveInfo()->GetPosAfterMove().ToString());
			if (map.GetMovesCount() != 1)
				label->setPosition(0, gui.get<tgui::ScrollablePanel>("ScrollablePanel")->get(std::to_string(map.GetMovesCount() - 1))->getPosition().y + 20);
		}
		gui.get<tgui::ScrollablePanel>("ScrollablePanel")->add(label);
		label->setWidgetName(std::to_string(map.GetMovesCount()));
	}
}
