#include "PlayerWithAIGame.h"
//#define RandBot
CHESSENGINE_API extern std::mutex mut1;
extern std::mutex mut3;
std::mutex mut2;

const int PlayerWithAIGame::figureWeight[FIGURE_TYPES] = { 9, 90, 3, 3, 5, 1, 9, 90, 3, 3, 5, 1 };
bool PlayerWithAIGame::isPlayerMoveFirst{ false };

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

const int PlayerWithAIGame::DEPTH{ 3 }; // searching depth of move
const int PlayerWithAIGame::countOfThreads{ 8 };

std::atomic<int> positionsCount{ 0 }; // debug counter

PlayerWithAIGame::Move PlayerWithAIGame::StartAI(double timeForWaiting)
{
#ifdef RandBot
	if (activePlayer->GetColor() == Color::Black && isPlayerMoveFirst
		|| activePlayer->GetColor() == Color::White && !isPlayerMoveFirst)
	{
#endif
		std::vector<std::pair<int, int>> movesScore;
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
					std::pair<int, int> score = std::pair<int, int>(j, PlayerWithAIGame::MiniMax(copyMap, false, DEPTH, -10'000, 10'000));
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

		std::sort(movesScore.begin(), movesScore.end(), [](const std::pair<int, int>& left, const std::pair<int, int>& right)
			{
				return left.second > right.second;
			});
		srand(time(NULL)); // Additional error rate
		if (rand() % 100 < 0)
		{
			for (int i = 0; i != 3; ++i)
				std::cout << "\n\t" + movesPositions[(*(movesScore.begin() + i)).first].from.ToString() + "  -->  " + movesPositions[(*(movesScore.begin() + i)).first].to.ToString();
			return movesPositions[(*(movesScore.begin() + rand() % std::min(3, static_cast<int>(movesPositions.size())))).first];
		}
		return movesPositions[(*movesScore.begin()).first];
		//return (rand() % 100 < 90) ? movesPositions[(*(movesScore.begin() + rand() % std::min(3, static_cast<int>(movesPositions.size())))).first] : movesPositions[(*movesScore.begin()).first];
#ifdef RandBot
	}
	else
	{
		srand(std::time(NULL));
		int rand1 = rand() % map.GetAllPossibleMoves().size();
		//sf::sleep(sf::seconds(0.5));
		return Move(map.GetAllPossibleMoves().at(rand1).from, map.GetAllPossibleMoves().at(rand1).to.at(rand() % map.GetAllPossibleMoves().at(rand1).to.size()));
	}
#endif
}

int PlayerWithAIGame::CalculatePositionScore(const Map& selectedMap, const Color playerColor)
{
	int score = 0;
	FigureType selected;
	for (int i = 0; i != 64; ++i)
	{
		selected = selectedMap.GetFigureType(i);
		if (selected != FigureType::Empty)
			score += figureWeight[static_cast<int>(selected)] * (selectedMap.GetColor(selected) == playerColor ? 1 : -1) * bitboards[static_cast<int>(selected)][i / 8][i % 8];
	}
	return score;
}

int PlayerWithAIGame::MiniMax(Map map, bool isAIMoveNow, int depth, int alpha, int beta)
{
	++positionsCount;
	if (depth == 0)
	{
		return -CalculatePositionScore(map, isPlayerMoveFirst ? Color::White : Color::Black);
	}
	if (isAIMoveNow)
	{
		Color AIColor = isPlayerMoveFirst ? Color::Black : Color::White;
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
	map.FindAllPossibleMoves(activePlayer->GetColor());
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
		map.FindAllPossibleMoves(activePlayer->GetColor());
		status = map.CheckGameFinal(activePlayer->GetColor());

		if (stopTime)
			activePlayer->StartTimer();
		isTimeLimited = stopTime;
	}
#ifdef RandBot
	if (status != GameStatus::Pat && status != GameStatus::Mat)
	{
		activePlayer = (activePlayer == player2) ? player1 : player2;
		ChangeActivePlayer();
	}
	else
	{
		std::cout << "\nFinish!\n\nStart new!";
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