#include "PlayerWithAIGame.h"
#include "Menu.h"

extern std::mutex mu;
extern std::mutex mut;

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

const int PlayerWithAIGame::DEPTH = 2;

void PlayerWithAIGame::StartAI()
{
	std::vector<Move> startedMovesPositions;
	for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
		for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
			startedMovesPositions.push_back(Move(*(*it1).figurePosition, *it2));
	volatile Element** const startedMoves = new volatile Element*[startedMovesPositions.size()];
	for (int i = 0; i != startedMovesPositions.size(); ++i)
	{
		mut.lock();
		startedMoves[i] = new volatile Element[DEPTH];
		startedMoves[i][0].countOfThread = 1;
		mut.unlock();
		Map a = map;
		Color activeColor = activePlayer->GetColor();
		std::thread th([i, startedMoves, a, startedMovesPositions, activeColor]() { PlayerWithAIGame::CalculateSecondPartOfMove(i, a, startedMovesPositions.at(i), startedMoves, 0, activeColor); });
		//std::thread th(PlayerWithAIGame::CalculateSecondPartOfMove, i, map, startedMovesPositions.at(i), std::cref(startedMoves), 0, activePlayer->GetColor());
		th.detach();
	}
	//sf::sleep(sf::seconds(10));
	while (!IsAllCalculated(startedMoves, startedMovesPositions.size()))
	{
		sf::sleep(sf::seconds(1));
	}
	int bestIndex = -1;
	int bestScore = -1;
	for (int i = 0; i != startedMovesPositions.size(); ++i)
	{
		if (startedMoves[i][DEPTH - 1].score > bestScore || bestIndex == -1)
		{
			bestScore = startedMoves[i][DEPTH - 1].score;
			bestIndex = i;
		}
	}

	map.RunMakeMove(startedMovesPositions.at(bestIndex).from, startedMovesPositions.at(bestIndex).to);
	for (int i = 0; i != startedMovesPositions.size(); ++i)
		delete[] startedMoves[i];
	delete startedMoves;
	/*srand(std::time(NULL));
	int rand1 = rand() % map.GetFigureWithAccessMoves().size();
	sf::sleep(sf::seconds(2));
	map.RunMakeMove(*map.GetFigureWithAccessMoves().at(rand1).figurePosition, map.GetFigureWithAccessMoves().at(rand1).possibleMoves->at(rand() % map.GetFigureWithAccessMoves().at(rand1).possibleMoves->size()));*/
}

void PlayerWithAIGame::CalculateFirstPartOfMove(int indexOfMove, Map map, Move current, volatile Element** const startedMoves, int depth, const Color activeColor)
{
	map.Move(current.from, current.to);
	map.RunFindMoves(activeColor);
	std::vector<int> movesScores;
	for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
		for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
		{
			FigureType movableFigure = map.GetFigureType(*(*it1).figurePosition), eatenFigure = map.GetFigureType((*it2));
			if (eatenFigure != FigureType::Empty)
				map.map[to_underlying(eatenFigure)] -= (*it2).ToBitboard();
			map.map[to_underlying(movableFigure)] -= (*it1).figurePosition->ToBitboard();
			map.map[to_underlying(movableFigure)] += (*it2).ToBitboard();
			movesScores.push_back(CalculatePositionScore(map, activeColor));
			map.map[to_underlying(movableFigure)] += (*it1).figurePosition->ToBitboard();
			map.map[to_underlying(movableFigure)] -= (*it2).ToBitboard();
			if (eatenFigure != FigureType::Empty)
				map.map[to_underlying(eatenFigure)] += (*it2).ToBitboard();
		}
	std::sort(movesScores.begin(), movesScores.end());
	//int k = 0;
	for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
		for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
		{
			//if (movesScores[k] == movesScores.back())
			//{
				mut.lock();
				++startedMoves[indexOfMove][depth].countOfThread;
				mut.unlock();
				Move nextMove(*(*it1).figurePosition, *it2);
				std::thread th([indexOfMove, map, nextMove, startedMoves, depth, activeColor]() { PlayerWithAIGame::CalculateSecondPartOfMove(indexOfMove, map, nextMove, startedMoves, depth, activeColor); });
				th.detach();
			//}
			//++k;
		}
}

