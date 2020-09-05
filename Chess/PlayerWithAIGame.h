#pragma once
#include "TwoPlayersGame.h"

class TwoPlayersGame;
class PlayerWithAIGame : public TwoPlayersGame
{
protected:
	bool isPlayerMoveFirst;
	/*
		methods for bot calculate move
	*/
public:
	PlayerWithAIGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& _mapProperties) 
		: TwoPlayersGame(window, resource, _mapProperties), isPlayerMoveFirst(true) {}

	void SetPlayers(std::string name, bool _isPlayerMoveFirst, sf::Time timeLimit = sf::seconds(0));

	void virtual SetPlayerChosenCell(int, int) override;
	void virtual ChangeActivePlayer() override;
	void virtual StartGame() override;

};
