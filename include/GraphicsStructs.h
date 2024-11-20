#pragma once

#include <MyMaths.h>

namespace NdRenderer
{

	class Vertice {
	public:
		Vertice(Vector3 position, Vector3 colour) { Position = position; Colour = colour; }
		Vector3 Position;
		Vector3 Colour;
	};

}