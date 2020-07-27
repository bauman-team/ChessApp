#pragma once
#include <string>
#include <assert.h>
#include "Figure.h"

#define FIGURE_TYPES 12 // 6 whites + 6 black figures

class Resources
{
	std::string pathToMapImage;
	std::string pathToFigureImage[FIGURE_TYPES];

public:
	Resources() {}
	Resources(std::string mapPath, Pos _playAreaTopLeft, int _squareSize) : pathToMapImage(mapPath) {}

	std::string GetPathToMapImage() const { return pathToMapImage; }
	std::string GetPathToFigure(FigureType figureType) const { return pathToFigureImage[to_underlying(figureType)]; }

	void SetMapImage(std::string mapPath) { pathToMapImage = mapPath; }
	void SetFigureImage(FigureType figureType, std::string figurePath)
	{
		assert(figureType != FigureType::Empty);
		pathToFigureImage[to_underlying(figureType)] = figurePath;
	}
};