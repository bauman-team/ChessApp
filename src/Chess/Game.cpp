#include "Game.h"
#include <fstream>
#include <iostream>

void Game::HandleEvent(sf::Event& event)
{
	drawer.HandleEvent(event);
}

void Game::Save() const
{
	std::vector<MoveInfo> movesHistory = map.GetMovesHistory();
	std::ofstream out("LastGame.log");
	Pos posBefore;
	Pos posAfter;
	out << "   \n";
	int numberOfMoves = movesHistory.size();
	for (int i = 0; i < numberOfMoves; ++i)
	{
		posBefore = movesHistory[i].GetPosBeforeMove();
		posAfter = movesHistory[i].GetPosAfterMove();
		if ((i == 0) || (Figure::GetFigureTypeColor(movesHistory[i].GetTypeActiveFigure()) != Figure::GetFigureTypeColor(movesHistory[i - 1].GetTypeActiveFigure())))
			out << static_cast<int>(posBefore.GetX()) << " "
			<< static_cast<int>(posBefore.GetY()) << " -> "
			<< static_cast<int>(posAfter.GetX()) << " " 
			<< static_cast<int>(posAfter.GetY()) << "\n";
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