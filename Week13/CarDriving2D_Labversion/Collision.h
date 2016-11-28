#pragma once

#include <glm/glm.hpp>

class Collision
{
public:
	typedef typename glm::vec3 Point;

	// http://flassari.is/2008/11/line-line-intersection-in-cplusplus/
	// epsilon was from a comment by varsha
	Point* intersection(Point p1, Point p2, Point p3, Point p4) 
	{
		const float epsilon = 1e-5;

		// Store the values for fast access and easy
		// equations-to-code conversion
		float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
		float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

		float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
		// If d is zero, there is no intersection
		if (d == 0) return nullptr;

		// Get the x and y
		float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
		float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
		float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

		// Check if the x and y coordinates are within both sensing_lines
		if (x < glm::min<float>(x1, x2) - epsilon || x > glm::max<float>(x1, x2) + epsilon ||
			x < glm::min<float>(x3, x4) - epsilon || x > glm::max<float>(x3, x4) + epsilon) return nullptr;
		if (y < glm::min<float>(y1, y2) - epsilon || y > glm::max<float>(y1, y2) + epsilon ||
			y < glm::min<float>(y3, y4) - epsilon || y > glm::max<float>(y3, y4) + epsilon) return nullptr;

		// Return the point of intersection
		Point* ret = new Point();
		ret->x = x;
		ret->y = y;
		return ret;
	}
};
