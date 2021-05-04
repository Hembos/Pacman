#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <array>

class Map
{
	using Vector = sf::Vector2f;
private:
	int mapWidth;
	int mapHeight;
	Vector size;
	sf::Sprite spriteBoard;
	sf::Sprite spriteFood;
	sf::Sprite spriteDoor;
	sf::Sprite spriteMegaFood;
	std::vector<std::string> map;
public:
	Map(int mapWidth, int mapHeight, const sf::Texture& texture, Vector size);
	void drawMap(sf::RenderWindow& window);
	char getField(int i, int j) const { return map[i][j]; }
	void setField(char field, int i, int j) { map[i][j] = field; }
	Vector getSize() const { return size; }
	//sf::Vector2i getCell(Vector pos) const;
};