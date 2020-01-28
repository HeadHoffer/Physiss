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
	sf::RenderWindow window(sf::VideoMode(500, 500), "Sometimes it doesn't just work");
	std::vector<sf::RectangleShape> arr;
	std::vector<sf::CircleShape> circles;

	float circleWidth = 30.f;
	std::vector<sf::RectangleShape> hitBoxes;
	std::vector<sf::CircleShape> hitBoxCorners;

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
				sf::RectangleShape newCube = cg.NewCube(mPos);
				std::vector<sf::CircleShape> corners = cg.CubeHitCircle(newCube, circleWidth);
				std::vector<sf::RectangleShape> boxes = cg.CubeHitbox(newCube, circleWidth);
				hitBoxCorners.insert(hitBoxCorners.end(), corners.begin(), corners.end());
				hitBoxes.insert(hitBoxes.end(), boxes.begin(), boxes.end());
				arr.push_back(newCube);
			}

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				sf::CircleShape newCircle = cg.NewCircle(mPos, circleWidth);
				circles.push_back(newCircle);
			}
		}

		if ((int)arr.size() > 0) {
			for (auto x = arr.begin(); x != arr.end(); x++) {
				for (auto y = arr.begin(); y != arr.end(); y++) {
					if (x != y && col.CheckCollision(*x, *y))
						//std::cout << "Collision happened\n";
						continue;
				}
			}
		}
		if ((int)circles.size() > 0) {
			for (auto z = circles.begin(); z != circles.end(); z++) {
				for (auto z2 = circles.begin(); z2 != circles.end(); z2++) {
					if (z != z2 && col.CheckCollision(*z, *z2))
						//std::cout << "Circle collision happened\n";
						continue;
				}
			}
		}
		if ((int)circles.size() > 0 && (int)arr.size() > 0) {
			for (auto r = arr.begin(); r != arr.end(); r++) {
				for (auto c = circles.begin(); c != circles.end(); c++) {
					if (col.CheckCollision(*c, *r))
						std::cout << "Circle has met rectangle\n";
				}
			}
		}
		window.clear();
		for (int i = 0; i < (int)hitBoxes.size(); ++i) {
			window.draw(hitBoxes[i]);
		}
		for (int i = 0; i < (int)hitBoxCorners.size(); ++i) {
			window.draw(hitBoxCorners[i]);
		}
		for (int i = 0; i < (int)circles.size(); ++i) {
			window.draw(circles[i]);
		}
		for (int i = 0; i < (int)arr.size(); ++i) {
			window.draw(arr[i]);
		}

		window.display();
	}

	return 0;
}