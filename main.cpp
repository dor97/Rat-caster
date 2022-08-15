#include <SFML/Graphics.hpp>
#include"map.h"
//----------------------------------------------------------------------------------------------------------------------------------------
/*
A 3D maze game using ray casting with SFML
*/
//----------------------------------------------------------------------------------------------------------------------------------------
int main() {
	constexpr int width = 1200, height = 530, mapS = 10;
	constexpr float Pi = 3.1415926535;
	srand(clock());
	sf::RenderWindow window(sf::VideoMode(width, height), "Ray Caster");
	window.setFramerateLimit(60);
	player p(15, 15, gameMap::pi / 2 , mapS);
	gameMap map(mapS, 460, 6);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			
		}
		map.movePlayer(p);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

		window.clear(sf::Color::Color(100, 100, 100, 255));
		map.drawMap2D(window);
		p.draw(window);
		map.drawRay3D(p, window);
		window.display();
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------