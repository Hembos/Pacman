#include "Pacman.h"
#include <iostream>

using namespace sf;

Pacman::Pacman(Vector pos, float speed, const Texture& texture, Vector size)
	: Entity(pos, speed, texture, size), score(0)
{
	currentFrame = 2;
	sprite.setTextureRect(IntRect(64, 0, size.x, size.y));
	sprite.setPosition(pos);
	healthPoint = 3;
	spriteHealth.setTexture(texture);
	spriteHealth.setTextureRect(IntRect(32, 0, size.x, size.y));
	spriteHealth.scale(Vector(0.8, 0.8));
}

bool Pacman::collision(Map& map, Vector2i target)
{
	int x = pos.x + size.x / 2;
	int y = pos.y + size.y / 2;

	int j = x / 27;
	int i = y / 27;

	if (x >= ((int)27 * j + (int)27 / 2 - 1) && x <= ((int)27 * j + (int)27 / 2 + 1) 
		&& y >= ((int)27 * i + (int)27 / 2 - 1) && y <= ((int)27 * i + (int)27 / 2 + 1))
	{
		if (map.getField(i, j) == '2' && delta.x < 0)
		{
			pos.x += 27 * 27;
			sprite.move(Vector(27 * 27, 0));
			return false;
		}
		if (map.getField(i, j) == '3' && delta.x > 0)
		{
			pos.x -= 27 * 27;
			sprite.move(Vector(-27 * 27, 0));
			return false;
		}
		
		if (dir == DIRECTION::right && map.getField(i, j + 1) != '1' && map.getField(i, j + 1) != '5'
			|| dir == DIRECTION::left && map.getField(i, j - 1) != '1' && map.getField(i, j - 1) != '5'
			|| dir == DIRECTION::up && map.getField(i - 1, j) != '1' && map.getField(i - 1, j) != '5'
			|| dir == DIRECTION::down && map.getField(i + 1, j) != '1' && map.getField(i + 1, j) != '5')
		{
			return false;
		}

		if (delta.x > 0 && (map.getField(i, j + 1) == '1' || map.getField(i, j + 1) == '5') && dir != DIRECTION::left
			|| delta.y > 0 && (map.getField(i + 1, j) == '1' || map.getField(i + 1, j) == '5') && dir != DIRECTION::up
			|| delta.x < 0 && (map.getField(i, j - 1) == '1' || map.getField(i, j - 1) == '5') && dir != DIRECTION::right
			|| delta.y < 0 && (map.getField(i - 1, j) == '1' || map.getField(i - 1, j) == '5') && dir != DIRECTION::down)
		{
			delta = Vector(0, 0);
		}	
	}

	return true;
}

void Pacman::control()
{
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		dir = DIRECTION::right;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		dir = DIRECTION::left;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		dir = DIRECTION::up;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		dir = DIRECTION::down;
	}
}

void Pacman::move(float time)
{
	pos += Vector(time * delta.x, time * delta.y);

	sprite.move(Vector(time * delta.x, time * delta.y));
}

void Pacman::update(float time, Map& map, Vector2i target)
{
	control();
	changeFrame(time);	

	if (!collision(map))
	{
		changeDelta();	
	}

	move(time);
}

void Pacman::changeFrame(float time)
{
	if (delta.x != 0 || delta.y != 0)
	{
		currentFrame += 0.01 * time;
		if (currentFrame > 3)
			currentFrame -= 3;
	}
	if (delta.x < 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 32 + size.x, 0, -size.x, size.y));
	}
	else if (delta.x > 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 32, 0, size.x, size.y));
	}
	else if (delta.y < 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 32, 32, size.x, size.y));
	}
	else if (delta.y > 0)
	{
		sprite.setTextureRect(IntRect(int(currentFrame) * 32, 32 + size.y, size.x, -size.y));
	}
}

void Pacman::drawHealthPoint(RenderWindow& window)
{
	for (int i = 0; i < healthPoint; i++)
	{
		spriteHealth.setPosition(Vector(27 * (i + 8), 27 * 31));
		window.draw(spriteHealth);
	}
}

void Pacman::damage() 
{ 
	healthPoint--; 
	pos = Vector(14 * 27, 23 * 27);
	dir = DIRECTION::stay;
	sprite.setPosition(pos);
	currentFrame = 2;
}

bool Pacman::eatFood(Map& map)
{
	sf::Vector2i cell;
	cell = getCell();
	if (map.getField(cell.x, cell.y) == ' ')
	{
		score += 10;
		map.setField('0', cell.x, cell.y);
	}
	if (map.getField(cell.x, cell.y) == '9')
	{
		score += 50;
		map.setField('0', cell.x, cell.y);

		return true;
	}
	return false;
}