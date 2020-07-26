#include <SFML/Graphics.hpp>
#include "Drawer.h"

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

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resource, const MapProperties& properties)
	: window(_window), mapProperties(properties)
{
	SetResources(resource);
	SetScale();
}

void Drawer::ShowMap(const Map& map)
{
	window->draw(mapSprite);
	figuresSprites[Pawn_black].setPosition(mapProperties.GetPlayAreaTopLeft().x, mapProperties.GetPlayAreaTopLeft().y + 6 * mapProperties.GetSquareSize());
	for (int i = 0; i < 8; ++i)
	{
		window->draw(figuresSprites[Pawn_black]);
		figuresSprites[Pawn_black].move(mapProperties.GetSquareSize(), 0);
	}
}