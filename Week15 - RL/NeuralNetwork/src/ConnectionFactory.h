/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FullConnection.h"
#include "ConvConnection2D.h"

typedef float T;

namespace ConnectionFactory
{
	FullConnection *getFullConnection(const int& num_next, const int& num_prev, const T& rand_scale, const T& rand_min)
	{
		FullConnection *new_conn = new FullConnection;

		// row x column = (dimension of next layer  - 1 for bias) x  (dimension of prev layer - this includes bias)
		// -1 is for bias. y = W [x b]^T. Don't subtract 1 if you want [y b]^T = W [x b]^T.
		//const int num_next = layers_[c + 1].act_.num_dimension_ - 1;
		//const int num_prev = layers_[c].act_.num_dimension_;
		new_conn->weights_.initialize(num_next, num_prev);
		new_conn->delta_weights_.initialize(num_next, num_prev);

		// random initialization
		new_conn->weights_.assignRandom(rand_scale, rand_min);
		new_conn->delta_weights_.assignAll(0.0);

		return new_conn;
	}

	ConvConnection2D *getConvConnection2D()
	{
		ConvConnection2D *new_conn = new ConvConnection2D;

		return new_conn;
	}

	//ConvConnection2D *getConvConnection2D(const int& _i_res, const int& _j_res, const int& _i_stride, const int& _j_stride, const int& _i_padding, const int& _j_padding, const T& _rand_scale, const T& _rand_min)
	//{
	//	assert(false); // not ready

	//	ConvConnection2D *new_conn = new ConvConnection2D;
	//	new_conn->filter_.initialize(_i_res, _j_res, _i_stride, _j_stride, _i_padding, _j_padding, _rand_scale, _rand_min);

	//	return new_conn;
	//}
};