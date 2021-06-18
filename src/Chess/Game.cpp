#include "Game.h"
#include <fstream>
#include <iostream>

void Game::HandleEvent(sf::Event& event)
{
	drawer.HandleEvent(event);
}

void Game::Save() const
{
	auto movesHistory{ map.GetMovesLog() };
	std::ofstream out{ "LastGame.log" };
	auto posBefore{ Pos::NULL_POS };
	auto posAfter{ Pos::NULL_POS };
	out << "   \n";
	auto numberOfMoves{ movesHistory.size() };
	for (auto i = 0; i < numberOfMoves; ++i)
	{
		posBefore = movesHistory[i].GetPosBeforeMove();
		posAfter = movesHistory[i].GetPosAfterMove();
		if ((i == 0) || (Figure::GetFigureTypeColor(movesHistory[i].GetTypeActiveFigure()) != Figure::GetFigureTypeColor(movesHistory[i - 1].GetTypeActiveFigure())))
			out << posBefore.ToString() << " -> "
			<< posAfter.ToString() << "\n";
		else
			--numberOfMoves;
	}
	out.seekp(0, std::ios::beg);
	out << numberOfMoves;
	out.close();
}

void Game::SetExitStatus()
{
	Save();
	status = GameStatus::Exit;
}

void Game::ActivateMenuSettings(Menu& menu)
{
	drawer.ResizeWindowForMenu(menu.GetMenuSize());
	menu.ActivateStartMenu();
}