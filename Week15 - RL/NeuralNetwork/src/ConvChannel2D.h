/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ConvFilter2D.h"
#include "ConvImage2D.h"

class ConvChannel2D
{
public:
	ConvFilter2D filter_;
	ConvImage2D input_image_;
	ConvImage2D output_image_; // need to store width and height

	int in_offset_, out_offset_;

	ConvChannel2D(const ConvFilter2D& _filter, const T& _rand_scale, const T& _rand_min, const ConvImage2D& _input_image, const int& _in_offset, const ConvImage2D& _output_image, const int& _out_offset)
	{
		initialize(_filter, _rand_scale, _rand_min, _input_image, _in_offset, _output_image, _out_offset);
	}

	void initialize(const ConvFilter2D& _filter, const T& _rand_scale, const T& _rand_min, const ConvImage2D& _input_image, const int& _in_offset, const ConvImage2D& _output_image, const int& _out_offset)
	{
		filter_.initialize(_filter, _rand_scale, _rand_min);

		input_image_.width_ = _input_image.width_;
		input_image_.height_ = _input_image.height_;
		output_image_.width_ = _output_image.width_;
		output_image_.height_ = _output_image.height_;

		in_offset_ = _in_offset;
		out_offset_ = _out_offset;
	}
};