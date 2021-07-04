#include "PlayerWithAIGame.h"
#include <math.h> // for supporting linux

//#define UseAvx

#ifdef UseAvx
#include <xmmintrin.h>
#include <intrin.h> 
#endif

#define DebugInfo
//#define SmartBotTest
//#define TestWeightAndBitboards
const float PlayerWithAIGame::figureWeight[FIGURE_TYPES]{ 900, 90, 30, 30, 50, 10, 900, 90, 30, 30, 50, 10 }; // black
int PlayerWithAIGame::winWhite{ 0 }; // TODO: delete test
int PlayerWithAIGame::winBlack{ 0 };

bool PlayerWithAIGame::isPlayerMoveFirst{ false };
const float PlayerWithAIGame::bitboards[FIGURE_TYPES][8][8]{
		{ { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 }, //King_black
		  { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		  { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		  { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		  { -2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0 },
		  { -1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0 },
		  {  2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0 },
		  {  2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0 } },
		    											   
		    											   
		{ { -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 }, //Queen_black
		  { -1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
		  { -1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0 },
		  { -0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5 },
		  { -0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0,  0.0 },
		  { -1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.5, -1.0 },
		  { -1.0,  0.0,  0.0,  0.0,  0.0,  0.5,  0.0, -1.0 },
		  { -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 } },
		    											   
		    											   
		{ { -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 }, //Bishop_black
		  { -1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
		  { -1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0 },
		  { -1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0 },
		  { -1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0 },
		  { -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0 },
		  { -1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0 },
		  { -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 } },
		    											   
		    											   
		{ { -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 }, //Knight_black
		  { -4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0 },
		  { -3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0 },
		  { -3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0 },
		  { -3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0 },
		  { -3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0 },
		  { -4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0 },
		  { -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 } },
		    											   
		    											   
		{ {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 }, //Rook_black
		  {  0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  {  0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0 } },
		    											   
		    											   
		{ {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 }, //Pawn_black
		  {  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0 },
		  {  1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0 },
		  {  0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5 },
		  {  0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0 },
		  {  0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5 },
		  {  0.5,  1.0,  1.0, -2.0, -2.0,  1.0,  1.0,  0.5 },
		  {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 } },
		    											   
		    											   
		{ {  2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0 }, //King_white
		  {  2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0 },
		  { -1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0 },
		  { -2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0 },
		  { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		  { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		  { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
		  { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 } },
		    											   
		    											   
		{ { -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 }, //Queen_white
		  { -1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0 },
		  { -1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0 },
		  {  0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5 },
		  { -0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5 },
		  { -1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0 },
		  { -1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
		  { -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 } },
		    											   
		    											   
		{ { -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 }, //Bishop_white
		  { -1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0 },
		  { -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0 },
		  { -1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0 },
		  { -1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0 },
		  { -1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0 },
		  { -1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
		  { -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 } },
		    											   
		    											   
		{ { -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 }, //Knight_white
		  { -4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0 },
		  { -3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0 },
		  { -3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0 },
		  { -3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0 },
		  { -3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0 },
		  { -4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0 },
		  { -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 } },
		    											   
		    											   
		{ {  0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0 }, //Rook_white
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
		  {  0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5 },
		  {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 } },
		    											   
		    											   
		{ {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 }, //Pawn_white
		  {  0.5,  1.0,  1.0, -2.0, -2.0,  1.0,  1.0,  0.5 },
		  {  0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5 },
		  {  0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0 },
		  {  0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5 },
		  {  1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0 },
		  {  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0 },
		  {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 } }
};
const float PlayerWithAIGame::infScore{ 9999 };
const int PlayerWithAIGame::errorRate{ 20 };
const int PlayerWithAIGame::DEPTH{ 2 }; // searching depth of move
const int PlayerWithAIGame::countOfThreads{ 8 };

std::atomic<int> positionsCount{ 0 }; // TODO: delete debug counter

std::vector<MoveInfo> PlayerWithAIGame::StartAI(double timeForWaiting)
{
	std::mutex mutPushScore;
	std::vector<std::pair<int, float>> movesScore{ };
	std::atomic<uint16_t> countWorkingThreads{ 0 };
	auto moves{ map.GetAllPossibleMoves() };
	auto countOfMovesInThread{ 0 }, from{ 0 }, countOfMoves{ static_cast<int>(moves.size()) };
	for (auto i = countOfThreads; i >= 0 && countOfMoves > 0; --i)
	{
		++countWorkingThreads; // add to active process count
		from = moves.size() - countOfMoves;
		countOfMovesInThread = ceil(countOfMoves / i); // count checking of moves in this thread
		countOfMoves -= countOfMovesInThread; // count checking of moves is left
		std::thread th{ [map = map, &mutPushScore, from, countOfMovesInThread, moves, &movesScore, &countWorkingThreads]() mutable {
			for (auto j = from; j != from + countOfMovesInThread; ++j)
			{
				map.Move(moves[j]);
				auto score{ std::pair<int, float>(j, PlayerWithAIGame::MiniMax(map, false, DEPTH, -10'000, 10'000)) };
				map.UndoMove();
				mutPushScore.lock();
				movesScore.push_back(score);
				mutPushScore.unlock();
			}
			--countWorkingThreads;
			} };
		th.detach();
	}

	sf::sleep(sf::seconds(timeForWaiting));

	if (timeForWaiting == 0) 
		while (countWorkingThreads != 0)
			sf::sleep(sf::seconds(0.03));

	std::sort(movesScore.begin(), movesScore.end(), [](const std::pair<int, float>& left, const std::pair<int, float>& right)
		{
			return left.second > right.second;
		});
	// Additional error rate
	if (rand() % 100 < errorRate && (*movesScore.begin()).second != infScore && movesScore.size() > 1)
	{
		for (auto i = 0; i != std::min(3, static_cast<int>(movesScore.size())); ++i)
			std::cout << "\n\t" + moves[(*(movesScore.begin() + i)).first][0].GetPosBeforeMove().ToString() + "  -->  " + moves[(*(movesScore.begin() + i)).first][0].GetPosAfterMove().ToString();
		return moves[(*(movesScore.begin() + (rand() % std::min(3, static_cast<int>(moves.size() - 1))) + 1)).first];
	}
	return moves[(*movesScore.begin()).first];
}

float PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color playerColor)
{
	auto score{ 0.0f }, scoreTest{ 0.0f };
	auto selected{ FigureType::Empty };
#ifdef UseAvx
		float x[64]{ 0.0 }, y[64]{ 0.0 }, z[64]{ 0.0 };
		for (auto i = 0; i != 64; ++i)
		{
			selected = selectedMap.GetFigureType(i);
			if (selected != FigureType::Empty)
			{
				x[i] = selectedMap.GetColor(selected) == playerColor ? 1.0 : -1.0;
				y[i] = figureWeight[toUType(selected)];
				z[i] = bitboards[toUType(selected)][i / 8][i % 8];
			}
		}
		for (auto i = 0; i != 64; i += 8)
		{
			auto xArr = _mm256_loadu_ps(x + i),
				yArr = _mm256_loadu_ps(y + i),
				zArr = _mm256_loadu_ps(z + i);
			yArr = _mm256_add_ps(yArr, zArr);
			xArr = _mm256_mul_ps(xArr, yArr);
			_mm256_store_ps(z, xArr);
			score += z[0] + z[1] + z[2] + z[3] + z[4] + z[5] + z[6] + z[7];
		}
#endif
#ifndef UseAvx
		// black
		for (auto i = 0; i != 64; ++i)
		{
			selected = selectedMap.GetFigureType(i);
			if (selected != FigureType::Empty)
				score += (selectedMap.GetColor(selected) == playerColor ? 1.0 : -1.0)
				* (figureWeight[toUType(selected)] + bitboards[toUType(selected)][i / 8][i % 8]);
		}
#endif
	return score;
}

float PlayerWithAIGame::MiniMax(Map map, bool isAIMoveNow, int depth, float alpha, float beta)
{
	++positionsCount;
	if (depth == 0)
	{
		return -CalculatePositionScore(map, isPlayerMoveFirst ? Color::White : Color::Black);
	}
	if (isAIMoveNow)
	{
		auto AIColor{ isPlayerMoveFirst ? Color::Black : Color::White };
		auto bestMove{ -infScore };
		auto start{ AIColor == Color::Black ? 0 : 6 }; // used fixed enum order
		if (Figure::IsShahFor(AIColor, map.GetMap()))
			map.DisableCastlingForKing(AIColor);
		map.DisableCastlingForRook(AIColor);
		for (auto i = start; i != start + 6; ++i)
		{
			uint64_t j{ 1 };
			while (j) // check all positions
			{
				if (j & map.GetMap()[i]) // is selected figure position
				{
					auto oneFigureMoves = Figure::FindPossibleMoves(Pos::BitboardToPosition(j), static_cast<FigureType>(i), map); // find figure possible moves without checking shah
					for (auto moveItr = oneFigureMoves.begin(); moveItr != oneFigureMoves.end(); ++moveItr)
					{
						map.Move(*moveItr);
						if (!Figure::IsShahFor(AIColor, map.GetMap()))
							bestMove = std::max(bestMove, MiniMax(map, !isAIMoveNow, depth, alpha, beta));
						map.UndoMove();
						alpha = std::max(bestMove, alpha);
						if (beta <= alpha)
							return bestMove;
					}
				}
				j <<= 1;
			}
		}
		return bestMove;
	}
	else
	{
		auto playerColor{ isPlayerMoveFirst ? Color::White : Color::Black };
		auto bestMove{ infScore };
		auto start{ playerColor == Color::Black ? 0 : 6 }; // used fixed enum order
		if (Figure::IsShahFor(playerColor, map.GetMap()))
			map.DisableCastlingForKing(playerColor);
		map.DisableCastlingForRook(playerColor);
		for (auto i = start; i != start + 6; ++i)
		{
			uint64_t j{ 1 };
			while (j) // check all positions
			{
				if (j & map.GetMap()[i]) // is selected figure position
				{
					auto oneFigureMoves = Figure::FindPossibleMoves(Pos::BitboardToPosition(j), static_cast<FigureType>(i), map); // find figure possible moves without checking shah
					for (auto moveItr = oneFigureMoves.begin(); moveItr != oneFigureMoves.end(); ++moveItr)
					{
						map.Move(*moveItr);
						if (!Figure::IsShahFor(playerColor, map.GetMap()))
							bestMove = std::min(bestMove, MiniMax(map, !isAIMoveNow, depth - 1, alpha, beta));
						map.UndoMove();
						beta = std::min(bestMove, beta);
						if (beta <= alpha)
							return bestMove;
					}
				}
				j <<= 1;
			}
		}
		return bestMove;
	}
}

void PlayerWithAIGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	if (timeLimit != sf::seconds(0))
		isTimeLimited = true;
	isPlayerMoveFirst = (name2 == Menu::GetBotName());
	player1 = new Player{ Color::White, name1, timeLimit, !isPlayerMoveFirst };
	player2 = new Player{ Color::Black, name2, timeLimit, isPlayerMoveFirst };
	activePlayer = isPlayerMoveFirst ? player1 : player2;
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	auto stopTime{ isTimeLimited };
	isTimeLimited = false;
	map.mut.lock();
	drawer.ClearSelect();
	map.mut.unlock();
	map.ClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
#ifndef SmartBotTest
	std::lock_guard<std::mutex> g(mut);
#endif
	status = map.CheckGameFinal(activePlayer->GetColor());
	if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
#ifdef DebugInfo
		std::cout << "\n\tStart!";
		positionsCount = 0;
		sf::Clock clock;
#endif
		auto bestMove{ StartAI() };
#ifdef DebugInfo
		auto time{ clock.getElapsedTime() };
		std::cout << "\n\tTime of calculating (in milliseconds): " << time.asMilliseconds()
			<< "\n\tCount of calculated positions: " << positionsCount << "\n\tEnd!";
#endif
		map.Move(bestMove);
		map.ClearPossibleMoves();
		activePlayer = (activePlayer == player2) ? player1 : player2;
		status = map.CheckGameFinal(activePlayer->GetColor());
		if (stopTime && status != GameStatus::Pat && status != GameStatus::Mat)
		{
			activePlayer->StartTimer();
			isTimeLimited = stopTime;
		}
	}
#ifdef SmartBotTest
	if (status != GameStatus::Pat && status != GameStatus::Mat && map.GetMovesCount() < 300)
	{
		activePlayer = (activePlayer == player2) ? player1 : player2;
		isPlayerMoveFirst = !isPlayerMoveFirst;
		ChangeActivePlayer();
	}
	else
	{
		if (map.GetMovesCount() < 300)
		{
			if (activePlayer->GetColor() == Color::White)
				++winBlack;
			else
				++winWhite;
		}
		std::cout << "\nFinish!\nCounts of moves: " << map.GetMovesCount() << "\nStats: winWhite = " << winWhite
			<< ", winBlack = " << winBlack << "\nStart new!";
		if ((winWhite + winBlack) % 10 == 0)
			system("pause");
		map.mut.lock();
		map.ClearPossibleMoves();
		map = Map();
		map.mut.unlock();
		ChangeActivePlayer();
	}
#endif
}

void PlayerWithAIGame::StartGame()
{
	if (!isPlayerMoveFirst)
	{
		drawer.RotateBoard();
		ChangeActivePlayer(); // if the first move of the bot
	}
	else
	{
		map.FindAllPossibleMoves(activePlayer->GetColor());
		if (isTimeLimited)
			activePlayer->StartTimer();
	}
}