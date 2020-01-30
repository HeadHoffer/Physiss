#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject(float mass, float friction, float size);
	~GameObject();

private:
	float _mass;
	float _friction;
	int _size;

	sf::RectangleShape rectangle;
	sf::CircleShape circle;
	
};

