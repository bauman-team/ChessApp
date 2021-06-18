#include "Drawer.h"

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resources, const MapProperties& properties, GameSet SetExitStatus, Game* game)
	: sideMenu{ _window, properties, SetExitStatus, game }, window{ _window }, mapProps{ properties }, windowScale{ 1.0, 1.0 },
	selectedPosFrom{ Pos::NULL_POS }, selectedPosTo{ Pos::NULL_POS }, selectedFigure{ FigureType::Empty }
{
	window->setFramerateLimit(60);

	SetResources(resources);
	sideMenu.SetSprites(figuresSprites);
	SetScale();
	isWhiteActive = true;

	circle.setFillColor(sf::Color{ 100, 200, 10, 200 }); // green

	// timer text styles
	timeText.setCharacterSize(28);
	timeText.setPosition(5, 7);
	timeText.setStyle(sf::Text::Bold);

	// active rectangle styles
	square.setFillColor(sf::Color{ 0, 0, 0, 50 });
	square.setOutlineThickness(4);
	square.setOutlineColor(sf::Color::Yellow);
}

sf::Vector2u Drawer::GetSideMenuSize() const
{
	return sf::Vector2u{ mapProps.GetSideMenuWidth(), mapProps.GetGameWindowHeight() };
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

	for (auto i = 0; i < FIGURE_TYPES; ++i)
		figuresTextures[i].loadFromFile(resources.GetPathToFigure(static_cast<FigureType>(i)));

	for (auto i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setTexture(figuresTextures[i]);
}

void Drawer::SetScale()
{
	auto mapScale{ static_cast<float>(mapProps.GetGameWindowHeight() / mapTexture.getSize().x) };
	auto figureScale{ mapScale * mapProps.GetSquareSize() / figuresTextures[0].getSize().x };

	mapSprite.setScale(mapScale, mapScale); // set the mapSprite size to the window size
	mapSprite.setOrigin(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);
	mapSprite.setPosition(mapSprite.getLocalBounds().width * mapScale / 2, mapSprite.getLocalBounds().height * mapScale / 2);

	auto radius{ 0.2f }; // relative to sqaure size
	circle.setRadius(radius * mapScale * mapProps.GetSquareSize());
	circle.setOrigin(circle.getRadius(), circle.getRadius());

	square.setSize(sf::Vector2f{ mapScale * mapProps.GetSquareSize(), mapScale * mapProps.GetSquareSize() });

	mapProps.SetScale(mapScale);

	for (auto i = 0; i < FIGURE_TYPES; ++i)
		figuresSprites[i].setScale(figureScale, figureScale);
}

void Drawer::ShowMap(const Map& map)
{
	window->draw(mapSprite);
	const auto squareSize{ mapProps.GetSquareSize() };
	for (uint8_t j = 0; j != 8; ++j)
	{
		for (uint8_t i = 0; i != 8; ++i)
		{
			auto position{ Pos{ i, j } };
			auto figureType{ map.GetFigureType(position) };
			if (figureType != FigureType::Empty &&
				(!selectedPosTo.IsValid() || selectedPosTo.IsValid() && selectedPosFrom != position && selectedPosTo != position))
			{
				auto coord{ (isWhiteActive) ? Pos{ i, static_cast<uint8_t>(7 - j) } : Pos{ static_cast<uint8_t>(7 - i), j } };
				if (!selectedPosTo.IsValid())
					figuresSprites[toUType(figureType)].setColor(sf::Color{ 255, 255, 255, 255 });
				else
					figuresSprites[toUType(figureType)].setColor(sf::Color{ 255, 255, 255, 100 }); // transparent figure
				figuresSprites[toUType(figureType)].setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize,
						mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize);
				window->draw(figuresSprites[toUType(figureType)]);
			}
			else if (selectedPosTo == position)
			{
				auto coord{ (isWhiteActive) ? Pos{ i, static_cast<uint8_t>(7 - j) } : Pos{ static_cast<uint8_t>(7 - i), j } };
				figureType = selectedFigure.front();
				figuresSprites[toUType(figureType)].setColor(sf::Color{ 255, 255, 255, 255 });
				figuresSprites[toUType(figureType)].setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize,
					mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize);
				window->draw(figuresSprites[toUType(figureType)]);
			}
		}
	}
	if (selectedPosTo.IsValid())
		ShowTransformFigure();
}

void Drawer::ShowTimer(sf::Time time)
{
	auto seconds{ static_cast<int>(time.asSeconds()) };
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

	auto timeString{ std::to_string(seconds / 60) + ":" };
	if (seconds % 60 < 10)
		timeString += "0";
	timeString += std::to_string(seconds % 60);

	if (timeText.getString() != timeString)
		timeText.setString(timeString);
	window->draw(timeText);
}

void Drawer::ShowActiveFigure(const Pos& chosenPos, const Map& map)
{
	auto coord{ isWhiteActive ? Pos{ chosenPos.GetX(), static_cast<uint8_t>(7 - chosenPos.GetY()) } : Pos{ static_cast<uint8_t>(7 - chosenPos.GetX()), chosenPos.GetY() } };
	square.setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * mapProps.GetSquareSize(),
					   mapProps.GetPlayAreaTopLeftY() + coord.GetY() * mapProps.GetSquareSize());
	window->draw(square);
}

void Drawer::RotateBoard()
{
	mapSprite.rotate(180);
	isWhiteActive = !isWhiteActive;
}

void Drawer::WindowIsResized()
{
	windowScale.x = static_cast<float>(window->getSize().x) / mapProps.GetGameWindowWidth();
	windowScale.y = static_cast<float>(window->getSize().y) / mapProps.GetGameWindowHeight();
}

