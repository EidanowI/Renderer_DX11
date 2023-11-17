#pragma once
#include "DX11_Base.h"
#include <math.h>

///DEFINE FLOAT COLORS WITH 1.0f ALPHA
#define RED_A_FLOAT {1.0f, 0.0f, 0.0f, 1.0f}
#define GREEN_A_FLOAT {0.0f, 1.0f, 0.0f, 1.0f}
#define BLUE_A_FLOAT {0.0f, 0.0f, 1.0f, 1.0f}
#define YELLOW_A_FLOAT {1.0f, 1.0f, 0.0f, 1.0f}
#define CYAN_A_FLOAT {0.0f, 1.0f, 1.0f, 1.0f}
#define MAGENTA_A_FLOAT {1.0f, 0.0f, 1.0f, 1.0f}
#define WHITE_A_FLOAT {1.0f, 1.0f, 1.0f, 1.0f}
#define BLACK_A_FLOAT {0.0f, 0.0f, 0.0f, 1.0f}

///DEFINE TEST ZERO PARAMETERES
#define NORM_0 {0.0f, 0.0f, 0.0f}
#define UV_0 {0.0f, 0.0f}


namespace GraphicsFundament {
	float Random01() noexcept;
	float RandomMinus11() noexcept;

	struct Vector3D {
		Vector3D() noexcept : x(0), y(0), z(0) {}

		Vector3D(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

		float Length() {
			return sqrt(x * x + y * y + z * z);
		}


		Vector3D operator +(const Vector3D& o) noexcept {
			return Vector3D(x + o.x, y + o.y, z + o.z);
		}

		Vector3D operator -(const Vector3D& o) noexcept {
			return Vector3D(x - o.x, y - o.y, z - o.z);
		}

		Vector3D operator *(const Vector3D& o) noexcept {
			return Vector3D(x * o.x, y * o.y, z * o.z);
		}
		Vector3D operator *(const float a) noexcept {
			return Vector3D(x * a, y * a, z * a);
		}

		Vector3D operator /(const Vector3D& o) noexcept {
			return Vector3D(x / o.x, y / o.y, z / o.z);
		}
		Vector3D operator /(const float a) noexcept {
			return Vector3D(x / a, y / a, z / a);
		}

		Vector3D operator +=(const Vector3D& o) noexcept {
			*this = *this + o;
			return *this;
		}

		Vector3D operator -=(const Vector3D& o) noexcept {
			*this = *this - o;
			return *this;
		}

		Vector3D operator *=(const Vector3D& o) noexcept {
			*this = *this * o;
			return *this;
		}
		Vector3D operator *=(const float a) noexcept {
				*this = *this * a;
				return *this;
		}

		Vector3D operator /=(const Vector3D& o) noexcept {
			*this = *this / o;
			return *this;
		}
		Vector3D operator /=(const float a) noexcept {
			*this = *this / a;
			return *this;
		}


		float x;
		float y;
		float z;

	};
}

struct Vertex48B {
	struct {
		float x;
		float y;
		float z;
	} position;
	struct {
		float nX;
		float nY;
		float nZ;
	} normal;
	struct {
		float x;
		float y;
	} UV;
	struct {
		float r;
		float g;
		float b;
		float a;
	} color;

	static D3D11_INPUT_ELEMENT_DESC inputElementDesc[4];
	static unsigned char elementDescNum;
};
struct TrianglePoly {
	unsigned short I1;
	unsigned short I2;
	unsigned short I3;
};

