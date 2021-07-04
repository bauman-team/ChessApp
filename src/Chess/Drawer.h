#pragma once
#include "SideMenu.h"

class SideMenu;
enum class GameMode;

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
	sf::Vector2f windowScale;
	MapProperties mapProps;
	SideMenu sideMenu;

	Pos selectedPosFrom, selectedPosTo;
	std::list<FigureType> selectedFigure;
	
	bool isWhiteActive; // for correct calculation figures positions after gameboard rotation

	void SetResources(const Resources& resources);
	void SetScale();

	int GetSizeMenuTopLeftX() const;
	sf::Vector2u GetSideMenuSize() const;
public:
	Drawer(sf::RenderWindow* window, const Resources& resources, const MapProperties& properties, GameMode mode, GameSet SetExitStatus, GameSet MakeUndoMove, Game* game);
	void ShowMap(const Map& map);
	void ShowTimer(sf::Time time);
	void ShowActiveFigure(const Pos& chosenPos, const Map& map);
	void ShowPossibleMoves(const Pos& chosenPos, const Map& map);
	void ShowTransformFigure();
	void ShowSideMenu(const Map& map);
	void HandleEvent(sf::Event& event);
	void DisplayWindow();

	void WindowIsResized();

	void RotateBoard();
	void ResizeWindowForMenu(const sf::Vector2f& menuSize);
	MapProperties GetMapProps() const;
	const sf::Sprite* GetFigureSprites() const { return figuresSprites; }

	
	Pos TransformMousePosition(int mouseX, int mouseY) const; // if coords isn't valid, return Pos::NULL_POS
	void SetSelectedPositionFrom(const Pos position) { selectedPosFrom = position; }
	void SetSelectedPositionTo(const Pos position, const Map& map);
	void SetSelectedFigure(int mouseX, int mouseY);
	void ClearSelect();

	Pos GetSelectedPositionFrom() const { return selectedPosFrom; }
	Pos GetSelectedPositionTo() const { return selectedPosTo; }
	FigureType GetSelectedFigure() const { return selectedFigure.empty() ? FigureType::Empty : selectedFigure.front(); }
};