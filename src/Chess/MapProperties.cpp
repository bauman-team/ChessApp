#include "MapProperties.h"

MapProperties::MapProperties(const MapProperties& copy)
{
	gameWindowHeight = copy.gameWindowHeight;
	playAreaTopLeftX = copy.playAreaTopLeftX;
	playAreaTopLeftY = copy.playAreaTopLeftY;
	squareSize = copy.squareSize;
	sideMenuWidth = copy.sideMenuWidth;
}

void MapProperties::SetPlayAreaTopLeft(uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY)
{ 
	playAreaTopLeftX = _playAreaTopLeftX; 
	playAreaTopLeftY = _playAreaTopLeftY; 
}

void MapProperties::SetGameWindowHeight(uint32_t windowHeight)
{
	gameWindowHeight = windowHeight;
}

void MapProperties::SetSideMenuWidth(uint32_t menuWidth)
{
	sideMenuWidth = menuWidth;
}

void MapProperties::SetSquareSize(uint8_t size)
{
	squareSize = size;
}

void MapProperties::SetScale(float scale)
{
	playAreaTopLeftX *= scale;
	playAreaTopLeftY *= scale;
	squareSize *= scale;
}