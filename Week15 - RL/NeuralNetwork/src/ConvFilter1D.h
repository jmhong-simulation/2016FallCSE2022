/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VectorND.h"

typedef float T;

class ConvFilter1D
{
public:
	int i_res_;
	int i_stride_;
	int i_padding_;

	VectorND<T> weights_;
	VectorND<T> delta_weights_; // momentum

	ConvFilter1D()
	{}

	ConvFilter1D(const int& _i_res, const int& _i_stride, const int& _i_padding, const T& _rand_scale, const T& _rand_min)
	{
		initialize(_i_res, _i_stride, _i_padding, _rand_scale, _rand_min);
	}

	ConvFilter1D(const ConvFilter1D& fi, const T& _rand_scale, const T& _rand_min)
	{
		initialize(fi.i_res_, fi.i_stride_, fi.i_padding_, _rand_scale, _rand_min);
	}

public:
	void initialize(const ConvFilter1D& fi, const T& _rand_scale, const T& _rand_min)
	{
		initialize(fi.i_res_, fi.i_stride_, fi.i_padding_, _rand_scale, _rand_min);
	}

	void initialize(const int& _i_res, const int& _i_stride, const int& _i_padding, const T& _rand_scale, const T& _rand_min)
	{
		i_res_ = _i_res;
		i_stride_ = _i_stride;
		i_padding_ = _i_padding;

		weights_.initialize(i_res_ + 1);		// +1 is for bias
		delta_weights_.initialize(i_res_ + 1);	// +1 is for bias

		for (int i = 0; i < weights_.num_dimension_; i++)
		{
			weights_[i] = (T)rand() / (T)RAND_MAX * _rand_scale + _rand_min;
			delta_weights_[i] = (T)0.0;
		}
	}

	int get1DIndex(const int& i) const
	{
		return i;
	}

	T& getWeight(const int& i) const
	{
		assert(i >= 0);
		assert(i < i_res_);

		return weights_[i];
	}

	T& getDeltaWeight(const int& i) const
	{
		assert(i >= 0);
		assert(i < i_res_);

		return delta_weights_[i];
	}

	T& getBias() const
	{
		return weights_[weights_.num_dimension_ - 1];	// last component is bias
	}

	T& getDeltaBias() const
	{
		return delta_weights_[weights_.num_dimension_ - 1];	// last component is bias
	}
};
