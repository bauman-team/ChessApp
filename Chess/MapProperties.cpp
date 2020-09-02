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

void MapProperties::SetGameWindowHeight(int windowHeight)
{
	gameWindowHeight = windowHeight;
}

void MapProperties::SetSideMenuWidth(int menuWidth)
{
	sideMenuWidth = menuWidth;
}

void MapProperties::SetScale(float scale)
{
	playAreaTopLeftX *= scale;
	playAreaTopLeftY *= scale;
	squareSize *= scale;
}