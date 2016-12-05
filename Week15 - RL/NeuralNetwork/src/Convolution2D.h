/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ConvFilter2D.h"
#include "ConvImage2D.h"

typedef float T;

namespace Convolution2D
{
	int getDesiredOutputRes(const int& width_in, const int& width_filter, const int& stride, const int& padding);

	// input_image = prev_laver, output_image = next_layer
	void forward(const ConvFilter2D& filter, const ConvImage2D& input_image, ConvImage2D& output_image);

	// intput_grad = gradient of prev layer, output_grad = gradient of next layer
	void backward(const ConvFilter2D& filter, const ConvImage2D& output_grad, ConvImage2D& input_grad);

	void updateWeights(const T& eta_, const T& alpha_, const ConvImage2D& output_grad, const ConvImage2D& input_act, ConvFilter2D& filter);
};

namespace Conv2D = Convolution2D;
