#pragma once
#include "Collision.h"

Collision::Collision()
{

}

Collision::~Collision()
{

}

bool Collision::CheckCollision(sf::RectangleShape rec1, sf::RectangleShape rec2)
{
	//Check X axis collision
	bool collisionX = rec1.getOrigin().x + rec1.getSize().x >= rec2.getOrigin().x && rec2.getOrigin().x + rec2.getSize().x >= rec1.getOrigin().x;

	//Check Y axis collision
	bool collisionY = rec1.getOrigin().y + rec1.getSize().y >= rec2.getOrigin().y && rec2.getOrigin().y + rec2.getSize().y >= rec1.getOrigin().y;
	
	
	// Collision between 2 boxes happen only when both X and Y are true

	return collisionX && collisionY;
}


//circle vs square 
bool Collision::CheckCollision(sf::CircleShape cir, sf::RectangleShape rec)
{
	//sf::Vector2f center(cir.getOrigin().x - cir.getRadius(), cir.getOrigin().y - cir.getRadius());
	sf::Vector2f aabb_half_extents(rec.getSize().x / 2, rec.getSize().y / 2);

	sf::Vector2f aabb_center(rec.getPosition().x + aabb_half_extents.x, rec.getPosition().y + aabb_half_extents.y);

	sf::Vector2f difference = cir.getOrigin() - aabb_center;
	sf::Vector2f clamped = std::clamp(difference, -aabb_half_extents, aabb_half_extents);

	sf::Vector2f closest = aabb_center + clamped;

	difference = closest - cir.getOrigin();

	return Collision::Distance(difference, difference ) < cir.getRadius();

	//sf::Vector2f difference = center - aabb_center;

}

//circle vs circle
bool Collision::CheckCollision(sf::CircleShape cir1, sf::CircleShape cir2)
{
	float r = cir1.getRadius() + cir2.getRadius();
	return r >= Collision::Distance(cir1.getOrigin(), cir2.getOrigin());
}

float Collision::Distance(sf::Vector2f pos1, sf::Vector2f pos2) {
	float x = pos1.x - pos2.x;
	float y = pos1.y - pos2.y;
	x *= x;
	y *= y;
	return sqrtf(x + y);
}

