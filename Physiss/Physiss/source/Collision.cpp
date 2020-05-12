#pragma once
#include "Collision.h"

Collision::Collision()
{

}

Collision::~Collision()
{

}

//Handle collision for all squares and circles
void Collision::HandleCollision(std::vector<sf::RectangleShape> rects, std::vector<sf::CircleShape> circles) {
	if ((int)rects.size() > 0) {
		for (auto x = rects.begin(); x != rects.end(); x++) {
			for (auto y = rects.begin(); y != rects.end(); y++) {
				if (x != y && CheckCollision(*x, *y))
					std::cout << "Squares collided!\n";
					//continue;
			}
		}
	}
	if ((int)circles.size() > 0) {
		for (auto &z : circles) {
			for (auto &z2 : circles) {
				if (&z != &z2 && CheckCollision(z, z2)) {
					//std::cout << "Circles collided!\n";
					//Pointers are fucking awful
					CollisionImpulse(z, z2);
				}
			}
		}
	}
	if ((int)circles.size() > 0 && (int)rects.size() > 0) {
		for (auto r = rects.begin(); r != rects.end(); r++) {
			for (auto c = circles.begin(); c != circles.end(); c++) {
				if (CheckCollision(*c, *r))
					std::cout << "Circle has met rectangle\n";
			}
		}
	}
}

//Rectangle collision
bool Collision::CheckCollision(sf::RectangleShape rec1, sf::RectangleShape rec2)
{
	//Check X axis collision
	bool collisionX = rec1.getPosition().x + rec1.getSize().x >= rec2.getPosition().x && rec2.getPosition().x + rec2.getSize().x >= rec1.getPosition().x;

	//Check Y axis collision
	bool collisionY = rec1.getPosition().y + rec1.getSize().y >= rec2.getPosition().y && rec2.getPosition().y + rec2.getSize().y >= rec1.getPosition().y;
	
	
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
	float orX = rec.getPosition().x - r;
	float orY = rec.getPosition().y - r;

	sf::Vector2f corner1(sf::Vector2f(orX, orY));
	sf::Vector2f corner2(sf::Vector2f(orX + sizeX, orY));
	sf::Vector2f corner3(sf::Vector2f(orX, orY + sizeY));
	sf::Vector2f corner4(sf::Vector2f(orX + sizeX, orY + sizeY));

	std::vector<sf::Vector2f> corners{ corner1, corner2, corner3, corner4 };

	for (auto x = corners.begin(); x != corners.end(); x++)
	{
		//std::cout << "DISTANCE: " << Collision::Distance(*x, cir.getOrigin()) << "\n";
		if ( r >= Collision::Distance(*x, cir.getPosition()))
		{
			std::cout << "Corner hit\n";
			return true;
		}
	}

	//If circle doesn't hit corners, test rectangle colliders
	sf::RectangleShape yRec(sf::Vector2f(rec.getSize().x, rec.getSize().y + 2 * r));
	sf::RectangleShape xRec(sf::Vector2f(rec.getSize().x + 2 * r, rec.getSize().y));
	
	yRec.setPosition(sf::Vector2f(rec.getPosition().x, rec.getPosition().y - 2 * r));
	xRec.setPosition(sf::Vector2f(rec.getPosition().x - 2 * r, rec.getPosition().y));

	float cirX = cir.getPosition().x;
	float cirY = cir.getPosition().y;

	if (yRec.getPosition().x + yRec.getSize().x >= cirX
		&& cirX >= yRec.getPosition().x
		&& yRec.getPosition().y + yRec.getSize().y >= cirY
		&& cirY >= yRec.getPosition().y)
	{
		std::cout << "Y-rectangle hit\n";
		return true;
	}

	if (xRec.getPosition().x + xRec.getSize().x >= cirX
		&& cirX >= xRec.getPosition().x
		&& xRec.getPosition().y + xRec.getSize().y >= cirY 
		&& cirY >= xRec.getPosition().y)
	{
		std::cout << "X-rectangle hit\n";
		return true;
	}

	return false;
}

//WORKING Circle Collision
void Collision::HandleCollision(sf::CircleShape *cir1, sf::CircleShape *cir2)
{
	if(CheckCollision(cir1, cir2))
		CollisionImpulse(cir1, cir2);
}

void Collision::CheckPosition(sf::CircleShape *cir)
{
	if (!(cir->getPosition().x < 500 && cir->getPosition().x > 0) || !(cir->getPosition().y < 500 && cir->getPosition().y > 0))
	{
		//std::cout << "Out of bounds!";
		MoveToPlayArea(cir);
	}
}

bool Collision::CheckCollision(sf::CircleShape* cir1, sf::CircleShape *cir2)
{
	float r = cir1->getRadius() + cir2->getRadius();
	return r >= Distance(cir1->getPosition(), cir2->getPosition());
}

void Collision::CollisionImpulse(sf::CircleShape *cir1, sf::CircleShape *cir2)
{
	sf::Vector2f r = VectorDistance(cir1->getPosition(), cir2->getPosition());
	cir1->move(r);
}

void Collision::MoveToMaster(sf::CircleShape *cir, sf::CircleShape *master)
{
	sf::Vector2f r = VectorDistance(cir->getPosition(), master->getPosition());
	float percentage = cir->getRadius() / (cir->getRadius() + master->getRadius());
	std::cout << percentage << "%\n";
	cir->move(r);
}

void Collision::MoveToPlayArea(sf::CircleShape *cir)
{
	int randX = rand() % 500;
	int randY = rand() % 500;
	cir->setPosition(sf::Vector2f(randX, randY));
}

//Circle collision
bool Collision::CheckCollision(sf::CircleShape cir1, sf::CircleShape cir2)
{
	float r = cir1.getRadius() + cir2.getRadius();
	return r >= Distance(cir1.getPosition(), cir2.getPosition());
}

//Returns distance between two points
float Collision::Distance(sf::Vector2f pos1, sf::Vector2f pos2) {
	float x = pos1.x - pos2.x;
	float y = pos1.y - pos2.y;
	x *= x;
	y *= y;
	return sqrtf(x + y);
}

//Returns a vector between two positions
sf::Vector2f Collision::VectorDistance(sf::Vector2f pos1, sf::Vector2f pos2) {
	return sf::Vector2f(pos1.x - pos2.x, pos1.y - pos2.y);
}

void Collision::CollisionImpulse(sf::CircleShape cir1, sf::CircleShape cir2) {
	sf::Vector2f r = VectorDistance(cir1.getPosition(), cir2.getPosition());
	std::cout << "Vector: x: " << r.x << " y: " << r.y << "\n";
	std::cout << "Cir1 position? " << cir1.getPosition().x << ", " << cir1.getPosition().y << "\n";
	std::cout << "Cir2 position? " << cir2.getPosition().x << ", " << cir2.getPosition().y << "\n";
	cir1.move(r.x, r.y);
}

