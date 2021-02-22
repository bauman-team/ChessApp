#pragma once
#include "TwoPlayersGame.h"
#include <iostream>
#include <map>
//#include <iomanip>

class PlayerWithAIGame : public TwoPlayersGame
{
private:
	const static int figureWeight[FIGURE_TYPES];
	const static float bitboards[FIGURE_TYPES][8][8];
protected:
	static bool isPlayerMoveFirst;
	const static int DEPTH;
	const static int countOfThreads;
	struct Move
	{
		Pos from;
		Pos to;
		Move(Pos _from, Pos _to)
		{
			from = _from;
			to = _to;
		}
	};

	Move StartAI(double timeForWaiting = 0);
	static int CalculatePositionScore(const Map& selectedMap, const Color AIColor);
	static int MiniMax(Map map, bool isAIMoveNow, int depth, int alpha, int beta);

public:
	PlayerWithAIGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& _mapProperties) 
		: TwoPlayersGame(window, resource, _mapProperties) {}

	void virtual SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0));

	void virtual ChangeActivePlayer() override;
	void virtual StartGame() override;

	bool GetIsPlayerMoveFirst() { return isPlayerMoveFirst; }

	/*void output()
	{
		for (int i = 0; i != 6; ++i)
		{
			std::cout << "{";
			for (int j = 0; j != 8; ++j)
			{
				std::cout << "{";
				for (int k = 0; k != 8; ++k)
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
