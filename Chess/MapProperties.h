#pragma once
#include <stdint.h>

class MapProperties
{
	uint8_t playAreaTopLeftY;
	uint8_t playAreaTopLeftX;
	uint8_t squareSize;
	int gameWindowHeight;
	int sideMenuWidth;
public:
	MapProperties() {};
	MapProperties(int _gameWindowHeight, int _sideMenuWidth, uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY, uint8_t _squareSize)
		: playAreaTopLeftX(_playAreaTopLeftX), playAreaTopLeftY(_playAreaTopLeftY), squareSize(_squareSize),
		gameWindowHeight(_gameWindowHeight), sideMenuWidth(_sideMenuWidth) {}
	MapProperties(const MapProperties& copy);

	uint8_t GetSquareSize() const { return squareSize; }
	int GetGameWindowHeight() const { return gameWindowHeight; }
	int GetSideMenuWidth() const { return sideMenuWidth; }
	int GetGameWindowWidth() const { return gameWindowHeight + sideMenuWidth; }
	uint8_t GetPlayAreaTopLeftX() const { return playAreaTopLeftX; }
	uint8_t GetPlayAreaTopLeftY() const { return playAreaTopLeftY; }

	void SetGameWindowHeight(int windowHeight);
	void SetSideMenuWidth(int menuWidth);
	void SetSquareSize(uint8_t size) { squareSize = size; }
	void SetPlayAreaTopLeft(uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY);
	void SetScale(float scale);
};