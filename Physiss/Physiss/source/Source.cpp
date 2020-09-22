#include <iostream>
#include <string>
#include <algorithm>

//#include "Window.h"
#include "olcConsoleGameEngine.h"

struct Circle
{
	float posX, posY;
	float velX, velY;
	float accX, accY;
	float oldX, oldY;
	float radius;
	float mass;

	float fSimTimeRemaining;

	int id;
};

struct LineSegment
{
	float startX, startY;
	float endX, endY;
	float radius;
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

	std::vector<LineSegment> lines;
	LineSegment* selectedLine = nullptr;
	bool selectedLineStart = false;

	Circle gravityBall;

	float gravity = 300.0f;
	float realGravity = gravity;
	bool noGrav = false;

	float defaultRad = 8.0f;
	float fLineRadius = 3.0f;

	int circleAmount = 40;

	/// <summary>
	/// Spawns a circle in given coordinates
	/// </summary>
	/// <param name="x">coordinate</param>
	/// <param name="y">coordinate</param>
	/// <param name="r">Circle radius</param>
	void AddCircle(float x, float y, float r = 5.0f, bool gravity = false)
	{
		Circle c;
		c.posX = x; c.posY = y;
		c.velX = 0; c.velY = 0;
		c.accX = 0; c.accY = 0;
		c.radius = r;
		c.mass = r * 10.0f;

		c.id = circles.size();
		if(!gravity)
			circles.emplace_back(c);
		if (gravity)
			gravityBall = c;
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

		//AddCircle(ScreenWidth() * 0.25f, ScreenHeight() * 0.5f, defaultRad);
		//AddCircle(ScreenWidth() * 0.75f, ScreenHeight() * 0.5f, defaultRad);
		AddCircle(ScreenWidth() - 10, 10, defaultRad, true);
		for (int i = 0; i < circleAmount; i++)
		{
			AddCircle(rand() % ScreenWidth(), rand() % ScreenHeight(), defaultRad);
		}

		lines.push_back({ 10.0f, 50.0f, 330.0f, 120.0f, fLineRadius });
		lines.push_back({ 450.0f, 140.0f, 150.0f, 210.0f, fLineRadius });
		lines.push_back({ 90.0f, 240.0f, 400.0f, 300.0f, fLineRadius });

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
		auto IsPointInCirclePosition = [](float x1, float y1, float r, float x2, float y2)
		{
			return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < (r * r);
		};
		auto Distance = [](float x1, float y1, float x2, float y2)
		{
			return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		};

		if (m_mouse[0].bPressed || m_mouse[1].bPressed)
		{
			selectedCircle = nullptr;
			selectedLine = nullptr;

			//Check for gravityball
			if (IsPointInCircle(gravityBall, m_mousePosX, m_mousePosY))
			{
				selectedCircle = &gravityBall;
			}

			//Check for selected circle
			for (auto& circle : circles)
			{
				if (IsPointInCircle(circle, m_mousePosX, m_mousePosY))
				{
					selectedCircle = &circle;
					break;
				}
			}

			//Check for selected line
			for (auto& line : lines)
			{
				if (IsPointInCirclePosition(line.startX, line.startY, line.radius, m_mousePosX, m_mousePosY))
				{
					selectedLine = &line;
					selectedLineStart = true;
					break;
				}
				if (IsPointInCirclePosition(line.endX, line.endY, line.radius, m_mousePosX, m_mousePosY))
				{
					selectedLine = &line;
					selectedLineStart = false;
					break;
				}
			}
		}

		if (m_mouse[0].bHeld)
		{
			if (selectedCircle != nullptr && selectedCircle != &gravityBall) //Dragging circles
			{
				selectedCircle->posX = m_mousePosX;
				selectedCircle->posY = m_mousePosY;
			}

			if (selectedLine != nullptr)
			{
				if (selectedLineStart)
				{
					selectedLine->startX = m_mousePosX;
					selectedLine->startY = m_mousePosY;
				}
				else
				{
					selectedLine->endX = m_mousePosX;
					selectedLine->endY = m_mousePosY;
				}
			}
		}

		if (m_mouse[0].bReleased)
		{
			if (selectedCircle == nullptr && selectedLine == nullptr)
			{
				AddCircle(m_mousePosX, m_mousePosY, defaultRad);
			}
			else if (selectedCircle == &gravityBall) //Change gravity
			{
				if (noGrav)
				{
					realGravity = gravity;
					noGrav = false;
				}
				else
				{
					realGravity = 0.0f;
					noGrav = true;
				}
			}
			else if (selectedCircle != nullptr) //Release dragged circle
			{
				selectedCircle->velX = 0;
				selectedCircle->velY = 0;
				selectedCircle->accX = 0;
				selectedCircle->accY = 0;
			}

			selectedCircle = nullptr;
			selectedLine = nullptr;
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
		std::vector<Circle*> fakeCircles;

		int nSimulationUpdates = 4;
		float fSimElapsedTime = fElapsedTime / (float)nSimulationUpdates;
		int maxSimulationSteps = 15;

		//Main simulation loop
		for (int i = 0; i < nSimulationUpdates; i++)
		{
			for (auto& circle : circles)
				circle.fSimTimeRemaining = fSimElapsedTime;

			for (int j = 0; j < maxSimulationSteps; j++)
			{
				//Update circle positions
				for (auto& circle : circles)
				{
					if (circle.fSimTimeRemaining > 0.0f)
					{
						//Old position
						circle.oldX = circle.posX;
						circle.oldY = circle.posY;

						//Friction
						circle.accX = -circle.velX * 0.8f;
						circle.accY = -circle.velY * 0.8f + realGravity;

						//Simulating movement
						circle.velX += circle.accX * circle.fSimTimeRemaining;
						circle.velY += circle.accY * circle.fSimTimeRemaining;
						circle.posX += circle.velX * circle.fSimTimeRemaining;
						circle.posY += circle.velY * circle.fSimTimeRemaining;

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
				}

				//Static collisions
				for (auto& circle : circles)
				{
					//Circles against lines
					for (auto& edge : lines)
					{
						float fLineX1 = edge.endX - edge.startX;
						float fLineY1 = edge.endY - edge.startY;

						float fLineX2 = circle.posX - edge.startX;
						float fLineY2 = circle.posY - edge.startY;

						float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

						float t = max(0.0f, min(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2))) / fEdgeLength;

						float fClosestPointX = edge.startX + t * fLineX1;
						float fClosestPointY = edge.startY + t * fLineY1;

						float fDistance = Distance(circle.posX, circle.posY, fClosestPointX, fClosestPointY);

						if (fDistance <= (circle.radius + edge.radius))
						{
							Circle* fakeCircle = new Circle();
							fakeCircle->radius = edge.radius;
							fakeCircle->mass = circle.mass * 0.8f;
							fakeCircle->posX = fClosestPointX;
							fakeCircle->posY = fClosestPointY;
							fakeCircle->velX = -circle.velX;
							fakeCircle->velY = -circle.velY;

							fakeCircles.push_back(fakeCircle);
							vecCollidingPairs.push_back({ &circle, fakeCircle });

							float fOverlap = 1.0f * (fDistance - circle.radius - fakeCircle->radius);

							circle.posX -= fOverlap * (circle.posX - fakeCircle->posX) / fDistance;
							circle.posY -= fOverlap * (circle.posY - fakeCircle->posY) / fDistance;
						}
					}

					//Circles against circles
					for (auto& target : circles)
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

					//Time displacement
					float fIntendedSpeed = sqrtf(circle.velX * circle.velX + circle.velY * circle.velY);
					float fIntendedDistance = fIntendedSpeed * circle.fSimTimeRemaining;
					float fActualDistance = sqrtf((circle.posX - circle.oldX) * (circle.posX - circle.oldX) + (circle.posY - circle.oldY) * (circle.posY - circle.oldY));
					float fActualTime = fActualDistance / fIntendedSpeed;

					circle.fSimTimeRemaining = circle.fSimTimeRemaining - fActualTime;
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

				//Memory cleanup
				for (auto& b : fakeCircles) delete b;
				fakeCircles.clear();
				vecCollidingPairs.clear();
			}
		}

		//Clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');

		//Draw line segments
		for (auto line : lines)
		{
			FillCircle(line.startX, line.startY, line.radius, PIXEL_HALF, FG_WHITE);
			FillCircle(line.endX, line.endY, line.radius, PIXEL_HALF, FG_WHITE);

			float nx = -(line.endY - line.startY);
			float ny = (line.endX - line.startX);
			float d = sqrt(nx * nx + ny * ny);
			nx /= d;
			ny /= d;

			DrawLine((line.startX + nx * line.radius), (line.startY + ny * line.radius), (line.endX + nx * line.radius), (line.endY + ny * line.radius));
			DrawLine((line.startX - nx * line.radius), (line.startY - ny * line.radius), (line.endX - nx * line.radius), (line.endY - ny * line.radius));
		}

		//Draw gravity ball
		FillCircle(gravityBall.posX, gravityBall.posY, gravityBall.radius, PIXEL_SOLID, FG_GREEN);

		//Draw circle wireframes
		for (auto circle : circles)
			//DrawWireFrameModel(modelCircle, circle.posX, circle.posY, atan2f(circle.velY, circle.velX), circle.radius, FG_WHITE);
			FillCircle(circle.posX, circle.posY, circle.radius, PIXEL_SOLID, FG_RED);

		//Draw circle collisions
		//for (auto c : vecCollidingPairs)
		//	DrawLine(c.first->posX, c.first->posY, c.second->posX, c.second->posY, PIXEL_SOLID, FG_RED);

		//Draw billiard line
		if (selectedCircle != nullptr && selectedCircle != &gravityBall && m_mouse[1].bHeld)
			DrawLine(selectedCircle->posX, selectedCircle->posY, m_mousePosX, m_mousePosY, PIXEL_SOLID, FG_BLUE);

		return true;
	}
};

int main()
{
	//Window window;
	//window.RenderWindow();
	CirclePhysics game;
	if (game.ConstructConsole(480, 360, 2, 2))
		game.Start();
	else
		std::wcout << L"It just doesn't work\n";

	return 0;
}