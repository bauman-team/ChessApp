#include "MapProperties.h"

MapProperties::MapProperties(const MapProperties& copy)
{
	gameWindowSize = copy.gameWindowSize;
	playAreaTopLeft = copy.playAreaTopLeft;
	squareSize = copy.squareSize;
}

void MapProperties::SetGameWindowSize(int windowSize)
{
	gameWindowSize = windowSize;
}

void MapProperties::SetScale(float scale)
{
	playAreaTopLeft *= scale;
	squareSize *= scale;
}