#include <SFML/Graphics.hpp>
#include "Drawer.h"

#define NUM_OF_FIGURE_TYPES 6

void Drawer::SetResources(const Resources& resource)
{
	mapTexture.loadFromFile(resource.GetPathToMapImage());
	figuresTexture.loadFromFile(resource.GetPathToFiguresImage());

	mapSprite.setTexture(mapTexture);

	// set the mapSprite size to the window size
	mapSprite.setScale((float)window->getSize().x / mapTexture.getSize().x, (float)window->getSize().y / mapTexture.getSize().y);

	// TODO: initialize sprites for all figures
	int figuresInRow = resource.GetColumnsInFiguresImage();
	int figuresInCol = resource.GetRowsInFiguresImage();
	int figureWidth = figuresTexture.getSize().x / figuresInRow;
	int figureHeight = figuresTexture.getSize().y / figuresInCol;
	for (int i = 0; i < figuresInCol; ++i)
	{
		for (int j = 0; j < figuresInRow; ++j)
		{
			FigureType figureId = resource.GetFigureTypeAt(i * figuresInRow + j);
			figuresSprites[figureId].setTexture(figuresTexture);
			figuresSprites[figureId].setTextureRect(sf::IntRect(j * figureWidth, i * figureHeight, figureWidth, figureHeight));
		}
	}
}

/*void Drawer::SetResources(const Resources* resource)
{
	mapImage.loadFromFile(resource->GetPathToMapImage());
	figuresImage.loadFromFile(resource->GetPathToFiguresImage());
}*/

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resource) : window(_window)
{
	SetResources(resource);
}

/*Drawer::Drawer(const Resources* resource)
{
	SetResources(resource);
}*/

void Drawer::ShowMap(const Map& map)
{
	window->draw(mapSprite);
	for (int i = 0; i < 12; ++i)
	{
		figuresSprites[i].setPosition(0, 0);
		window->draw(figuresSprites[i]);
	}
	
	//window->draw(figuresSprites[Pawn_black]);
}

