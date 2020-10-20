#include "PlayerWithAIGame.h"

extern std::mutex mut1;
extern std::mutex mut2;
extern std::mutex mut3;

const int PlayerWithAIGame::figureWeight[12] = { 900, 90, 30, 30, 50, 30, 900, 90, 30, 30, 50, 10 };

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

const int PlayerWithAIGame::DEPTH = 1;//4


PlayerWithAIGame::Move PlayerWithAIGame::StartAI(double timeForWaiting = 0)
{
	/*if (activePlayer->GetColor() != Color::Black)
	{*/
		std::vector<Move> startedAccessMovesPositions;
		for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
			for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
				startedAccessMovesPositions.push_back(Move(*(*it1).figurePosition, *it2));
		volatile TheWhorstCalculatedScoreOnDepth* const startedMovesScore = new volatile TheWhorstCalculatedScoreOnDepth[startedAccessMovesPositions.size()];
		bool* isThreadCompleted = new bool[startedAccessMovesPositions.size()];
		Color AIColor = activePlayer->GetColor();
		for (int i = 0; i != startedAccessMovesPositions.size(); ++i)
		{
			isThreadCompleted[i] = false;
			std::list<Map> listOfMaps;
			mut2.lock();
			listOfMaps.push_back(map);
			mut2.unlock();
			listOfMaps.back().Move(startedAccessMovesPositions[i].from, startedAccessMovesPositions[i].to);
			//isThreadCompleted[i] = PlayerWithAIGame::CalculationScoreOfMoveInThread(listOfMaps, startedMovesScore[i], false, AIColor);
			std::thread th([i, startedMovesScore, listOfMaps, AIColor, isThreadCompleted]() {
				isThreadCompleted[i] = PlayerWithAIGame::CalculationScoreOfMoveInThread(listOfMaps, startedMovesScore[i], false, AIColor);
				});
			th.detach();
		}

		sf::sleep(sf::seconds(timeForWaiting));

		if (timeForWaiting == 0)
			while (!IsAllThreadsOfMovesCompleted(isThreadCompleted, startedAccessMovesPositions.size()))
				sf::sleep(sf::seconds(0.1));

		int bestIndex = 0;
		mut2.lock();
		for (int i = 1; i != startedAccessMovesPositions.size(); ++i)
			if (startedMovesScore[i].score > startedMovesScore[bestIndex].score)
				bestIndex = i;
			else if (startedMovesScore[i].score == startedMovesScore[bestIndex].score
				&& startedMovesScore[i].depth < startedMovesScore[bestIndex].depth)
				bestIndex = i;
		mut2.unlock();


		delete[] isThreadCompleted;
		delete[] startedMovesScore;

		return startedAccessMovesPositions[bestIndex];
	/*}
	else
	{
		srand(std::time(NULL));
		int rand1 = rand() % map.GetFigureWithAccessMoves().size();
		sf::sleep(sf::seconds(0.5));
		return Move(*map.GetFigureWithAccessMoves().at(rand1).figurePosition, map.GetFigureWithAccessMoves().at(rand1).possibleMoves->at(rand() % map.GetFigureWithAccessMoves().at(rand1).possibleMoves->size()));
	}*/
}

int PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color AIColor)
{
	int score = 0;
	FigureType selected;
	for (int i = 0; i != 64; ++i)
	{
		selected = selectedMap.GetFigureType(Pos::IndexToPosition(i));
		if (selected != FigureType::Empty)
			score += figureWeight[(int)(selected)] * (selectedMap.GetColor(selected) == AIColor ? 1 : -1); // * bitboards[(int)(selected)][i % 8][i / 8] 
	}
	return score;
}

