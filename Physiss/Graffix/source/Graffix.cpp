#include <iostream>
#include <string>

#include "olcConsoleGameEngine.h"


enum class Axis
{
	x = 1,
	y = 2,
	z = 3
};

/// <summary>
/// [x,y,z] point
/// </summary>
struct vec3d
{
	float x, y, z;
};

/// <summary>
/// An array of three [x,y,z] coordinates
/// </summary>
struct triangle
{
	vec3d p[3];
};

/// <summary>
/// A collection of triangles in 3D space
/// </summary>
struct mesh
{
	std::vector<triangle> triangles;
};

/// <summary>
/// Matrix for calculations
/// </summary>
struct mat4x4
{
	float m[4][4] = { 0 };
};

class Graphics : public olcConsoleGameEngine
{
public:
	Graphics()
	{
		m_sAppName = L"Graphics";
	}

private:
	mesh cube;
	mat4x4 projMatrix;

	float fTheta;

	/// <summary>
	/// Projects vectors with the help of a matrix (or some shit like that)
	/// </summary>
	/// <param name="in">Input vector</param>
	/// <param name="out">Output vector</param>
	/// <param name="matrix">Projection matrix</param>
	void MultiplyMatrixVector(vec3d& in, vec3d& out, mat4x4& matrix)
	{
		out.x = in.x * matrix.m[0][0] + in.y * matrix.m[1][0] + in.z * matrix.m[2][0] + matrix.m[3][0];
		out.y = in.x * matrix.m[0][1] + in.y * matrix.m[1][1] + in.z * matrix.m[2][1] + matrix.m[3][1];
		out.z = in.x * matrix.m[0][2] + in.y * matrix.m[1][2] + in.z * matrix.m[2][2] + matrix.m[3][2];
		float w = in.x * matrix.m[0][3] + in.y * matrix.m[1][3] + in.z * matrix.m[2][3] + matrix.m[3][3];

		if (w != 0.0f)
		{
			out.x /= w;
			out.y /= w;
			out.z /= w;
		}
	}

	/// <summary>
	/// Projects triangles with the help of a matrix (or some shit like that)
	/// </summary>
	/// <param name="in">Input triangle</param>
	/// <param name="out">Output triangle</param>
	/// <param name="matrix">Projection matrix</param>
	void MultiplyMatrixTriangle(triangle& in, triangle& out, mat4x4& matrix)
	{
		for (int i = 0; i < 3; i++)
		{
			MultiplyMatrixVector(in.p[i], out.p[i], matrix);
		}
	}

	/// <summary>
	/// Moves a triangle a given distance on a given axis
	/// </summary>
	/// <param name="in">Input triangle</param>
	/// <param name="out">Output triangle</param>
	/// <param name="dist">Distance to move</param>
	/// <param name="axis">Movement axis</param>
	void TranslateTriangle(triangle& in, triangle& out, float dist, Axis axis)
	{
		for (int i = 0; i < 3; i++)
		{
			if (axis == Axis::x)
			{
				out.p[i].x = in.p[i].x + dist;
			}
			if (axis == Axis::y)
			{
				out.p[i].y = in.p[i].y + dist;
			}
			if (axis == Axis::z)
			{
				out.p[i].z = in.p[i].z + dist;
			}
		}
	}

	/// <summary>
	/// Scales a triangle onto visible screen
	/// </summary>
	void ScaleTriangleIntoView(triangle& tri)
	{
		for (int i = 0; i < 3; i++)
		{
			tri.p[i].x += 1.0f;
			tri.p[i].y += 1.0f;
			tri.p[i].x *= 0.5f * (float)ScreenWidth();
			tri.p[i].y *= 0.5f * (float)ScreenHeight();
		}
		//tri.p[0].x += 1.0f; tri.p[0].y += 1.0f;
		//tri.p[1].x += 1.0f; tri.p[1].y += 1.0f;
		//tri.p[2].x += 1.0f; tri.p[2].y += 1.0f;

		//tri.p[0].x *= 0.5f * (float)ScreenWidth();
		//tri.p[0].y *= 0.5f * (float)ScreenHeight();
		//tri.p[1].x *= 0.5f * (float)ScreenWidth();
		//tri.p[1].y *= 0.5f * (float)ScreenHeight();
		//tri.p[2].x *= 0.5f * (float)ScreenWidth();
		//tri.p[2].y *= 0.5f * (float)ScreenHeight();
	}

public:
	bool OnUserCreate() override
	{
		cube.triangles = {
			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		};

		//Projection matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float FOV = 90.0f;
		float AspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(FOV * 0.5f / 180.0f * 3.14159f);

		projMatrix.m[0][0] = AspectRatio * fFovRad;
		projMatrix.m[1][1] = fFovRad;
		projMatrix.m[2][2] = fFar / (fFar - fNear);
		projMatrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		projMatrix.m[2][3] = 1.0f;
		projMatrix.m[3][3] = 0.0f;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		mat4x4 matRotationX, matRotationZ;

		fTheta += 1.0f * fElapsedTime;

		//X Rotation
		matRotationX.m[0][0] = 1;
		matRotationX.m[1][1] = cosf(fTheta * 0.5f);
		matRotationX.m[1][2] = sinf(fTheta * 0.5f);
		matRotationX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotationX.m[2][2] = cosf(fTheta * 0.5f);
		matRotationX.m[3][3] = 1;

		//Z Rotation
		matRotationZ.m[0][0] = cosf(fTheta);
		matRotationZ.m[0][1] = sinf(fTheta);
		matRotationZ.m[1][0] = -sinf(fTheta);
		matRotationZ.m[1][1] = cosf(fTheta);
		matRotationZ.m[2][2] = 1;
		matRotationZ.m[3][3] = 1;

		//Draw triangles
		for (auto tri : cube.triangles)
		{
			triangle projTriangle, triTranslated, triRotatedZ, triRotatedZX;

			//Rotation
			MultiplyMatrixTriangle(tri, triRotatedZ, matRotationZ);
			MultiplyMatrixTriangle(triRotatedZ, triRotatedZX, matRotationX);

			//Translation
			triTranslated = triRotatedZX;
			TranslateTriangle(triRotatedZX, triTranslated, 3.0f, Axis::z);

			//Projection
			MultiplyMatrixTriangle(triTranslated, projTriangle, projMatrix);
			ScaleTriangleIntoView(projTriangle);

			DrawTriangle(projTriangle.p[0].x, projTriangle.p[0].y,
				projTriangle.p[1].x, projTriangle.p[1].y,
				projTriangle.p[2].x, projTriangle.p[2].y,
				PIXEL_SOLID, FG_WHITE);
		}

		return true;
	}
};

int main()
{
	Graphics demo;
	if (demo.ConstructConsole(256, 240, 4, 4))
		demo.Start();

	return 0;
}