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
	MapProperties mapProperties;
	
	bool isWhiteActive; // for correct calculation figures positions after gameboard rotation

	void SetResources(const Resources& resource);
	void SetScale();
public:
	Drawer(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties);

	sf::Vector2u GetSideMenuSize() const;
	int GetSizeMenuTopLeftX() const;
	MapProperties GetMapProps() const;

	void ShowMap(const Map& map);
	void ShowTimer(sf::Time time, Color activeColor);
	void ShowActiveFigure(const Map&, const Pos&);
	void ShowPossibleMoves(const Map&, const Pos&);
	void DisplayWindow();
	void RotateBoard();
	void ResizeWindowForGame(const sf::Vector2f& menuSize = sf::Vector2f(0, 0));
	void ShowGuiElems(tgui::Gui& gui);

	Pos* TransformMousePosition(int, int) const; // if coords isn't valid, return nullptr
};