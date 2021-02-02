#include "Drawer.h"

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resources, const MapProperties& properties)
	: window(_window), mapProps(properties)
{
	ResizeWindowForGame();
	window->setFramerateLimit(60);

	SetResources(resources);
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

sf::Vector2u Drawer::GetSideMenuSize() const
{
	return sf::Vector2u(mapProps.GetSideMenuWidth(), mapProps.GetGameWindowHeight());
}

int Drawer::GetSizeMenuTopLeftX() const
{
	return mapProps.GetGameWindowHeight(); // because board height = board width
}

MapProperties Drawer::GetMapProps() const
{
	return mapProps;
}

void Drawer::SetResources(const Resources& resources)
{
	mapTexture.loadFromFile(resources.GetPathToMapImage());
	mapSprite.setTexture(mapTexture);	

	font.loadFromFile(resources.GetPathToFont());
	timeText.setFont(font);

	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresTextures[i].loadFromFile(resources.GetPathToFigure((FigureType)i));

	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setTexture(figuresTextures[i]);
}

void Drawer::SetScale()
{
	float mapScale = (float)mapProps.GetGameWindowHeight() / mapTexture.getSize().x;
	float figureScale = mapScale * mapProps.GetSquareSize() / figuresTextures[0].getSize().x;

	mapSprite.setScale(mapScale, mapScale); // set the mapSprite size to the window size
	mapSprite.setOrigin(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);
	mapSprite.setPosition(mapSprite.getLocalBounds().width * mapScale / 2, mapSprite.getLocalBounds().height * mapScale / 2);

	float radius = 0.2; // relative to sqaure size
	circle.setRadius(radius * mapScale * mapProps.GetSquareSize());
	circle.setOrigin(circle.getRadius(), circle.getRadius());

	square.setSize(sf::Vector2f(mapScale * mapProps.GetSquareSize(), mapScale * mapProps.GetSquareSize()));

	mapProps.SetScale(mapScale);

	for (int i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setScale(figureScale, figureScale);
}

void Drawer::ShowMap(const Map& map)
{
	window->draw(mapSprite);
	const uint8_t squareSize = mapProps.GetSquareSize();
	for (int j = 0; j != 8; ++j)
	{
		for (int i = 0; i != 8; ++i)
		{
			FigureType figureType = map.GetFigureType(Pos(i, j));
			if (figureType != FigureType::Empty)
			{
				Pos coord = (isWhiteActive) ? Pos(i, 7 - j) : Pos(7 - i, j);
				figuresSprites[static_cast<int>(figureType)].setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize,
																			 mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize);
				window->draw(figuresSprites[static_cast<int>(figureType)]);
			}
		}
	}
}

void Drawer::ShowTimer(sf::Time time)
{
	int seconds = time.asSeconds();
	if (seconds > 60)
	{
		if (isWhiteActive)
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

void Drawer::ShowActiveFigure(const Pos& chosenPos, const Map& map)
{
	Pos coord = (isWhiteActive) ? Pos(chosenPos.GetX(), 7 - chosenPos.GetY()) : Pos(7 - chosenPos.GetX(), chosenPos.GetY());
	square.setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * mapProps.GetSquareSize(),
					   mapProps.GetPlayAreaTopLeftY() + coord.GetY() * mapProps.GetSquareSize());
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
	window->create(sf::VideoMode(mapProps.GetGameWindowWidth(), mapProps.GetGameWindowHeight()), "Chess");
}

void Drawer::ResizeWindowForMenu(const sf::Vector2f& menuSize)
{
	window->setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - menuSize.x) / 2, 
									 (sf::VideoMode::getDesktopMode().height - menuSize.y) / 2));
	window->setSize((sf::Vector2u)menuSize);
}

void Drawer::ShowGuiElems(tgui::Gui& gui)
{
	gui.draw();
}

Pos Drawer::TransformMousePosition(int mouseX, int mouseY) const
{
	if (mouseX >= mapProps.GetPlayAreaTopLeftX() && mouseX <= mapProps.GetPlayAreaTopLeftX() + mapProps.GetSquareSize() * 8 &&
		mouseY >= mapProps.GetPlayAreaTopLeftY() && mouseY <= mapProps.GetPlayAreaTopLeftY() + mapProps.GetSquareSize() * 8)
	{
		int coordX = (mouseX - mapProps.GetPlayAreaTopLeftX()) / mapProps.GetSquareSize();
		int coordY = 7 - (mouseY - mapProps.GetPlayAreaTopLeftY()) / mapProps.GetSquareSize();
		return Pos(coordX, coordY);
	}
	return Pos::NULL_POS;
}

void Drawer::ShowPossibleMoves(const Pos& chosenPos, const Map& map)
{
	std::vector<Pos> possiblePositions = map.GetPossibleMovesFrom(chosenPos);
	std::vector<Pos>::iterator posItr = possiblePositions.begin();
	std::vector<Pos>::iterator end = possiblePositions.end();

	float offset = 0.5; // to draw circles in the center of squares
	for (; posItr != end; ++posItr)
	{
		Pos coord = (isWhiteActive) ? Pos(posItr->GetX(), 7 - posItr->GetY()) : Pos(7 - posItr->GetX(), posItr->GetY());
		circle.setPosition(mapProps.GetPlayAreaTopLeftX() + (coord.GetX() + offset) * mapProps.GetSquareSize(),
						   mapProps.GetPlayAreaTopLeftY() + (coord.GetY() + offset) * mapProps.GetSquareSize());
		window->draw(circle);
	}
}

void Drawer::DisplayWindow()
{
	window->display();
}