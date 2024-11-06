#pragma once
#include <cmath>

namespace NdRenderer
{
	class Vector3{
	public:
		double x, y, z;
		Vector3(double ix, double iy, double iz){
			x = ix; y = iy; z = iz;
		}
		Vector3 operator+(Vector3& v) { return Vector3(x + v.x, y + v.y, z + v.z); }
		Vector3 operator-(Vector3& v) { return Vector3(x - v.x, y - v.y, z - v.z); }
		Vector3& operator+=(Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		Vector3& operator-=(Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	};
}