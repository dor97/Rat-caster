#include"player.h"
//----------------------------------------------------------------------------------------------------------------------------------------
player::player(float xPos, float yPos, float pa, int mapS) : xPos(xPos), yPos(yPos), pa(pa), mapS(mapS)
{
	rect.setPosition(xPos, yPos);
	rect.setSize(sf::Vector2f(2, 2));
	rect.setFillColor(sf::Color::Yellow);
	pdx = cos(pa) * mapS / 16;
	pdy = sin(pa) * mapS / 16;

}
//----------------------------------------------------------------------------------------------------------------------------------------
void player::draw(sf::RenderWindow& window)
{
	rect.setPosition(xPos, yPos);
	sf::Vertex line[] = { sf::Vector2f(xPos + 1,yPos + 1), sf::Vector2f(xPos + pdx * 5,yPos + pdy * 5) };
	window.draw(line, 2, sf::Lines);
	window.draw(rect);
}
//----------------------------------------------------------------------------------------------------------------------------------------

