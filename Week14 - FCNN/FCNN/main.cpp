/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "NeuralNetwork.h"

void main()
{
	VectorND<D> x(2);
	x[0] = 0.0; x[1] = 0.0;

	VectorND<D> y_target(2);
	y_target[0] = 0.3f;

	VectorND<D> y_temp(2);

	NeuralNetwork nn_;
	nn_.initialize(2, 1, 1);
	nn_.alpha_ = 0.1;

	for (int i = 0; i < 100; i++)
	{
		nn_.setInputVector(x);
		nn_.propForward();

		nn_.copyOutputVector(y_temp);
		std::cout << y_temp << std::endl;

		nn_.propBackward(y_target);
	}
}