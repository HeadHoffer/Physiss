#include "GameObject.h"



GameObject::GameObject(float mass, float friction, float size)
{
	_mass = mass;
	_friction = friction;
	_size = size;
}


GameObject::~GameObject()
{
}

