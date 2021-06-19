#include "pch.h"
#include "Map.h"

TEST(MoveTest, PawnTest) {
	std::array<uint64_t, FIGURE_TYPES> map;
	map[toUType(FigureType::Rook_black)] = 0;
	map[toUType(FigureType::Knight_black)] = 0;
	map[toUType(FigureType::Bishop_black)] = 2'305'843'009'213'693'952;
	map[toUType(FigureType::Queen_black)] = 0;
	map[toUType(FigureType::King_black)] = 1'152'921'504'606'846'976;
	map[toUType(FigureType::Pawn_black)] = 2'251'799'817'879'552;
	map[toUType(FigureType::Rook_white)] = 0;
	map[toUType(FigureType::Knight_white)] = 0;
	map[toUType(FigureType::Bishop_white)] = 0;
	map[toUType(FigureType::Queen_white)] = 0;
	map[toUType(FigureType::King_white)] = 16;
	map[toUType(FigureType::Pawn_white)] = 18'014'467'228'991'488;
	Map testingMap{ map };
	testingMap.FindAllPossibleMoves(Color::Black);
	auto moves{ testingMap.GetAllPossibleMoves() };
	auto existMoves{ 0 };
	for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		if ((*it)[0] == MoveInfo{ Pos{ 3, 6 }, Pos{ 3, 5 }, FigureType::Pawn_black, FigureType::Empty, testingMap.GetPossibleCastling(), false, 1 })
			existMoves++;
		else if ((*it)[0] == MoveInfo{ Pos{ 3, 6 }, Pos{ 3, 4 }, FigureType::Pawn_black, FigureType::Empty, testingMap.GetPossibleCastling(), false, 1 })
		{
			testingMap.Move(*it);
			existMoves++;
		}
		else if ((*it)[0].GetPosBeforeMove() == Pos{ 3, 6 })
			existMoves--;

	EXPECT_TRUE(existMoves == 2);

	
	testingMap.ClearPossibleMoves();
	testingMap.FindAllPossibleMoves(Color::White);
	moves = testingMap.GetAllPossibleMoves();
	existMoves = 0;
	for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		if ((*it)[0] == MoveInfo{ Pos{ 4, 4 }, Pos{ 3, 5 }, FigureType::Pawn_white, FigureType::Empty, testingMap.GetPossibleCastling(), true, 2 })
		{
			testingMap.Move(*it);
			existMoves++;
		}
		else if ((*it)[0] == MoveInfo{ Pos{ 4, 4 }, Pos{ 4, 5 }, FigureType::Pawn_white, FigureType::Empty, testingMap.GetPossibleCastling(), false, 2 })
			existMoves++;
		else if ((*it)[0].GetPosBeforeMove() == Pos{ 4, 4 })
			existMoves--;

	EXPECT_TRUE(existMoves == 2);

	testingMap.ClearPossibleMoves();
	testingMap.FindAllPossibleMoves(Color::Black);
	moves = testingMap.GetAllPossibleMoves();
	existMoves = 0;
	for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		if ((*it)[0] == MoveInfo{ Pos{ 6, 2 }, Pos{ 7, 1 }, FigureType::Pawn_black, FigureType::Pawn_white, testingMap.GetPossibleCastling(), false, 3 })
		{
			testingMap.Move(*it);
			existMoves++;
		}
		else if ((*it)[0] == MoveInfo{ Pos{ 6, 2 }, Pos{ 6, 1 }, FigureType::Pawn_black, FigureType::Empty, testingMap.GetPossibleCastling(), false, 3 })
			existMoves++;
		else if ((*it)[0].GetPosBeforeMove() == Pos{ 6, 2 })
			existMoves--;

	EXPECT_TRUE(existMoves == 2);

	testingMap.ClearPossibleMoves();
	testingMap.FindAllPossibleMoves(Color::White);
	moves = testingMap.GetAllPossibleMoves();
	existMoves = 0;
	for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		if ((*it)[0] == MoveInfo{ Pos{ 6, 6 }, Pos{ 5, 7 }, FigureType::Pawn_white, FigureType::Bishop_black, testingMap.GetPossibleCastling(), false, 4 })
		{
			if ((*it)[1].GetTypeActiveFigure() == FigureType::Knight_white)
				testingMap.Move(*it);
			existMoves++;
		}
		else if ((*it)[0] == MoveInfo{ Pos{ 6, 6 }, Pos{ 6, 7 }, FigureType::Pawn_white, FigureType::Empty, testingMap.GetPossibleCastling(), false, 4 })
			existMoves++;
		else if ((*it)[0].GetPosBeforeMove() == Pos{ 6, 6 })
			existMoves--;

	EXPECT_TRUE(existMoves == 8);

	testingMap.ClearPossibleMoves();
	testingMap.FindAllPossibleMoves(Color::Black);
	moves = testingMap.GetAllPossibleMoves();
	existMoves = 0;
	for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		if ((*it)[0] == MoveInfo{ Pos{ 7, 1 }, Pos{ 7, 0 }, FigureType::Pawn_black, FigureType::Empty, testingMap.GetPossibleCastling(), false, 5 })
		{
			if ((*it)[1].GetTypeActiveFigure() == FigureType::Rook_black)
				testingMap.Move(*it);
			existMoves++;
		}
		else if ((*it)[0].GetPosBeforeMove() == Pos{ 7, 1 })
			existMoves--;

	EXPECT_TRUE(existMoves == 4);

	testingMap.ClearPossibleMoves();
	testingMap.FindAllPossibleMoves(Color::White);
	moves = testingMap.GetAllPossibleMoves();
	existMoves = 0;
	for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		if ((*it)[0].GetPosBeforeMove() == Pos{ 3, 5 })
			existMoves--;

	EXPECT_TRUE(existMoves == 0);
}