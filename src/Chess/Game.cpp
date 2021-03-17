#include "Game.h"
#include <fstream>
#include <iostream>

Game::Game(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: drawer(window, resource, properties), gameGui(*window), status(GameStatus::Play)
{
	tgui::Button::Ptr exitButton = tgui::Button::create();
	exitButton->setText("Exit");
	exitButton->setEnabled(true);
	exitButton->setSize(drawer.GetMapProps().GetSideMenuWidth() * 0.5, 50);
	exitButton->setPosition((drawer.GetMapProps().GetGameWindowWidth() - drawer.GetMapProps().GetSideMenuWidth() / 2 - exitButton->getSize().x / 2),
		(drawer.GetMapProps().GetGameWindowHeight() - exitButton->getSize().y - 5));
	exitButton->connect(tgui::Signals::Button::Clicked, &Game::SetExitStatus, this);
	gameGui.add(exitButton);
	exitButton->setWidgetName("ExitButton");
	
	tgui::ScrollablePanel::Ptr panel = tgui::ScrollablePanel::create();
	panel->setPosition((drawer.GetMapProps().GetGameWindowWidth() - drawer.GetMapProps().GetSideMenuWidth() + 10), 0);
	panel->setSize(280, 600);
	gameGui.add(panel);
	panel->tgui::Widget::setWidgetName("ScrollablePanel");
}

volatile GameStatus Game::GetStatus() const
{
	return status;
}

void Game::Save() const
{
	std::vector<MoveInfo> movesHistory = map.GetMovesHistory();
	std::ofstream out("LastGame.log");
	Pos posBefore;
	Pos posAfter;
	out << "   \n";
	int numberOfMoves = movesHistory.size();
	for (int i = 0; i < numberOfMoves; ++i)
	{
		posBefore = movesHistory[i].GetPosBeforeMove();
		posAfter = movesHistory[i].GetPosAfterMove();
		if ((i == 0) || (Figure::GetFigureTypeColor(movesHistory[i].GetTypeActiveFigure()) != Figure::GetFigureTypeColor(movesHistory[i - 1].GetTypeActiveFigure())))
			out << static_cast<int>(posBefore.GetX()) << " " 
			<< static_cast<int>(posBefore.GetY()) << " -> "
			<< static_cast<int>(posAfter.GetX()) << " " 
			<< static_cast<int>(posAfter.GetY()) << "\n";
		else
			--numberOfMoves;
	}
	out.seekp(0, std::ios::beg);
	out << numberOfMoves;
	out.close();
}

void Game::HandleEvent(sf::Event& event)
{
	gameGui.handleEvent(event);
}

void Game::SetExitStatus()
{
	Save();
	status = GameStatus::Exit;
}

void Game::ActivateMenuSettings(Menu& menu)
{
	drawer.ResizeWindowForMenu(menu.GetMenuSize());
	menu.ActivateStartMenu();
}

void Game::UpdateSideMenu()
{
	auto info = map.GetMovesHistory();
	tgui::ScrollablePanel::Ptr panel = gameGui.get<tgui::ScrollablePanel>("ScrollablePanel");
	for (auto it = info.begin(); it != info.end(); ++it) // TODO: Rook? maybe only text 'r'
		if (!panel->get(std::to_string(it->GetNumOfMove()))) // TODO: Pawn to Queen
		{
			tgui::Label::Ptr labelFrom = tgui::Label::create(), labelTo = tgui::Label::create(), labelId = tgui::Label::create();
			std::string moveId = std::to_string(it->GetNumOfMove()) + ')';
			labelFrom->setText(it->GetPosBeforeMove().ToString());
			labelFrom->setTextSize(20);
			labelFrom->setPosition(moveId.length() * 14 + 50, 20);
			labelTo->setText(it->GetPosAfterMove().ToString());
			labelTo->setTextSize(20);
			labelTo->setPosition(moveId.length() * 14 + 120, 20);
			labelId->setText(moveId);
			labelId->setTextSize(20);
			labelId->setPosition(0, 20);
			tgui::Canvas::Ptr drawFrame = tgui::Canvas::create();
			drawFrame->setSize(tgui::Layout2d(260, 70));
			if (it->GetNumOfMove() != 1)
			{
				drawFrame->setPosition(0, gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + 50);
				labelFrom->setPosition(moveId.length() * 14 + 50, gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + 70);
				labelTo->setPosition(moveId.length() * 14 + 120, gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + 70);
				labelId->setPosition(0, gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + 70);
			}
			drawFrame->clear(tgui::Color(255, 255, 255));
			gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->add(drawFrame);
			sf::RectangleShape line(sf::Vector2f(20, 2));
			line.setFillColor(sf::Color::Black);
			line.setPosition(moveId.length() * 14 + 85, 32);
			sf::CircleShape triangle(7, 3);
			triangle.setFillColor(sf::Color::Black);
			triangle.rotate(90);
			triangle.setPosition(moveId.length() * 14 + 115, 26);
			tgui::Sprite sp[FIGURE_TYPES];
			for (int i = 0; i < FIGURE_TYPES; ++i)
			{
				sp[i].setTexture(drawer.figuresTextures[i]);
				sp[i].setSize(tgui::Vector2f(50, 50));
			}
			sp[static_cast<int>(it->GetTypeActiveFigure())].setPosition(moveId.length() * 14 - 5, 0);
			if (it->GetTypeEatenFigure() != FigureType::Empty)
				sp[static_cast<int>(it->GetTypeEatenFigure())].setPosition(moveId.length() * 14 + 150, 0);
			drawFrame->draw(sp[static_cast<int>(it->GetTypeActiveFigure())]);
			if (it->GetTypeEatenFigure() != FigureType::Empty)
				drawFrame->draw(sp[static_cast<int>(it->GetTypeEatenFigure())]);
			drawFrame->draw(triangle);
			drawFrame->draw(line);
			gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->add(labelFrom);
			gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->add(labelTo);
			gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->add(labelId);
			drawFrame->setWidgetName(std::to_string(it->GetNumOfMove())); // TODO: add names to other widgets
		}
}