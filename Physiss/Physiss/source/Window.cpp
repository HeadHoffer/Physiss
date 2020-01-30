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
	std::vector<sf::RectangleShape> arr;
	std::vector<sf::CircleShape> circles;

	std::vector<sf::RectangleShape> hitBoxes;
	std::vector<sf::CircleShape> hitBoxCorners;
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
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				sf::RectangleShape newCube = cg.NewCube(mPos, rectHeight);
				std::vector<sf::CircleShape> corners = cg.CubeHitCircle(newCube, circleWidth);
				std::vector<sf::RectangleShape> boxes = cg.CubeHitbox(newCube, circleWidth);
				hitBoxCorners.insert(hitBoxCorners.end(), corners.begin(), corners.end());
				hitBoxes.insert(hitBoxes.end(), boxes.begin(), boxes.end());
				arr.push_back(newCube);
			}

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				sf::CircleShape newCircle = cg.NewCircle(mPos, circleWidth);
				//newCircle.setPosition(0 - mPos.x, 0 - mPos.y);
				circles.push_back(newCircle);
			}
		}
		col.HandleCollision(arr, circles);

		window.clear();
		Draw(hitBoxes);
		Draw(hitBoxCorners);
		Draw(arr);
		Draw(circles);

		window.display();
	}
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
