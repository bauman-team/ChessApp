#pragma once
#include <TGUI/TGUI.hpp>
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
	sf::Font font;
	sf::Text timeText;
	sf::CircleShape circle; // for showing possible moves
	sf::RectangleShape square; // for showing active figure
	MapProperties mapProps;
	
	bool isWhiteActive; // for correct calculation figures positions after gameboard rotation

	void SetResources(const Resources& resources);
	void SetScale();

	void ResizeWindowForGame();
	int GetSizeMenuTopLeftX() const;
	sf::Vector2u GetSideMenuSize() const;
public:
	Drawer(sf::RenderWindow* window, const Resources& resources, const MapProperties& properties);

	void ShowMap(const Map& map);
	void ShowTimer(sf::Time time);
	void ShowActiveFigure(const Pos& chosenPos, const Map& map);
	void ShowPossibleMoves(const Pos& chosenPos, const Map& map);
	void ShowGuiElems(tgui::Gui& gui);
	void DisplayWindow();

	void RotateBoard();
	void ResizeWindowForMenu(const sf::Vector2f& menuSize);
	MapProperties GetMapProps() const;

	// if coords isn't valid, return Pos::NULL_POS
	Pos TransformMousePosition(int mouseX, int mouseY) const; 
};