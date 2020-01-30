#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();

	float mass;
	float friction;
	int size;

	sf::RectangleShape rectangle;
	sf::CircleShape circle;
	
};

