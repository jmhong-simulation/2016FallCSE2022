/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////


#pragma once

#include <iostream>
#include "VectorND.h"

typedef float T;

class ConnectionBase
{
public:
	virtual void forward(const VectorND<T>& pre_layer_acts_, VectorND<T>& next_layer_acts) const
	{
		std::cout << "ConnectionBase::forward()" << std::endl;
		exit(1);
	}

	virtual void backward(const VectorND<T>& next_layer_grads_, VectorND<T>& pre_layer_grads_) const
	{
		std::cout << "ConnectionBase::backward()" << std::endl;
		exit(1);
	}

	virtual void updateWeights(const T& eta, const T& alpha, const VectorND<T>& next_layer_grad, const VectorND<T>& prev_layer_act)
	{
		std::cout << "ConnectionBase::updateWeights()" << std::endl;
		exit(1);
	}

	virtual void writeTXT(std::ofstream& of) const
	{
		std::cout << "ConnectionBase::writeTXT()" << std::endl;
		exit(1);
	}

	virtual void check() const
	{
		std::cout << "ConnectionBase::check()" << std::endl;
		exit(1);
	}
};
