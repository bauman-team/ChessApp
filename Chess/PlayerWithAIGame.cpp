#include "PlayerWithAIGame.h"

extern std::mutex mut1;
extern std::mutex mut3;

const int PlayerWithAIGame::figureWeight[12] = { 900, 90, 30, 30, 50, 10, 900, 90, 30, 30, 50, 10 };

const float PlayerWithAIGame::bitboards[12][8][8] = {
		{{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0}, //King_black
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
		 {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
		 {2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0},
		 {2.0, 3.0, 1.0, 0.0, 0.0, 1.0, 3.0, 2.0}},


		{{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}, //Queen_black
		 {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
		 {-1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
		 {-0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
		 {-0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0},
		 {-1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, -1.0},
		 {-1.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, -1.0},
		 {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}},


		{{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}, //Bishop_black
		 {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
		 {-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},
		 {-1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0},
		 {-1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, -1.0},
		 {-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0},
		 {-1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, -1.0},
		 {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}},


		{{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}, //Knight_black
		 {-4.0, -2.0, 0.0, 0.0, 0.0, 0.0, -2.0, -4.0},
		 {-3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0.0, -3.0},
		 {-3.0, 0.5, 1.5, 2.0, 2.0, 1.5, 0.5, -3.0},
		 {-3.0, 0.0, 1.5, 2.0, 2.0, 1.5, 0.0, -3.0},
		 {-3.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -3.0},
		 {-4.0, -2.0, 0.0, 0.5, 0.5, 0.0, -2.0, -4.0},
		 {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}},


		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, //Rook_black
		 {0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}},


		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, //Pawn_black
		 {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
		 {1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0},
		 {0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5},
		 {0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0},
		 {0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5},
		 {0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5},
		 {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},


		{{2.0, 3.0, 1.0, 0.0, 0.0, 1.0, 3.0, 2.0}, //King_white
		 {2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0},
		 {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
		 {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
		 {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0}},


		{{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}, //Queen_white
		 {-1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, -1.0},
		 {-1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
		 {0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
		 {-0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
		 {-1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
		 {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
		 {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}},


		 {{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}, //Bishop_white
		 {-1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, -1.0},
		 {-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0},
		 {-1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, -1.0},
		 {-1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0},
		 {-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},
		 {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
		 {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}},


		 {{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}, //Knight_white
		 {-4.0, -2.0, 0.0, 0.5, 0.5, 0.0, -2.0, -4.0},
		 {-3.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -3.0},
		 {-3.0, 0.0, 1.5, 2.0, 2.0, 1.5, 0.0, -3.0},
		 {-3.0, 0.5, 1.5, 2.0, 2.0, 1.5, 0.5, -3.0},
		 {-3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0.0, -3.0},
		 {-4.0, -2.0, 0.0, 0.0, 0.0, 0.0, -2.0, -4.0},
		 {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}},


		 {{0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, //Rook_white
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
		 {0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5},
		 {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},


		 {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, //Pawn_white
		 {0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5},
		 {0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5},
		 {0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0},
		 {0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5},
		 {1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0},
		 {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
		 {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
};

const int PlayerWithAIGame::DEPTH{ 3 };

std::atomic<int> positionsCount{ 0 }; // debug counter

PlayerWithAIGame::Move PlayerWithAIGame::StartAI(double timeForWaiting)
{
	//if (activePlayer->GetColor() != Color::Black)
	//{
		int bestIndex = 0;
		std::vector<Move> movesPositions; // fill vector all possible moves
		for (auto it1 = map.GetAllPossibleMoves().begin(); it1 != map.GetAllPossibleMoves().end(); ++it1)
			for (auto it2 = (*it1).to.begin(); it2 != (*it1).to.end(); ++it2)
				movesPositions.push_back(Move((*it1).from, *it2));
		int* const movesScore = new int[movesPositions.size()];
		std::atomic<int> countOfThreads = movesPositions.size();
		Color playerColor = activePlayer->GetColor() == Color::White ? Color::Black : Color::White; // TODO: maybe static const?
		for (int i = 0; i != movesPositions.size(); ++i)
		{
			Map copyMap(map);
			copyMap.Move(movesPositions[i].from, movesPositions[i].to);
			//movesScore[i] = PlayerWithAIGame::MiniMax(copyMap, countOfThreads, false, playerColor, DEPTH, -10'000, 10'000);
			std::thread th([i, movesScore, copyMap, playerColor, &countOfThreads]() {
				movesScore[i] = PlayerWithAIGame::MiniMax(copyMap, countOfThreads, false, playerColor, DEPTH, -10'000, 10'000);
				});
			th.detach();
		}

		sf::sleep(sf::seconds(timeForWaiting));

		if (timeForWaiting == 0) 
			while (countOfThreads != 0)
				sf::sleep(sf::seconds(0.1));

		for (int i = 1; i != movesPositions.size(); ++i) // TODO: add error rate
			if (movesScore[i] > movesScore[bestIndex])
				bestIndex = i;

		delete[] movesScore;

		return movesPositions[bestIndex];
	/*}
	else
	{
		srand(std::time(NULL));
		int rand1 = rand() % map.GetAllPossibleMoves().size();
		sf::sleep(sf::seconds(0.5));
		return Move(map.GetAllPossibleMoves().at(rand1).from, map.GetAllPossibleMoves().at(rand1).to.at(rand() % map.GetAllPossibleMoves().at(rand1).to.size()));
	}*/
}

int PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color playerColor)
{
	int score = 0;
	FigureType selected;
	for (int i = 0; i != 64; ++i)
	{
		selected = selectedMap.GetFigureType(Pos::IndexToPosition(i));
		if (selected != FigureType::Empty)
			score += figureWeight[static_cast<int>(selected)] * (selectedMap.GetColor(selected) == playerColor ? 1 : -1) * bitboards[static_cast<int>(selected)][i % 8][i / 8];
	}
	return score;
}

int PlayerWithAIGame::MiniMax(Map map, std::atomic<int> &countOfThreads, bool isAIMoveNow, const Color playerColor, int depth, int alpha, int beta)
{
	++positionsCount;
	if (depth == 0)
	{
		return -CalculatePositionScore(map, playerColor);
	}
	if (isAIMoveNow)
	{
		Color AIColor = playerColor == Color::Black ? Color::White : Color::Black;
		int bestMove = -9999, start = (AIColor == Color::Black ? 0 : 6); // used fixed enum order
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
							bestMove = std::max(bestMove, MiniMax(map, countOfThreads, !isAIMoveNow, playerColor, depth, alpha, beta));
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
		int bestMove = 9999, start = (playerColor == Color::Black ? 0 : 6); // used fixed enum order
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
							bestMove = std::min(bestMove, MiniMax(map, countOfThreads, !isAIMoveNow, playerColor, depth - 1, alpha, beta));
						map.UndoMove();
						beta = std::min(bestMove, beta);
						if (beta <= alpha)
						{
							if (depth == DEPTH)
								--countOfThreads;
							return bestMove;
						}
					}
				}
				j <<= 1;
			}
		}
		if (depth == DEPTH)
			--countOfThreads;
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
	map.FindAllPossibleMoves(activePlayer->GetColor());
	status = CheckGameFinal();
	if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
		std::cout << "Start!\n";
		positionsCount = 0;
		sf::Clock clock;
		Move bestMove = StartAI();
		sf::Time time = clock.getElapsedTime();
		std::cout << "End!\nTime of calculating (in milliseconds): " << time.asMilliseconds()
			<< "\n Count of calculated positions: " << positionsCount << "\n";
		map.MakeMove(bestMove.from, bestMove.to);
		mut3.lock();
		UpdateSideMenu();
		mut3.unlock();
		map.ClearPossibleMoves();
		activePlayer = (activePlayer == player2) ? player1 : player2;
		map.FindAllPossibleMoves(activePlayer->GetColor());
		status = CheckGameFinal();

		if (stopTime)
			activePlayer->StartTimer();
		isTimeLimited = stopTime;
	}
	/*if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
		activePlayer = (activePlayer == player2) ? player1 : player2;
		ChangeActivePlayer();
	}*/
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