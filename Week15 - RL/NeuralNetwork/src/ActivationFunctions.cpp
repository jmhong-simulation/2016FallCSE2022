/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////


#include "ActivationFunctions.h"
#include "ConventionalMacros.h"
#include <math.h>

namespace ActFunc
{
	T getSigmoid(const T& x)
	{
		return 1.0 / (1.0 + exp(-x));
	}

	T getSigmoidGradFromY(const T& y)   // not from x. y = getSigmoid(x).
	{
		return (1.0 - y) * y;
	}

	T getRELU(const T& x)
	{
		return MAX2(0.0, x);
	}

	T getRELUGradFromY(const T& x) // RELU Grad from X == RELU Grad from Y
	{
		if (x > 0.0) return 1.0;
		else return 0.0;
	}

	T getLRELU(const T& x)
	{
		return x > 0.0 ? x : 0.01*x;
	}

	T getLRELUGradFromY(const T& x) // RELU Grad from X == RELU Grad from Y
	{
		if (x > 0.0) return 1.0;
		else return 0.01;
	}

	void applySigmoidToVector(const int& num_bias, VectorND<T>& vector)
	{
		const int num = vector.num_dimension_ - num_bias;
		for (int d = 0; d < num; d++) // don't apply activation function to bias
			vector[d] = getSigmoid(vector[d]);
	}

	void applyRELUToVector(const int& num_bias, VectorND<T>& vector)
	{
		const int num = vector.num_dimension_ - num_bias;
		for (int d = 0; d < num; d++) // don't apply activation function to bias
			vector[d] = getRELU(vector[d]);
	}

	void applyLRELUToVector(const int& num_bias, VectorND<T>& vector)
	{
		const int num = vector.num_dimension_ - num_bias;
		for (int d = 0; d < num; d++) // don't apply activation function to bias
			vector[d] = getLRELU(vector[d]);
	}
}