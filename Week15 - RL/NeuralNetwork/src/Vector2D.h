/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>

template<class TT>
class Vector2D
{
public:// member variables
	union{
		struct{ TT x_, y_; };
		struct{ TT i_, j_; };
		struct{ TT u_, v_; };
		struct{ TT t_min_, t_max_; };
		struct{ TT v0_, v1_; };
		TT values_[2];};//zero based indexing

public:// constructors
	Vector2D(void)
		: x_(TT()),y_(TT())
	{}

	Vector2D(const TT& x_input,const TT& y_input)
		: x_(x_input), y_(y_input)
	{};

	Vector2D(const Vector2D& vector_input)
		: x_(vector_input.x_), y_(vector_input.y_)
	{}

	Vector2D(const TT values_input[2])
		: x_(values_input[0]), y_(values_input[1])
	{}	

public://destructor
	~Vector2D(void)
	{};

public:
	void operator = (const Vector2D& v)
	{
		x_ = v.x_;
		y_ = v.y_;
	}

	void operator += (const Vector2D& v)
	{
		x_ += v.x_;
		y_ += v.y_;
	}

	void operator -= (const Vector2D& v)
	{
		x_ -= v.x_;
		y_ -= v.y_;
	}

	void operator *= (const TT& s)
	{
		x_ *= s;
		y_ *= s;
	}

	void operator /= (const TT& s)
	{
		const TT one_over_s = (TT)1/s;
		x_ *= one_over_s;
		y_ *= one_over_s;
	}

	Vector2D<TT> operator + (const Vector2D<TT>& v) const
	{
		return Vector2D<TT>(x_+v.x_, y_+v.y_);
	}

	Vector2D<TT> operator - (const Vector2D<TT>& v) const
	{
		return Vector2D<TT>(x_-v.x_, y_-v.y_);
	}

	Vector2D<TT> operator * (const TT& a) const
    {
		return Vector2D<TT>(x_*a, y_*a);
	}

	Vector2D<TT> operator / (const TT& a) const
    {
		TT one_over_a = (TT)1/a;
		return Vector2D<TT>(x_*one_over_a, y_*one_over_a);
	}

	TT getMagnitude() const
	{
		return sqrt(x_*x_ + y_*y_);
	}

	TT SqrMagnitude() const
	{
		return x_*x_ + y_*y_;
	}

	bool compareNonorderly(const Vector2D<TT>& to_compare) const		// returns true if both have the same components regardless of the order of components.
	{
		if ((i_ == to_compare.i_ && j_ == to_compare.j_) || (i_ == to_compare.j_ && j_ == to_compare.i_)) return true;
		else return false;
	}

	bool isSqrMagnitudeSmallerThan(const TT& sqrmagnitude) const
	{
		if(sqrmagnitude > (x_*x_ + y_*y_)) return true;
		else return false;
	}

	void normalize()
	{
		TT magnitude = getMagnitude();

//		assert(magnitude > (TT)0);

		if(magnitude != 0)
		{
			TT s = 1/magnitude;

			x_ *= s;
			y_ *= s;
		}
	}

	void safeNormalize()
	{
		TT magnitude = getMagnitude();

		if(magnitude > 1e-8)
		{
			TT s = 1/magnitude;

			x_ *= s;
			y_ *= s;
		}
		else
		{
			x_ = 0;
			y_ = 0;
		}
	}

	Vector2D<TT> getNormalized() const
	{
		Vector2D<TT> normalized_vector(x_, y_);

		normalized_vector.normalize();

		return normalized_vector;
	}

	Vector2D<TT> getSafeNormalized() const
	{
		Vector2D<TT> normalized_vector(x_, y_);

		normalized_vector.safeNormalize();

		return normalized_vector;
	}

	void scalingComponents(const Vector2D<TT>& normal, const TT& normal_coef, const TT& tangential_coef)
	{
		const TT alpha = dotProduct(*this, normal);
		(*this) -= alpha*normal;// remove normal component and leave tangential component only
		(*this) = (normal_coef*alpha)*normal + tangential_coef*(*this);
	}

	void assign(const TT& x_input, const TT& y_input)
	{
		x_ = x_input;
		y_ = y_input;
	}

	void assignZeroVector()
	{
		x_ = (TT)0;
		y_ = (TT)0;
	}

	// this = v1 - v2
	void assignDifference(const Vector2D<TT>& v1, const Vector2D<TT>& v2)
	{
		x_ = v1.x_ - v2.x_;
		y_ = v1.y_ - v2.y_;
	}

	// this = (p1-p2) + (v1-v2)*dt
	void assignDifferencePlusScaledDifference(const Vector2D<TT>& p1, const Vector2D<TT>& p2, const Vector2D<TT>& v1, const Vector2D<TT>& v2, const TT& dt)
	{
		x_ = (p1.x_-p2.x_) + (v1.x_-v2.x_)*dt;
		y_ = (p1.y_-p2.y_) + (v1.y_-v2.y_)*dt;
	}

	void assignScaledDifference(const TT& scalar, const Vector2D<TT>& v1, const Vector2D<TT>& v2)
	{
		x_ = scalar*(v1.x_ - v2.x_);
		y_ = scalar*(v1.y_ - v2.y_);
	}

	void assignScaledVector(const TT& scalar, const Vector2D<TT>& v1)
	{
		x_ = scalar*v1.x_;
		y_ = scalar*v1.y_;
	}

	void add(const TT& x_input, const TT& y_input)
	{
		x_ += x_input;
		y_ += y_input;
	}

	void addSum(const Vector2D<TT>& v1, const Vector2D<TT>& v2)
	{
		x_ += (v1.x_ + v2.x_);
		y_ += (v1.y_ + v2.y_);
	}

	void subtractSum(const Vector2D<TT>& v1, const Vector2D<TT>& v2)
	{
		x_ -= (v1.x_ + v2.x_);
		y_ -= (v1.y_ + v2.y_);
	}

	bool isInside(const TT& t)
	{
		if (t < t_min_) return false;
		else if (t > t_max_) return false;
		else return true;
	}

	Vector2D<TT> operator - () const
	{
		return Vector2D<TT>(-x_, -y_);
	}
};

// miscellaneous free operators and functions

template<class TT> bool operator == (const Vector2D<TT>& lhs, const Vector2D<TT>& rhs)
{
	if (lhs.i_ != rhs.i_) return false;
	else if (lhs.j_ != rhs.j_) return false;
	else return true;
}

template<class TT> const Vector2D<TT> operator * (const TT& a, const Vector2D<TT>& v)
{
	return Vector2D<TT>(a*v.x_, a*v.y_);
}

template<class TT> const TT dotProduct(const Vector2D<TT>& v1, const Vector2D<TT>& v2)
{
	return v1.x_*v2.x_ + v1.y_*v2.y_;
}

template<class TT> const TT crossProduct(const Vector2D<TT>& v1,const Vector2D<TT>& v2)
{
	return v1.x_*v2.y_ - v2.x_*v1.y_;
}

template<class TT> static bool isSqrDistanceSmallerThan(const Vector2D<TT>& a, const Vector2D<TT>& b, const TT& sqrmagnitude)
{
	const TT diff_x(a.x_ - b.x_), diff_y(a.y_ - b.y_);
	if(sqrmagnitude > (diff_x*diff_x + diff_y*diff_y)) return true;
	else return false;
}

template<class TT> std::ostream&
operator<<(std::ostream& output,const Vector2D<TT>& v)
{
	return output<<v.x_<<" "<<v.y_;
}