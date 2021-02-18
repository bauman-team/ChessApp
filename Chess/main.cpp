/* Bugs:
	1) add transformation pawn to other figures (not only queen)
	2) fix scaling window
*/

#include "PlayerWithAIGame.h"
#include "Game.h"

extern std::mutex mut4;


int main()
{
	setlocale(0, "ru");
	sf::RenderWindow window;
	window.create(sf::VideoMode(700, 550), "Chess");
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

	Game* game = nullptr;
	Menu menu(window, "form.txt");

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
				if (game)
					game->Save();
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (game && (game->GetStatus() == GameStatus::Play || game->GetStatus() == GameStatus::Shah) && event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					if (thSetCellIsFinished)
					{
						thSetCellIsFinished = false;
						Game* ptrGame = game;
						thSetCell = new std::thread([&ptrGame, &thSetCellIsFinished, mousePos]() {
							ptrGame->SetPosition(mousePos.x, mousePos.y);
							thSetCellIsFinished = true; });
						thSetCell->detach();
					}
				}
				break;
			case sf::Event::KeyPressed:
				if (!game && (event.key.code == sf::Keyboard::Enter)) // button Enter press handler (start game)
				{
					if (menu.CanStartGame())
					{
						InitialData inputValues = menu.GetInitialData();
						if (inputValues.mode == GameMode::TwoPlayers)
							game = new TwoPlayersGame(&window, res, prop);
						else
							game = new PlayerWithAIGame(&window, res, prop);
						game->SetPlayers(inputValues.firstName, inputValues.secondName, inputValues.time);
						game->StartGame();
						if (typeid(*game) == typeid(PlayerWithAIGame))
						{
							if (!((PlayerWithAIGame*)game)->GetIsPlayerMoveFirst()) // if the first move of the bot
							{
								while (!thSetCellIsFinished) sf::sleep(sf::seconds(0.1));
								thSetCellIsFinished = false;
								Game* ptrGame = game;
								thSetCell = new std::thread([&ptrGame, &thSetCellIsFinished]() {
									ptrGame->ChangeActivePlayer();
									thSetCellIsFinished = true; });
								thSetCell->detach();
							}
						}
					}
				}
				else if (!game && (event.key.code == sf::Keyboard::Escape))
				{
					if (game)
						game->Save();
					window.close();
				}
				break;
			}
			if (game)
			{
				game->HandleEvent(event); // for side menu
				if (game->GetStatus() == GameStatus::Exit) // button exit is clicked (was opened start menu)
				{
					game->ActivateMenuSettings(menu);
					if (thSetCellIsFinished)
						delete game; // TODO: ??? MEMORY LEAK game delete (why memory clear, when theard don't finished?) 
					game = nullptr;
				}
			}
			if (!game)
				menu.HandleEvent(event); // menu handler
		}
		if (!game && menu.NeedStartGame()) // after click "start" button, handler start game
		{
			InitialData inputValues = menu.GetInitialData();
			if (inputValues.mode == GameMode::TwoPlayers)
				game = new TwoPlayersGame(&window, res, prop);
			else
				game = new PlayerWithAIGame(&window, res, prop);
			game->SetPlayers(inputValues.firstName, inputValues.secondName, inputValues.time);
			game->StartGame();
			if (typeid(*game) == typeid(PlayerWithAIGame))
			{
				if (!((PlayerWithAIGame*)game)->GetIsPlayerMoveFirst()) // if the first move of the bot
				{
					while (!thSetCellIsFinished) sf::sleep(sf::seconds(0.1));
					thSetCellIsFinished = false;
					Game* ptrGame = game;
					thSetCell = new std::thread([&ptrGame, &thSetCellIsFinished]() {
						ptrGame->ChangeActivePlayer();
						thSetCellIsFinished = true; });
					thSetCell->detach();
				}
			}
		}
		(game) ? game->Show() : menu.Show();
		window.display();
	}
	delete game;
	return EXIT_SUCCESS;
}