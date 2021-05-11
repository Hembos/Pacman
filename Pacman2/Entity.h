#pragma once

#include <SFML/Graphics.hpp>
#include "Map.h"

enum class DIRECTION
{
	right,
	left,
	up,
	down,
	stay
};

class Entity
{
	using Vector = sf::Vector2f;
protected:
	Vector pos;
	Vector delta;
	float speed;
	float currentFrame;
	DIRECTION dir;
	sf::Sprite sprite;
	Vector size;
public:
	Entity(Vector pos, float speed, const sf::Texture& texture, Vector size);
	virtual void update(float time, Map& map, sf::Vector2i target = sf::Vector2i(0, 0)) = 0;
	virtual bool collision(Map& map, sf::Vector2i target = sf::Vector2i(0, 0)) = 0;
	void draw(sf::RenderWindow& window) { window.draw(sprite); }
	void changeDelta();
	virtual void move(float time) = 0;
	sf::Vector2i getCell();
	void setTexture(const sf::Texture& texture) { sprite.setTexture(texture); }
};