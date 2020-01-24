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


	bool CheckCollsion(sf::CircleShape cir, sf::RectangleShape rec);

	bool CheckCollision(sf::CircleShape cir1, sf::CircleShape cir2);
	
};

//TODO: Collsion