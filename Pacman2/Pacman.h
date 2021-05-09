#pragma once

#include "Entity.h"

class Pacman : public Entity
{
	using Vector = sf::Vector2f;
public:
	Pacman(Vector pos, float speed, const sf::Texture& texture, Vector size);
	void update(float time, Map& map, sf::Vector2i target = sf::Vector2i(0, 0)) final;
	bool collision(Map& map, sf::Vector2i target = sf::Vector2i(0, 0)) final;
	int getScore() { return score; }
	void drawHealthPoint(sf::RenderWindow& window);
	void damage();
	int getHP() { return healthPoint; }
	DIRECTION getDir() { return dir; }
	bool eatFood(Map& map);
	bool checkCollWithEnemy(const Entity* enemy, const Map& map);
	void giveScorePoint(int point) { score += point; }
private:
	void control();
	void move(float time);
	void changeFrame(float time);
private:
	int score;
	int healthPoint;
	sf::Sprite spriteHealth;
};