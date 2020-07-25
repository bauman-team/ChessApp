#include <SFML/Graphics.hpp>
#include "TwoPlayersGame.h"
#include "Game.h"

int main()
{
	
	sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");

	Resources res;
	res.SetPathToMapImage("images/map.jpg");
	res.SetPathToFiguresImage("images/figures.png");

	Game* game = new TwoPlayersGame(&window, res);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::White);
		game->StartGame();
		window.display();
	}
	return 0;
}