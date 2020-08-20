#include "MapProperties.h"

MapProperties::MapProperties(const MapProperties& copy)
{
	gameWindowSize = copy.gameWindowSize;
	playAreaTopLeftX = copy.playAreaTopLeftX;
	playAreaTopLeftY = copy.playAreaTopLeftY;
	squareSize = copy.squareSize;
}

void MapProperties::SetPlayAreaTopLeft(uint8_t _playAreaTopLeftX, uint8_t _playAreaTopLeftY)
{ 
	playAreaTopLeftX = _playAreaTopLeftX; 
	playAreaTopLeftY = _playAreaTopLeftY; 
}

void MapProperties::SetGameWindowSize(int windowSize)
{
	gameWindowSize = windowSize;
}

void MapProperties::SetScale(float scale)
{
	playAreaTopLeftX *= scale;
	playAreaTopLeftY *= scale;
	squareSize *= scale;
}