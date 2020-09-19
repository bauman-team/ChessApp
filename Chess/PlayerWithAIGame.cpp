#include "PlayerWithAIGame.h"
#include "Menu.h"

extern std::mutex mu;
extern std::mutex mut;

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

const int PlayerWithAIGame::DEPTH = 4;//4
/*
void PlayerWithAIGame::StartAI()
{
	std::vector<Move> startedMovesPositions;
	for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
		for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
			startedMovesPositions.push_back(Move(*(*it1).figurePosition, *it2));
	volatile Element** const startedMoves = new volatile Element*[startedMovesPositions.size()];
	Color activeColor = activePlayer->GetColor();
	for (int i = 0; i != startedMovesPositions.size(); ++i)
	{
		mut.lock();
		startedMoves[i] = new volatile Element[DEPTH];
		startedMoves[i][0].countOfThread = 1;
		mut.unlock();
		Map *copyMap = new Map(map);
		std::thread th([i, startedMoves, copyMap, startedMovesPositions, activeColor]() { PlayerWithAIGame::CalculateSecondPartOfMove(i, copyMap, startedMovesPositions.at(i), startedMoves, 0, activeColor); });
		th.detach();
	}
	//sf::sleep(sf::seconds(10));
	while (!IsAllCalculated(startedMoves, startedMovesPositions.size()))
	{
		sf::sleep(sf::seconds(0.01));
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
	mu.lock();
	map.RunMakeMove(startedMovesPositions.at(bestIndex).from, startedMovesPositions.at(bestIndex).to);
	mu.unlock();
	for (int i = 0; i != startedMovesPositions.size(); ++i)
		delete[] startedMoves[i];
	delete startedMoves;
	
}

void PlayerWithAIGame::CalculateFirstPartOfMove(int indexOfMove, Map *map, Move current, volatile Element** const startedMoves, int depth, const Color activeColor)
{
	map->Move(current.from, current.to);
	map->RunFindMoves(activeColor);
	if (!map->GetFigureWithAccessMoves().empty())
	{
		std::vector<int> movesScores;
		for (auto it1 = map->GetFigureWithAccessMoves().begin(); it1 != map->GetFigureWithAccessMoves().end(); ++it1)
			for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
			{
				FigureType movableFigure = map->GetFigureType(*(*it1).figurePosition), eatenFigure = map->GetFigureType((*it2));
				if (eatenFigure != FigureType::Empty)
					map->map[to_underlying(eatenFigure)] -= (*it2).ToBitboard();
				map->map[to_underlying(movableFigure)] -= (*it1).figurePosition->ToBitboard();
				map->map[to_underlying(movableFigure)] += (*it2).ToBitboard();
				movesScores.push_back(CalculatePositionScore(*map, activeColor));
				map->map[to_underlying(movableFigure)] += (*it1).figurePosition->ToBitboard();
				map->map[to_underlying(movableFigure)] -= (*it2).ToBitboard();
				if (eatenFigure != FigureType::Empty)
					map->map[to_underlying(eatenFigure)] += (*it2).ToBitboard();
			}

		int maxScore = movesScores[0];
		int k = 0;
		for (int i = 1; i != movesScores.size(); ++i)
			if (movesScores[i] > maxScore)
				maxScore = movesScores[i];

		for (auto it1 = map->GetFigureWithAccessMoves().begin(); it1 != map->GetFigureWithAccessMoves().end(); ++it1)
			for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
				if (movesScores[k++] > maxScore - 30)
				{
					mut.lock();
					++startedMoves[indexOfMove][depth].countOfThread;
					mut.unlock();
					Map* copyMap = new Map(*map);
					Move nextMove(*(*it1).figurePosition, *it2);
					std::thread th([indexOfMove, copyMap, nextMove, startedMoves, depth, activeColor]() { PlayerWithAIGame::CalculateSecondPartOfMove(indexOfMove, copyMap, nextMove, startedMoves, depth, activeColor); });
					th.detach();
				}
		delete map;
	}
}

void PlayerWithAIGame::CalculateSecondPartOfMove(int indexOfMove, Map *map, Move current, volatile Element** const startedMoves, int depth, const Color activeColor)
{
	map->Move(current.from, current.to);
	map->RunFindMoves(activeColor == Color::Black ? Color::White : Color::Black);
	if (!map->GetFigureWithAccessMoves().empty())
	{
		std::vector<int> movesScores;
		for (auto it1 = map->GetFigureWithAccessMoves().begin(); it1 != map->GetFigureWithAccessMoves().end(); ++it1)
			for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
			{
				FigureType movableFigure = map->GetFigureType(*(*it1).figurePosition), eatenFigure = map->GetFigureType((*it2));
				if (eatenFigure != FigureType::Empty)
					map->map[to_underlying(eatenFigure)] -= (*it2).ToBitboard();
				map->map[to_underlying(movableFigure)] -= (*it1).figurePosition->ToBitboard();
				map->map[to_underlying(movableFigure)] += (*it2).ToBitboard();
				movesScores.push_back(CalculatePositionScore(*map, activeColor));
				map->map[to_underlying(movableFigure)] += (*it1).figurePosition->ToBitboard();
				map->map[to_underlying(movableFigure)] -= (*it2).ToBitboard();
				if (eatenFigure != FigureType::Empty)
					map->map[to_underlying(eatenFigure)] += (*it2).ToBitboard();
			}

		int minScore = movesScores[0];
		int k = 0;
		for (int i = 1; i != movesScores.size(); ++i)
			if (movesScores[i] < minScore)
				minScore = movesScores[i];

		mut.lock();
		if (startedMoves[indexOfMove][depth].score > minScore || !startedMoves[indexOfMove][depth].isStarted)
			startedMoves[indexOfMove][depth].score = minScore;
		startedMoves[indexOfMove][depth].isStarted = true;
		--startedMoves[indexOfMove][depth].countOfThread;
		mut.unlock();

		if (depth + 1 < DEPTH)
			for (auto it1 = map->GetFigureWithAccessMoves().begin(); it1 != map->GetFigureWithAccessMoves().end(); ++it1)
				for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
					if (movesScores[k++] < minScore + 30)
					{
						Map* copyMap = new Map(*map);
						Move nextMove(*(*it1).figurePosition, *it2);
						std::thread th([indexOfMove, copyMap, nextMove, startedMoves, depth, activeColor]() { PlayerWithAIGame::CalculateFirstPartOfMove(indexOfMove, copyMap, nextMove, startedMoves, depth + 1, activeColor); });
						th.detach();
					}
		delete map;
	}
	else
	{
		mut.lock();
		if (!startedMoves[indexOfMove][depth].isStarted && startedMoves[indexOfMove][depth].countOfThread == 1)
			for (int i = depth; i != DEPTH; ++i)
			{
				startedMoves[indexOfMove][i].score = INT32_MIN;
				startedMoves[indexOfMove][i].isStarted = true;
			}
		--startedMoves[indexOfMove][depth].countOfThread;
		mut.unlock();
	}
}
*/

