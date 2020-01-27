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
	std::vector<sf::RectangleShape> arr;
	std::vector<sf::CircleShape> circles;

	//sf::CircleShape shape(100.f);
	//sf::RectangleShape rec(sf::Vector2f(30,30));
	//rec.setOrigin(sf::Vector2f(-40, -40));
	//rec.setFillColor(sf::Color::Red);
	//shape.setFillColor(sf::Color::Green);
	//arr.push_back(rec);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			//if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
			//	arr.push_back(cg.NewCube());

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				arr.push_back(cg.NewCube(mPos));
			}

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				circles.push_back(cg.NewCircle(mPos));
			}
		}

		window.clear();
		//window.draw(shape);
		for (int i = 0; i < (int)arr.size(); ++i) {
			window.draw(arr[i]);
		}

		for (int i = 0; i < (int)circles.size(); ++i) {
			window.draw(circles[i]);
		}

		if ((int)arr.size() > 0)
		{
			for (auto x = arr.begin(); x != arr.end(); x++) {
				for (auto y = arr.begin(); y != arr.end(); y++) {
					if (x != y && col.CheckCollision(*x, *y))
						std::cout << "Collision happened\n";
				}
			}
		}
		if ((int)circles.size() > 0) 
		{
			for (auto z = circles.begin(); z != circles.end(); z++) {
				for (auto z2 = circles.begin(); z2 != circles.end(); z2++) {
					if (z != z2 && col.CheckCollision(*z, *z2))
						std::cout << "Circle collision happened\n";
				}
			}
		}

		window.display();
	}

	return 0;
}