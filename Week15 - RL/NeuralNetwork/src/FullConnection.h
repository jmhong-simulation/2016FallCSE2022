/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <fstream>
#include "ConnectionBase.h"
#include "Array1D.h"
#include "MatrixMN.h"

typedef float T;

class FullConnection : public ConnectionBase
{
public:
	// fully connected layer
	MatrixMN<T> weights_;
	MatrixMN<T> delta_weights_; // Temporary array to store weight matrices from previous step for momentum term.

	void forward(const VectorND<T>& pre_layer_acts_, VectorND<T>& next_layer_acts) const;
	void backward(const VectorND<T>& next_layer_grads_, VectorND<T>& pre_layer_grads_) const;

	//Note: next_layer_grad and prev_layer_act
	// eta is learning rate, alpha is momentum coefficient.
	void updateWeights(const T& eta, const T& alpha, const VectorND<T>& next_layer_grad, const VectorND<T>& prev_layer_act);

	void writeTXT(std::ofstream& of) const
	{
		weights_.writeTXT(of);
	}

	void check() const
	{
		weights_.check();
		delta_weights_.check();
	}
	//void write(std::ofstream& os) const
	//{
	//	os.write((char*)&i_start_, sizeof(i_start_));
	//	os.write((char*)&j_start_, sizeof(j_start_));
	//	os.write((char*)&k_start_, sizeof(k_start_));

	//	os.write((char*)&i_res_, sizeof(i_res_));
	//	os.write((char*)&j_res_, sizeof(j_res_));
	//	os.write((char*)&k_res_, sizeof(k_res_));

	//	for (int p = 0; p < ijk_res_; p++)
	//		os.write((char*)&values_[p], sizeof(TT));
	//}
};

