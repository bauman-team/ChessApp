#include <SFML/Graphics.hpp>
#include "TwoPlayersGame.h"

int main()
{
	int windowSize = 920;
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Chess");

	Resources res;
	res.SetMapImage("images/map.png");
	res.SetFigureImage(FigureType::King_black, "images/King_black.png");
	res.SetFigureImage(FigureType::Queen_black, "images/Queen_black.png");
	res.SetFigureImage(FigureType::Rook_black, "images/Rook_black.png");
	res.SetFigureImage(FigureType::Bishop_black, "images/Bishop_black.png");
	res.SetFigureImage(FigureType::Knight_black, "images/Knight_black.png");
	res.SetFigureImage(FigureType::Pawn_black, "images/Pawn_black.png");
	res.SetFigureImage(FigureType::King_white, "images/King_white.png");
	res.SetFigureImage(FigureType::Queen_white, "images/Queen_white.png");
	res.SetFigureImage(FigureType::Rook_white, "images/Rook_white.png");
	res.SetFigureImage(FigureType::Bishop_white, "images/Bishop_white.png");
	res.SetFigureImage(FigureType::Knight_white, "images/Knight_white.png");
	res.SetFigureImage(FigureType::Pawn_white, "images/Pawn_white.png");
	res.SetFont("fonts/calibri.ttf");

	MapProperties prop;
	prop.SetPlayAreaTopLeft(Pos(60, 60));
	prop.SetSquareSize(100);

	Game* game = new TwoPlayersGame(&window, res, prop);

	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					game->SetPlayerChosenCell(mousePos.x, mousePos.y);
				}
				break;
			}
		}
		game->StartGame();
		window.display();
	}
	return 0;
}