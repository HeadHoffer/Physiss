#include <iostream>
#include <string>
#include <fstream>
#include <strstream>
#include <algorithm>

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

	wchar_t sym;
	short col;
};

/// <summary>
/// A collection of triangles in 3D space
/// </summary>
struct mesh
{
	std::vector<triangle> triangles;

	/// <summary>
	/// <para>Loads the triangles from an object file.</para>
	/// <para>Use filename to search the file.</para>
	/// </summary>
	bool LoadFromObjectFile(std::string fileName)
	{
		std::ifstream f(fileName);
		if (!f.is_open())
			return false;

		//Local cache of verts
		std::vector<vec3d> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);
			std::strstream s;
			s << line;

			char junk;
			if (line[0] == 'v')
			{
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				triangles.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}

		return true;
	}
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

	vec3d mainCamera;

	float fTheta;

	float DotProduct(vec3d& vec1, vec3d& vec2)
	{
		return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
	}

	/// <summary>
	/// Normalizes a given vector
	/// </summary>
	void Normalize(vec3d& in)
	{
		float l = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
		in.x /= l; in.y /= l; in.z /= l;
	}

	/// <summary>
	/// Returns a normal vector of a given triangle
	/// </summary>
	vec3d GetNormalFromTriangle(triangle& in)
	{
		vec3d normal, line1, line2;
		line1.x = in.p[1].x - in.p[0].x;
		line1.y = in.p[1].y - in.p[0].y;
		line1.z = in.p[1].z - in.p[0].z;

		line2.x = in.p[2].x - in.p[0].x;
		line2.y = in.p[2].y - in.p[0].y;
		line2.z = in.p[2].z - in.p[0].z;

		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;

		//float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		//normal.x /= l; normal.y /= l; normal.z /= l;
		Normalize(normal);
		return normal;
	}

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

	/// <summary>
	/// Returns a color value used by the console
	/// </summary>
	/// <param name="lum">Luminence</param>
	CHAR_INFO GetColour(float lum)
	{
		short bg_col, fg_col;
		wchar_t sym;
		int pixel_bw = (int)(13.0f * lum);
		switch (pixel_bw)
		{
		case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

		case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
		case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
		case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
		case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

		case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
		case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
		case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
		case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

		case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
		case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
		case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
		case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
		default:
			bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
		}

		CHAR_INFO c;
		c.Attributes = bg_col | fg_col;
		c.Char.UnicodeChar = sym;
		return c;
	}

public:
	bool OnUserCreate() override
	{
		//cube.triangles = {
		//	// SOUTH
		//	{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		//	{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		//	// EAST                                                      
		//	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		//	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		//	// NORTH                                                     
		//	{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		//	{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		//	// WEST                                                      
		//	{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		//	{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		//	// TOP                                                       
		//	{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		//	{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		//	// BOTTOM                                                    
		//	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		//	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		//};
		cube.LoadFromObjectFile("assets\\teapot.obj");

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

		std::vector<triangle> vecTrianglesToDraw;

		//Transform triangles
		for (auto tri : cube.triangles)
		{
			triangle projTriangle, triTranslated, triRotatedZ, triRotatedZX;

			//Rotation
			MultiplyMatrixTriangle(tri, triRotatedZ, matRotationZ);
			MultiplyMatrixTriangle(triRotatedZ, triRotatedZX, matRotationX);

			//Translation
			triTranslated = triRotatedZX;
			TranslateTriangle(triRotatedZX, triTranslated, 5.0f, Axis::z);

			//Calculating normals
			vec3d normal = GetNormalFromTriangle(triTranslated);

			//Check if triangle is within camera view
			if(	normal.x * (triTranslated.p[0].x - mainCamera.x) +
				normal.y * (triTranslated.p[0].y - mainCamera.y) +
				normal.z * (triTranslated.p[0].z - mainCamera.z) < 0.0f)
			{
				//Illumination
				vec3d lightDir = { 0.0f, 0.0f, -1.0f };
				Normalize(lightDir);

				float dp = DotProduct(normal, lightDir);

				CHAR_INFO c = GetColour(dp);
				triTranslated.col = c.Attributes;
				triTranslated.sym = c.Char.UnicodeChar;

				//Projection
				MultiplyMatrixTriangle(triTranslated, projTriangle, projMatrix);
				projTriangle.col = triTranslated.col;
				projTriangle.sym = triTranslated.sym;
				ScaleTriangleIntoView(projTriangle);

				//Store triangle for sorting
				vecTrianglesToDraw.push_back(projTriangle);
			}
		}

		//Sort triangles from back to front
		std::sort(vecTrianglesToDraw.begin(), vecTrianglesToDraw.end(), [](triangle& t1, triangle& t2)
		{
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;

		});

		//Draw sorted triangles
		for (auto& projTriangle : vecTrianglesToDraw)
		{
			FillTriangle(projTriangle.p[0].x, projTriangle.p[0].y,
				projTriangle.p[1].x, projTriangle.p[1].y,
				projTriangle.p[2].x, projTriangle.p[2].y,
				projTriangle.sym, projTriangle.col);

			//Black wireframe
			//DrawTriangle(projTriangle.p[0].x, projTriangle.p[0].y,
			//	projTriangle.p[1].x, projTriangle.p[1].y,
			//	projTriangle.p[2].x, projTriangle.p[2].y,
			//	PIXEL_SOLID, FG_BLACK);
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