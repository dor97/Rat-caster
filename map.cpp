#include"map.h"
//----------------------------------------------------------------------------------------------------------------------------------------
//change the player position 
void gameMap::movePlayer(player& p, minotaur& m)
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m.getMove() < 4)
	{
		if (map[ipy * mapX + ipx_add_xo] == 0) { p.addxPos(p.getPdx()); }
		if (map[ipy_add_yo * mapX + ipx] == 0) { p.addyPos(p.getPdy()); }
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m.getMove() < 4)
	{
		if (map[ipy * mapX + ipx_sub_xo] == 0)
			p.addxPos(-p.getPdx());
		if (map[ipy_sub_yo * mapX + ipx] == 0)
			p.addyPos(-p.getPdy());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { p.addPa(-0.03); if (p.getPa() <= 0) p.addPa(2 * pi); p.setPdx(cos(p.getPa()) * mapS / 26); p.setPdy(sin(p.getPa()) * mapS / 26); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { p.addPa(0.03); if (p.getPa() >= 2 * pi) p.addPa(-2 * pi); p.setPdx(cos(p.getPa()) * mapS / 26); p.setPdy(sin(p.getPa()) * mapS / 26); }

}
//----------------------------------------------------------------------------------------------------------------------------------------
float gameMap::negativeOrPositive(float num)
{
	if (num < 0)
		return -1;
	else
		if(num > 0)
			return 1;
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void gameMap::findXandY(minotaur& m, player& p, float& x, float& y)
{
	float dis = dist(m.getX(), m.getY(), m.getPlayerSeenX(), m.getPlayerSeenY());
	if (dis == 0)
	{
		x = 0;
		y = 0;
	}
	else
	{
		x = (m.getPlayerSeenX() - m.getX()) / dis;
		y = (m.getPlayerSeenY() - m.getY()) / dis;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void gameMap::moveingToCloseToWall(minotaur& m, float& x, float& y)
{
	bool canMoveX = true;
	if ((map[(int)((m.getX() - 6) / mapS) + mapX * (int)(m.getY() / mapS)] != 0 && m.getPlayerSeenX() - m.getX() < 0) || (map[(int)((m.getX() + 6) / mapS) + mapX * (int)(m.getY() / mapS)] != 0 && m.getPlayerSeenX() - m.getX() > 0))
	{
		canMoveX = false;
		x = 0;
		y = negativeOrPositive(y);
	}
	if ((map[(int)(m.getX() / mapS) + mapX * (int)((m.getY() - 6) / mapS)] != 0 && m.getPlayerSeenY() - m.getY() < 0) || (map[(int)(m.getX() / mapS) + mapX * (int)((m.getY() + 6) / mapS)] != 0 && m.getPlayerSeenY() - m.getY() > 0))
	{
		y = 0;
		if (canMoveX)
			x = negativeOrPositive(x);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
std::pair<int, int> gameMap::moveTo()
{
	int newX = rand() % 31;
	int newY = rand() % 31;
	if (newX % (blockSize + 1) == 0)
		if (newX == mapX - 1)
			--newX;
		else
			++newX;
	if (newY % (blockSize + 1) == 0)
		if (newY == mapY - 1)
			--newY;
		else
			++newY;
	return std::make_pair(newX, newY);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void gameMap::moveMinotaur(minotaur& m, player& p)
{
	isMinotaurSee(p, m);
	if(m.getSeePlayer())
	{
		m.setSeeEnemyTimer(20);
		m.setSpeed(0.2);
		//if((map[(int)((p.getxPos() - 6) / mapS) + mapX * (int)(p.getyPos() / mapS)] == 0 && p.getxPos() - m.getX() < 0) || (map[(int)((p.getxPos() + 6) / mapS) + mapX * (int)(p.getyPos() / mapS)] == 0 && p.getxPos() - m.getX() > 0))
		m.setPlayerSeenX(p.getxPos());
		//if ((map[(int)(p.getxPos() / mapS) + mapX * (int)((p.getyPos() - 6) / mapS)] == 0 && p.getyPos() - m.getY() < 0) || (map[(int)(p.getxPos() / mapS) + mapX * (int)((p.getyPos() + 6) / mapS)] == 0 && p.getyPos() - m.getY() > 0))
		m.setPlayerSeenY(p.getyPos());
		passDestinationX = false; passDestinationY = false;
	}
	float x, y;
	findXandY(m, p, x, y);

	float disToPlayer = dist(m.getX(), m.getY(),p.getxPos(), p.getyPos());
	if (disToPlayer > 13 || !m.getSeePlayer())
	{
		moveingToCloseToWall(m, x, y);
		m.addX(x * m.getSpeed());
		m.addY(y * m.getSpeed());
	}
	
	if (!m.getSeePlayer() && (m.getX() * negativeOrPositive(x) >= m.getPlayerSeenX() * negativeOrPositive(x) || x == 0))
		passDestinationX = true;
	if (!m.getSeePlayer() && (m.getY() * negativeOrPositive(y) >= m.getPlayerSeenY() * negativeOrPositive(y) || y == 0))
		passDestinationY = true;

	if (!m.getSeePlayer() && passDestinationX && passDestinationY)
	{
		m.setSpeed(0.1);
		passDestinationX = false; passDestinationY = false;
		if (parent.empty())
		{

			bfs(moveTo(), mapX * mapY, std::make_pair((int)(m.getX() / mapS), (int)(m.getY() / mapS)), parent);
			pInd = ((int)(m.getY() / mapS)) * mapX + ((int)(m.getX() / mapS));
		}
		if (parent[pInd] == -2)
			parent.clear();
		else
		{
			m.setPlayerSeenX((parent[pInd] % mapX) * mapS + 5);
			m.setPlayerSeenY((parent[pInd] / mapX) * mapS + 5);
			pInd = parent[pInd];
		}
	}
	
	/*
	if (m.getX() > p.getxPos() + 8 && map[(int)((m.getX() - 5) / mapS) + mapX * (int)(m.getY() / mapS)] == 0)
	{
		m.addX(x * speed);
		flage = false;
	}
	if (m.getX() < p.getxPos() - 8 && map[(int)((m.getX() + 5) / mapS) + mapX * (int)(m.getY() / mapS)] == 0)
	{
		m.addX(x * speed);
		flage = false;
	}

	if (m.getY() > p.getyPos() + 8 && map[(int)(m.getX() / mapS) + mapX * (int)((m.getY() - 5) / mapS)] == 0)
	{
		m.addY(y * speed);
		flage = false;
	}
	if (m.getY() < p.getyPos() - 8 && map[(int)(m.getX() / mapS) + mapX * (int)((m.getY() + 5) / mapS)] == 0)
	{
		m.addY(y * speed);
		flage = false;
	}
	*/
	//if (m.getPlayerSeenX() != p.getxPos() || m.getPlayerSeenY() != p.getyPos())
		//flage = false;
	//if (!flage)
		//std::cout << "playerPos " << m.getPlayerSeenX() << " " << m.getPlayerSeenY() << "\n";
}
//----------------------------------------------------------------------------------------------------------------------------------------
//draw a 2D map of the maze
void gameMap::drawMap2D(sf::RenderWindow& window, player& p)
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
	if(!rays.empty())
		for (int r = 0; r < 140; ++r)
		{
			sf::Vertex line2[] = { sf::Vertex(sf::Vector2f(p.getxPos() + 2,p.getyPos() + 2)), sf::Vertex(sf::Vector2f(rays[r].first,rays[r].second)) };
			window.draw(line2, 2, sf::Lines);
		}
}
//----------------------------------------------------------------------------------------------------------------------------------------
float min(float a, float b)
{
	if (a > b)
		return b;
	return a;
}
//----------------------------------------------------------------------------------------------------------------------------------------
float max(float a, float b)
{
	if (a < b)
		return b;
	return a;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void gameMap::isMinotaurSee(player& p, minotaur& m)
{
	float incline = (p.getyPos() - m.getY()) / (p.getxPos() - m.getX());
	float a = atan(incline);
	if (a < 0)
	{
		a += 2 * pi;
		if (p.getyPos() < m.getY() && p.getxPos() > m.getX())
			a -= pi;
	}
	else
		if (p.getyPos() > m.getY() && p.getxPos() > m.getX())
			a += pi;
	float x, y, xo, yo;

	int dof = 0;
	float disH = 1000000, hx = p.getxPos(), hy = p.getyPos();
	upAndDown(dof, a, x, y, xo, yo, p);
	findBorder(dof, x, y, xo, yo, p, hx, hy, disH);

	//check Vertical line
	dof = 0;
	float disV = 1000000, vx = p.getxPos(), vy = p.getyPos();
	leftAndRight(dof, a, x, y, xo, yo, p);
	findBorder(dof, x, y, xo, yo, p, vx, vy, disV);

	if (disV < disH)	//vertical wall hit
	{
		x = vx;
		y = vy;
	}
	if (disH <= disV)	//horizontal wall hit
	{
		x = hx;
		y = hy;
	}
	//std::cout << x << ", " << y << ", " << p.getxPos() << ", " << p.getyPos() << ", " << m.getX() << ", " << m.getY() << "\n";
	if((min(p.getxPos(), x) <= m.getX() && m.getX() <= max(p.getxPos(), x)) && (min(p.getyPos(), y) <= m.getY() && m.getY() <= max(p.getyPos(), y)))
		m.setSeePlayer(true);
	else
		m.setSeePlayer(false);
}

//----------------------------------------------------------------------------------------------------------------------------------------
//draw the 3D map
void gameMap::drawRay3D(player& p, sf::RenderWindow& window, minotaur& m, float deltaTime)
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, disT;
	ra = p.getPa() - DR * 35;
	ra = encounterRa(ra);
	std::vector<std::pair<float, sf::Color>> toDraw;

	drawBackGround(window);
	rays.clear();
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

		rays.push_back(std::make_pair(rx, ry));

		//Draw 3D walls
		disT = fixFishEye(disT, p, ra);	//fix fisheye
		toDraw.push_back(std::make_pair(disT, c));
		/*
		float lineH, lineO;
		rectYCoordination(lineH, lineO, disT);
	
		sf::RectangleShape wall(sf::Vector2f(squareWidth, lineH));
		wall.setPosition(r * squareWidth + mapS * mapX + 20, lineO);
		wall.setFillColor(c);
		window.draw(wall);
		*/
		ra = encounterRa(ra);

	}
	float playerToEnemy = dist(p.getxPos(), p.getyPos(), m.getX(), m.getY());
	for (r = 0; r < 140; ++r)
		if (playerToEnemy <= toDraw[r].first)
			drawRectangle(toDraw, r, window);
			//ra = encounterRa(ra);

	m.drow(p, window, deltaTime, playerToEnemy);

	for (r = 0; r < 140; ++r)
		if (playerToEnemy > toDraw[r].first)
			drawRectangle(toDraw, r, window);
			//ra = encounterRa(ra);

}
//----------------------------------------------------------------------------------------------------------------------------------------
void gameMap::drawRectangle(std::vector<std::pair<float, sf::Color>>& toDraw, int r, sf::RenderWindow& window)
{
	float lineH, lineO;
	rectYCoordination(lineH, lineO, toDraw[r].first);

	sf::RectangleShape wall(sf::Vector2f(rectangleWidth, lineH));
	wall.setPosition(r * rectangleWidth + mapS * mapX + 20, lineO);
	wall.setFillColor(toDraw[r].second);
	window.draw(wall);
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
	if (ra > P2 && ra < P3)		//looking left
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
	lineH = (mapS * rectangleLength) / disT;
	if (lineH > rectangleLength)
		lineH = rectangleLength;
	lineO = rectangleLength / 2 - lineH / 2;
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
	sf::RectangleShape sky(sf::Vector2f(rectangleWidth * 140, rectangleLength / 2));
	sf::RectangleShape ground(sf::Vector2f(rectangleWidth * 140, rectangleLength / 2));
	sky.setPosition(mapS * mapX + 20, 0);
	ground.setPosition(mapS * mapX + 20, rectangleLength / 2);
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
void gameMap::bfs(std::pair<int, int> s, int n, std::pair<int, int> t, std::vector<int>& p)
{
	std::queue<std::pair<int, int>> q;
	q.push(s);
	for (int i = 0; i < n; ++i)
		p.push_back(-1);
	//d[s - 1] = 0;
	p[s.second * mapX + s.first] = -2;
	while (!q.empty())
	{
		appendBfs(q, p);
		q.pop();
		if (p[t.second * mapX + t.first] != -1)
			return;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void gameMap::appendBfs(std::queue<std::pair<int, int>>& q, std::vector<int>& p)
{
	if (q.front().second - 1 >= 0 && map[(q.front().second - 1) * mapX + q.front().first] == 0 && p[(q.front().second - 1) * mapX + q.front().first] == -1)
	{
		q.push(std::make_pair(q.front().first, q.front().second - 1));	//up
		p[(q.front().second - 1) * mapX + q.front().first] = q.front().second * mapX + q.front().first;
	}
	if (q.front().first + 1 < mapX && map[q.front().second * mapX + q.front().first + 1] == 0 && p[q.front().second * mapX + q.front().first + 1] == -1)
	{
		q.push(std::make_pair(q.front().first + 1, q.front().second));	//right
		p[q.front().second * mapX + q.front().first + 1] = q.front().second * mapX + q.front().first;
	}

	if (q.front().second + 1 < mapY && map[(q.front().second + 1) * mapX + q.front().first] == 0 && p[(q.front().second + 1) * mapX + q.front().first] == -1)
	{
		q.push(std::make_pair(q.front().first, q.front().second + 1));	//down
		p[(q.front().second + 1) * mapX + q.front().first] = q.front().second * mapX + q.front().first;
	}
		
	if (q.front().first - 1 >= 0 && map[q.front().second * mapX + q.front().first - 1] == 0 && p[q.front().second * mapX + q.front().first - 1] == -1)
	{
		q.push(std::make_pair(q.front().first - 1, q.front().second));	//left
		p[q.front().second * mapX + q.front().first - 1] = q.front().second * mapX + q.front().first;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------