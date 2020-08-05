#include <SFML/Graphics.hpp>
#include "Drawer.h"

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resource, const MapProperties& properties)
	: window(_window), mapProperties(properties)
{
	SetResources(resource);
	SetScale();
	isWhiteActive = true;

	circle.setFillColor(sf::Color(100, 200, 10, 200)); // green

	// timer text styles
	timeText.setCharacterSize(28);
	timeText.setPosition(5, 7);
	timeText.setStyle(sf::Text::Bold);

	// active rectangle styles
	square.setFillColor(sf::Color(0, 0, 0, 50));
	square.setOutlineThickness(4);
	square.setOutlineColor(sf::Color::Yellow);
}

void Drawer::SetResources(const Resources& resource)
{
	mapTexture.loadFromFile(resource.GetPathToMapImage());
	mapSprite.setTexture(mapTexture);	

	font.loadFromFile(resource.GetPathToFont());
	timeText.setFont(font);

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
	mapSprite.setOrigin(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);
	mapSprite.setPosition(mapSprite.getLocalBounds().width * mapScale / 2, mapSprite.getLocalBounds().height * mapScale / 2);

	circle.setRadius(mapScale * 0.2 * (float)mapProperties.GetSquareSize());
	circle.setOrigin(circle.getRadius(), circle.getRadius());

	square.setSize(sf::Vector2f(mapScale * mapProperties.GetSquareSize(), mapScale * mapProperties.GetSquareSize()));

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
				if (isWhiteActive)
					figuresSprites[to_underlying(selectedFigure)].setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + i * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (7 - j) * mapProperties.GetSquareSize());
				else
					figuresSprites[to_underlying(selectedFigure)].setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + (7 - i) * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + j * mapProperties.GetSquareSize());
				window->draw(figuresSprites[to_underlying(selectedFigure)]);
			}
		}
	}
}

void Drawer::ShowTimer(sf::Time time, Color activeColor)
{
	int seconds = time.asSeconds();
	if (seconds > 60)
	{
		if (activeColor == Color::White)
		{
			timeText.setFillColor(sf::Color::White);
			timeText.setOutlineThickness(1);
		}
		else
		{
			timeText.setFillColor(sf::Color::Black);
			timeText.setOutlineThickness(0);
		}
	}
	else
		timeText.setFillColor(sf::Color::Red);

	std::string timeString = std::to_string(seconds / 60) + ":";
	if (seconds % 60 < 10)
		timeString += "0";
	timeString += std::to_string(seconds % 60);

	timeText.setString(timeString);
	window->draw(timeText);
}

void Drawer::ShowActiveFigure(const Map& map, const Pos& chosenPos)
{
	if (isWhiteActive)
		square.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + chosenPos.GetX() * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (7 - chosenPos.GetY()) * mapProperties.GetSquareSize());
	else
		square.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + (7 - chosenPos.GetX()) * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + chosenPos.GetY() * mapProperties.GetSquareSize());
	window->draw(square);
}

void Drawer::RotateBoard()
{
	mapSprite.rotate(180);
	isWhiteActive = !isWhiteActive;
}

Pos* Drawer::TransformMousePosition(int mouseX, int mouseY) const
{
	return new Pos((mouseX - mapProperties.GetPlayAreaTopLeft().GetX()) / mapProperties.GetSquareSize(), 7 - (mouseY - mapProperties.GetPlayAreaTopLeft().GetY()) / mapProperties.GetSquareSize());
}

void Drawer::ShowPossibleMoves(const Map& map, const Pos& chosenFigure)
{
	for (std::vector<Pos>::const_iterator it = map.GetFigureAt(chosenFigure)->GetPossibleMoves().begin(); it != map.GetFigureAt(chosenFigure)->GetPossibleMoves().end(); ++it)
	{
		if (isWhiteActive)
			circle.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + (it->GetX() + 0.5) * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (7.5 - it->GetY()) * mapProperties.GetSquareSize());
		else
			circle.setPosition(mapProperties.GetPlayAreaTopLeft().GetX() + (7.5 - it->GetX()) * mapProperties.GetSquareSize(), mapProperties.GetPlayAreaTopLeft().GetY() + (it->GetY() + 0.5) * mapProperties.GetSquareSize());
		window->draw(circle);
	}
}

void Drawer::DisplayWindow()
{
	window->display();
}
