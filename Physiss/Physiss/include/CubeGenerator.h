#include <SFML/Graphics.hpp>
#include <time.h>


class CubeGenerator
{
public:
	CubeGenerator();
	~CubeGenerator();

	sf::RectangleShape NewCube();
	sf::RectangleShape NewCube(sf::Vector2i position);

	sf::CircleShape NewCircle(sf::Vector2i position);
};

