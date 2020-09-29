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
	//float x, y, z;
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
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

	void Normalize(vec3d& in)
	{
		float l = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
		in.x /= l; in.y /= l; in.z /= l;
	}

	vec3d Vector_Normalize(vec3d& in)
	{
		float l = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
		return { in.x /= l, in.y /= l, in.z /= l };
	}

	vec3d Vector_Add(vec3d& v1, vec3d& v2)
	{
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	vec3d Vector_Subtr(vec3d& v1, vec3d& v2)
	{
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	vec3d Vector_Mult(vec3d& v1, float k)
	{
		return { v1.x * k, v1.y * k, v1.z * k };
	}

	vec3d Vector_Div(vec3d& v1, float k)
	{
		return { v1.x / k, v1.y / k, v1.z / k };
	}

	float DotProduct(vec3d& v1, vec3d& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2)
	{
		vec3d v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
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

		//normal.x = line1.y * line2.z - line1.z * line2.y;
		//normal.y = line1.z * line2.x - line1.x * line2.z;
		//normal.z = line1.x * line2.y - line1.y * line2.x;

		normal = Vector_CrossProduct(line1, line2);

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

	vec3d Matrix_MultiplyVector(mat4x4& matrix, vec3d& in)
	{
		vec3d v;
		v.x = in.x * matrix.m[0][0] + in.y * matrix.m[1][0] + in.z * matrix.m[2][0] + in.w * matrix.m[3][0];
		v.y = in.x * matrix.m[0][1] + in.y * matrix.m[1][1] + in.z * matrix.m[2][1] + in.w * matrix.m[3][1];
		v.z = in.x * matrix.m[0][2] + in.y * matrix.m[1][2] + in.z * matrix.m[2][2] + in.w * matrix.m[3][2];
		v.w = in.x * matrix.m[0][3] + in.y * matrix.m[1][3] + in.z * matrix.m[2][3] + in.w * matrix.m[3][3];
		return v;
	}

	mat4x4 Matrix_MakeIdentity()
	{
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	mat4x4 Matrix_MakeRotationX(float fAngleRad)
	{
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = cosf(fAngleRad);
		matrix.m[1][2] = sinf(fAngleRad);
		matrix.m[2][1] = -sinf(fAngleRad);
		matrix.m[2][2] = cosf(fAngleRad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	mat4x4 Matrix_MakeRotationY(float fAngleRad)
	{
		mat4x4 matrix;
		matrix.m[0][0] = cosf(fAngleRad);
		matrix.m[0][2] = sinf(fAngleRad);
		matrix.m[2][0] = -sinf(fAngleRad);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(fAngleRad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	mat4x4 Matrix_MakeRotationZ(float fAngleRad)
	{
		mat4x4 matrix;
		matrix.m[0][0] = cosf(fAngleRad);
		matrix.m[0][1] = sinf(fAngleRad);
		matrix.m[1][0] = -sinf(fAngleRad);
		matrix.m[1][1] = cosf(fAngleRad);
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	mat4x4 Matrix_MakeTranslation(float x, float y, float z)
	{
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;
		return matrix;
	}

	mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
	{
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
		mat4x4 matrix;
		matrix.m[0][0] = fAspectRatio * fFovRad;
		matrix.m[1][1] = fFovRad;
		matrix.m[2][2] = fFar / (fFar - fNear);
		matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;
		return matrix;
	}

	mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2)
	{
		mat4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;
	}

	/// <summary>
	/// Projects triangles with the help of a matrix (or some shit like that)
	/// </summary>
	/// <param name="matrix">Projection matrix</param>
	/// <param name="in">Input triangle</param>
	/// <param name="out">Output triangle</param>
	void MultiplyMatrixTriangle(triangle& in, triangle& out, mat4x4& matrix)
	{
		for (int i = 0; i < 3; i++)
		{
			MultiplyMatrixVector(in.p[i], out.p[i], matrix);
		}
	}

	triangle Matrix_MultiplyTriangle(mat4x4 &matrix, triangle &in)
	{
		triangle out;
		for (int i = 0; i < 3; i++)
		{
			out.p[i] = Matrix_MultiplyVector(matrix, in.p[i]);
		}
		return out;
	}

	triangle NormalizeMultipliedTriangle(triangle& in)
	{
		triangle out;
		for (int i = 0; i < 3; i++)
		{
			out.p[i] = Vector_Div(in.p[i], in.p[i].w);
		}
		return out;
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
	void ScaleTriangleIntoView(triangle& tri, vec3d &offset)
	{
		for (int i = 0; i < 3; i++)
		{
			tri.p[i] = Vector_Add(tri.p[i], offset);
			tri.p[i].x += 1.0f;
			tri.p[i].y += 1.0f;
			tri.p[i].x *= 0.5f * (float)ScreenWidth();
			tri.p[i].y *= 0.5f * (float)ScreenHeight();
		}
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
		cube.LoadFromObjectFile("assets\\teapot.obj");

		//Projection matrix
		projMatrix = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		mat4x4 matRotationX, matRotationZ;

		fTheta += 1.0f * fElapsedTime;

		//Rotation matrices
		matRotationX = Matrix_MakeRotationX(fTheta);
		matRotationZ = Matrix_MakeRotationZ(fTheta * 0.5f);

		mat4x4 matTrans;
		matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 0.0f);

		mat4x4 matWorld;
		matWorld = Matrix_MakeIdentity();
		matWorld = Matrix_MultiplyMatrix(matRotationZ, matRotationX);
		matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

		std::vector<triangle> vecTrianglesToDraw;

		//Transform triangles
		for (auto tri : cube.triangles)
		{
			triangle projTriangle, triTransformed;

			triTransformed = Matrix_MultiplyTriangle(matWorld, tri);

			//Calculating normals
			vec3d normal = GetNormalFromTriangle(triTransformed);

			//Get ray from triangle to main camera
			vec3d cameraRay = Vector_Subtr(triTransformed.p[0], mainCamera);

			//Check if triangle is within camera view
			if(	DotProduct(normal, cameraRay) < 0.0f)
			{
				//Illumination
				vec3d lightDir = { 0.0f, 0.0f, -1.0f };
				//Normalize(lightDir);
				lightDir = Vector_Normalize(lightDir);

				float dp = max(0.1f, DotProduct(lightDir, normal));

				CHAR_INFO c = GetColour(dp);
				triTransformed.col = c.Attributes;
				triTransformed.sym = c.Char.UnicodeChar;

				//Projection
				projTriangle = Matrix_MultiplyTriangle(projMatrix, triTransformed);
				projTriangle = NormalizeMultipliedTriangle(projTriangle);
				projTriangle.col = triTransformed.col;
				projTriangle.sym = triTransformed.sym;

				vec3d offset = { 1,1,0 };
				ScaleTriangleIntoView(projTriangle,offset);

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