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
	/*rect.setPosition(sf::Vector2f(rand1, rand2));*/
	rect.setOrigin(sf::Vector2f(rand1, rand2));
	rect.setFillColor(sf::Color::Red);
	return rect;
}

sf::RectangleShape CubeGenerator::NewCube(sf::Vector2i position) {
	float length = 200;
	sf::RectangleShape rect(sf::Vector2f(length, length));
	std::cout << "ORIGINAL X: " << position.x + length / 2 << "\n";
	std::cout << "ORIGINAL Y: " << position.y + length / 2 << "\n";
	std::cout << "REAL CORNER4: " << position.x - length / 2 << ", " << 0 - position.y - length / 2 << "\n";
	//Position fuckery: window positions are actually negative
	/*rect.setPosition(sf::Vector2f(0 - position.x + 15, 0 - position.y + 15));*/
	rect.setPosition(sf::Vector2f(position.x + length / 2, position.y + length / 2));
	rect.setFillColor(sf::Color::Cyan);
	return rect;
}

sf::RectangleShape CubeGenerator::NewCube(sf::Vector2i position, float height) {
	sf::RectangleShape rect(sf::Vector2f(height, height));
	std::cout << "ORIGINAL X: " << position.x - height / 2 << "\n";
	std::cout << "ORIGINAL Y: " << position.y - height / 2 << "\n";
	std::cout << "REAL CORNER4: " << position.x + height / 2 << ", " << position.y + height / 2 << "\n";
	rect.setPosition(sf::Vector2f(position.x - height / 2, position.y - height / 2));
	rect.setFillColor(sf::Color::Cyan);
	return rect;
}

sf::CircleShape CubeGenerator::NewCircle(sf::Vector2i position) {
	sf::CircleShape circ(30.f);
	//std::cout << "X: " << position.x << "\n";
	//std::cout << "Y: " << position.y << "\n";
	circ.setPosition(sf::Vector2f(position.x - 30, position.y - 30));
	circ.setFillColor(sf::Color::Green);
	return circ;
}

sf::CircleShape CubeGenerator::NewCircle(sf::Vector2i position, float width) {
	sf::CircleShape circ(width);
	std::cout << "X: " << position.x - width << "\n";
	std::cout << "Y: " << position.y - width << "\n";
	circ.setPosition(sf::Vector2f(position.x - width, position.y - width));
	circ.setFillColor(sf::Color::Green);
	return circ;
}

std::vector <sf::RectangleShape> CubeGenerator::CubeHitbox(sf::RectangleShape rect, float width) {
	sf::RectangleShape yRec(sf::Vector2f(rect.getSize().x, rect.getSize().y + 2 * width));
	sf::RectangleShape xRec(sf::Vector2f(rect.getSize().x + 2 * width, rect.getSize().y));
	yRec.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y - width));
	xRec.setPosition(sf::Vector2f(rect.getPosition().x - width, rect.getPosition().y));
	std::cout << "REAL YREC ORIGIN: X: " << yRec.getPosition().x << " Y: " << yRec.getPosition().y << "\n";
	std::cout << "REAL XREC ORIGIN: X: " << xRec.getPosition().x << " Y: " << xRec.getPosition().y << "\n";

	xRec.setFillColor(sf::Color::Yellow);
	yRec.setFillColor(sf::Color::Yellow);
	return std::vector<sf::RectangleShape> {xRec, yRec};
}

std::vector <sf::CircleShape> CubeGenerator::CubeHitCircle(sf::RectangleShape rect, float width) {
	std::vector<sf::CircleShape> cornerCircles;

	float sizeX = rect.getSize().x;
	float sizeY = rect.getSize().y;
	float orX = rect.getPosition().x - width;
	float orY = rect.getPosition().y - width;

	sf::Vector2f corner1(sf::Vector2f(orX, orY));
	sf::Vector2f corner2(sf::Vector2f(orX + sizeX, orY));
	sf::Vector2f corner3(sf::Vector2f(orX, orY + sizeY));
	sf::Vector2f corner4(sf::Vector2f(orX + sizeX, orY + sizeY));
	std::vector<sf::Vector2f> corners{ corner1,corner2,corner3,corner4 };

	for (auto x = corners.begin(); x != corners.end(); x++) {
		sf::CircleShape circ(width);
		circ.setPosition(*x);
		circ.setFillColor(sf::Color::Red);
		cornerCircles.push_back(circ);
	}

	return cornerCircles;
}