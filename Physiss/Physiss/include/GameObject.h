#pragma once
#include <SFML/Graphics.hpp>
#include <Collision.h>

class GameObject : Collision
{
public:
	GameObject(float mass, float friction, float size);
	~GameObject();


	float mass;
	float friction;
	int size;

	sf::RectangleShape rectangle;
	sf::CircleShape circle;
	
};

