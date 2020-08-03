#include <SFML/Graphics.hpp>
#include "Drawer.h"

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resource, const MapProperties& properties)
	: window(_window), mapProperties(properties)
{
	SetResources(resource);
	SetScale();
	IsWhiteActive = true;
}

void Drawer::SetResources(const Resources& resource)
{
	mapTexture.loadFromFile(resource.GetPathToMapImage());
	mapSprite.setTexture(mapTexture);	
	mapSprite.setOrigin(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);
	mapSprite.setPosition(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);

	chosenCellTexture.loadFromFile(resource.GetPathToChosenCellImage());
	chosenCellSprite.setTexture(chosenCellTexture);

	possibleCellTexture.loadFromFile(resource.GetPathToPossibleCellImage());
	possibleCellSprite.setTexture(possibleCellTexture);
	
	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresTextures[i].loadFromFile(resource.GetPathToFigure((FigureType)i));

	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setTexture(figuresTextures[i]);
}

void Drawer::SetScale()
{
	float mapScale = (float)window->getSize().x / mapTexture.getSize().x;
	float figureScale = mapScale * (float)mapProperties.GetSquareSize() / figuresTextures[0].getSize().x;
	float cellScale = mapScale * (float)mapProperties.GetSquareSize() / possibleCellTexture.getSize().x;
	mapSprite.setScale(mapScale, mapScale); // set the mapSprite size to the window size
	mapProperties.SetScale(mapScale);
	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setScale(figureScale, figureScale);
	possibleCellSprite.setScale(cellScale, cellScale);
	chosenCellSprite.setScale(cellScale, cellScale);
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
				if (IsWhiteActive)
					figuresSprites[to_underlying(selectedFigure)].setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + i * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (7 - j) * mapProperties.GetSquareSize());
				else
					figuresSprites[to_underlying(selectedFigure)].setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + (7 - i) * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + j * mapProperties.GetSquareSize());
				window->draw(figuresSprites[to_underlying(selectedFigure)]);
				//figuresSprites[to_underlying(x)].move(mapProperties.GetSquareSize(), 0);
			}
		}
	}
}

void Drawer::ShowActiveFigure(const Map& map, const Pos& chosenFigure)
{
	if (IsWhiteActive)
		chosenCellSprite.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + chosenFigure.GetX() * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (7 - chosenFigure.GetY()) * mapProperties.GetSquareSize());
	else
		chosenCellSprite.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + (7 - chosenFigure.GetX()) * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + chosenFigure.GetY() * mapProperties.GetSquareSize());
	window->draw(chosenCellSprite);
	for (std::vector<Pos>::const_iterator it = map.GetFigureAt(chosenFigure)->GetPossibleMoves().begin(); it != map.GetFigureAt(chosenFigure)->GetPossibleMoves().end(); ++it)
	{
		if (IsWhiteActive)
			possibleCellSprite.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + it->GetX() * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (7 - it->GetY()) * mapProperties.GetSquareSize());
		else
			possibleCellSprite.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + (7 - it->GetX()) * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + it->GetY() * mapProperties.GetSquareSize());
		window->draw(possibleCellSprite);
	}
}

void Drawer::RotateBoard()
{
	mapSprite.rotate(180);
	IsWhiteActive = !IsWhiteActive;
}

Pos* Drawer::TransformMousePosition(int mouseX, int mouseY)
{
	return new Pos((mouseX - mapProperties.GetPlayAreaTopLeft().GetX()) / mapProperties.GetSquareSize(), 7 - (mouseY - mapProperties.GetPlayAreaTopLeft().GetY()) / mapProperties.GetSquareSize());
}
