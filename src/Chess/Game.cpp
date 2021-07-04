#include "Game.h"
#include <fstream>
#include <iostream>

std::string EnumFigureTypeToString(const FigureType type)
{
	switch (type)
	{
	case FigureType::Bishop_black:
		return "Bishop_black";
	case FigureType::Bishop_white:
		return "Bishop_white";
	case FigureType::King_black:
		return "King_black";
	case FigureType::King_white:
		return "King_white";
	case FigureType::Knight_black:
		return "Knight_black";
	case FigureType::Knight_white:
		return "Knight_white";
	case FigureType::Pawn_black:
		return "Pawn_black";
	case FigureType::Pawn_white:
		return "Pawn_white";
	case FigureType::Queen_black:
		return "Queen_black";
	case FigureType::Queen_white:
		return "Queen_white";
	case FigureType::Rook_black:
		return "Rook_black";
	case FigureType::Rook_white:
		return "Rook_white";
	default:
		return "Empty";
	}
}

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
		if ((i == 0) || (movesHistory[i].GetNumOfMove() != movesHistory[i - 1].GetNumOfMove()))
			out << posBefore.ToString() << " -> "
			<< posAfter.ToString() << "\n";
			/*out << "MoveInfo(Pos(" << std::to_string(posBefore.GetX()) << ", " << std::to_string(posBefore.GetY()) << "), Pos(" << std::to_string(posAfter.GetX()) << ", " << std::to_string(posAfter.GetY()) << "), FigureType::"
			<< EnumFigureTypeToString(movesHistory[i].GetTypeActiveFigure()) << ", FigureType::" << EnumFigureTypeToString(movesHistory[i].GetTypeEatenFigure()) << ", std::array<bool, 4>{ "
			<< movesHistory[i].GetPossibleCastling()[0] << ", " << movesHistory[i].GetPossibleCastling()[1] << ", " << movesHistory[i].GetPossibleCastling()[2] << ", " << movesHistory[i].GetPossibleCastling()[3]
			<< " }, " << movesHistory[i].GetCaptureEnPassant() << ", " << movesHistory[i].GetNumOfMove() << ");\n";*/
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