/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "FullConnection.h"

void FullConnection::forward(const VectorND<T>& pre_layer_acts_, VectorND<T>& next_layer_acts) const
{
	// The last component of acts_ is the bias and thus shouldn't be updated. 
	weights_.multiply(pre_layer_acts_, next_layer_acts);
}

void FullConnection::backward(const VectorND<T>& next_layer_grads_, VectorND<T>& pre_layer_grads_) const
{
	weights_.multiplyTransposed(next_layer_grads_, pre_layer_grads_);
}

//Note: next_layer_grad and prev_layer_act
void FullConnection::updateWeights(const T& eta, const T& alpha, const VectorND<T>& next_layer_grad, const VectorND<T>& prev_layer_act)
{
	// eta : learning rate
	// alpha : momentum coefficient

	const int num_rows = weights_.num_rows_;
	const int num_cols = weights_.num_cols_;

	for (int row = 0; row < num_rows; row++)
	{
		for (int col = 0; col < num_cols; col++)
		{
			T &old_delta_w = delta_weights_.getValue(row, col);

			const T delta_w = eta * next_layer_grad[row] * prev_layer_act[col] + alpha * old_delta_w;

			weights_.getValue(row, col) += delta_w;

			old_delta_w = delta_w;           // update for the momentum term in next time step
		}
	}

	//TODO: make an option
	//weights_.normalizeAllRows(1e-3);
}