bool PlayerWithAIGame::CalculationScoreOfMoveInThread(std::list<Map> listOfMaps, volatile TheWhorstCalculatedScoreOnDepth& startedMoves, bool isAIMoveNow, const Color AIColor)
{
	int depth = 0, countOfMapsInList, countNewCreatedMap = 30; // if change countNewCreatedMap look to the func end 
	while (depth != DEPTH)
	{
		countOfMapsInList = listOfMaps.size();
		for (int k = 0; k != countOfMapsInList; ++k)
		{
			Map* itMap = &listOfMaps.front();
			itMap->RunFindMoves(isAIMoveNow ? AIColor : (AIColor == Color::Black ? Color::White : Color::Black));

			if (!itMap->GetFigureWithAccessMoves().empty())
			{
				std::vector<int> movesScores;
				for (auto it1 = itMap->GetFigureWithAccessMoves().begin(); it1 != itMap->GetFigureWithAccessMoves().end(); ++it1) // calculate score
					for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
					{
						FigureType movableFigure = itMap->GetFigureType(*(*it1).figurePosition), eatenFigure = itMap->GetFigureType((*it2));
						if (eatenFigure != FigureType::Empty)
							itMap->map[(int)(eatenFigure)] -= (*it2).ToBitboard();
						itMap->map[(int)(movableFigure)] -= (*it1).figurePosition->ToBitboard();
						itMap->map[(int)(movableFigure)] += (*it2).ToBitboard();
						movesScores.push_back(CalculatePositionScore(*itMap, AIColor));
						itMap->map[(int)(movableFigure)] -= (*it2).ToBitboard();
						itMap->map[(int)(movableFigure)] += (*it1).figurePosition->ToBitboard();
						if (eatenFigure != FigureType::Empty)
							itMap->map[(int)(eatenFigure)] += (*it2).ToBitboard();
					}

				int sizeOfArrayIndexOfMoves = countNewCreatedMap > movesScores.size() ? movesScores.size() : countNewCreatedMap;
				int* indexOfMovesWithBestScore = new int[sizeOfArrayIndexOfMoves]; //index of maps witch create later

				if (isAIMoveNow)
				{
					for (int i = 0; i != sizeOfArrayIndexOfMoves; ++i) // max score for bot
					{
						indexOfMovesWithBestScore[i] = 0;
						for (int j = 1; j != movesScores.size(); ++j)
						{
							if (movesScores[j] > movesScores[indexOfMovesWithBestScore[i]])
								indexOfMovesWithBestScore[i] = j;
						}
						movesScores[indexOfMovesWithBestScore[i]] = INT32_MIN;
					}
				}
				else
				{
					int minScore;
					for (int i = 0; i != sizeOfArrayIndexOfMoves; ++i) // min score for opponent
					{
						indexOfMovesWithBestScore[i] = 0;
						for (int j = 1; j != movesScores.size(); ++j)
						{
							if (movesScores[j] < movesScores[indexOfMovesWithBestScore[i]])
								indexOfMovesWithBestScore[i] = j;
						}
						if (i == 0)
							minScore = movesScores[indexOfMovesWithBestScore[i]];
						movesScores[indexOfMovesWithBestScore[i]] = INT32_MAX;
					}
					mut2.lock();
					if (startedMoves.depth == -1 || startedMoves.score > minScore)
					{
						startedMoves.depth = depth + 1;
						startedMoves.score = minScore;
					}
					mut2.unlock();
				}

				int j = 0;
				if (depth + 1 < DEPTH || depth + 1 == DEPTH && isAIMoveNow) // create maps
					for (auto it1 = itMap->GetFigureWithAccessMoves().begin(); it1 != itMap->GetFigureWithAccessMoves().end(); ++it1)
						for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2, ++j)
							for (int i = 0; i != sizeOfArrayIndexOfMoves; ++i)
								if (j == indexOfMovesWithBestScore[i])
								{
									listOfMaps.push_back(*itMap);
									listOfMaps.back().Move(*(*it1).figurePosition, *it2);
								}
				delete[] indexOfMovesWithBestScore;
				itMap->RunClearPossibleMoves();
			}
			listOfMaps.pop_front();
		}
		depth += isAIMoveNow ? 0 : 1;
		/*if (!isAIMoveNow)
			countNewCreatedMap = countNewCreatedMap > 3 ? countNewCreatedMap / 3 : 1;*/
		isAIMoveNow = !isAIMoveNow;
	}
	mut2.lock();
	if (startedMoves.depth == -1) // if bot win (add checking PAT)
		startedMoves.score = INT32_MAX;
	mut2.unlock();
	return true;
}

bool PlayerWithAIGame::IsAllThreadsOfMovesCompleted(bool* threadsInfo, int range)
{
	for (int i = 0; i != range; ++i)
		if (!threadsInfo[i])
			return false;
	return true;
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
	mut3.lock();
	UpdateSideMenu();
	mut3.unlock();
	mut1.lock();
	activePlayer->SetChosenPosition(Pos::NULL_POS);
	mut1.unlock();
	map.RunClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	map.RunFindMoves(activePlayer->GetColor());
	status = CheckGameFinal();
	if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
		Move bestMove = StartAI();
		mut1.lock();
		map.RunMakeMove(bestMove.from, bestMove.to);
		mut1.unlock();
		mut3.lock();
		UpdateSideMenu();
		mut3.unlock();
		map.RunClearPossibleMoves();
		activePlayer = (activePlayer == player2) ? player1 : player2;
		map.RunFindMoves(activePlayer->GetColor());
		status = CheckGameFinal();

		if (isTimeLimited)
			activePlayer->StartTimer();
	}
	/*if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
		activePlayer = (activePlayer == player2) ? player1 : player2;
		ChangeActivePlayer();
	}*/
}

void PlayerWithAIGame::StartGame()
{
	Game::StartGame();
	if (!isPlayerMoveFirst)
	{
		drawer.RotateBoard();
	}
	else
	{
		map.RunFindMoves(activePlayer->GetColor());
		if (isTimeLimited)
			activePlayer->StartTimer();
	}
}
