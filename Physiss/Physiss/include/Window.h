#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Collision.h"

class Window
{
public:
	Window();
	~Window();

	void RenderWindow();

	float circleWidth = 30.f;
	float rectHeight = 150;

	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(500, 500), "Sometimes it doesn't just work");

private:
	void Draw(std::vector<sf::RectangleShape> shapes);
	void Draw(std::vector<sf::CircleShape> shapes);
};

