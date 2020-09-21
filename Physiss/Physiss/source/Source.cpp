#include <iostream>
#include <string>

#include "Window.h"
#include "olcConsoleGameEngine.h"

struct Circle
{
	float posX, posY;
	float velX, velY;
	float accX, accY;
	float radius;
	float mass;

	int id;
};

class CirclePhysics : public olcConsoleGameEngine
{
public:
	CirclePhysics()
	{
		m_sAppName = L"Physiss";
	}

private:
	std::vector<std::pair<float, float>> modelCircle;
	std::vector<Circle> circles;
	Circle* selectedCircle = nullptr;

	/// <summary>
	/// Spawns a circle in given coordinates
	/// </summary>
	/// <param name="x">coordinate</param>
	/// <param name="y">coordinate</param>
	/// <param name="r">Circle radius</param>
	void AddCircle(float x, float y, float r = 5.0f)
	{
		Circle c;
		c.posX = x; c.posY = y;
		c.velX = 0; c.velY = 0;
		c.accX = 0; c.accY = 0;
		c.radius = r;
		c.mass = r * 10.0f;

		c.id = circles.size();
		circles.emplace_back(c);
	}



public:
	/// <summary>
	/// Window startup function
	/// </summary>
	bool OnUserCreate()
	{
		modelCircle.push_back({ 0.0f, 0.0f });
		int nPoints = 20;
		for (int i = 0; i < nPoints; i++)
			modelCircle.push_back({ cosf(i / (float)(nPoints - 1) * 2.0f * 3.14159f) , sinf(i / (float)(nPoints - 1) * 2.0f * 3.14159f) });

		float defaultRad = 10.0f;
		//AddCircle(ScreenWidth() * 0.25f, ScreenHeight() * 0.5f, defaultRad);
		//AddCircle(ScreenWidth() * 0.75f, ScreenHeight() * 0.5f, defaultRad);
		for (int i = 0; i < 10; i++)
		{
			AddCircle(rand() % ScreenWidth(), rand() % ScreenHeight(), rand() % 16 + 2);
		}

		return true;
	}

