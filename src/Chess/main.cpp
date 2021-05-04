#include "PlayerWithAIGame.h"
#include "Game.h"
std::mutex mut4;


int main()
{
	srand(time(NULL));
	setlocale(0, "ru");
	sf::RenderWindow window;
	window.create(sf::VideoMode{ 700, 550 }, "Chess");
	Resources res;
	res.SetMapImage("../res/images/map.png");
	res.SetFigureImage(FigureType::King_black, "../res/images/King_black.png");
	res.SetFigureImage(FigureType::Queen_black, "../res/images/Queen_black.png");
	res.SetFigureImage(FigureType::Rook_black, "../res/images/Rook_black.png");
	res.SetFigureImage(FigureType::Bishop_black, "../res/images/Bishop_black.png");
	res.SetFigureImage(FigureType::Knight_black, "../res/images/Knight_black.png");
	res.SetFigureImage(FigureType::Pawn_black, "../res/images/Pawn_black.png");
	res.SetFigureImage(FigureType::King_white, "../res/images/King_white.png");
	res.SetFigureImage(FigureType::Queen_white, "../res/images/Queen_white.png");
	res.SetFigureImage(FigureType::Rook_white, "../res/images/Rook_white.png");
	res.SetFigureImage(FigureType::Bishop_white, "../res/images/Bishop_white.png");
	res.SetFigureImage(FigureType::Knight_white, "../res/images/Knight_white.png");
	res.SetFigureImage(FigureType::Pawn_white, "../res/images/Pawn_white.png");
	res.SetFont("../res/fonts/calibri.ttf");

	MapProperties prop;
	prop.SetPlayAreaTopLeft(60, 60);
	prop.SetSquareSize(100);
	prop.SetGameWindowHeight(920);
	prop.SetSideMenuWidth(300);

	sf::Color backgroundGameColor{ 208, 167, 130, 255 };

	Game* game = nullptr; // TODO: goto stack ???
	Menu menu{ window, "../res/form.txt" };

	std::thread thSetCell{ };
	#ifdef _WIN32
	std::thread thDraw;
	#endif
	auto thSetCellIsFinished{ true }; // TODO: because main thread is not joinable???

	//((PlayerWithAIGame*)game)->output();
	while(sf::sleep(sf::seconds(0.05)), window.isOpen())
	{
		if (!game)
			window.clear(sf::Color::White); // TODO: select back color in menu/game
		#ifndef _WIN32
		else
			window.clear(backgroundGameColor);
		#endif
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
			case sf::Event::Closed:
				if (game)
					game->Save();
				#ifdef _WIN32
				mut4.lock();
				#endif
				window.close();
				#ifdef _WIN32
				mut4.unlock();
				#endif
				break;
			case sf::Event::MouseButtonPressed:
				if (game && (game->GetStatus() == GameStatus::Play || game->GetStatus() == GameStatus::Shah) && event.mouseButton.button == sf::Mouse::Left)
				{
					auto mousePos{ sf::Mouse::getPosition(window) };
					if (thSetCellIsFinished)
					{
						thSetCellIsFinished = false;
						thSetCell = std::thread{ [&game, &thSetCellIsFinished, mousePos]() {
							game->SetPosition(mousePos.x, mousePos.y);
							thSetCellIsFinished = true; } };
						thSetCell.detach();
					}
				}
				break;
			case sf::Event::KeyPressed:
				if (!game && (event.key.code == sf::Keyboard::Enter)) // button Enter press handler (start game)
				{
					if (menu.CanStartGame())
					{
						auto inputValues{ menu.GetInitialData() };
						if (inputValues.mode == GameMode::TwoPlayers)
							game = new TwoPlayersGame{ &window, res, prop };
						else
							game = new PlayerWithAIGame{ &window, res, prop };
						game->SetPlayers(inputValues.firstName, inputValues.secondName, inputValues.time);
						game->StartGame();
						#ifdef _WIN32
						window.setActive(false);
						thDraw = std::thread{ [&game, &window]() {
							while (sf::sleep(sf::seconds(0.05)), mut4.lock(), game && (window.setActive(true), window.isOpen()))
							{
								window.clear(backgroundGameColor);
								if (game)
									game->Show();
								window.display();
								window.setActive(false);
								mut4.unlock();
							}
							window.setActive(false);
							mut4.unlock();
							} };
						thDraw.detach();
						#endif
						if (typeid(*game) == typeid(PlayerWithAIGame))
						{
							if (!((PlayerWithAIGame*)game)->GetIsPlayerMoveFirst()) // if the first move of the bot
							{
								while (!thSetCellIsFinished) sf::sleep(sf::seconds(0.1));
								thSetCellIsFinished = false;
								thSetCell = std::thread{ [&game, &thSetCellIsFinished]() {
									game->ChangeActivePlayer();
									thSetCellIsFinished = true; } };
								thSetCell.detach();
							}
						}
					}
				}
				else if (!game && (event.key.code == sf::Keyboard::Escape))
				{
					if (game)
						game->Save();
					#ifdef _WIN32
					mut4.lock();
					#endif
					window.close();
					#ifdef _WIN32
					mut4.unlock();
					#endif
				}
				break;
			}
			if (game)
			{
				game->HandleEvent(event); // for side menu
				if (game->GetStatus() == GameStatus::Exit) // button exit is clicked (was opened start menu)
				{
					#ifdef _WIN32
					mut4.lock();
					#endif
					game->ActivateMenuSettings(menu);
					if (thSetCellIsFinished)
						delete game; // TODO: ??? MEMORY LEAK game delete (why memory clear, when theard don't finished?) 
					game = nullptr;
					#ifdef _WIN32
					mut4.unlock();
					#endif
				}
			}
			if (!game)
				menu.HandleEvent(event); // menu handler
		}
		if (!game && menu.NeedStartGame()) // after click "start" button, handler start game
		{
			auto inputValues{ menu.GetInitialData() };
			if (inputValues.mode == GameMode::TwoPlayers)
				game = new TwoPlayersGame{ &window, res, prop };
			else
				game = new PlayerWithAIGame{ &window, res, prop };
			game->SetPlayers(inputValues.firstName, inputValues.secondName, inputValues.time);
			game->StartGame();
			#ifdef _WIN32
			window.setActive(false);
			thDraw = std::thread{ [&game, &window]() {
				while (sf::sleep(sf::seconds(0.05)), mut4.lock(), game && (window.setActive(true), window.isOpen()))
				{
					window.clear(backgroundGameColor);
					if (game)
						game->Show();
					window.display();
					window.setActive(false);
					mut4.unlock();
				}
				window.setActive(false);
				mut4.unlock();
				} };
			thDraw.detach();
			#endif
			if (typeid(*game) == typeid(PlayerWithAIGame))
			{
				if (!((PlayerWithAIGame*)game)->GetIsPlayerMoveFirst()) // if the first move of the bot
				{
					while (!thSetCellIsFinished) sf::sleep(sf::seconds(0.1));
					thSetCellIsFinished = false;
					thSetCell = std::thread{ [&game, &thSetCellIsFinished]() {
						game->ChangeActivePlayer();
						thSetCellIsFinished = true; } };
					thSetCell.detach();
				}
			}
		}
		#ifndef _WIN32
		(game) ? game->Show() : menu.Show();
		window.display();
		#endif
		#ifdef _WIN32
		if (!game)
		{
			mut4.lock();
			menu.Show();
			window.display();
			mut4.unlock();
		}
		#endif
	}
	delete game;
	return EXIT_SUCCESS;
}