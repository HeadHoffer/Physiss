#pragma once
#include <iostream>
#include <algorithm>
#include <GameObject.h>
#include <Collision.h>
#include <SFML/System.hpp>

class Physics
{
public:
	Physics();
	~Physics();

	float Velocity(GameObject go);
	float Impulse(GameObject go);
	float Restitution(GameObject go);

	void ResolveCollision(GameObject go1, GameObject go2);

private:
	float g = 9.81;
};

