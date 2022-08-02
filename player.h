#pragma once
#include<math.h>
#include <SFML/Graphics.hpp>
//----------------------------------------------------------------------------------------------------------------------------------------
class player {

	float xPos, yPos, pa, pdx , pdy ;
	sf::RectangleShape rect;
	int mapS;


public:

	player(float xPos, float yPos, float pa, int mapS);
	void draw(sf::RenderWindow& window);
	float getxPos() { return xPos; }
	float getyPos() { return yPos; }
	float getPa() { return pa; }
	float getPdx() { return pdx; }
	float getPdy() { return pdy; }
	void setxPos(float x) { xPos = x; }
	void setyPos(float y) { yPos = y; }
	void setPa(float a) { pa = a; }
	void setPdx(float dx) { pdx = dx; }
	void setPdy(float dy) { pdy = dy; }
	void addxPos(float x) { xPos += x; }
	void addyPos(float y) { yPos += y; }
	void addPa(float a) { pa += a; }

};
//----------------------------------------------------------------------------------------------------------------------------------------