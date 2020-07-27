#include <SFML/Graphics.hpp>
#include "TwoPlayersGame.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(920, 920), "Chess");

	Resources res;
	res.SetMapImage("images/map.png");
	res.SetFigureImage(King_black, "images/King_black.png");
	res.SetFigureImage(Queen_black, "images/Queen_black.png");
	res.SetFigureImage(Rook_black, "images/Rook_black.png");
	res.SetFigureImage(Bishop_black, "images/Bishop_black.png");
	res.SetFigureImage(Knight_black, "images/Knight_black.png");
	res.SetFigureImage(Pawn_black, "images/Pawn_black.png");
	res.SetFigureImage(King_white, "images/King_white.png");
	res.SetFigureImage(Queen_white, "images/Queen_white.png");
	res.SetFigureImage(Rook_white, "images/Rook_white.png");
	res.SetFigureImage(Bishop_white, "images/Bishop_white.png");
	res.SetFigureImage(Knight_white, "images/Knight_white.png");
	res.SetFigureImage(Pawn_white, "images/Pawn_white.png");

	MapProperties prop;
	prop.SetPlayAreaTopLeft(Pos(60, 60));
	prop.SetSquareSize(100);

	Game* game = new TwoPlayersGame(&window, res, prop);

	while (window.isOpen())
	{
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
					//TODO: get coordinates
				}
				break;
			}
		}
		window.clear(sf::Color::White);
		game->StartGame();
		window.display();
	}
	return 0;
}