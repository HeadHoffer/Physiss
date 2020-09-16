#include "GameObject.h"

struct GameObject::AABB
{
	sf::Vector2f min;
	sf::Vector2f max;
};

struct GameObject::Circle
{
	float radius;
	sf::Vector2f position;
};

GameObject::GameObject(float _mass, float _friction, float _size)
{
	mass = _mass;
	friction = _friction;
	size = _size;
}

GameObject::~GameObject()
{
}

void GameObject::SetValues(float _mass)
{
	mass = _mass;
}

void GameObject::SetValues(float _mass, float _friction)
{
	mass = _mass;
	friction = _friction;
}

void GameObject::SetValues(float _mass, float _friction, float _size)
{
	mass = _mass;
	friction = _friction;
	size = _size;
}
