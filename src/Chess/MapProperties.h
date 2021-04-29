#pragma once
#include <stdint.h>

class MapProperties
{
	uint8_t playAreaTopLeftX;
	uint8_t playAreaTopLeftY;
	uint8_t squareSize;
	uint32_t gameWindowHeight;
	uint32_t sideMenuWidth;
public:
	MapProperties() { };
	MapProperties(uint32_t _gameWindowHeight, uint32_t _sideMenuWidth, uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY, uint8_t _squareSize)
		: playAreaTopLeftX{ _playAreaTopLeftX }, playAreaTopLeftY{ _playAreaTopLeftY }, squareSize{ _squareSize },
		gameWindowHeight{ _gameWindowHeight }, sideMenuWidth{ _sideMenuWidth } { }
	MapProperties(const MapProperties& copy);

	uint8_t GetSquareSize() const { return squareSize; }
	uint32_t GetGameWindowHeight() const { return gameWindowHeight; }
	uint32_t GetSideMenuWidth() const { return sideMenuWidth; }
	uint32_t GetGameWindowWidth() const { return gameWindowHeight + sideMenuWidth; }
	uint8_t GetPlayAreaTopLeftX() const { return playAreaTopLeftX; }
	uint8_t GetPlayAreaTopLeftY() const { return playAreaTopLeftY; }

	void SetGameWindowHeight(uint32_t windowHeight);
	void SetSideMenuWidth(uint32_t menuWidth);
	void SetSquareSize(uint8_t size);
	void SetPlayAreaTopLeft(uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY);
	void SetScale(float scale);
};