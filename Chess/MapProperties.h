#pragma once
#include "Pos.h"

class MapProperties
{
	int gameWindowSize; // not for the menu window
	Pos playAreaTopLeft;
	int squareSize;
public:
	MapProperties() {};
	MapProperties(int _gameWindowSize, Pos _playAreaTopLeft, int _squareSize)
		: gameWindowSize(_gameWindowSize), playAreaTopLeft(_playAreaTopLeft), squareSize(_squareSize) {}
	MapProperties(const MapProperties& copy);

	int GetSquareSize() const { return squareSize; }
	int GetGameWindowSize() const { return gameWindowSize; }
	int GetPlayAreaTopLeftX() const { return playAreaTopLeft.GetX(); }
	int GetPlayAreaTopLeftY() const { return playAreaTopLeft.GetY(); }

	void SetGameWindowSize(int windowSize);
	void SetSquareSize(int size) { squareSize = size; }
	void SetPlayAreaTopLeft(Pos _playAreaTopLeft) { playAreaTopLeft = _playAreaTopLeft; }
	void SetScale(float scale);
};