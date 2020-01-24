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
bool Collision::CheckCollsion(sf::CircleShape cir, sf::RectangleShape rec)
{
	sf::Vector2f center(cir.getOrigin().x - cir.getRadius(), cir.getOrigin.y - cir.getRadius());
	sf::Vector2f aabb_half_extents(rec.getSize().x / 2, rec.getSize().y / 2);

	sf::Vector2f aabb_center(rec.getPosition().x + aabb_half_extents.x, rec.getPosition().y + aabb_half_extents.y);

	sf::Vector2f difference = center - aabb_center;
}

//circle vs circle
bool Collision::CheckCollision(sf::CircleShape cir1, sf::CircleShape cir2)
{
	float r = cir1.getRadius() + cir2.getRadius();
	r *= r;

	sf::Vector2f center1(cir1.getOrigin().x - cir1.getRadius(), cir1.getOrigin().y - cir1.getRadius());
	sf::Vector2f center2(cir2.getOrigin().x - cir2.getRadius(), cir2.getOrigin().y - cir2.getRadius());

	/*if (r < (center1.x + center2.x) ^ 2 + (center1.y + center2.y) ^ 2)
	{
		return true;
	}
	else
		return false;*/

	
}

