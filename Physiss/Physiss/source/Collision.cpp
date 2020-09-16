#pragma once
#include "Collision.h"

Collision::Collision()
{

}

Collision::~Collision()
{

}

struct Collision::AABB
{
	sf::Vector2f min;
	sf::Vector2f max;
};

struct Collision::Circle
{
	float radius;
	sf::Vector2f position;
};

bool Collision::AABBvsAABB(AABB a, AABB b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

	return true;
}

bool Collision::CirclevsCircleUnoptimized(Circle a, Circle b)
{
	float r = a.radius + b.radius;
	return r < Distance(a.position, b.position);
}

bool Collision::CirclevsCircleOptimized(Circle a, Circle b)
{
	float r = a.radius + b.radius;
	r *= r;
	return r < (a.position.x + b.position.x) * (a.position.x + b.position.x) + (a.position.y + b.position.y) * (a.position.y + b.position.y);
}

void Collision::ResolveCollision(GameObject a, GameObject b)
{
	sf::Vector2f relativeVelocity = b.velocity - a.velocity;

	//asdf
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

/********************************************************/

//WORKING Circleshape Collision (OLD)
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

