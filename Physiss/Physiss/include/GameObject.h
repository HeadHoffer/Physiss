#pragma once
#include <SFML/Graphics.hpp>
//#include <Collision.h>

class GameObject
{
public:
	GameObject(float _mass, float _friction, float _size);
	~GameObject();

	struct AABB;
	struct Circle;

	void SetValues(float _mass);
	void SetValues(float _mass, float _friction);
	void SetValues(float _mass, float _friction, float _size);

	//void SetAABB(AABB a);
	//void SetCircle(Circle a);

	float mass = 0.f;
	float friction = 0.f;
	int size = 0.f;

	sf::Vector2f velocity;

	/*************************************************/
	sf::RectangleShape rectangle;
	sf::CircleShape circle;
	
};

