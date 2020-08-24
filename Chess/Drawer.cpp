#include <SFML/Graphics.hpp>
#include "Drawer.h"

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resource, const MapProperties& properties)
	: window(_window), mapProperties(properties)
{
	window->setFramerateLimit(60);

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
	float mapScale = (float)mapProperties.GetGameWindowSize() / mapTexture.getSize().x;
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
			FigureType selectedFigure = map.GetFigureType(Pos(i, j));
			if (selectedFigure != FigureType::Empty)
			{
				Pos coeff = (isWhiteActive) ? Pos(i, 7 - j) : Pos(7 - i, j);
				figuresSprites[to_underlying(selectedFigure)].setPosition(mapProperties.GetPlayAreaTopLeftX() + coeff.GetX() * mapProperties.GetSquareSize(),
																		  mapProperties.GetPlayAreaTopLeftY() + coeff.GetY() * mapProperties.GetSquareSize());
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

	if (timeText.getString() != timeString)
		timeText.setString(timeString);
	window->draw(timeText);
}

void Drawer::ShowActiveFigure(const Map& map, const Pos& chosenPos)
{
	Pos coeff = (isWhiteActive) ? Pos(chosenPos.GetX(), 7 - chosenPos.GetY()) : Pos(7 - chosenPos.GetX(), chosenPos.GetY());
	square.setPosition(mapProperties.GetPlayAreaTopLeftX() + coeff.GetX() * mapProperties.GetSquareSize(), 
					   mapProperties.GetPlayAreaTopLeftY() + coeff.GetY() * mapProperties.GetSquareSize());
	window->draw(square);
}

void Drawer::RotateBoard()
{
	mapSprite.rotate(180);
	isWhiteActive = !isWhiteActive;
}

void Drawer::ResizeWindowForGame()
{
	// using create() because setSize() works incorrectly
	window->create(sf::VideoMode(mapProperties.GetGameWindowSize(), mapProperties.GetGameWindowSize()), "Chess");
}

Pos* Drawer::TransformMousePosition(int mouseX, int mouseY) const
{
	if (mouseX >= mapProperties.GetPlayAreaTopLeftX() && mouseX <= mapProperties.GetPlayAreaTopLeftX() + mapProperties.GetSquareSize() * 8 &&
		mouseY >= mapProperties.GetPlayAreaTopLeftY() && mouseY <= mapProperties.GetPlayAreaTopLeftY() + mapProperties.GetSquareSize() * 8)
	{
		int coordX = (mouseX - mapProperties.GetPlayAreaTopLeftX()) / mapProperties.GetSquareSize();
		int coordY = 7 - (mouseY - mapProperties.GetPlayAreaTopLeftY()) / mapProperties.GetSquareSize();
		return new Pos(coordX, coordY);
	}
	return nullptr;
}

void Drawer::ShowPossibleMoves(const Map& map, const Pos& selectedFigure)
{
	const std::vector<Pos>* possiblePositions = map.GetPossibleMoves(selectedFigure);
	if (possiblePositions)
	{
		std::vector<Pos>::const_iterator it = possiblePositions->begin();
		std::vector<Pos>::const_iterator end = possiblePositions->end();
		for (; it != end; ++it)
		{
			Pos coeff = (isWhiteActive) ? Pos(it->GetX(), 7 - it->GetY()) : Pos(7 - it->GetX(), it->GetY());
			circle.setPosition(mapProperties.GetPlayAreaTopLeftX() + (coeff.GetX() + 0.5) * mapProperties.GetSquareSize(),
				mapProperties.GetPlayAreaTopLeftY() + (coeff.GetY() + 0.5) * mapProperties.GetSquareSize());
			window->draw(circle);
		}
	}
}

void Drawer::DisplayWindow()
{
	window->display();
}
