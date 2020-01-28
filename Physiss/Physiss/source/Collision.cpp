#pragma once
#include "Collision.h"

Collision::Collision()
{

}

Collision::~Collision()
{

}

//Rectangle collision
bool Collision::CheckCollision(sf::RectangleShape rec1, sf::RectangleShape rec2)
{
	//Check X axis collision
	bool collisionX = rec1.getOrigin().x + rec1.getSize().x >= rec2.getOrigin().x && rec2.getOrigin().x + rec2.getSize().x >= rec1.getOrigin().x;

	//Check Y axis collision
	bool collisionY = rec1.getOrigin().y + rec1.getSize().y >= rec2.getOrigin().y && rec2.getOrigin().y + rec2.getSize().y >= rec1.getOrigin().y;
	
	
	// Collision between 2 boxes happen only when both X and Y are true
	return collisionX && collisionY;
}


//Circle and rectangle collision
bool Collision::CheckCollision(sf::CircleShape cir, sf::RectangleShape rec)
{
	//Corners of a rectangle work as circle colliders
	float r = cir.getRadius();
	float sizeX = rec.getSize().x;
	float sizeY = rec.getSize().y;

	//Fuckery because of circle origins
	float orX = rec.getOrigin().x + r;
	float orY = rec.getOrigin().y + r;

	sf::Vector2f corner1(sf::Vector2f(orX, orY));
	sf::Vector2f corner2(sf::Vector2f(orX - sizeX, orY));
	sf::Vector2f corner3(sf::Vector2f(orX, orY - sizeY));
	sf::Vector2f corner4(sf::Vector2f(orX - sizeX, orY - sizeY));

	std::vector<sf::Vector2f> corners{ corner1, corner2, corner3, corner4 };

	for (auto x = corners.begin(); x != corners.end(); x++)
	{
		//std::cout << "DISTANCE: " << Collision::Distance(*x, cir.getOrigin()) << "\n";
		if ( r >= Collision::Distance(*x, cir.getOrigin()))
		{
			std::cout << "Corner hit\n";
			return true;
		}
	}

	//If circle doesn't hit corners, test rectangle colliders
	sf::RectangleShape yRec(sf::Vector2f(rec.getSize().x, rec.getSize().y + 2 * r));
	sf::RectangleShape xRec(sf::Vector2f(rec.getSize().x + 2 * r, rec.getSize().y));
	
	//Position is negative, therefore origin is origin minus size
	yRec.setOrigin(sf::Vector2f(rec.getOrigin().x - rec.getSize().x + r, rec.getOrigin().y - rec.getSize().y));
	xRec.setOrigin(sf::Vector2f(rec.getOrigin().x - rec.getSize().x, rec.getOrigin().y - rec.getSize().y + r));

	float cirX = cir.getOrigin().x;
	float cirY = cir.getOrigin().y;

	if (yRec.getOrigin().x + yRec.getSize().x >= cirX
		&& cirX >= yRec.getOrigin().x
		&& yRec.getOrigin().y + yRec.getSize().y >= cirY
		&& cirY >= yRec.getOrigin().y)
	{
		std::cout << "Y-rectangle hit\n";
		return true;
	}

	if (xRec.getOrigin().x + xRec.getSize().x >= cirX
		&& cirX >= xRec.getOrigin().x
		&& xRec.getOrigin().y + xRec.getSize().y >= cirY 
		&& cirY >= xRec.getOrigin().y)
	{
		std::cout << "X-rectangle hit\n";
		return true;
	}

	return false;
}

//Circle collision
bool Collision::CheckCollision(sf::CircleShape cir1, sf::CircleShape cir2)
{
	float r = cir1.getRadius() + cir2.getRadius();
	return r >= Collision::Distance(cir1.getOrigin(), cir2.getOrigin());
}

//Returns distance between two points
float Collision::Distance(sf::Vector2f pos1, sf::Vector2f pos2) {
	float x = pos1.x - pos2.x;
	float y = pos1.y - pos2.y;
	x *= x;
	y *= y;
	return sqrtf(x + y);
}

