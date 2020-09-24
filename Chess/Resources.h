#pragma once
#include <string>
#include <assert.h>
#include "Figure.h"

#define FIGURE_TYPES 12 // 6 whites + 6 black figures

class Resources
{
	std::string pathToMapImage;
	std::string pathToFigureImage[FIGURE_TYPES];
	std::string pathToFont;
public:
	Resources() {}
	Resources(std::string mapPath, Pos _playAreaTopLeft, int _squareSize) : pathToMapImage(mapPath) {}

	std::string GetPathToMapImage() const { return pathToMapImage; }
	std::string GetPathToFigure(FigureType figureType) const { return pathToFigureImage[(int)(figureType)]; }
	std::string GetPathToFont() const { return pathToFont; }

	void SetMapImage(std::string mapPath) { pathToMapImage = mapPath; }
	void SetFont(std::string fontPath) { pathToFont = fontPath; }
	void SetFigureImage(FigureType figureType, std::string figurePath)
	{
		assert(figureType != FigureType::Empty);
		pathToFigureImage[(int)(figureType)] = figurePath;
	}
};