/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

typedef float T;

class ConvImage2D // temporary data storage for two-dimensional convolution
{
public:
	int width_, height_;
	T* values_;

	ConvImage2D()
		: values_(nullptr)
	{}

	ConvImage2D(const int& _width, const int& _height, T* _values)
	{
		initialize(_width, _height, _values);
	}

	~ConvImage2D()
	{
		// don't delete values_. This class is a temporary workplace. data will be handled in other places.
	}

	void initialize(const int& _width, const int& _height, T* _values)
	{
		width_ = _width;
		height_ = _height;
		values_ = _values;
	}

	int getIndex(const int& i, const int& j) const
	{
		assert(isValid(i, j) == true);

		return i + width_ * j;
	}

	bool isValid(const int& i, const int& j) const
	{
		if (i < 0) return false;
		if (i >= width_) return  false;
		if (j < 0) return  false;
		if (j >= height_) return  false;

		return true;
	}

	T& getValue(const int& i, const int& j) const
	{
		assert(values_ != nullptr);
		assert(isValid(i, j) == true);

		return values_[i + width_*j];
	}
};