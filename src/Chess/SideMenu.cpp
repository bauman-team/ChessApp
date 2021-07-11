#include "SideMenu.h"
#include "Game.h"

SideMenu::SideMenu(sf::RenderWindow* window, const MapProperties& properties, GameMode mode, GameSet SetExitStatus, GameSet MakeUndoMove, Game* game)
	: gameGui{ (ResizeWindowForGame(window, properties), *window) } // Resize window before set GUI
{
	auto exitButton{ tgui::Button::create() };
	exitButton->setText("Exit");
	exitButton->setEnabled(true);
	exitButton->setSize(properties.GetSideMenuWidth() * 0.5, 50);
	exitButton->setPosition((properties.GetGameWindowWidth() - properties.GetSideMenuWidth() / 2 - exitButton->getSize().x / 2),
		(properties.GetGameWindowHeight() - exitButton->getSize().y - 5));
	exitButton->connect(tgui::Signals::Button::Clicked, SetExitStatus, game);
	gameGui.add(exitButton);
	exitButton->setWidgetName("ExitButton");
	if (mode == GameMode::PlayerWithBot)
	{
		auto undoMoveButton{ tgui::Button::create() };
		undoMoveButton->setText("UNDO");
		undoMoveButton->setEnabled(true);
		undoMoveButton->setSize(50, 50);
		undoMoveButton->setPosition((properties.GetGameWindowWidth() - properties.GetSideMenuWidth() / 2 - undoMoveButton->getSize().x / 2),
			(properties.GetGameWindowHeight() - undoMoveButton->getSize().y - 60));
		undoMoveButton->connect(tgui::Signals::Button::Clicked, MakeUndoMove, game);
		gameGui.add(undoMoveButton);
		undoMoveButton->setWidgetName("UndoMoveButton");
	}

	auto panel{ tgui::ScrollablePanel::create() };
	panel->setPosition((properties.GetGameWindowWidth() - properties.GetSideMenuWidth()), 0);
	panel->setSize(infoBlockWidth, infoBlockWidth * 2);
	panel->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	gameGui.add(panel);
	panel->tgui::Widget::setWidgetName("ScrollablePanel");
	auto drawFrame{ tgui::Canvas::create() };
	drawFrame->setSize(tgui::Layout2d{ infoBlockWidth, infoBlockWidth * 2 });
	drawFrame->clear(tgui::Color{ 208, 150, 90, 255 });
	gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->add(drawFrame);
}

void SideMenu::SetSprites(sf::Sprite* _figuresSprites)
{
	for (auto i = 0; i < FIGURE_TYPES; ++i)
	{
		figuresSprites[i] = _figuresSprites[i];
		figuresSprites[i].setScale(0.4, 0.4);
	}
}

void SideMenu::ResizeWindowForGame(sf::RenderWindow* window, const MapProperties& properties)
{
	// using create() because setSize() works incorrectly
	window->create(sf::VideoMode{ properties.GetGameWindowWidth(), properties.GetGameWindowHeight() }, "Chess");
}


void SideMenu::HandleEvent(sf::Event& event)
{
	gameGui.handleEvent(event);
}

