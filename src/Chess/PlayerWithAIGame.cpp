#include "PlayerWithAIGame.h"
#include <math.h> // for supporting linux
//#define RandBotTest
//#define SmartBotTest
//#define TestWeightAndBitboards
CHESSENGINE_API extern std::mutex mut1;
extern std::mutex mut3;
std::mutex mut2;
// TODO: delete test
const float PlayerWithAIGame::figureWeight[FIGURE_TYPES] = { 900, 90, 30, 30, 50, 10, 900, 90, 30, 30, 50, 10 }; // black
const float PlayerWithAIGame::figureWeightTest[FIGURE_TYPES] = { 900, 90, 30, 30, 60, 10, 900, 90, 30, 30, 60, 10 }; // white
int PlayerWithAIGame::winWhite = 0;
int PlayerWithAIGame::winBlack = 0;

bool PlayerWithAIGame::isPlayerMoveFirst{ false };
// TODO: take test bitboards
const float PlayerWithAIGame::bitboards[FIGURE_TYPES][8][8] = {
		{{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0}, //King_black
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
		 {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
		 { 2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
		 { 2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0}},


		{{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}, //Queen_black
		 {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
		 {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
		 {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
		 {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0,  0.0},
		 {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.5, -1.0},
		 {-1.0,  0.0,  0.0,  0.0,  0.0,  0.5,  0.0, -1.0},
		 {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}},


		{{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}, //Bishop_black
		 {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
		 {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
		 {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
		 {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
		 {-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
		 {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
		 {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}},


		{{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}, //Knight_black
		 {-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
		 {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
		 {-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
		 {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0},
		 {-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
		 {-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
		 {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}},


		{{ 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}, //Rook_black
		 { 0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 { 0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0}},


		{{ 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}, //Pawn_black
		 { 5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
		 { 1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
		 { 0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
		 { 0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
		 { 0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
		 { 0.5,  1.0,  1.0, -2.0, -2.0,  1.0,  1.0,  0.5},
		 { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}},


		{{ 2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0}, //King_white
		 { 2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
		 {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
		 {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0}},


		{{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}, //Queen_white
		 {-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0},
		 {-1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
		 { 0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
		 {-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
		 {-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
		 {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
		 {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}},


		{{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}, //Bishop_white
		 {-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
		 {-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
		 {-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
		 {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
		 {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
		 {-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
		 {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}},


		{{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}, //Knight_white
		 {-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
		 {-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
		 {-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0},
		 {-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
		 {-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
		 {-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
		 {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}},


		{{ 0.0,  0.0,  0.0,  0.5,  0.5,  0.0,  0.0,  0.0}, //Rook_white
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 {-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
		 { 0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5},
		 { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}},


		{{ 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}, //Pawn_white
		 { 0.5,  1.0,  1.0, -2.0, -2.0,  1.0,  1.0,  0.5},
		 { 0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5},
		 { 0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0},
		 { 0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5},
		 { 1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0},
		 { 5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0},
		 { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0}}
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
		std::vector<std::pair<int, float>> movesScore;
		std::vector<Move> movesPositions; // fill vector all possible moves
		for (auto it1 = map.GetAllPossibleMoves().begin(); it1 != map.GetAllPossibleMoves().end(); ++it1)
			for (auto it2 = (*it1).to.begin(); it2 != (*it1).to.end(); ++it2)
				movesPositions.push_back(Move((*it1).from, *it2));
		std::atomic<uint16_t> countWorkingThreads{ 0 };
		int countOfMovesInThread, from, countOfMoves = movesPositions.size();
		for (int i = countOfThreads; i >= 0 && countOfMoves > 0; --i)
		{
			++countWorkingThreads; // add to active process count
			from = movesPositions.size() - countOfMoves; 
			countOfMovesInThread = ceil(countOfMoves / i); // count checking of moves in this thread
			countOfMoves -= countOfMovesInThread; // count checking of moves is left
			Map MAP(map);
			std::thread th([from, countOfMovesInThread, movesPositions, &movesScore, MAP, &countWorkingThreads]() {
				for (int j = from; j != from + countOfMovesInThread; ++j)
				{
					Map copyMap(MAP);
					copyMap.Move(movesPositions[j].from, movesPositions[j].to);
					std::pair<int, float> score = std::pair<int, float>(j, PlayerWithAIGame::MiniMax(copyMap, false, DEPTH, -10'000, 10'000));
					mut2.lock();
					movesScore.push_back(score);
					mut2.unlock();
				}
				--countWorkingThreads;
				});
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
			for (int i = 0; i != std::min(3, static_cast<int>(movesScore.size())); ++i)
				std::cout << "\n\t" + movesPositions[(*(movesScore.begin() + i)).first].from.ToString() + "  -->  " + movesPositions[(*(movesScore.begin() + i)).first].to.ToString();
			return movesPositions[(*(movesScore.begin() + (rand() % std::min(3, static_cast<int>(movesPositions.size() - 1))) + 1)).first];
		}
		return movesPositions[(*movesScore.begin()).first];
#ifdef RandBotTest
	}
	else
	{
		//srand(std::time(NULL));
		int rand1 = rand() % map.GetAllPossibleMoves().size();
		//sf::sleep(sf::seconds(0.5));
		return Move(map.GetAllPossibleMoves().at(rand1).from, map.GetAllPossibleMoves().at(rand1).to.at(rand() % map.GetAllPossibleMoves().at(rand1).to.size()));
	}
#endif
}

float PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color playerColor)
{
	float score = 0;
	FigureType selected;
#ifdef TestWeightAndBitboards
	if (isPlayerMoveFirst) // for testing
	{
#endif
		// black
		for (int i = 0; i != 64; ++i)
		{
			selected = selectedMap.GetFigureType(i);
			if (selected != FigureType::Empty)
				score += (selectedMap.GetColor(selected) == playerColor ? 1.0 : -1.0)
				* (figureWeight[static_cast<int>(selected)] + bitboards[static_cast<int>(selected)][i / 8][i % 8]);
		}
#ifdef TestWeightAndBitboards
	}
	else
	{
		// white
		for (int i = 0; i != 64; ++i)
		{
			selected = selectedMap.GetFigureType(i);
			if (selected != FigureType::Empty)
				score += (selectedMap.GetColor(selected) == playerColor ? 1.0 : -1.0)
				* (figureWeightTest[static_cast<int>(selected)] + bitboards[static_cast<int>(selected)][i / 8][i % 8]);
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
		Color AIColor = isPlayerMoveFirst ? Color::Black : Color::White;
		float bestMove = -infScore;
		int start = (AIColor == Color::Black ? 0 : 6); // used fixed enum order
		if (map.IsShahFor(AIColor))
			map.DisableCastlingForKing(AIColor); // if King is attacked => castling disabled
		for (int i = start; i != start + 6; ++i)
		{
			uint64_t j = 1;
			while (j) // check all positions
			{
				if (j & map.map[i]) // is selected figure position
				{
					OneFigureMoves moves;
					moves.from = Pos::BitboardToPosition(j);
					moves.to = Figure::FindPossibleMoves(moves.from, (FigureType)i, map); // find figure possible moves without checking shah 
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
		Color playerColor = isPlayerMoveFirst ? Color::White : Color::Black;
		float bestMove = infScore;
		int start = (playerColor == Color::Black ? 0 : 6); // used fixed enum order
		if (map.IsShahFor(playerColor))
			map.DisableCastlingForKing(playerColor); // if King is attacked => castling disabled
		for (int i = start; i != start + 6; ++i)
		{
			uint64_t j = 1;
			while (j) // check all positions
			{
				if (j & map.map[i]) // is selected figure position
				{
					OneFigureMoves moves;
					moves.from = Pos::BitboardToPosition(j);
					moves.to = Figure::FindPossibleMoves(moves.from, (FigureType)i, map); // find figure possible moves without checking shah 
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
	player1 = new Player(Color::White, name1, timeLimit, name2 != Menu::GetBotName());
	player2 = new Player(Color::Black, name2, timeLimit, name2 == Menu::GetBotName());
	activePlayer = (isPlayerMoveFirst) ? player1 : player2;
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	bool stopTime = isTimeLimited;
	isTimeLimited = false;
	mut3.lock();
	UpdateSideMenu();
	mut3.unlock();
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
		Move bestMove = StartAI();
		sf::Time time = clock.getElapsedTime();
		std::cout << "\n\tTime of calculating (in milliseconds): " << time.asMilliseconds()
			<< "\n\tCount of calculated positions: " << positionsCount << "\n\tEnd!";
		map.MakeMove(bestMove.from, bestMove.to);
		mut3.lock();
		UpdateSideMenu();
		mut3.unlock();
		map.ClearPossibleMoves();
		activePlayer = (activePlayer == player2) ? player1 : player2;
		status = map.CheckGameFinal(activePlayer->GetColor());

		if (stopTime)
			activePlayer->StartTimer();
		isTimeLimited = stopTime;
	}
#ifdef SmartBotTest
	if (status != GameStatus::Pat && status != GameStatus::Mat && map.movesHistory.size() < 1000)
	{
		activePlayer = (activePlayer == player2) ? player1 : player2;
		isPlayerMoveFirst = !isPlayerMoveFirst;
		ChangeActivePlayer();
	}
	else
	{
		if (map.movesHistory.size() < 1000)
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