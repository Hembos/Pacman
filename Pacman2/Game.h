#pragma once
#include "Map.h"
#include "Pacman.h"
#include "Enemy.h"

enum class GAME_MODE
{
	eatingGhosts,
	normal
};

struct Targets
{
	sf::Vector2i targetBlinky;
	sf::Vector2i targetPinky;
	sf::Vector2i targetClyde;
	sf::Vector2i targetInky;
};

class Game
{
private:
	int spriteSize;
	int mapHeight;
	int mapWidth;

	sf::RenderWindow* window;
	sf::Sprite numbersSprite;
	sf::Texture mapTexture;
	sf::Texture pacmanTexture;
	sf::Texture numbersTexture;
	sf::Texture blinkyTexture;
	sf::Texture pinkyTexture;
	sf::Texture clydeTexture;
	sf::Texture inkyTexture;
	sf::Texture frightTexture;

	Map* map;
	Pacman* pacman;
	Enemy* blinky;
	Enemy* pinky;
	Enemy* clyde;
	Enemy* inky;

	sf::Time time;
	float pastTime;
	float elapsedTimeEating;

	Targets targets;
	int countEaten = 0;
	GAME_MODE gameMode;
public:
	Game(int spriteSize, int mapHeight, int mapWidth);
	void renderScene();
private:
	void drawScore();
	void updateEntities(float pt);
	void giveTargets();
	void setBeginGame();
};