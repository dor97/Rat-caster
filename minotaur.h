#pragma once
#include"player.h"
//----------------------------------------------------------------------------------------------------------------------------------------
class minotaur {

	float x, y, z;
	int mapX = 31, mapY = 31, mapS;
	sf::Sprite sprite;
	sf::Texture texture;
	int move = 0;
	float totalTime = 0.0;
	float playerx, playery;
	bool seePlayer = false;
	float speed = 0.2;
	int blockSize;
	sf::RectangleShape enemy;
	float seeEnemyTimer = 0;

public:
	minotaur(int mapS) : mapS(mapS), x(4 * mapS + 5), y(4 * mapS + 5), z(12), playerx(x), playery(y){
		texture.loadFromFile("./test2.png"); 	
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(165, 20, 55, 89));
		blockSize = 2;
		//setStartingPoint();
		enemy.setFillColor(sf::Color::Magenta);
		enemy.setSize(sf::Vector2f(4, 4));
		
	};
	void drow(player& p, sf::RenderWindow& window, float deltaTime, float playerToEnemy);
	float getX() { return x; }
	float getY() { return y; }
	void addX(float xAdd) { x += xAdd; }
	void addY(float yAdd) { y += yAdd; }
	float getPlayerSeenX()  {return playerx; }
	float getPlayerSeenY() { return playery; }
	void setPlayerSeenX(float x) { playerx = x; }
	void setPlayerSeenY(float y) { playery = y; }
	float getWidthX() { return sprite.getLocalBounds().width * sprite.getScale().x; }
	float get3Dx() { return sprite.getPosition().x; }
	bool isNotInScreen() { return (sprite.getPosition().x + (sprite.getLocalBounds().width * sprite.getScale().x / 2) < 330 || sprite.getPosition().x + (sprite.getLocalBounds().width * sprite.getScale().x / 2) > 1170); }
	void setSeePlayer(bool see) { seePlayer = see; }
	bool getSeePlayer() { return seePlayer; }
	float getSpeed() { return speed; }
	void setSpeed(float speed) { this->speed = speed; }
	int getMove() { return move; }
	void setSeeEnemyTimer(float time) { seeEnemyTimer = time; }
	void drawOnMap(sf::RenderWindow& window, float deltaTime);
	
private:
	void setStartingPoint();
};
//----------------------------------------------------------------------------------------------------------------------------------------