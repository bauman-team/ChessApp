#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Map.h"

#define NUM_OF_FIGURE_TYPES 6

class Drawer
{
	sf::RenderWindow* window;
	sf::Texture mapTexture;
	sf::Texture figuresTexture;
	sf::Sprite mapSprite;
	sf::Sprite figuresSprites[NUM_OF_FIGURE_TYPES * 2]; // array for saving white and black figures sprites

	void SetResources(const Resources& resource);
public:
	//Drawer() {};
	Drawer(sf::RenderWindow* window, const Resources& resource);
	//Drawer(const Resources* resource);

	//void SetResources(const Resources* resource);

	void ShowMap(const Map& map);
	void ShowActiveFigure();
	void ShowPossibleMoves();
};