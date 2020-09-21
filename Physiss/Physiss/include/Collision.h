#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <GameObject.h>

class Collision
{
public:
	Collision();
	~Collision();

	struct AABB;
	struct Circle;

	//New collisions that hopefully work
	bool AABBvsAABB(AABB a, AABB b);
	bool CirclevsCircleUnoptimized(Circle a, Circle b);
	bool CirclevsCircleOptimized(Circle a, Circle b);

	void ResolveCollision(GameObject a, GameObject b);

	//Distance
	float Distance(sf::Vector2f pos1, sf::Vector2f pos2);
	sf::Vector2f VectorDistance(sf::Vector2f pos1, sf::Vector2f pos2);

	/*******************************************************************/

	//Circleshape collision
	void HandleCollision(sf::CircleShape* cir1, sf::CircleShape* cir2);
	bool CheckCollision(sf::CircleShape* cir1, sf::CircleShape* cir2);
	void CheckPosition(sf::CircleShape* cir);
	void CollisionImpulse(sf::CircleShape* cir1, sf::CircleShape* cir2);

	//Circleshape positioning
	void MoveToMaster(sf::CircleShape* cir, sf::CircleShape* master);
	void MoveToPlayArea(sf::CircleShape* cir);
};