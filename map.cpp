#include"map.h"
#include"iostream"
//----------------------------------------------------------------------------------------------------------------------------------------
//change the player position 
void gameMap::movePlayer(player& p)
{
	int xo = 0, yo = 0;
	if (p.getPdx() < 0)
		xo = -mapS / 3;
	else
		xo = mapS / 3;
	if (p.getPdy() < 0)
		yo = -mapS / 3;
	else
		yo = mapS / 3;
	int ipx = p.getxPos() / mapS, ipx_add_xo = (p.getxPos() + xo) / mapS, ipx_sub_xo = (p.getxPos() - xo) / mapS;
	int ipy = p.getyPos() / mapS, ipy_add_yo = (p.getyPos() + yo) / mapS, ipy_sub_yo = (p.getyPos() - yo) / mapS;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (map[ipy * mapX + ipx_add_xo] == 0) { p.addxPos(p.getPdx()); }
		if (map[ipy_add_yo * mapX + ipx] == 0) { p.addyPos(p.getPdy()); }
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (map[ipy * mapX + ipx_sub_xo] == 0)
			p.addxPos(-p.getPdx());
		if (map[ipy_sub_yo * mapX + ipx] == 0)
			p.addyPos(-p.getPdy());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { p.addPa(-0.05); if (p.getPa() <= 0) p.addPa(2 * pi); p.setPdx(cos(p.getPa()) * mapS / 16); p.setPdy(sin(p.getPa()) * mapS / 16); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { p.addPa(0.05); if (p.getPa() >= 2 * pi) p.addPa(-2 * pi); p.setPdx(cos(p.getPa()) * mapS / 16); p.setPdy(sin(p.getPa()) * mapS / 16); }

}
//----------------------------------------------------------------------------------------------------------------------------------------
//draw a 2D map of the maze
void gameMap::drawMap2D(sf::RenderWindow& window)
{
	int x, y, xo, yo;
	sf::Color c;
	for (y = 0; y < mapY; ++y)
		for (x = 0; x < mapX; ++x)
		{
			if (map[y * mapX + x] == 1)
				c = sf::Color::Red;
			else if (map[y * mapX + x] == 2)
				c = sf::Color::Color(110, 110, 0, 255);
			else
				c = sf::Color::Black;
			xo = x * mapS + 1;
			yo = y * mapS + 1;
			sf::RectangleShape r;
			r.setFillColor(c);
			r.setPosition(xo, yo);
			r.setSize(sf::Vector2f(mapS - 1, mapS - 1));
			window.draw(r);
		}
}
//----------------------------------------------------------------------------------------------------------------------------------------
//draw the 3D map
void gameMap::drawRay3D(player& p, sf::RenderWindow& window)
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, disT;
	ra = p.getPa() - DR * 35;
	ra = encounterRa(ra);

	drawBackGround(window);

	for (r = 0; r < 140; ++r)
	{
		//check Horizontal line
		dof = 0;
		float disH = 1000000, hx = p.getxPos(), hy = p.getyPos();
		upAndDown(dof, ra, rx, ry, xo, yo, p);
		findBorder(dof, rx, ry, xo, yo, p, hx, hy, disH);

		//check Vertical line
		dof = 0;
		float disV = 1000000, vx = p.getxPos(), vy = p.getyPos();
		leftAndRight(dof, ra, rx, ry, xo, yo, p);
		findBorder(dof, rx, ry, xo, yo, p, vx, vy, disV);

		sf::Color c = sf::Color::Green;
		if (disV < disH)	//vertical wall hit
		{
			rx = vx;
			ry = vy;
			disT = disV;
			if(map[((int)ry) / mapS * mapX + (int)rx / mapS] == 1)
				c = sf::Color::Color(0, 230, 0, 255);
			else
				c = sf::Color::Color(230, 230, 0, 255);
		}
		if (disH <= disV)	//horizontal wall hit
		{
			rx = hx;
			ry = hy;
			disT = disH;
			if (map[((int)ry) / mapS * mapX + (int)rx / mapS] == 1)
				c = sf::Color::Color(0, 110, 0, 255);
			else
				c = sf::Color::Color(110, 110, 0, 255);
		}

		sf::Vertex line2[] = { sf::Vertex(sf::Vector2f(p.getxPos() + 2,p.getyPos() + 2)), sf::Vertex(sf::Vector2f(rx,ry)) };
		window.draw(line2, 2, sf::Lines);

		//Draw 3D walls
		disT = fixFishEye(disT, p, ra);	//fix fisheye
	
		float lineH, lineO;
		rectYCoordination(lineH, lineO, disT);
	
		sf::RectangleShape wall(sf::Vector2f(squareWidth, lineH));
		wall.setPosition(r * squareWidth + mapS * mapX + 20, lineO);
		wall.setFillColor(c);
		window.draw(wall);
		ra = encounterRa(ra);

	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
//finding the first vertical line for looking up or down
void gameMap::upAndDown(int& dof, float ra, float& rx, float& ry, float& xo, float& yo, player& p)
{
	float aTan = -1 / tan(ra);
	if (ra > pi)	//looking up
	{ 
		ry = (int)p.getyPos() - (int)p.getyPos() % mapS - 0.0001;
		rx = (p.getyPos() - ry) * aTan + p.getxPos();
		yo = -mapS;
		xo = -yo * aTan;
	}
	if (ra < pi)	//looking down
	{
		ry = (int)p.getyPos() - (int)p.getyPos() % mapS + mapS;
		rx = (p.getyPos() - ry) * aTan + p.getxPos();
		yo = mapS;
		xo = -yo * aTan;
	}
	if (ra == 0 || ra == pi)
	{
		rx = p.getxPos();
		ry = p.getyPos();
		dof = 10;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
//finding the first horizontal line for looking left or right
void gameMap::leftAndRight(int& dof, float ra, float& rx, float& ry, float& xo, float& yo, player& p)
{
	float nTan = -tan(ra);
	if (ra > P2 && ra < P3)		//looking leaft
	{
		rx = (int)p.getxPos() - (int)p.getxPos() % mapS - 0.0001;
		ry = (p.getxPos() - rx) * nTan + p.getyPos();
		xo = -mapS;
		yo = -xo * nTan;
	}
	if (ra < P2 || ra > P3)		//looking right
	{
		rx = (int)p.getxPos() - (int)p.getxPos() % mapS + mapS;
		ry = (p.getxPos() - rx) * nTan + p.getyPos();
		xo = mapS;
		yo = -xo * nTan;
	}
	if (ra == 0 || ra == pi)
	{
		rx = p.getxPos();
		ry = p.getyPos();
		dof = 10;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
//find the first wall hit vertical or horizontal
void gameMap::findBorder(int dof, float rx, float ry, float xo, float yo, player &p, float& x, float& y, float& dis)
{
	while (dof < mapX)
	{
		int mx = (int)rx / mapS;
		int my = (int)ry / mapS;
		int mp = my * mapX + mx;
		if (mp > 0 && mp < mapX * mapY && map[mp] > 0)
		{
			dof = mapX;
			x = rx;
			y = ry;
			dis = dist(p.getxPos(), p.getyPos(), x, y);
		}
		else
		{
			rx += xo;
			ry += yo;
			dof += 1;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
//makeing sure ra values between 0 to 2*PI radiang
float gameMap::checkRaLim(float ra)
{
	if (ra < 0)
		ra += 2 * pi;
	if (ra > 2 * pi)
		ra -= 2 * pi;
	return ra;
}
//----------------------------------------------------------------------------------------------------------------------------------------
//increase ra by one unit
float gameMap::encounterRa(float ra)
{
	ra += DR / 2;
	return checkRaLim(ra);
}
//----------------------------------------------------------------------------------------------------------------------------------------
//finding the start and end Y coordination for the rectangles
void gameMap::rectYCoordination(float& lineH, float& lineO, float disT)
{
	lineH = (mapS * squareLength) / disT;
	if (lineH > squareLength)
		lineH = squareLength;
	lineO = squareLength / 2 - lineH / 2;
}
//----------------------------------------------------------------------------------------------------------------------------------------
//fix fish eyes
float gameMap::fixFishEye(float disT, player &p, float ra)
{
	float ca = p.getPa() - ra;
	if (ca < 0)
		ca += 2 * pi;
	if (ca > 2 * pi)
		ca -= 2 * pi;
	return disT * cos(ca);
}
//----------------------------------------------------------------------------------------------------------------------------------------
//draw background
void gameMap::drawBackGround(sf::RenderWindow& window)
{
	sf::RectangleShape sky(sf::Vector2f(squareWidth * 140, squareLength / 2));
	sf::RectangleShape ground(sf::Vector2f(squareWidth * 140, squareLength / 2));
	sky.setPosition(mapS * mapX + 20, 0);
	ground.setPosition(mapS * mapX + 20, squareLength / 2);
	sky.setFillColor(sf::Color::Color(100, 100, 255, 255));
	ground.setFillColor(sf::Color::Color(0, 0, 100, 255));
	window.draw(sky);
	window.draw(ground);
}
//----------------------------------------------------------------------------------------------------------------------------------------
float gameMap::dist(float ax, float ay, float bx, float by)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}
//----------------------------------------------------------------------------------------------------------------------------------------
//makes a maze in the map
void gameMap::makeMaze()
{
	std::stack<std::pair<int, int>> s;
	s.push(std::make_pair(1, 1));
	std::vector<int> neighbours;
	neighbours.clear();

	while (!s.empty())
	{
		appendDir(neighbours, s);

		if (!neighbours.empty())
		{
			int next_cell_dir = neighbours[rand() % neighbours.size()];
			moveToNextNode(next_cell_dir, s);
			neighbours.clear();
		}
		else
			s.pop();
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
//append to the vector of neighbours the valid neighbours
void gameMap::appendDir(std::vector<int>& neighbours, const std::stack<std::pair<int, int>>& s)
{
	if (s.top().second - blockSize - 1 >= 0 && map[(s.top().second - blockSize - 1) * mapX + s.top().first] == -1)
		neighbours.push_back(0);	//up

	if (s.top().first + blockSize + 1 < mapX && map[s.top().second * mapX + s.top().first + blockSize + 1] == -1)
		neighbours.push_back(1);	//right

	if (s.top().second + blockSize + 1 < mapY && map[(s.top().second + blockSize + 1) * mapX + s.top().first] == -1)
		neighbours.push_back(2);	//down

	if (s.top().first - blockSize - 1 >= 0 && map[s.top().second * mapX + s.top().first - blockSize - 1] == -1)
		neighbours.push_back(3);	//left
}
//----------------------------------------------------------------------------------------------------------------------------------------
//move to the next node in the dfs algorithm
void gameMap::moveToNextNode(int next_cell_dir, std::stack<std::pair<int, int>>& s)
{
	switch (next_cell_dir)
	case 0:		//up
	{
		for (int i = 0; i < blockSize; ++i)
			for (int j = 0; j < blockSize + 1; ++j)
				map[(s.top().second - j - 1) * mapX + s.top().first + i] = 0;
		s.push(std::make_pair(s.top().first, s.top().second - blockSize - 1));
		break;

	case 1:		//right
		for (int i = 0; i < blockSize + 1; ++i)
			for (int j = 0; j < blockSize; ++j)
				map[(s.top().second + j) * mapX + s.top().first + i + 2] = 0;
		s.push(std::make_pair(s.top().first + blockSize + 1, s.top().second));
		break;

	case 2:		//down
		for (int i = 0; i < blockSize; ++i)
			for (int j = 0; j < blockSize + 1; ++j)
				map[(s.top().second + j + 2) * mapX + s.top().first + i] = 0;
		s.push(std::make_pair(s.top().first, s.top().second + blockSize + 1));
		break;

	case 3:		//left
		for (int i = 0; i < blockSize + 1; ++i)
			for (int j = 0; j < blockSize; ++j)
				map[(s.top().second + j) * mapX + s.top().first - i - 1] = 0;
		s.push(std::make_pair(s.top().first - blockSize - 1, s.top().second));
		break;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------