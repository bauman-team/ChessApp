#include "PlayerWithAIGame.h"
#include <math.h> // for supporting linux

//#define UseAvx

#ifdef UseAvx
#include <xmmintrin.h>
#include <intrin.h> 
#endif

//#define RandBotTest
//#define SmartBotTest
//#define TestWeightAndBitboards
CHESSENGINE_API extern std::mutex mut1;
std::mutex mut2;
// TODO: delete test
const float PlayerWithAIGame::figureWeight[FIGURE_TYPES]{ 900, 90, 30, 30, 50, 10, 900, 90, 30, 30, 50, 10 }; // black
const float PlayerWithAIGame::figureWeightTest[FIGURE_TYPES]{ 900, 90, 30, 30, 50, 10, 900, 90, 30, 30, 50, 10 }; // white
int PlayerWithAIGame::winWhite{ 0 };
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

PlayerWithAIGame::Move PlayerWithAIGame::StartAI(double timeForWaiting)
{
#ifdef RandBotTest
	if (activePlayer->GetColor() == Color::Black && isPlayerMoveFirst
		|| activePlayer->GetColor() == Color::White && !isPlayerMoveFirst)
	{
#endif
		std::vector<std::pair<int, float>> movesScore{ };
		std::vector<Move> movesPositions{ }; // fill vector all possible moves
		for (auto it1 = map.GetAllPossibleMoves().begin(); it1 != map.GetAllPossibleMoves().end(); ++it1)
			for (auto it2 = (*it1).to.begin(); it2 != (*it1).to.end(); ++it2)
				movesPositions.push_back(Move{ (*it1).from, *it2 });
		std::atomic<uint16_t> countWorkingThreads{ 0 };
		auto countOfMovesInThread{ 0 }, from{ 0 }, countOfMoves{ static_cast<int>(movesPositions.size()) };
		for (auto i = countOfThreads; i >= 0 && countOfMoves > 0; --i)
		{
			++countWorkingThreads; // add to active process count
			from = movesPositions.size() - countOfMoves; 
			countOfMovesInThread = ceil(countOfMoves / i); // count checking of moves in this thread
			countOfMoves -= countOfMovesInThread; // count checking of moves is left
			auto MAP{ map };
			std::thread th{ [from, countOfMovesInThread, movesPositions, &movesScore, MAP, &countWorkingThreads]() {
				for (auto j = from; j != from + countOfMovesInThread; ++j)
				{
					auto copyMap{ MAP };
					copyMap.Move(movesPositions[j].from, movesPositions[j].to);
					auto score{ std::pair<int, float>(j, PlayerWithAIGame::MiniMax(copyMap, false, DEPTH, -10'000, 10'000)) }; // isPlayerMoveFirst ? DEPTH + 1 : 
					mut2.lock();
					movesScore.push_back(score);
					mut2.unlock();
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
				std::cout << "\n\t" + movesPositions[(*(movesScore.begin() + i)).first].from.ToString() + "  -->  " + movesPositions[(*(movesScore.begin() + i)).first].to.ToString();
			return movesPositions[(*(movesScore.begin() + (rand() % std::min(3, static_cast<int>(movesPositions.size() - 1))) + 1)).first];
		}
		return movesPositions[(*movesScore.begin()).first];
#ifdef RandBotTest
	}
	else
	{
		//srand(std::time(NULL));
		auto rand1 = rand() % map.GetAllPossibleMoves().size();
		//sf::sleep(sf::seconds(0.5));
		return Move(map.GetAllPossibleMoves().at(rand1).from, map.GetAllPossibleMoves().at(rand1).to.at(rand() % map.GetAllPossibleMoves().at(rand1).to.size()));
	}
#endif
}

float PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color playerColor)
{
	auto score{ 0.0f }, scoreTest{ 0.0f };
	auto selected{ FigureType::Empty };
#ifdef TestWeightAndBitboards
	if (isPlayerMoveFirst) // for testing
	{
#endif
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
#ifdef TestWeightAndBitboards
	}
	else
	{
		// white
		for (auto i = 0; i != 64; ++i)
		{
			selected = selectedMap.GetFigureType(i);
			if (selected != FigureType::Empty)
				score += (selectedMap.GetColor(selected) == playerColor ? 1.0 : -1.0)
				* (figureWeightTest[toUType(selected)] + bitboards[toUType(selected)][i / 8][i % 8]);
		}
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
		if (map.IsShahFor(AIColor))
			map.DisableCastlingForKing(AIColor); // if King is attacked => castling disabled
		for (auto i = start; i != start + 6; ++i)
		{
			uint64_t j{ 1 };
			while (j) // check all positions
			{
				if (j & map.map[i]) // is selected figure position
				{
					OneFigureMoves moves;
					moves.from = Pos::BitboardToPosition(j);
					moves.to = Figure::FindPossibleMoves(moves.from, static_cast<FigureType>(i), map); // find figure possible moves without checking shah 
					for (auto posItr = moves.to.begin(); posItr != moves.to.end(); ++posItr)
					{
						map.Move(moves.from, *posItr);
						if (!map.IsShahFor(AIColor))
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
		if (map.IsShahFor(playerColor))
			map.DisableCastlingForKing(playerColor); // if King is attacked => castling disabled
		for (auto i = start; i != start + 6; ++i)
		{
			uint64_t j{ 1 };
			while (j) // check all positions
			{
				if (j & map.map[i]) // is selected figure position
				{
					OneFigureMoves moves;
					moves.from = Pos::BitboardToPosition(j);
					moves.to = Figure::FindPossibleMoves(moves.from, static_cast<FigureType>(i), map); // find figure possible moves without checking shah 
					for (auto posItr = moves.to.begin(); posItr != moves.to.end(); ++posItr)
					{
						map.Move(moves.from, *posItr);
						if (!map.IsShahFor(playerColor))
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
	player1 = new Player{ Color::White, name1, timeLimit, name2 != Menu::GetBotName() };
	player2 = new Player{ Color::Black, name2, timeLimit, name2 == Menu::GetBotName() };
	activePlayer = isPlayerMoveFirst ? player1 : player2;
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	auto stopTime{ isTimeLimited };
	isTimeLimited = false;
	mut1.lock();
	activePlayer->SetChosenPosition(Pos::NULL_POS);
	mut1.unlock();
	map.ClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	status = map.CheckGameFinal(activePlayer->GetColor());
	if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
		std::cout << "\n\tStart!";
		positionsCount = 0;
		sf::Clock clock;
		auto bestMove{ StartAI() };
		auto time{ clock.getElapsedTime() };
		std::cout << "\n\tTime of calculating (in milliseconds): " << time.asMilliseconds()
			<< "\n\tCount of calculated positions: " << positionsCount << "\n\tEnd!";
		map.MakeMove(bestMove.from, bestMove.to);
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
	if (status != GameStatus::Pat && status != GameStatus::Mat && map.movesHistory.size() < 300)
	{
		activePlayer = (activePlayer == player2) ? player1 : player2;
		isPlayerMoveFirst = !isPlayerMoveFirst;
		ChangeActivePlayer();
	}
	else
	{
		if (map.movesHistory.size() < 300)
		{
			if (activePlayer->GetColor() == Color::White)
				++winBlack;
			else
				++winWhite;
		}
		std::cout << "\nFinish!\nCounts of moves: " << map.movesHistory.size() << "\nStats: winWhite = " << winWhite 
			<< ", winBlack = " << winBlack << "\nStart new!";
		if ((winWhite + winBlack) % 10 == 0)
			system("pause");
		mut1.lock();
		map.ClearPossibleMoves();
		map = Map();
		mut1.unlock();
		ChangeActivePlayer();
	}
#endif
#ifdef RandBotTest
	if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
		activePlayer = (activePlayer == player2) ? player1 : player2;
		ChangeActivePlayer();
	}
	else
	{
		std::cout << "\nFinish!\nCounts of moves: " << map.movesHistory.size() << "\nStart new!";
		system("pause");
		mut1.lock();
		map = Map();
		mut1.unlock();
		ChangeActivePlayer();
	}
#endif
}

void PlayerWithAIGame::StartGame()
{
	if (!isPlayerMoveFirst)
	{
		drawer.RotateBoard();
	}
	else
	{
		map.FindAllPossibleMoves(activePlayer->GetColor());
		if (isTimeLimited)
			activePlayer->StartTimer();
	}
}