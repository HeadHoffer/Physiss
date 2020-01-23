#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Collision
{
public:
	Collision();
	~Collision();


	bool CheckCollision(sf::RectangleShape rec1, sf::RectangleShape rec2);


	/*bool CheckCollsion(sf::CircleShape cir, sf::RectangleShape rec)
	{
		sf::Vector2f center(cir.getOrigin().x - cir.getRadius(), cir.getOrigin.y - cir.getRadius());
		sf::Vector2f aabb_half_extents(rec.getSize().x / 2, rec.getSize().y / 2);

		sf::Vector2f aabb_center( rec.getPosition().x + aabb_half_extents.x, rec.getPosition().y + aabb_half_extents.y) ;

		sf::Vector2f difference = center - aabb_center;

		

	}*/
};

//TODO: Collsion