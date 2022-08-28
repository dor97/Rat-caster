#include <SFML/Graphics.hpp>
#include"map.h"
#include"minotaur.h"
//----------------------------------------------------------------------------------------------------------------------------------------
/*
A 3D maze game using ray casting with SFML
*/
//----------------------------------------------------------------------------------------------------------------------------------------
int main() {
	constexpr int width = 1170, height = 520, mapS = 10;
	constexpr float Pi = 3.1415926535;
	srand(clock());
	sf::RenderWindow window(sf::VideoMode(width, height), "Ray Caster");
	window.setFramerateLimit(60);
	player p(15, 15, gameMap::pi / 2 , mapS);
	gameMap map(mapS, 520, 6);
	minotaur m(mapS);
	float deltaTime = 0.0;
	sf::RectangleShape sideBackGrownd(sf::Vector2f(0,0));
	sideBackGrownd.setFillColor(sf::Color::Color(100, 100, 100, 255));
	sideBackGrownd.setSize(sf::Vector2f(mapS * 31 + 20, height));
	
	sf::Clock clock;
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			
		}
		
		map.movePlayer(p, m);
		map.moveMinotaur(m, p);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

		window.clear(sf::Color::Color(100, 100, 100, 255));
		map.drawRay3D(p, window, m, deltaTime);
		window.draw(sideBackGrownd);
		map.drawMap2D(window, p);
		p.draw(window);
		m.drawOnMap(window, deltaTime);
		window.display();
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------