void Drawer::ResizeWindowForMenu(const sf::Vector2f& menuSize)
{
	window->setPosition(sf::Vector2i{ static_cast<int>((sf::VideoMode::getDesktopMode().width - menuSize.x) / 2),
									 static_cast<int>((sf::VideoMode::getDesktopMode().height - menuSize.y) / 2) });
	window->setSize(static_cast<sf::Vector2u>(menuSize));
}

void Drawer::ShowSideMenu(const Map& map)
{
	sideMenu.UpdateSideMenu(map.GetMovesLog());
}

Pos Drawer::TransformMousePosition(int mouseX, int mouseY) const
{
	if (mouseX >= mapProps.GetPlayAreaTopLeftX() * windowScale.x && mouseX <= (mapProps.GetPlayAreaTopLeftX() + mapProps.GetSquareSize() * 8) * windowScale.x &&
		mouseY >= mapProps.GetPlayAreaTopLeftY() * windowScale.y && mouseY <= (mapProps.GetPlayAreaTopLeftY() + mapProps.GetSquareSize() * 8) * windowScale.y)
	{
		auto coordX{ static_cast<uint8_t>((mouseX - mapProps.GetPlayAreaTopLeftX() * windowScale.x) / mapProps.GetSquareSize() / windowScale.x) };
		auto coordY{ static_cast<uint8_t>(8 - (mouseY - mapProps.GetPlayAreaTopLeftY() * windowScale.y) / mapProps.GetSquareSize() / windowScale.y) };
		return Pos{ coordX, coordY };
	}
	return {};
}

void Drawer::SetSelectedPositionTo(const Pos position, const Map& map)
{
	auto moves{ map.GetAllPossibleMoves() };
	auto it{ moves.cbegin() }, end{ moves.cend() };
	for (; it != end; ++it)
		if (((*it)[0].GetPosBeforeMove() == selectedPosFrom && (*it)[0].GetPosAfterMove() == position)) // search move in the vector of possible moves
			selectedFigure.push_back((*it)[1].GetTypeActiveFigure());
	selectedPosTo = position;
}

void Drawer::SetSelectedFigure(int mouseX, int mouseY)
{
	const auto squareSize{ mapProps.GetSquareSize() };
	auto coord{ (isWhiteActive) ? Pos{ selectedPosTo.GetX(), static_cast<uint8_t>(7 - selectedPosTo.GetY()) } : Pos{ static_cast<uint8_t>(7 - selectedPosTo.GetX()), selectedPosTo.GetY() } };
	if (mouseX < mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize - 2 && mouseX > mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize - 44 &&
		mouseY < mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 68 && mouseY > mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 24)
	{
		
		selectedFigure.push_front(selectedFigure.back());
		selectedFigure.pop_back();
	}
	else if (mouseX < mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize + 146 && mouseX > mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize + 102 &&
		mouseY < mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 68 && mouseY > mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 24)
	{
		selectedFigure.push_back(selectedFigure.front());
		selectedFigure.pop_front();
	}
}

void Drawer::ClearSelect()
{ 
	selectedPosFrom = Pos::NULL_POS;
	selectedPosTo = Pos::NULL_POS;
	selectedFigure.clear();
}

void Drawer::ShowPossibleMoves(const Pos& chosenPos, const Map& map)
{
	auto possiblePositions{ map.GetPossibleMovesFrom(chosenPos) };
	auto posItr{ possiblePositions.begin() };
	auto end{ possiblePositions.end() };

	auto offset{ 0.5f }; // to draw circles in the center of squares
	for (; posItr != end; ++posItr)
	{
		auto coord{ (isWhiteActive) ? Pos{ posItr->GetX(), static_cast<uint8_t>(7 - posItr->GetY()) } : Pos{ static_cast<uint8_t>(7 - posItr->GetX()), posItr->GetY() } };
		circle.setPosition(mapProps.GetPlayAreaTopLeftX() + (coord.GetX() + offset) * mapProps.GetSquareSize(),
						   mapProps.GetPlayAreaTopLeftY() + (coord.GetY() + offset) * mapProps.GetSquareSize());
		window->draw(circle);
	}
}

void Drawer::ShowTransformFigure()
{
	const auto squareSize{ mapProps.GetSquareSize() };
	auto coord{ (isWhiteActive) ? Pos{ selectedPosTo.GetX(), static_cast<uint8_t>(7 - selectedPosTo.GetY()) } : Pos{ static_cast<uint8_t>(7 - selectedPosTo.GetX()), selectedPosTo.GetY() } };
	sf::CircleShape triangle1{ 25, 3 }, triangle2{ 25, 3 }, triangle3{ 29, 3 }, triangle4{ 29, 3 };
	triangle3.setFillColor(sf::Color::Black);
	triangle3.rotate(270);
	triangle3.setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize - 43, mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 74);
	triangle1.setFillColor(sf::Color::White);
	triangle1.rotate(270);
	triangle1.setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize - 39, mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 70);
	triangle4.setFillColor(sf::Color::Black);
	triangle4.rotate(90);
	triangle4.setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize + 143, mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 16);
	triangle2.setFillColor(sf::Color::White);
	triangle2.rotate(90);
	triangle2.setPosition(mapProps.GetPlayAreaTopLeftX() + coord.GetX() * squareSize + 139, mapProps.GetPlayAreaTopLeftY() + coord.GetY() * squareSize + 20);
	window->draw(triangle3);
	window->draw(triangle1);
	window->draw(triangle4);
	window->draw(triangle2);
}

void Drawer::HandleEvent(sf::Event& event)
{
	sideMenu.HandleEvent(event);
}

void Drawer::DisplayWindow()
{
	window->display();
}