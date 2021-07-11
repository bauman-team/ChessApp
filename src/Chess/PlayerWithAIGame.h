#pragma once
#include "TwoPlayersGame.h"
#include <iostream>
#include <map>
//#include <iomanip>

class PlayerWithAIGame : public TwoPlayersGame
{
private:
	const static float figureWeight[FIGURE_TYPES];
	static int winWhite;
	static int winBlack;
	const static float bitboards[FIGURE_TYPES][64];
protected:
	const static float infScore;
	const static int errorRate;
	const int countOfThreads;
	std::vector<std::future<void>> Threads;
	static bool isPlayerMoveFirst;
	const int DEPTH; // searching depth of move

	std::vector<MoveInfo> StartAI(double timeForWaiting = 0);
	static float CalculatePositionScore(const Map& selectedMap, const Color AIColor);
	static float MiniMax(Map map, bool isAIMoveNow, int depth, float alpha, float beta);

public:
	PlayerWithAIGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& _mapProperties, GameMode mode, std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0));

	void virtual ChangeActivePlayer() override;
	void virtual StartGame() override;

	bool GetIsPlayerMoveFirst() { return isPlayerMoveFirst; }

	/*void output()
	{
		for (auto i = 0; i != 6; ++i)
		{
			std::cout << "{";
			for (auto j = 0; j != 8; ++j)
			{
				std::cout << "{";
				for (auto k = 0; k != 8; ++k)
				{
					std::cout << bitboards[i][7 - j][7 - k];
					if (bitboards[i][7 - j][7 - k] == (static_cast<int>bitboards[i][7 - j][7 - k]))
						std::cout << ".0";
					if (k != 7)
						std::cout << ", ";
				}
				std::cout << "},\n";
			}
			std::cout << "},\n";
		}
	}*/
};
