#pragma once

#include<glm\glm.hpp>

template<class TT>
class Box2D
{
public:
	union
	{
		struct{TT x_min_, y_min_,x_max_,y_max_;};
		struct{TT i_start_,j_start_,i_end_,j_end_;};
	};

public:
	Box2D(void)
	{}

	Box2D(const Box2D<TT>& box_input)
		: x_min_(box_input.x_min_), y_min_(box_input.y_min_), x_max_(box_input.x_max_), y_max_(box_input.y_max_)
	{}

	Box2D(const TT& x_min_input, const TT& y_min_input, const TT& x_max_input, const TT& y_max_input)
		: x_min_(x_min_input), y_min_(y_min_input), x_max_(x_max_input), y_max_(y_max_input)
	{}

	~Box2D(void)
	{}

	bool isInside(const TT& x, const TT& y);

	TT getSignedDistance(const glm::vec3& pt) const
	{
		const glm::vec2 box_center = glm::vec2((x_min_ + x_max_)*(TT)0.5, (y_min_ + y_max_)*(TT)0.5);
		const glm::vec2 box_half_edge = glm::vec2((x_max_ - x_min_)*(TT)0.5, (y_max_ - y_min_)*(TT)0.5);
		const glm::vec2 temp = glm::vec2(pt.x, pt.y) - box_center;
		glm::vec2 d = glm::vec2(glm::abs(temp.x), glm::abs(temp.y)) - box_half_edge;
		d = glm::vec2(glm::max(d.x, 0.0f), glm::max(d.y, 0.0f));

		return sqrt(glm::dot(d, d)) + glm::min((glm::max(d.x, d.y)), 0.0f);
	}

	glm::vec3 getNormal(const glm::vec3& pt) const
	{
		const TT ep = 1e-5;

		const float dx = getSignedDistance(glm::vec3(pt.x + ep, pt.y, pt.z)) - getSignedDistance(glm::vec3(pt.x - ep, pt.y, pt.z));
		const float dy = getSignedDistance(glm::vec3(pt.x, pt.y + ep, pt.z)) - getSignedDistance(glm::vec3(pt.x, pt.y - ep, pt.z));

		glm::vec2 temp(dx, dy);
		temp = glm::normalize(temp);

		return glm::vec3(temp.x, temp.y, 0.0f);
	}
};
