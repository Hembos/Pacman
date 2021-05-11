#include "Game.h"
#include <string>
#include <sstream>
#include <cmath>

using namespace sf;

Game::Game(int spriteSize, int mapHeight, int mapWidth)
	: spriteSize(spriteSize), mapHeight(mapHeight), mapWidth(mapWidth), pastTime(0), elapsedTimeEating(0)
{
	window = new RenderWindow(VideoMode(mapWidth * spriteSize, mapHeight * spriteSize), "Pacman");
	window->setPosition(Vector2i(300, 100));

	mapTexture.loadFromFile("src/Map.png");
	map = new Map(mapWidth, mapHeight, mapTexture, Vector2f(spriteSize, spriteSize));

	pacmanTexture.loadFromFile("src/Pacman.png");
	pacman = new Pacman(Vector2f(spriteSize * 14, spriteSize * 23), 0.2, pacmanTexture, Vector2f(spriteSize, spriteSize));

	blinkyTexture.loadFromFile("src/Blinky.png");
	blinky = new Enemy(Vector2f(spriteSize * 14, spriteSize * 11), 0.18, blinkyTexture, Vector2f(spriteSize, spriteSize), DIRECTION::left, Vector2i(13, 11), BEHAVIOR_MODE::pursuit);

	pinkyTexture.loadFromFile("src/Pinky.png");
	pinky = new Enemy(Vector2f(spriteSize * 14, spriteSize * 14), 0.17, pinkyTexture, Vector2f(spriteSize, spriteSize), DIRECTION::up, Vector2i(14, 13), BEHAVIOR_MODE::house);

	clydeTexture.loadFromFile("src/Clyde.png");
	clyde = new Enemy(Vector2f(spriteSize * 13, spriteSize * 14), 0.17, clydeTexture, Vector2f(spriteSize, spriteSize), DIRECTION::up, Vector2i(13, 13), BEHAVIOR_MODE::house);

	inkyTexture.loadFromFile("src/Inky.png");
	inky = new Enemy(Vector2f(spriteSize * 15, spriteSize * 14), 0.17, inkyTexture, Vector2f(spriteSize, spriteSize), DIRECTION::up, Vector2i(15, 13), BEHAVIOR_MODE::house);

	numbersTexture.loadFromFile("src/Numbers.png");
	numbersSprite.setTexture(numbersTexture);

	frightTexture.loadFromFile("src/Fright.png");

	gameMode = GAME_MODE::normal;
}

void Game::renderScene()
{
	sf::Clock clock;

	while (window->isOpen())
	{
		Event event;
		time = clock.restart();
		float pt = time.asMicroseconds();
		pt /= 1000;
		pastTime += time.asSeconds();
		totalTime += time.asSeconds();

		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}

		window->clear();
 
		map->drawMap(*window);

		updateEntities(pt);

		//////////////////////////
		if (pacman->getHP() == 0)
		{
		 	break;
		}

		blinky->draw(*window);
		pinky->draw(*window);
		clyde->draw(*window);
		inky->draw(*window);
		pacman->draw(*window);
		
		drawScore();
		pacman->drawHealthPoint(*window);

		window->display();
	}
}

void Game::giveTargets()
{
	targets.targetBlinky = pacman->getCell();
	targets.targetPinky = pacman->getCell();
	targets.targetClyde = pacman->getCell();
	targets.targetInky = pacman->getCell();
	DIRECTION dir = pacman->getDir();

	switch (dir)
	{
	case DIRECTION::right:
		targets.targetPinky.x += 4;
		targets.targetInky.x += 2;
		break;
	case DIRECTION::left:
		targets.targetPinky.x -= 4;
		targets.targetInky.x -= 2;
		break;
	case DIRECTION::up:
		targets.targetPinky.y -= 4;
		targets.targetInky.y -= 2;
		break;
	case DIRECTION::down:
		targets.targetPinky.y += 4;
		targets.targetInky.y += 2;
		break;
	default:
		break;
	}

	targets.targetInky.x = abs(blinky->getCell().x + 2 * (blinky->getCell().x - targets.targetInky.x));
	targets.targetInky.y = abs(blinky->getCell().y + 2 * (blinky->getCell().y - targets.targetInky.y));

	if (sqrt(pow(targets.targetClyde.x - clyde->getCell().x, 2) + pow(targets.targetClyde.y - clyde->getCell().y, 2)) <= 8 || clyde->getMode() == BEHAVIOR_MODE::fright)
	{
		targets.targetClyde = Vector2i(32, 0);
	}
	if (blinky->getMode() == BEHAVIOR_MODE::fright)
	{
		targets.targetBlinky = Vector2i(0, 27);
	}
	if (pinky->getMode() == BEHAVIOR_MODE::fright)
	{
		targets.targetPinky = Vector2i(0, 0);
	}
	if (inky->getMode() == BEHAVIOR_MODE::fright)
	{
		targets.targetInky = Vector2i(32, 27);
	}
}

