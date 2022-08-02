#pragma once
#include"player.h"
//----------------------------------------------------------------------------------------------------------------------------------------
class gameMap {

	int mapX = 10, mapY = 10, mapS;

	int map[100] = {
		1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 1, 1, 1, 0, 1,
		1, 0, 1, 1, 0, 1, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	};

	int squareLength = 460, squareWidth = 6;

public:
	constexpr static float pi = 3.1415926532;
	constexpr static float P2 = pi / 2;
	constexpr static float P3 = 3 * pi / 2;
	constexpr static float DR = 0.0174533; //one degree in radians


public:
	gameMap(int mapS, int squareLength, int squareWidth) : mapS(mapS), squareLength(squareLength), squareWidth(squareWidth){}
	void movePlayer(player &p);
	void drawMap2D(sf::RenderWindow& window);
	void drawRay3D(player& p, sf::RenderWindow& window);



private:
	float dist(float ax, float ay, float bx, float by);
	float fixFishEye(float disT, player& p, float ra);
	void rectYCoordination(float& lineH, float& lineO, float disT);
	float encounterRa(float ra);
	void drawBackGround(sf::RenderWindow& window);
	float checkRaLim(float ra);
	void findBorder(int dof, float rx, float ry, float xo, float yo, player &p, float& x, float& y, float& dis);
	void upAndDown(int& dof, float ra, float& rx, float& ry, float& xo, float& yo, player& p);
	void leftAndRight(int& dof, float ra, float& rx, float& ry, float& xo, float& yo, player& p);

};
//----------------------------------------------------------------------------------------------------------------------------------------