#pragma once
#include "Pos.h"

class MapProperties
{
	Pos playAreaTopLeft;
	int squareSize;
public:
	MapProperties() {};
	MapProperties(Pos _playAreaTopLeft, int _squareSize) : playAreaTopLeft(_playAreaTopLeft), squareSize(_squareSize) {}
	MapProperties(const MapProperties& copy);

	int GetSquareSize() const { return squareSize; }
	Pos GetPlayAreaTopLeft() const { return playAreaTopLeft; }

	void SetSquareSize(int size) { squareSize = size; }
	void SetPlayAreaTopLeft(Pos _playAreaTopLeft) { playAreaTopLeft = _playAreaTopLeft; }
	void SetScale(float scale);
};