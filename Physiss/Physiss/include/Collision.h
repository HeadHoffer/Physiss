#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Collision
{
public:
	Collision();
	~Collision();


	bool CheckCollision(sf::RectangleShape rec1, sf::RectangleShape rec2)
	{
		//Check X axis collision
		bool collisionX = rec1.getPosition().x + rec1.getSize().x >= rec2.getPosition().x && rec2.getPosition().x + rec2.getSize().x >= rec1.getPosition().x;

		//Check Y axis collision
		bool collisionY = rec1.getPosition().y + rec1.getSize().y >= rec2.getPosition().y && rec2.getPosition().y + rec2.getSize().y >= rec1.getPosition().y;

		// Collision between 2 boxes happen only when both X and Y are true

		return collisionX && collisionY;
	}

	/*bool CheckCollsion(sf::CircleShape cir, sf::RectangleShape rec)
	{
		sf::Vector2f center(cir.getOrigin().x - cir.getRadius(), cir.getOrigin.y - cir.getRadius());
		sf::Vector2f aabb_half_extents(rec.getSize().x / 2, rec.getSize().y / 2);

		sf::Vector2f aabb_center( rec.getPosition().x + aabb_half_extents.x, rec.getPosition().y + aabb_half_extents.y) ;

		sf::Vector2f difference = center - aabb_center;

		

	}*/
};

//TODO: Collsion