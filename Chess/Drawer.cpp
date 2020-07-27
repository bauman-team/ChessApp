#include <SFML/Graphics.hpp>
#include "Drawer.h"

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resource, const MapProperties& properties)
	: window(_window), mapProperties(properties)
{
	SetResources(resource);
	SetScale();
}

void Drawer::SetResources(const Resources& resource)
{
	mapTexture.loadFromFile(resource.GetPathToMapImage());
	mapSprite.setTexture(mapTexture);	
	
	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresTextures[i].loadFromFile(resource.GetPathToFigure((FigureType)i));

	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setTexture(figuresTextures[i]);
}

void Drawer::SetScale()
{
	float mapScale = (float)window->getSize().x / mapTexture.getSize().x;
	float figureScale = mapScale * (float)mapProperties.GetSquareSize() / figuresTextures[0].getSize().x;
	mapSprite.setScale(mapScale, mapScale); // set the mapSprite size to the window size
	mapProperties.SetScale(mapScale);
	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setScale(figureScale, figureScale);
}

void Drawer::ShowMap(const Map& map)
{
	window->draw(mapSprite);
	for (int j = 0; j != 8; ++j)
	{
		for (int i = 0; i != 8; ++i)
		{
			FigureType selectedFigure = map.GetFigureAt(Pos(i, j))->GetType();
			if (selectedFigure != FigureType::Empty)
			{
				figuresSprites[to_underlying(selectedFigure)].setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + i * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (7 - j) * mapProperties.GetSquareSize());
				window->draw(figuresSprites[to_underlying(selectedFigure)]);
				//figuresSprites[to_underlying(x)].move(mapProperties.GetSquareSize(), 0);
			}
		}
	}
}