#include <SFML/Graphics.hpp>
#include "TwoPlayersGame.h"
#include "PlayerWithAIGame.h"
#include "Menu.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(700, 550), "Chess");

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
	prop.SetPlayAreaTopLeft(60, 60);
	prop.SetSquareSize(100);
	prop.SetGameWindowHeight(920);
	prop.SetSideMenuWidth(300);

	Game* game = new TwoPlayersGame(&window, res, prop);
	//Game* game = new PlayerWithAIGame(&window, res, prop);
	Menu menu(window, game, "form.txt");

	std::thread *thSetCell = nullptr;
	bool thSetCellIsFinished = true;
	
	//((PlayerWithAIGame*)game)->output();
	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed:
				game->Save();
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if ((game->GetStatus() == GameStatus::Play || game->GetStatus() == GameStatus::Shah) && event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					if (thSetCellIsFinished)
					{
						thSetCellIsFinished = false;
						thSetCell = new std::thread([game, mousePos, &thSetCellIsFinished]() { thSetCellIsFinished = game->SetPlayerChosenCell(mousePos.x, mousePos.y); });
						thSetCell->detach();
					}
				}
				break;
			case sf::Event::KeyPressed:
				if (game->GetStatus() == GameStatus::Menu && event.key.code == sf::Keyboard::Enter)
					menu.PrestartChecks();
				break;
			}
			if (game->GetStatus() == GameStatus::Menu)
				menu.HandleEvent(event);
		}

		switch (game->GetStatus())
		{
		case GameStatus::Menu:
			menu.Show();
			break;
		case GameStatus::Play:
			game->Show();
			break;
		default:
			game->Show();
			break;
		}
		window.display();
	}
	delete game;
	return EXIT_SUCCESS;
}