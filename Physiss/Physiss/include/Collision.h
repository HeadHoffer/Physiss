#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

class Collision
{
public:
	Collision();
	~Collision();

	void HandleCollision(std::vector<sf::RectangleShape> rects, std::vector<sf::CircleShape> circles);

	bool CheckCollision(sf::RectangleShape rec1, sf::RectangleShape rec2);
	bool CheckCollision(sf::CircleShape cir, sf::RectangleShape rec);
	bool CheckCollision(sf::CircleShape cir1, sf::CircleShape cir2);

	void CollisionImpulse(sf::CircleShape cir1, sf::CircleShape cir2);

	float Distance(sf::Vector2f pos1, sf::Vector2f pos2);
	sf::Vector2f VectorDistance(sf::Vector2f pos1, sf::Vector2f pos2);
	
};