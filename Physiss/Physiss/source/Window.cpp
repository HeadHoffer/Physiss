#include "Window.h"
#include "CubeGenerator.h"
#include "Collision.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>

Window::Window()
{
}


Window::~Window()
{
}

void Window::RenderWindow() {
	CubeGenerator cg;
	Collision col;
	//std::vector<sf::RectangleShape> arr;
	std::vector<sf::CircleShape> circles;
	//sf::CircleShape masterCircle = cg.NewCircle(sf::Vector2f(200, 200), 200, sf::Color::Cyan);

	//std::vector<sf::RectangleShape> hitBoxes;
	//std::vector<sf::CircleShape> hitBoxCorners;
	//std::cout << "Give square height: ";
	//std::cin >> rectHeight;
	//std::cout << "\nGive circle radius: ";
	//std::cin >> circleWidth;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				//sf::Vector2i mPos = sf::Mouse::getPosition(window);
				//sf::RectangleShape newCube = cg.NewCube(mPos, rectHeight);
				//std::vector<sf::CircleShape> corners = cg.CubeHitCircle(newCube, circleWidth);
				//std::vector<sf::RectangleShape> boxes = cg.CubeHitbox(newCube, circleWidth);
				//hitBoxCorners.insert(hitBoxCorners.end(), corners.begin(), corners.end());
				//hitBoxes.insert(hitBoxes.end(), boxes.begin(), boxes.end());
				//arr.push_back(newCube);

				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				sf::CircleShape newCircle = cg.NewCircle(mPos, circleWidth);
				//newCircle.setPosition(0 - mPos.x, 0 - mPos.y);
				circles.push_back(newCircle);
			}

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				sf::CircleShape newCircle = cg.NewCircle(mPos, circleWidth);
				//newCircle.setPosition(0 - mPos.x, 0 - mPos.y);
				circles.push_back(newCircle);
			}
		}
		for (int a = 0; a < (int)circles.size(); ++a) {
			col.CheckPosition(&circles[a]);
			for (int b = 0; b < (int)circles.size(); ++b) {
				if (a != b) {
					col.HandleCollision(&circles[a], &circles[b]/*, &masterCircle*/);
				}
			}
		}

		window.clear();
		//Draw(hitBoxes);
		//Draw(hitBoxCorners);
		//Draw(arr);
		//Draw(masterCircle);
		Draw(circles);

		window.display();
	}
}

void Window::Draw(sf::CircleShape circle) {
	window.draw(circle);
}

void Window::Draw(std::vector<sf::RectangleShape> shapes) {
	for (int i = 0; i < (int)shapes.size(); ++i) {
		window.draw(shapes[i]);
	}
}

void Window::Draw(std::vector<sf::CircleShape> shapes) {
	for (int i = 0; i < (int)shapes.size(); ++i) {
		window.draw(shapes[i]);
	}
}

void Window::Move(sf::CircleShape *circle) {
	circle->move(1, 1);
}
