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

	//std::cout << "COLLIDER X: " << rec.getOrigin().x << "\n";
	//std::cout << "COLLIDER Y: " << rec.getOrigin().y << "\n";

	float sizeX = rec.getSize().x;
	float sizeY = rec.getSize().y;
	float orX = rec.getOrigin().x;
	float orY = rec.getOrigin().y;

	sf::Vector2f corner1(sf::Vector2f(orX, orY));
	sf::Vector2f corner2(sf::Vector2f(orX - sizeX, orY));
	sf::Vector2f corner3(sf::Vector2f(orX, orY - sizeY));
	sf::Vector2f corner4(sf::Vector2f(orX - sizeX, orY - sizeY));
	//std::cout << "CORNER4: " << orX - sizeX << ", " << orY - sizeY << "\n";

	std::vector<sf::Vector2f> corners{ corner1, corner2, corner3, corner4 };

	float r = cir.getRadius();
	

	for (auto x = corners.begin(); x != corners.end(); x++)
	{
		std::cout << "DISTANCE: " << Collision::Distance(*x, cir.getOrigin()) << "\n";
		if ( r >= Collision::Distance(*x, cir.getOrigin()))
		{
			std::cout << "Corners work\n";
			return true;
		}
	}

	sf::RectangleShape yRec(sf::Vector2f(rec.getSize().x, rec.getSize().y + 2 * r));
	sf::RectangleShape xRec(sf::Vector2f(rec.getSize().x + 2 * r, rec.getSize().y));
	yRec.setOrigin(sf::Vector2f(rec.getOrigin().x, rec.getOrigin().y + r));
	xRec.setOrigin(sf::Vector2f(rec.getOrigin().x + r, rec.getOrigin().y));

	if (yRec.getOrigin().x + yRec.getSize().x >= cir.getOrigin().x && cir.getOrigin().x >= yRec.getOrigin().x
		&& yRec.getOrigin().y + yRec.getSize().y >= cir.getOrigin().y && cir.getOrigin().y >= yRec.getOrigin().y)
	{
		std::cout << "YREC WORKS\n";
		return true;
	}

	if (xRec.getOrigin().x + xRec.getSize().x >= cir.getOrigin().x && cir.getOrigin().x >= xRec.getOrigin().x
		&& xRec.getOrigin().y + xRec.getSize().y >= cir.getOrigin().y && cir.getOrigin().y >= xRec.getOrigin().y)
	{
		std::cout << "XREC WORKS\n";
		return true;
	}


	////sf::Vector2f center(cir.getOrigin().x - cir.getRadius(), cir.getOrigin().y - cir.getRadius());
	//sf::Vector2f aabb_half_extents(rec.getSize().x / 2, rec.getSize().y / 2);

	//sf::Vector2f aabb_center(rec.getPosition().x + aabb_half_extents.x, rec.getPosition().y + aabb_half_extents.y);

	//sf::Vector2f difference(cir.getOrigin() - aabb_center);
	//sf::Vector2f clamped(std::clamp(difference, -aabb_half_extents, aabb_half_extents));

	//sf::Vector2f closest(aabb_center + clamped);

	//rec.

	//difference = closest - cir.getOrigin();

	//return Collision::Distance(difference, difference ) < cir.getRadius();

	//sf::Vector2f difference = center - aabb_center;

	return false;
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

