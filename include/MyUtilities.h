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
	};
}