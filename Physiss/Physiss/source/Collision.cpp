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
	
	//std::cout << "rec1 X pos: " <<rec1.getOrigin().x << "rec2 x pos: " <<rec2.getOrigin().x << "rec1 size X: " <<rec1.getSize().x << "rec2 size X: " << rec2.getSize().x;
	//std::cout << "rec1 y pos: " << rec1.getOrigin().y << "rec2 y pos: " << rec2.getOrigin().y << "rec1 size y: " << rec1.getSize().y << "rec2 size y: " << rec2.getSize().y;
	// Collision between 2 boxes happen only when both X and Y are true

	return collisionX && collisionY;
}