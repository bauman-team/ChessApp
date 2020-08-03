#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Map.h"
#include "MapProperties.h"

class Drawer
{
	sf::RenderWindow* window;
	sf::Texture mapTexture;
	sf::Sprite mapSprite;
	sf::Texture chosenCellTexture;
	sf::Sprite chosenCellSprite;
	sf::Texture possibleCellTexture;
	sf::Sprite possibleCellSprite;
	sf::Texture figuresTextures[FIGURE_TYPES];
	sf::Sprite figuresSprites[FIGURE_TYPES];
	MapProperties mapProperties;

	bool isWhiteActive; // for correct calculation figures positions after gameboard rotation

	void SetResources(const Resources& resource);
	void SetScale();
public:
	Drawer(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	void ShowMap(const Map& map);
	void ShowActiveFigure(const Map&, const Pos&);
	void ShowPossibleMoves(const Map&, const Pos&);

	void RotateBoard();

	Pos* TransformMousePosition(int, int) const; // fix negative coords
};