PlayerWithAIGame::Move PlayerWithAIGame::StartAI(double timeForWaiting = 0)
{
	std::vector<Move> startedAccessMovesPositions;
	for (auto it1 = map.GetFigureWithAccessMoves().begin(); it1 != map.GetFigureWithAccessMoves().end(); ++it1)
		for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
			startedAccessMovesPositions.push_back(Move(*(*it1).figurePosition, *it2));
	volatile TheWhorstCalculatedScoreOnDepth* const startedMovesScore = new volatile TheWhorstCalculatedScoreOnDepth[startedAccessMovesPositions.size()];
	bool *isThreadCompleted = new bool[startedAccessMovesPositions.size()];
	Color AIColor = activePlayer->GetColor();
	for (int i = 0; i != startedAccessMovesPositions.size(); ++i)
	{
		isThreadCompleted[i] = false;
		std::list<Map> listOfMaps;
		mut.lock();
		listOfMaps.push_back(map);
		mut.unlock();
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
	for (int i = 1; i != startedAccessMovesPositions.size(); ++i) // !!!!!! -1
		if (startedMovesScore[i].score > startedMovesScore[bestIndex].score)
			bestIndex = i;
		else if (startedMovesScore[i].score == startedMovesScore[bestIndex].score
			&& startedMovesScore[i].depth < startedMovesScore[bestIndex].depth)
			bestIndex = i;

	

	delete isThreadCompleted;
	delete startedMovesScore;

	return startedAccessMovesPositions[bestIndex];

	//rand bot, activate from ChangeActivePlayer
	/*srand(std::time(NULL));
	int rand1 = rand() % map.GetFigureWithAccessMoves().size();
	sf::sleep(sf::seconds(2));
	map.RunMakeMove(*map.GetFigureWithAccessMoves().at(rand1).figurePosition, map.GetFigureWithAccessMoves().at(rand1).possibleMoves->at(rand() % map.GetFigureWithAccessMoves().at(rand1).possibleMoves->size()));*/
}

int PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color AIColor)
{
	int score = 0;
	FigureType selected;
	for (int i = 0; i != 64; ++i)
	{
		selected = selectedMap.GetFigureType(Pos::IndexToPosition(i));
		if (selected != FigureType::Empty)
			score += figureWeight[to_underlying(selected)] * bitboards[to_underlying(selected)][i % 8][i / 8] * (selectedMap.GetColor(selected) == AIColor ? 1 : -1);
	}
	return score;
}

