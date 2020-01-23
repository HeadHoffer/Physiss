#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/System.hpp>
#include "CubeGenerator.h"
#include "Collision.h"
#include <iostream>

int main()
{
	CubeGenerator cg;
	Collision col;
	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	sf::CircleShape shape(100.f);
	sf::RectangleShape rec(sf::Vector2f(30,30));
	rec.setOrigin(sf::Vector2f(-40, -40));
	rec.setFillColor(sf::Color::Red);
	shape.setFillColor(sf::Color::Green);
	std::vector<sf::RectangleShape> arr;
	arr.push_back(rec);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
				arr.push_back(cg.NewCube());

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				arr.push_back(cg.NewCube(mPos));
				
			}

		}

		window.clear();
		window.draw(shape);
		for (int i = 0; i < (int)arr.size(); ++i) {
			window.draw(arr[i]);
		}

		if ((int)arr.size() > 1)
		{	
			if (col.CheckCollision(arr[0], arr[1]) == true)
				std::cout << "Collision happened";
			else
				std::cout << "no collision";

			//col.CheckCollision(arr[0], arr[1]);
			
		}

		window.display();
	}

	return 0;
}