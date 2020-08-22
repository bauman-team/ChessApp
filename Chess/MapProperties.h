#pragma once
#include <stdint.h>

class MapProperties
{
	uint8_t playAreaTopLeftY;
	uint8_t playAreaTopLeftX;
	uint8_t squareSize;
public:
	MapProperties() {};
	MapProperties(uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY, uint8_t _squareSize) : playAreaTopLeftX(_playAreaTopLeftX), playAreaTopLeftY(_playAreaTopLeftY), squareSize(_squareSize) {}
	MapProperties(const MapProperties& copy);

	uint8_t GetSquareSize() const { return squareSize; }
	uint8_t GetPlayAreaTopLeftX() const { return playAreaTopLeftX; }
	uint8_t GetPlayAreaTopLeftY() const { return playAreaTopLeftY; }

	void SetSquareSize(uint8_t size) { squareSize = size; }
	void SetPlayAreaTopLeft(uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY);
	void SetScale(float scale);
};