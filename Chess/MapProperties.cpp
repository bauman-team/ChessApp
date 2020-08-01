#include "MapProperties.h"

MapProperties::MapProperties(const MapProperties& copy)
{
	playAreaTopLeft = copy.playAreaTopLeft;
	squareSize = copy.squareSize;
}

void MapProperties::SetScale(float scale)
{
	playAreaTopLeft *= scale;
	squareSize *= scale;
}