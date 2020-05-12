#include <SFML/Graphics.hpp>
#include <time.h>


class CubeGenerator
{
public:
	CubeGenerator();
	~CubeGenerator();

	sf::RectangleShape NewCube();
	sf::RectangleShape NewCube(sf::Vector2i position);
	sf::RectangleShape NewCube(sf::Vector2i position, float height);

	std::vector<sf::RectangleShape> CubeHitbox(sf::RectangleShape rect, float width);
	std::vector<sf::CircleShape> CubeHitCircle(sf::RectangleShape rect, float width);

	sf::CircleShape NewCircle(sf::Vector2i position);
	sf::CircleShape NewCircle(sf::Vector2i position, float width);
	sf::CircleShape NewCircle(sf::Vector2f position, float width, sf::Color color);
};

