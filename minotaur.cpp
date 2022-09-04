#include"minotaur.h"
#include<iostream>
//----------------------------------------------------------------------------------------------------------------------------------------
void minotaur::drow(player& p, sf::RenderWindow& window, float deltaTime, float playerToEnemy)
{
	float sx = x - p.getxPos();
	float sy = y - p.getyPos();
	float sz = z;
	float cs = cos(-p.getPa()), sn = sin(-p.getPa());
	float a = sy * cs + sx * sn;
	float b = sx * cs - sy * sn;
	sx = a;
	sy = b;

	sx = (sx * 600.0 / sy) + 735;
	sy = (sz * 80.0 / sy) + 260;
	sprite.setScale(37 / b, 37 / b);
	sprite.setPosition(sx - sprite.getLocalBounds().width * (37 / b) / 2, sy - sprite.getLocalBounds().height * (37 / b) / 2);
	if (sx > 330 - sprite.getLocalBounds().width * (37 / b) / 2 && sprite.getPosition().x < 1170  && b > 0)
	{
		window.draw(sprite);
	}
	totalTime += deltaTime;
	if (totalTime > 0.3)
	{
		totalTime -= 0.3;
		if(playerToEnemy > 13 || !seePlayer)
			move = (move + 1) % 4;
		else
		{
			if (move < 4)
				move = 3;
			if(move < 8)
				++move;
		}
	}

	switch (move)
	{
	case 0:
		sprite.setTextureRect(sf::IntRect(34, 20, 57, 89));
		break;
	case 1:
		sprite.setTextureRect(sf::IntRect(165, 20, 57, 89));
		break;
	case 2:
		sprite.setTextureRect(sf::IntRect(290, 20, 57, 89));
		break;
	case 3:
		sprite.setTextureRect(sf::IntRect(418, 20, 57, 89));
		break;
	case 4:
		sprite.setTextureRect(sf::IntRect(546, 20, 57, 89));
		break;
	case 5:
		sprite.setTextureRect(sf::IntRect(674, 20, 57, 89));
		break;
	case 6:
		sprite.setTextureRect(sf::IntRect(802, 20, 57, 89));
		break;
	case 7:
		sprite.setTextureRect(sf::IntRect(930, 20, 57, 89));
		break;

	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void minotaur::setStartingPoint()
{
	int startX = rand() % 28 + 3;
	int starty = rand() % 28 + 3;
	if (startX % (blockSize + 1) == 0)
		if (startX == mapX - 1)
			--startX;
		else
			++startX;
	if (starty % (blockSize + 1) == 0)
		if (starty == mapY - 1)
			--starty;
		else
			++starty;
	x = startX * mapS + 5;
	y = starty * mapS + 5;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void minotaur::drawOnMap(sf::RenderWindow& window, float deltaTime)
{
	if (seeEnemyTimer > 0)
	{
		seeEnemyTimer -= deltaTime;
		enemy.setPosition(x - enemyRectSize / 2, y - enemyRectSize / 2);
		window.draw(enemy);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------