void Game::setBeginGame()
{
	pacman->damage();
	blinky->beginState(blinkyTexture);
	pinky->beginState(pinkyTexture);
	clyde->beginState(clydeTexture);
	inky->beginState(inkyTexture);
	blinky->setMode(BEHAVIOR_MODE::pursuit);
	pinky->setMode(BEHAVIOR_MODE::house);
	clyde->setMode(BEHAVIOR_MODE::house);
	inky->setMode(BEHAVIOR_MODE::house);
	countEaten = 0;
	pastTime = 0;
}

bool checkCollEntities(Enemy& enemy, Pacman& pacman,  int& countEaten, const Texture& texture)
{
	if (enemy.getCell() == pacman.getCell())
	{
		if (enemy.getMode() == BEHAVIOR_MODE::fright)
		{
			enemy.beginState(texture);
			countEaten++;
			pacman.giveScorePoint(countEaten * 200);
		}
		else
		{
			return true;
		}
	}

	return false;
}

void Game::updateEntities(float pt)
{
	pacman->update(pt, *map);

	if (pastTime >= 2 && pinky->getMode() == BEHAVIOR_MODE::house)
	{
		pinky->setMode(BEHAVIOR_MODE::leaveHouse);
	}
	if (pastTime >= 4 && clyde->getMode() == BEHAVIOR_MODE::house)
	{
		clyde->setMode(BEHAVIOR_MODE::leaveHouse);
	}
	if (pastTime >= 6 && inky->getMode() == BEHAVIOR_MODE::house)
	{
		inky->setMode(BEHAVIOR_MODE::leaveHouse);
	}
	if (totalTime >= 5 && totalTime <= 10 && numFruit < 2 && map->getField(17, 14) == '0')
	{
		map->setField('6', 17, 14);
		numFruit += 1;
	}
	if (totalTime > 10 && map->getField(17, 14) == '6')
	{
		map->setField('0', 17, 14);
	}

	if (pacman->eatFood(*map))
	{
		blinky->setMode(BEHAVIOR_MODE::fright);
		blinky->setTexture(frightTexture);
		pinky->setMode(BEHAVIOR_MODE::fright);
		pinky->setTexture(frightTexture);
		clyde->setMode(BEHAVIOR_MODE::fright);
		clyde->setTexture(frightTexture);
		inky->setMode(BEHAVIOR_MODE::fright);
		inky->setTexture(frightTexture);
		countEaten = 0;
		elapsedTimeEating = 3;
		gameMode = GAME_MODE::eatingGhosts;
	}

	if (elapsedTimeEating > 0)
	{
		elapsedTimeEating -= time.asSeconds();
	}
	else if (gameMode == GAME_MODE::eatingGhosts)
	{
		blinky->setMode(BEHAVIOR_MODE::pursuit);
		pinky->setMode(BEHAVIOR_MODE::pursuit);
		clyde->setMode(BEHAVIOR_MODE::pursuit);
		inky->setMode(BEHAVIOR_MODE::pursuit);
		blinky->setTexture(blinkyTexture);
		pinky->setTexture(pinkyTexture);
		clyde->setTexture(clydeTexture);
		inky->setTexture(inkyTexture);
		elapsedTimeEating = 0;
		gameMode = GAME_MODE::normal;
	}

	giveTargets();

	blinky->update(pt, *map, targets.targetBlinky);
	pinky->update(pt, *map, targets.targetPinky);
	clyde->update(pt, *map, targets.targetClyde);
	inky->update(pt, *map, targets.targetInky);
	
	if (checkCollEntities(*blinky, *pacman, countEaten, blinkyTexture) == true
		|| checkCollEntities(*pinky, *pacman, countEaten, pinkyTexture) == true
		|| checkCollEntities(*clyde, *pacman, countEaten, clydeTexture) == true
		|| checkCollEntities(*inky, *pacman, countEaten, inkyTexture) == true)
	{
		setBeginGame();
		return;
	}
}

void Game::drawScore()
{
	std::ostringstream osstr;

	osstr << pacman->getScore();

	std::string strScore = osstr.str();

	std::string numbers = "0123456789";

	int j = 1;
	for (const auto& num : strScore)
	{
		numbersSprite.setTextureRect(sf::IntRect(0, 8 * numbers.find(num), 7, 7));
		numbersSprite.setPosition(sf::Vector2f(27 * j, 27 * 31));
		numbersSprite.setScale(sf::Vector2f(2, 2));
		window->draw(numbersSprite);
		j++;
	}

	std::ostringstream osstr1;
	osstr1 << totalTime;
	std::string pt = osstr1.str();

	j = 0;

	for (const auto& c : pt)
	{
		numbersSprite.setTextureRect(sf::IntRect(0, 8 * numbers.find(c), 7, 7));
		numbersSprite.setPosition(sf::Vector2f(27 * (j + 15), 27 * 31));
		numbersSprite.setScale(sf::Vector2f(2, 2));
		window->draw(numbersSprite);
		j++;
	}
}