#include "Entity.h"

using namespace sf;

Entity::Entity(Vector pos, float speed, const Texture& texture, Vector size)
	: pos(pos), speed(speed), size(size), currentFrame(0), dir(DIRECTION::left)
{
	sprite.setTexture(texture);
}

void Entity::changeDelta()
{
	switch (dir)
	{
	case DIRECTION::left:
		delta = Vector(-speed, 0);
		break;
	case DIRECTION::right:
		delta = Vector(speed, 0);
		break;
	case DIRECTION::up:
		delta = Vector(0, -speed);
		break;
	case DIRECTION::down:
		delta = Vector(0, speed);
		break;
	case DIRECTION::stay:
		delta = Vector(0, 0);
		break;
	default:
		break;
	}
}

Vector2i Entity::getCell()
{
	int x = pos.x + size.x / 2;
	int y = pos.y + size.y / 2;

	int j = x / 27;
	int i = y / 27;

	return Vector2i(i, j);
}