/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "Convolution2D.h"

namespace Convolution2D
{
	int getDesiredOutputRes(const int& width_in, const int& width_filter, const int& stride, const int& padding)
	{
		return (width_in - width_filter + 2 * padding) / stride + 1;
	}

	// input_image = prev_laver, output_image = next_layer
	void forward(const ConvFilter2D& filter, const ConvImage2D& input_image, ConvImage2D& output_image)
	{
		for (int j_in_base = -filter.j_padding_, j_out = 0; j_in_base < input_image.height_ + filter.j_padding_ - filter.j_res_ + 1; j_in_base += filter.j_stride_, j_out++)
			for (int i_in_base = -filter.i_padding_, i_out = 0; i_in_base < input_image.width_ + filter.i_padding_ - filter.i_res_ + 1; i_in_base += filter.i_stride_, i_out++)
			{
				T sum = (T)0;
				for (int j_f = 0, j_in = j_in_base; j_f < filter.j_res_; j_f++, j_in++)
					for (int i_f = 0, i_in = i_in_base; i_f < filter.i_res_; i_f++, i_in++)
					{
						if (input_image.isValid(i_in, j_in) == true)
							sum += input_image.getValue(i_in, j_in) * filter.getWeight(i_f, j_f);
						// else // zero padding
					}

				sum += filter.getBias();

				output_image.getValue(i_out, j_out) = sum;
			}
	}

	// intput_grad = gradient of prev layer, output_grad = gradient of next layer
	void backward(const ConvFilter2D& filter, const ConvImage2D& output_grad, ConvImage2D& input_grad)
	{
		for (int i = 0; i < input_grad.width_ * input_grad.height_; i++)
			input_grad.values_[i] = 0.0;

		for (int j_in_base = -filter.j_padding_, j_out = 0; j_in_base < input_grad.height_ + filter.j_padding_ - filter.j_res_ + 1; j_in_base += filter.j_stride_, j_out++)
			for (int i_in_base = -filter.i_padding_, i_out = 0; i_in_base < input_grad.width_ + filter.i_padding_ - filter.i_res_ + 1; i_in_base += filter.i_stride_, i_out++)
			{
				for (int j_f = 0, j_in = j_in_base; j_f < filter.j_res_; j_f++, j_in++)
					for (int i_f = 0, i_in = i_in_base; i_f < filter.i_res_; i_f++, i_in++)
					{
						if (input_grad.isValid(i_in, j_in) == true)
							input_grad.getValue(i_in, j_in) += filter.getWeight(i_f, j_f) * output_grad.getValue(i_out, j_out);
						// else // zero padding
					}
			}
	}

	void updateWeights(const T& eta_, const T& alpha_, const ConvImage2D& output_grad, const ConvImage2D& input_act, ConvFilter2D& filter)
	{
		VectorND<T> filter_weights_temp(filter.i_res_ * filter.j_res_ + 1);
		filter_weights_temp.copyPartial(filter.weights_, 0, 0, filter.weights_.num_dimension_);
		filter.weights_.assignAllValues((T)0);

		for (int j_in_base = -filter.j_padding_, j_out = 0; j_in_base < input_act.height_ + filter.j_padding_ - filter.j_res_ + 1; j_in_base += filter.j_stride_, j_out++)
		{
			for (int i_in_base = -filter.i_padding_, i_out = 0; i_in_base < input_act.width_ + filter.i_padding_ - filter.i_res_ + 1; i_in_base += filter.i_stride_, i_out++)
			{
				for (int j_f = 0, j_in = j_in_base; j_f < filter.j_res_; j_f++, j_in++)
					for (int i_f = 0, i_in = i_in_base; i_f < filter.i_res_; i_f++, i_in++)
					{
						if (input_act.isValid(i_in, j_in) == true)
						{
							filter.getWeight(i_f, j_f) += eta_ * output_grad.getValue(i_out, j_out) * input_act.getValue(i_in, j_in);
						}
						// else // zero padding  
					}

				// update bias
				filter.getBias() += eta_ * output_grad.getValue(i_out, j_out); // bias = 1 is assumed
			}
		}

		// momentum
		for (int f = 0; f < filter.weights_.num_dimension_; f++)
		{
			filter.weights_.values_[f] += alpha_ * filter.delta_weights_.values_[f];
			filter.delta_weights_.values_[f] = filter.weights_.values_[f];
			filter.weights_.values_[f] += filter_weights_temp.values_[f];
		}
	}

}
