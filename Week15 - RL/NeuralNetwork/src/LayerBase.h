/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MatrixMN.h"

typedef float T;

class LayerBase
{
public:
	typedef enum { Sigmoid, ReLU, LReLU } ActType;

	ActType act_type_;

	VectorND<T> act_;	// activation values of neurons
	VectorND<T> grad_;	// gradients for back-propagation

public:
	void initialize(const int& _dim, const ActType& _type);
	void resize(const int& _dim);
	void assignErrorToGrad(const VectorND<T>& target);
	void activate();
	void multiplyActGradToGrad();
	void setActType(const ActType& type);

	void check() const
	{
		act_.check();
		grad_.check();
	}
};
