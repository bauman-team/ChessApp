#pragma once
#pragma pointers_to_members(full_generality, multiple_inheritance)
#include <TGUI/TGUI.hpp>
#include "Resources.h"
#include "MapProperties.h"

class Map;
class Game;

using GameSet = void(Game::*)();

class SideMenu
{
	tgui::Gui gameGui;
	sf::Sprite figuresSprites[FIGURE_TYPES];
	const uint8_t textSize{ 20 };
	const uint16_t beforeMoveColumn{ 50 };
	const uint16_t afterMoveColumn{ 120 };
	const uint8_t scale{ 14 };
	const uint16_t infoBlockWidth{ 300 };
	const uint16_t infoBlockHeight{ 70 };
	const uint8_t splitLineWidth{ 3 };

public:
	SideMenu(sf::RenderWindow* window, const MapProperties& properties, GameSet SetExitStatus, Game* game);

	void SetSprites(sf::Sprite *_figuresSprites);
	void ResizeWindowForGame(sf::RenderWindow* window, const MapProperties& properties);
	void HandleEvent(sf::Event& event);
	void UpdateSideMenu(std::vector<MoveInfo> info);
};
