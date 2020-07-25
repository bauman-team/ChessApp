#pragma once
#include <string>

class Resources
{
	std::string pathToMapImage;
	std::string pathToFiguresImage;
public:
	Resources() {}
	Resources(std::string mapPath, std::string figuresPath) : pathToMapImage(mapPath), pathToFiguresImage(figuresPath) {}

	std::string GetPathToMapImage() const { return pathToMapImage; }
	std::string GetPathToFiguresImage() const { return pathToFiguresImage; }

	void SetPathToMapImage(std::string mapPath) { pathToMapImage = mapPath; }
	void SetPathToFiguresImage(std::string figuresPath) { pathToFiguresImage = figuresPath; }
};