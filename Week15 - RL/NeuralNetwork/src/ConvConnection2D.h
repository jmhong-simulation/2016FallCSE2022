/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ConnectionBase.h"
#include "Convolution2D.h"
#include "ConvChannel2D.h"
#include <vector>

typedef float T;

class ConvConnection2D : public ConnectionBase
{
public:
	//ConvChannel2D channel_;

	std::vector<ConvChannel2D*> channel_list_;

	//ConvFilter2D filter_;
	//ConvImage2D input_image_;
	//ConvImage2D output_image_; // need to store width and height
		
	//ConvConnection2D(const ConvFilter2D& _filter, const T& _rand_scale, const T& _rand_min, const ConvImage2D& _input_image, const ConvImage2D& _output_image)
	//{
	//	initialize(_filter, _rand_scale, _rand_min, _input_image, _output_image);
	//}

	~ConvConnection2D()
	{
		for (int i = 0; i < channel_list_.size(); i++)
		{
			SAFE_DELETE(channel_list_[i]);
		}
	}

	//void initialize(const ConvFilter2D& _filter, const T& _rand_scale, const T& _rand_min, const ConvImage2D& _input_image, const ConvImage2D& _output_image)
	//{
	//	filter_.initialize(_filter, _rand_scale, _rand_min);

	//	input_image_.width_ = _input_image.width_;
	//	input_image_.height_ = _input_image.height_;
	//	output_image_.width_ = _output_image.width_;
	//	output_image_.height_ = _output_image.height_;
	//}

	void forward(const VectorND<T>& pre_layer_acts_, VectorND<T>& next_layer_acts) const;
	void backward(const VectorND<T>& next_layer_grads_, VectorND<T>& pre_layer_grads_) const;
	void updateWeights(const T& eta, const T& alpha, const VectorND<T>& next_layer_grad, const VectorND<T>& prev_layer_act);
};

//
//class ConvFilter
//{
//public:
//	// convolution filter
//	int width_filter_, height_filter_;
//	int stride_;
//	VectorND<D> filter_;
//
//	void initialize(const int& _width, const int& _height, const int& _stride)
//	{
//		width_filter_ = _width;
//		height_filter_ = _height;
//		stride_ = _stride;          // stride = 1 means sweeping all pixels.
//
//		filter_.initialize(width_filter_ * height_filter_ + 1, false);
//
//		for (int d = 0; d < filter_.num_dimension_; d++)
//			filter_[d] = (D)rand() / (D)RAND_MAX * 0.1 + (D)0.01;
//	}
//
//	//void forward(const int& pre_offset, const int& next_offset, const int& width_in, const int& height_in, const VectorND<D>& pre_layer_acts, VectorND<D>& next_layer_acts) const
//	//{
//	//    // width_in : width of input image, height_in : height of input image
//
//	//    const int width_out = width_in - (width_filter_ - stride_);
//	//    const int height_out = height_in - (height_filter_ - stride_);
//
//	//    for (int j_f = 0; j_f < height_filter_; j_f++)
//	//        for (int i_f = 0; i_f < width_filter_; i_f++)
//	//        {
//	//            const D w = filter_[i_f + width_filter_*j_f];
//
//	//            for (int j = 0; j < height_in; j++)
//	//                for (int i = 0; i < width_in; i++)
//	//                { 
//	//                    next_layer_acts[...] += pre_layer_acts_[...] * w;
//	//                }
//	//        }
//	//}
//};
//
