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
	sf::Texture figuresTextures[FIGURE_TYPES];
	sf::Sprite figuresSprites[FIGURE_TYPES];
	MapProperties mapProperties;
	void SetResources(const Resources& resource);
	void SetScale();
public:
	Drawer(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	void ShowMap(const Map& map);
	void ShowActiveFigure();
	void ShowPossibleMoves();
};