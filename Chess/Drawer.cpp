#include <SFML/Graphics.hpp>
#include "Drawer.h"

void Drawer::SetResources(const Resources& resource)
{
	mapTexture.loadFromFile(resource.GetPathToMapImage());
	figuresTexture.loadFromFile(resource.GetPathToFiguresImage());

	mapSprite.setTexture(mapTexture);

	// set the mapSprite size to the window size
	mapSprite.setScale((float)window->getSize().x / mapTexture.getSize().x, (float)window->getSize().y / mapTexture.getSize().y);

	// TODO: initialize sprites for all figures
}

/*void Drawer::SetResources(const Resources* resource)
{
	mapImage.loadFromFile(resource->GetPathToMapImage());
	figuresImage.loadFromFile(resource->GetPathToFiguresImage());
}*/

Drawer::Drawer(sf::RenderWindow* _window, const Resources& resource) : window(_window)
{
	SetResources(resource);
}

/*Drawer::Drawer(const Resources* resource)
{
	SetResources(resource);
}*/

void Drawer::ShowMap(const Map& map)
{
	window->draw(mapSprite);
}

