#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/System.hpp>
#include "CubeGenerator.h"

int main()
{
	CubeGenerator cg;
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

		}

		window.clear();
		window.draw(shape);
		for (int i = 0; i < (int)arr.size(); ++i) {
			window.draw(arr[i]);
		}
		window.display();
	}

	return 0;
}