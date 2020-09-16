#pragma once
#include "TwoPlayersGame.h"
#include <iostream>
#include <iomanip>

enum class FigureType;
class TwoPlayersGame;
class PlayerWithAIGame : public TwoPlayersGame
{
private:
	const static int figureWeight[12];
	const static float bitboards[12][8][8];
protected:
	bool isPlayerMoveFirst;
	const static int DEPTH;
	/*
		methods for bot calculate move
	*/
	
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
	struct Element
	{
		bool isStarted;
		int countOfThread;
		int score;
		Element()
		{
			isStarted = false;
			countOfThread = 0;
			score = 0;
		}
		Element(bool _isStarted, int _countOfThread, int _score)
		{
			isStarted = _isStarted;
			countOfThread = _countOfThread;
			score = _score;
		}
	};
	void StartAI(); //first call to SecondCalc
	static void CalculateFirstPartOfMove(int indexOfMove, Map map, Move current, volatile Element** startedMoves, int depth, const Color); //indexOfMove, map, struct MOVE(current), **array, depth
	static void CalculateSecondPartOfMove(int indexOfMove, Map map, Move current, volatile Element** startedMoves, int depth, const Color);
	static int CalculatePositionScore(const Map&, const Color); //&map
	bool IsAllCalculated(volatile Element** elems, int range) const;
public:
	PlayerWithAIGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& _mapProperties) 
		: TwoPlayersGame(window, resource, _mapProperties), isPlayerMoveFirst(true) {}

	void virtual SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0));

	bool virtual SetPlayerChosenCell(int, int) override;
	void virtual ChangeActivePlayer() override;
	void virtual StartGame() override;


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
					if (bitboards[i][7 - j][7 - k] == ((int)bitboards[i][7 - j][7 - k]))
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
