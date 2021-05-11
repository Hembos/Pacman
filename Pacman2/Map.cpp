#include "Map.h"

using namespace sf;

Map::Map(int mapWidth, int mapHeight, const Texture& texture, sf::Vector2f size)
	: mapWidth(mapWidth), mapHeight(mapHeight), size(size)
{
	spriteBoard.setTexture(texture);
	spriteFood.setTexture(texture);
	spriteDoor.setTexture(texture);
	spriteMegaFood.setTexture(texture);
	fruitTexture.loadFromFile("src/Fruit.png");
	fruitSprite.setTexture(fruitTexture);

	spriteBoard.setTextureRect(IntRect(0, 0, size.x, size.y));
	spriteFood.setTextureRect(IntRect(size.x, 0, size.x, size.y));
	spriteDoor.setTextureRect(IntRect(2 * size.x, 0, size.x, size.y));
	spriteMegaFood.setTextureRect(IntRect(3 * size.x, 0, size.x, size.y));
	fruitSprite.setTextureRect(IntRect(0, 0, size.x, size.y));

	map =
	{
		"1111111111111111111111111111",
		"1            11            1",
		"1 1111 11111 11 11111 1111 1",
		"191111 11111 11 11111 111191",
		"1 1111 11111 11 11111 1111 1",
		"1                          1",
		"1 1111 11 11111111 11 1111 1",
		"1 1111 11 11111111 11 1111 1",
		"1      11    11    11      1",
		"111111 11111011011111 111111",
		"111111 11111011011111 111111",
		"111111 11000000000011 111111",
		"111111 11011155111011 111111",
		"111111 11014444441011 111111",
		"200000 00014444441000 000003",
		"111111 11014444441011 111111",
		"111111 11011111111011 111111",
		"111111 11000000000011 111111",
		"111111 11011111111011 111111",
		"111111 11011111111011 111111",
		"1            11            1",
		"1 1111 11111 11 11111 1111 1",
		"191111 11111 11 11111 111191",
		"1   11       00       11   1",
		"111 11 11 11111111 11 11 111",
		"111 11 11 11111111 11 11 111",
		"1      11    11    11      1",
		"1 1111111111 11 1111111111 1",
		"1 1111111111 11 1111111111 1",
		"1                          1",
		"1111111111111111111111111111",
		"0000000000000000000000000000"
	};
}

void Map::drawMap(RenderWindow& window)
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (map[i][j] == '1')
			{
				spriteBoard.setPosition(j * size.x, i * size.y);
				window.draw(spriteBoard);
			}
			if (map[i][j] == ' ')
			{
				spriteFood.setPosition(j * size.x, i * size.y);
				window.draw(spriteFood);
			}
			if (map[i][j] == '5')
			{
				spriteDoor.setPosition(j * size.x, i * size.y);
				window.draw(spriteDoor);
			}
			if (map[i][j] == '9')
			{
				spriteMegaFood.setPosition(j * size.x, i * size.y);
				window.draw(spriteMegaFood);
			}
			if (map[i][j] == '6')
			{
				fruitSprite.setPosition(j * size.x, i * size.y);
				window.draw(fruitSprite);
			}
		}
	}
}