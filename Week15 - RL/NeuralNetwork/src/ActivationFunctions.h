/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////


#pragma once

#include "VectorND.h"

namespace ActFunc
{
	typedef float T;

	T getLRELU(const T& x);
	T getRELU(const T& x);
	T getSigmoid(const T& x);

	T getLRELUGradFromY(const T& x);
	T getRELUGradFromY(const T& x);
	T getSigmoidGradFromY(const T& y);

	void applyLRELUToVector(const int& num_bias, VectorND<T>& vector);
	void applyRELUToVector(const int& num_bias, VectorND<T>& vector);
	void applySigmoidToVector(const int& num_bias, VectorND<T>& vector);
}