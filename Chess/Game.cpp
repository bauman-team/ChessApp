#include "Game.h"
#include <fstream>

Game::Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: drawer(window, resource, properties)
{
	Figure::SetMapPtr(&map);
}

void Game::StartGame()
{
	status = GameStatus::Play;
	drawer.ResizeWindowForGame();
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