void PlayerWithAIGame::CalculateSecondPartOfMove(int indexOfMove, Map map, Move current, volatile Element** const startedMoves, int depth, const Color activeColor)
{
	map.Move(current.from, current.to);
	map.RunFindMoves((activeColor == Color::Black) ? Color::White : Color::Black);
	std::vector<int> movesScores;
	for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
		for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
		{
			FigureType movableFigure = map.GetFigureType(*(*it1).figurePosition), eatenFigure = map.GetFigureType((*it2));
			if (eatenFigure != FigureType::Empty)
				map.map[to_underlying(eatenFigure)] -= (*it2).ToBitboard();
			map.map[to_underlying(movableFigure)] -= (*it1).figurePosition->ToBitboard();
			map.map[to_underlying(movableFigure)] += (*it2).ToBitboard();
			movesScores.push_back(CalculatePositionScore(map, activeColor));
			map.map[to_underlying(movableFigure)] += (*it1).figurePosition->ToBitboard();
			map.map[to_underlying(movableFigure)] -= (*it2).ToBitboard();
			if (eatenFigure != FigureType::Empty)
				map.map[to_underlying(eatenFigure)] += (*it2).ToBitboard();
		}
	std::sort(movesScores.begin(), movesScores.end());
	
	mut.lock();
	if (startedMoves[indexOfMove][depth].score > movesScores[0] || !startedMoves[indexOfMove][depth].isStarted)
		startedMoves[indexOfMove][depth].score = movesScores[0];
	startedMoves[indexOfMove][depth].isStarted = true;
	--startedMoves[indexOfMove][depth].countOfThread;
	mut.unlock();

	//int k = 0;
	if (depth + 1 < DEPTH)
		for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
			for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
			{
				//if (movesScores[k] == movesScores[0])
				//{
					Move nextMove(*(*it1).figurePosition, *it2);
					std::thread th([indexOfMove, map, nextMove, startedMoves, depth, activeColor]() { PlayerWithAIGame::CalculateSecondPartOfMove(indexOfMove, map, nextMove, startedMoves, depth + 1, activeColor); });
					//std::thread th(PlayerWithAIGame::CalculateFirstPartOfMove, indexOfMove, map, nextMove, std::ref(startedMoves), depth + 1, activeColor);
					th.detach();
				//}
				//++k;
			}
}

int PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color activeColor)
{
	int score = 0;
	FigureType selected;
	for (int i = 0; i != 64; ++i)
	{
		selected = selectedMap.GetFigureType(Pos::IndexToPosition(i));
		if (selected != FigureType::Empty)
			score += figureWeight[to_underlying(selected)] * bitboards[to_underlying(selected)][i % 8][i / 8] * (selectedMap.GetColor(selected) == activeColor ? 1 : -1);
	}
	return score;
}

bool PlayerWithAIGame::IsAllCalculated(volatile Element** elems, int range) const
{
	for (int i = 0; i != range; ++i)
		if (!elems[i][DEPTH - 1].isStarted || elems[i][DEPTH - 1].countOfThread > 0)
			return false;
	return true;
}

void PlayerWithAIGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	if (timeLimit != sf::seconds(0))
		isTimeLimited = true;
	isPlayerMoveFirst = (name2 == Menu::GetBotName());
	player1 = new Player(Color::White, name1, timeLimit);
	player2 = new Player(Color::Black, name2, timeLimit);
	activePlayer = (isPlayerMoveFirst) ? player1 : player2;
}

bool PlayerWithAIGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	return TwoPlayersGame::SetPlayerChosenCell(mouseX, mouseY);
	// bot functions
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	mu.lock();
	if (isTimeLimited)
		drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	activePlayer->SetChosenPosition(Pos::NULL_POS);
	mu.unlock();

	map.RunClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	map.RunFindMoves(activePlayer->GetColor());
	status = CheckGameFinal();
	//
	if (status != GameStatus::Pat || status != GameStatus::Mat)
	{
		//
		StartAI();
		map.RunClearPossibleMoves();
		activePlayer = (activePlayer == player2) ? player1 : player2;
		map.RunFindMoves(activePlayer->GetColor());
		status = CheckGameFinal();

		if (isTimeLimited)
			activePlayer->StartTimer();
	}
}

void PlayerWithAIGame::StartGame()
{
	Game::StartGame();
	if (!isPlayerMoveFirst)
	{
		drawer.RotateBoard();
		std::thread th(&PlayerWithAIGame::ChangeActivePlayer, std::ref(*this));
		th.detach();
	}
	else
	{
		map.RunFindMoves(activePlayer->GetColor());
		if (isTimeLimited)
			activePlayer->StartTimer();
	}
}