void SideMenu::UpdateSideMenu(std::vector<MoveInfo> info)
{
	auto panel{ gameGui.get<tgui::ScrollablePanel>("ScrollablePanel") };
	auto countOfMoves{ info.empty() ? 0 : info.back().GetNumOfMove() };
	if (countOfRecords > countOfMoves) // when player make UndoMove clear
	{
		panel->removeAllWidgets();
		auto drawFrame{ tgui::Canvas::create() };
		drawFrame->setSize(tgui::Layout2d{ infoBlockWidth, infoBlockWidth * 2 });
		drawFrame->clear(tgui::Color{ 208, 150, 90, 255 });
		gameGui.get<tgui::ScrollablePanel>("ScrollablePanel")->add(drawFrame);
		countOfRecords = 0;
	}
	if (countOfRecords < countOfMoves)
	{
		for (auto it = info.begin(); it != info.end(); ++it)
			if (!panel->get(std::to_string(it->GetNumOfMove())))
			{
				auto labelFrom{ tgui::Label::create() }, labelTo{ tgui::Label::create() }, labelId{ tgui::Label::create() };
				auto moveId{ std::to_string(it->GetNumOfMove()) + ')' };
				labelFrom->setText(it->GetPosBeforeMove().ToString());
				labelFrom->setTextSize(textSize);
				labelFrom->setPosition(moveId.length() * scale + beforeMoveColumn, 27);
				labelTo->setText(it->GetPosAfterMove().ToString());
				labelTo->setTextSize(textSize);
				labelTo->setPosition(moveId.length() * scale + afterMoveColumn, 27);
				labelId->setText(moveId);
				labelId->setTextSize(textSize);
				labelId->setPosition(0, 27);
				auto drawFrame{ tgui::Canvas::create() }, border{ tgui::Canvas::create() };
				drawFrame->setSize(tgui::Layout2d{ infoBlockWidth, infoBlockHeight });
				drawFrame->clear(tgui::Color{ 208, 150, 90, 255 });
				border->setSize(tgui::Layout2d{ infoBlockWidth, splitLineWidth });
				border->clear(tgui::Color::Black);
				if (it->GetNumOfMove() != 1)
				{
					drawFrame->setPosition(0, panel->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + infoBlockHeight + splitLineWidth);
					labelFrom->setPosition(moveId.length() * scale + beforeMoveColumn, panel->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + 96);
					labelTo->setPosition(moveId.length() * scale + afterMoveColumn, panel->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + 96);
					labelId->setPosition(0, panel->get(std::to_string(it->GetNumOfMove() - 1))->getPosition().y + 96);
				}
				else
				{
					panel->add(border);
					border = tgui::Canvas::create();
					border->setSize(tgui::Layout2d{ infoBlockWidth, splitLineWidth });
					border->clear(tgui::Color::Black);
					drawFrame->setPosition(0, splitLineWidth);
				}
				panel->add(drawFrame);
				drawFrame->setWidgetName(std::to_string(it->GetNumOfMove()));
				border->setPosition(0, panel->get(std::to_string(it->GetNumOfMove()))->getPosition().y + infoBlockHeight);
				panel->add(border);
				sf::RectangleShape line{ sf::Vector2f{ 20, 2 } };
				line.setFillColor(sf::Color::Black);
				line.setPosition(moveId.length() * scale + 85, 36);
				sf::CircleShape triangle{ 7, 3 };
				triangle.setFillColor(sf::Color::Black);
				triangle.rotate(90);
				triangle.setPosition(moveId.length() * scale + afterMoveColumn - 5, 30);

				figuresSprites[toUType(it->GetTypeActiveFigure())].setColor(sf::Color{ 255, 255, 255, 255 });
				figuresSprites[toUType(it->GetTypeActiveFigure())].setPosition(moveId.length() * scale - 5, 10);
				drawFrame->draw(figuresSprites[toUType(it->GetTypeActiveFigure())]);
				if (it->isEatenFigureExists())
				{
					figuresSprites[toUType(it->GetTypeEatenFigure())].setPosition(moveId.length() * scale + afterMoveColumn + 30, 10);
					figuresSprites[toUType(it->GetTypeEatenFigure())].setColor(sf::Color{ 255, 255, 255, 100 }); // transparent figure
					drawFrame->draw(figuresSprites[toUType(it->GetTypeEatenFigure())]);
				}
				else if (it->GetCaptureEnPassant())
				{
					figuresSprites[toUType(it->GetTypeActiveFigure() == FigureType::Pawn_black ?
						FigureType::Pawn_white : FigureType::Pawn_black)].setPosition(moveId.length() * scale + afterMoveColumn + 30, 10);
					figuresSprites[toUType(it->GetTypeActiveFigure() == FigureType::Pawn_black ?
						FigureType::Pawn_white : FigureType::Pawn_black)].setColor(sf::Color{ 255, 255, 255, 100 }); // transparent figure
					drawFrame->draw(figuresSprites[toUType(it->GetTypeActiveFigure() == FigureType::Pawn_black ?
						FigureType::Pawn_white : FigureType::Pawn_black)]);
				}
				drawFrame->draw(triangle);
				drawFrame->draw(line);
				panel->add(labelFrom);
				panel->add(labelTo);
				panel->add(labelId);
				panel->setVerticalScrollbarValue(73 * it->GetNumOfMove());
			}
	}
	countOfRecords = countOfMoves;
	gameGui.draw();
}
