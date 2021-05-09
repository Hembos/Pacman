#include "Enemy.h"

using namespace sf;

void Enemy::changeFrame(float time)
{
	if (delta.x != 0 || delta.y != 0)
	{
		currentFrame += 0.01 * time;
		if (currentFrame > 2)
			currentFrame -= 2;
	}
	if (delta.x < 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 27, 28, size.x + 1, size.y));
	}
	else if (delta.x > 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 27, 0, size.x + 1, size.y));
	}
	else if (delta.y < 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 27, 56, size.x + 1, size.y));
	}
	else if (delta.y > 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 27, 84, size.x + 1, size.y));
	}
}

Enemy::Enemy(Vector pos, float speed, const sf::Texture& texture, Vector size, DIRECTION d, Vector2i c, BEHAVIOR_MODE mode)
	: Entity(pos, speed, texture, size), mode(mode)
{
	sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
	sprite.setPosition(pos);
	dir = d;
	nextDir = d;
	nextCell = c;

	beginPos = pos;
	beginNextCell = c;
	beginDir = d;
}

void Enemy::update(float time, Map& map, Vector2i target)
{
	
	if (mode != BEHAVIOR_MODE::house)
	{
		if (mode == BEHAVIOR_MODE::leaveHouse && getCell().y == 10)
		{
			mode = BEHAVIOR_MODE::pursuit;
		}
		else if (mode == BEHAVIOR_MODE::leaveHouse)
		{
			target = Vector2i(0, 0);
		}

		changeFrame(time);

		for (int i = 0; i < time; i++)
		{
			collision(map, target);
			changeDelta();
			move(time);
		}
	}
}

int length(Vector2i posP, Vector2i posB)
{
	return (posP.x - posB.x) * (posP.x - posB.x) + (posP.y - posB.y) * (posP.y - posB.y);
}

bool Enemy::collision(Map& map, Vector2i target)
{
	int x = pos.x + size.x / 2;
	int y = pos.y + size.y / 2;

	int j = x / 27;
	int i = y / 27;

	if (map.getField(i, j) == '2' && delta.x < 0)
	{
		nextCell = Vector2i(26, 14);
		pos.x += 27 * 27;
		sprite.move(Vector(27 * 27, 0));
		return false;
	}
	if (map.getField(i, j) == '3' && delta.x > 0)
	{
		nextCell = Vector2i(1, 14);
		pos.x -= 27 * 27;
		sprite.move(Vector(-27 * 27, 0));
		return false;
	}

	if (x == (((int)27 * j + (int)27 / 2))
		&& y == ((int)27 * i + (int)27 / 2)
		&& nextCell == Vector2i(j, i))
	{
		dir = nextDir;

		switch (dir)
		{
		case DIRECTION::right:
			j++;
			break;
		case DIRECTION::left:
			j--;
			break;
		case DIRECTION::up:
			i--;
			break;
		case DIRECTION::down:
			i++;
			break;
		}

		nextCell = Vector2i(j, i);

		if (map.getField(i, j) == '2' || map.getField(i, j) == '3')
		{
			return false;
		}

		int min = INT32_MAX;
		DIRECTION tmp = dir;

		if (dir != DIRECTION::left && map.getField(i, j + 1) != '1' && 
			(map.getField(i, j + 1) != '5' || mode == BEHAVIOR_MODE::leaveHouse) && min >= length(Vector2i(i, j + 1), target))
		{
			min = length(Vector2i(i, j + 1), target);
			tmp = DIRECTION::right;
		}
		if (dir != DIRECTION::right && map.getField(i, j - 1) != '1' && (map.getField(i, j - 1) != '5' || mode == BEHAVIOR_MODE::leaveHouse)
			&& min >= length(Vector2i(i, j - 1), target))
		{
			min = length(Vector2i(i, j - 1), target);
			tmp = DIRECTION::left;
		}
		if (dir != DIRECTION::up && map.getField(i + 1, j) != '1' && (map.getField(i + 1, j) != '5' || mode == BEHAVIOR_MODE::leaveHouse)
			&& min >= length(Vector2i(i + 1, j), target))
		{
			min = length(Vector2i(i + 1, j), target);
			tmp = DIRECTION::down;
		}
		if (dir != DIRECTION::down && map.getField(i - 1, j) != '1' && (map.getField(i - 1, j) != '5' || mode == BEHAVIOR_MODE::leaveHouse)
			&& min >= length(Vector2i(i - 1, j), target))
		{
			min = length(Vector2i(i - 1, j), target);
			tmp = DIRECTION::up;
		}

		nextDir = tmp;
	}

	return false;
}

void Enemy::move(float time)
{
	pos += Vector(delta.x, delta.y);

	sprite.move(Vector(delta.x, delta.y));
}

void Enemy::beginState(const Texture& texture)
{
	pos = beginPos;
	nextCell = beginNextCell;
	sprite.setPosition(pos);
	dir = beginDir;
	nextDir = beginDir;
	sprite.setTexture(texture);
}