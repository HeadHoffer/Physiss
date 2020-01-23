#include "CubeGenerator.h"
#include <iostream>
#include <string.h>

CubeGenerator::CubeGenerator()
{
}


CubeGenerator::~CubeGenerator()
{
}

sf::RectangleShape CubeGenerator::NewCube() {
	sf::RectangleShape rect(sf::Vector2f(30, 30));
	float rand1 = 0 - rand() / 100;
	float rand2 = 0 - rand() / 100;
	rect.setOrigin(sf::Vector2f(rand1, rand2));
	rect.setFillColor(sf::Color::Red);
	return rect;
}

sf::RectangleShape CubeGenerator::NewCube(sf::Vector2i position) {
	sf::RectangleShape rect(sf::Vector2f(30, 30));
	std::cout << "X: " << position.x << "\n";
	std::cout << "Y: " << position.y << "\n";
	//Position fuckery: window positions are actually negative
	rect.setOrigin(sf::Vector2f(0 - position.x + 15, 0 - position.y + 15));
	rect.setFillColor(sf::Color::Cyan);
	return rect;
}
