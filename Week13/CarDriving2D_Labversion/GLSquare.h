#pragma once

#include "Box2D.h"
#include "GLObject.h"

class GLSquare : public GLObject
{
public:
	using GLObject::vertices;
	using GLObject::vertexbuffer;
	using GLObject::genVertexBuffer;

	Box2D<float> bb;

	GLSquare()
	{

	}

	GLSquare(const glm::vec3& center, const float& half_dx, const float& half_dy)
	{
		update(center, half_dx, half_dy);
	}

	void update(const glm::vec3& center, const float& half_dx, const float& half_dy)
	{
		center_ = center;

		bb = Box2D<float>(center.x - half_dx, center.y - half_dy, center.x + half_dx, center.y + half_dy);

		const glm::vec3 v0(center.x - half_dx, center.y - half_dy, center.z);
		const glm::vec3 v1(center.x + half_dx, center.y - half_dy, center.z);
		const glm::vec3 v2(center.x + half_dx, center.y + half_dy, center.z);
		const glm::vec3 v3(center.x - half_dx, center.y + half_dy, center.z);

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		genVertexBuffer();
	}

	bool isInside(const glm::vec3& pt)
	{
		const glm::mat4 inv_m = glm::inverse(model_matrix_);

		const glm::vec4 pt4(pt.x, pt.y, pt.z, 1.0f);

		const glm::vec4 pt4_inv = inv_m * pt4;

		return bb.isInside(pt4_inv.x, pt4_inv.y);
	}
};