bool PlayerWithAIGame::CalculationScoreOfMoveInThread(std::list<Map> listOfMaps, volatile TheWhorstCalculatedScoreOnDepth& startedMoves, bool isAIMoveNow, const Color AIColor)
{
	int depth = 0, countOfMapsInList, countNewCreatedMap = 20; // if change countNewCreatedMap look to the func end 
	while (depth != DEPTH)
	{
		countOfMapsInList = listOfMaps.size();
		for (int k = 0; k != countOfMapsInList; ++k)
		{
			auto itMap = listOfMaps.front();
			itMap.RunFindMoves(isAIMoveNow ? AIColor : (AIColor == Color::Black ? Color::White : Color::Black));

			if (!itMap.GetFigureWithAccessMoves().empty())
			{
				std::vector<int> movesScores;
				for (auto it1 = itMap.GetFigureWithAccessMoves().begin(); it1 != itMap.GetFigureWithAccessMoves().end(); ++it1) // calculate score
					for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2)
					{
						FigureType movableFigure = itMap.GetFigureType(*(*it1).figurePosition), eatenFigure = itMap.GetFigureType((*it2));
						if (eatenFigure != FigureType::Empty)
							itMap.map[to_underlying(eatenFigure)] -= (*it2).ToBitboard();
						itMap.map[to_underlying(movableFigure)] -= (*it1).figurePosition->ToBitboard();
						itMap.map[to_underlying(movableFigure)] += (*it2).ToBitboard();
						movesScores.push_back(CalculatePositionScore(listOfMaps.front(), AIColor));
						itMap.map[to_underlying(movableFigure)] += (*it1).figurePosition->ToBitboard();
						itMap.map[to_underlying(movableFigure)] -= (*it2).ToBitboard();
						if (eatenFigure != FigureType::Empty)
							itMap.map[to_underlying(eatenFigure)] += (*it2).ToBitboard();
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
					mu.lock();
					if (startedMoves.depth == -1 || startedMoves.score > minScore)
					{
						startedMoves.depth = depth + 1;
						startedMoves.score = minScore;
					}
					mu.unlock();
				}

				int j = 0;
				if (depth + 1 < DEPTH || depth + 1 == DEPTH && isAIMoveNow) // create maps
					for (auto it1 = itMap.GetFigureWithAccessMoves().begin(); it1 != itMap.GetFigureWithAccessMoves().end(); ++it1)
						for (auto it2 = (*it1).possibleMoves->begin(); it2 != (*it1).possibleMoves->end(); ++it2, ++j)
							for (int i = 0; i != sizeOfArrayIndexOfMoves; ++i)
								if (j == indexOfMovesWithBestScore[i])
								{
									listOfMaps.push_back(itMap);
									listOfMaps.back().Move(*(*it1).figurePosition, *it2);
								}
				delete[] indexOfMovesWithBestScore;
			}
			itMap.RunClearPossibleMoves();
			listOfMaps.pop_front();
		}
		depth += isAIMoveNow ? 0 : 1;
		if (!isAIMoveNow)
			countNewCreatedMap = countNewCreatedMap > 3 ? countNewCreatedMap / 3 : 1;
		isAIMoveNow = !isAIMoveNow;
	}
	mu.lock();
	if (startedMoves.depth == -1) // if bot win
		startedMoves.score = INT32_MAX;
	mu.unlock();
	return true;
}

bool PlayerWithAIGame::IsAllThreadsOfMovesCompleted(bool* threadsInfo, int range)
{
	for (int i = 0; i != range; ++i)
		if (!threadsInfo[i])
			return false;
	return true;
}
/*
bool PlayerWithAIGame::IsAllCalculated(volatile Element** elems, int range) const
{
	for (int i = 0; i != range; ++i)
		if (!elems[i][DEPTH - 1].isStarted || elems[i][DEPTH - 1].countOfThread > 0)
			return false;
	return true;
}
*/
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
		Move bestMove = StartAI();


	/*	std::list<Map> a;
		a.push_back(Map(map));
		for (int i = 0; i != 100000; ++i)
		{
			a.push_back(Map(a.front()));
			a.back().Move(bestMove.from, bestMove.to);
		}
		a.pop_front();*/


		mu.lock();
		map.RunMakeMove(bestMove.from, bestMove.to);
		mu.unlock();
		map.RunClearPossibleMoves();
		activePlayer = (activePlayer == player2) ? player1 : player2;
		map.RunFindMoves(activePlayer->GetColor());
		status = CheckGameFinal();

		if (isTimeLimited)
			activePlayer->StartTimer();
	}
	activePlayer = (activePlayer == player2) ? player1 : player2;
	ChangeActivePlayer();
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
