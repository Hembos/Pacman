#pragma once
#include "Entity.h"

enum class BEHAVIOR_MODE
{
	pursuit,
	runaway,
	fright,
	house,
	leaveHouse
};

class Enemy : public Entity
{
	using Vector = sf::Vector2f;
private:
	DIRECTION nextDir;
	sf::Vector2i nextCell;
	BEHAVIOR_MODE mode;
	Vector beginPos;
	sf::Vector2i beginNextCell;
	DIRECTION beginDir;
public:
	Enemy(Vector pos, float speed, const sf::Texture& texture, Vector size, DIRECTION d, sf::Vector2i c, BEHAVIOR_MODE mode);
	void changeFrame(float time);
	void update(float time, Map& map, sf::Vector2i target) final;
	bool collision(Map& map, sf::Vector2i target) final;
	void move(float time) final;
	BEHAVIOR_MODE getMode() { return mode; }
	void setMode(BEHAVIOR_MODE mode) { this->mode = mode; }
	void beginState(const sf::Texture& texture);
};