	/// <summary>
	/// Basic update function
	/// </summary>
	bool OnUserUpdate(float fElapsedTime)
	{
		//auto CircleCollision = [](Circle *cir1, Circle *cir2)
		//{
		//	return fabs((cir1->posX - cir2->posX) * (cir1->posX - cir2->posX) + (cir1->posY - cir2->posY) * (cir1->posY - cir2->posY) <= (cir1->radius + cir2->radius) * (cir1->radius + cir2->radius));
		//};
		auto CircleCollision = [](Circle cir1, Circle cir2)
		{
			return fabs((cir1.posX - cir2.posX) * (cir1.posX - cir2.posX) + (cir1.posY - cir2.posY) * (cir1.posY - cir2.posY)) <= (cir1.radius + cir2.radius) * (cir1.radius + cir2.radius);
		};
		auto IsPointInCircle = [](Circle circle, float x, float y)
		{
			return fabs((circle.posX - x) * (circle.posX - x) + (circle.posY - y) * (circle.posY - y)) < (circle.radius * circle.radius);
		};
		auto Distance = [](float x1, float y1, float x2, float y2)
		{
			return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		};

		if (m_mouse[0].bPressed || m_mouse[1].bPressed)
		{
			selectedCircle = nullptr;
			for (auto& circle : circles)
			{
				if (IsPointInCircle(circle, m_mousePosX, m_mousePosY))
				{
					selectedCircle = &circle;
					break;
				}
			}
		}

		if (m_mouse[0].bHeld)
		{
			if (selectedCircle != nullptr) //Dragging circles
			{
				selectedCircle->posX = m_mousePosX;
				selectedCircle->posY = m_mousePosY;
			}
		}

		if (m_mouse[0].bReleased)
		{
			selectedCircle = nullptr;
		}

		if (m_mouse[1].bReleased) //Apply velocity with right mouse button
		{
			if (selectedCircle != nullptr)
			{
				selectedCircle->velX = 5.0f * ((selectedCircle->posX) - (float)m_mousePosX);
				selectedCircle->velY = 5.0f * ((selectedCircle->posY) - (float)m_mousePosY);
			}
			selectedCircle = nullptr;
		}

		std::vector<std::pair<Circle*, Circle*>> vecCollidingPairs;

		//Update circle positions
		for (auto& circle : circles)
		{
			circle.accX = -circle.velX * 0.8f;
			circle.accY = -circle.velY * 0.8f;
			circle.velX += circle.accX * fElapsedTime;
			circle.velY += circle.accY * fElapsedTime;
			circle.posX += circle.velX * fElapsedTime;
			circle.posY += circle.velY * fElapsedTime;

			//Circles come back around if they leave the screen area
			if (circle.posX < 0) circle.posX += (float)ScreenWidth();
			if (circle.posX > ScreenWidth()) circle.posX -= (float)ScreenWidth();
			if (circle.posY < 0) circle.posY += (float)ScreenHeight();
			if (circle.posY > ScreenHeight()) circle.posY -= (float)ScreenHeight();

			//Estimated stopping point
			if (fabs(circle.velX * circle.velX + circle.velY * circle.velY) < 0.01f)
			{
				circle.velX = 0;
				circle.velY = 0;
			}
		}

		for (auto &circle : circles)
		{
			for (auto &target : circles)
			{
				if (circle.id != target.id)
				{
					if (CircleCollision(circle, target))
					{
						//Collision has occured
						vecCollidingPairs.push_back({ &circle, &target });

						//Distance calculation
						float fDistance = Distance(circle.posX, circle.posY, target.posX, target.posY);
						float fOverlap = 0.5f * (fDistance - circle.radius - target.radius);

						//Displace
						circle.posX -= fOverlap * (circle.posX - target.posX) / fDistance;
						circle.posY -= fOverlap * (circle.posY - target.posY) / fDistance;
						target.posX += fOverlap * (circle.posX - target.posX) / fDistance;
						target.posY += fOverlap * (circle.posY - target.posY) / fDistance;
					}
				}
			}
		}

		//Dynamic collisions
		for (auto c : vecCollidingPairs)
		{
			Circle* c1 = c.first;
			Circle* c2 = c.second;

			float fDistance = Distance(c1->posX, c1->posY, c2->posX, c2->posY);

			//Normals
			float nx = (c2->posX - c1->posX) / fDistance;
			float ny = (c2->posY - c1->posY) / fDistance;

			//Tangent
			float tx = -ny;
			float ty = nx;

			//Dot product tangent
			float dpTan1 = c1->velX * tx + c1->velY * ty;
			float dpTan2 = c2->velX * tx + c2->velY * ty;

			//Dot product normal
			float dpNorm1 = c1->velX * nx + c1->velY * ny;
			float dpNorm2 = c2->velX * nx + c2->velY * ny;

			//Conservation of momentum in 1D
			float m1 = (dpNorm1 * (c1->mass - c2->mass) + 2.0f * c2->mass * dpNorm2) / (c1->mass + c2->mass);
			float m2 = (dpNorm2 * (c2->mass - c1->mass) + 2.0f * c1->mass * dpNorm1) / (c1->mass + c2->mass);

			c1->velX = tx * dpTan1 + nx * m1;
			c1->velY = ty * dpTan1 + ny * m1;
			c2->velX = tx * dpTan2 + nx * m2;
			c2->velY = ty * dpTan2 + ny * m2;
		}

		//Clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');

		for (auto circle : circles)
			DrawWireFrameModel(modelCircle, circle.posX, circle.posY, atan2f(circle.velY, circle.velX), circle.radius, FG_WHITE);

		for (auto c : vecCollidingPairs)
			DrawLine(c.first->posX, c.first->posY, c.second->posX, c.second->posY, PIXEL_SOLID, FG_RED);

		if (selectedCircle != nullptr)
			DrawLine(selectedCircle->posX, selectedCircle->posY, m_mousePosX, m_mousePosY, PIXEL_SOLID, FG_BLUE);

		return true;
	}
};

int main()
{
	//Window window;
	//window.RenderWindow();
	CirclePhysics game;
	if (game.ConstructConsole(160, 120, 8, 8))
		game.Start();
	else
		std::wcout << L"It just doesn't work\n";

	return 0;
}