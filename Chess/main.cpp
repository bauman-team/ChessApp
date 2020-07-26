#include <SFML/Graphics.hpp>
#include "TwoPlayersGame.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");

	Resources res;
	FigureType sequence[NUM_OF_FIGURE_TYPES * 2] =
	{ King_black, Queen_black, Rook_black, Knight_black, Bishop_black, Pawn_black,
	  King_white, Queen_white, Rook_white, Knight_white, Bishop_white, Pawn_white };
	res.SetPathToMapImage("images/map.png");
	res.SetPathToFiguresImage("images/figures.png");
	res.SetSequenceOfFiguresInImage(sequence);
	res.SetRowsAndColsInFiguresImage(2, 6);

	Game* game = new TwoPlayersGame(&window, res);

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