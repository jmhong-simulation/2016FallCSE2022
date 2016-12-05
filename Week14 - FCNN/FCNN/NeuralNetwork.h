/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include "Array1D.h"
#include "MatrixMN.h"

#define MAX2(a, b)							((a) > (b) ? (a) : (b))

typedef double D;

class NeuralNetwork
{
public:
    int num_input_;
    int num_output_;
    int num_all_layers_; // num_all_layers_ = num_hidden_layers_ + 2

    D   bias_;	  // constant bias
    D   alpha_;   // learning rate

    Array1D<VectorND<D> > layer_neuron_act_;         // layer_neuron_act_[0] = input layer, layer_neuron_act_[num_all_layers_-1] = output_layer, layer_neuron_act_[ix_layer][ix_neuron] = activation value
    Array1D<VectorND<D> > layer_neuron_grad_;        // gradient values for back propagation
    Array1D<MatrixMN<D> > weights_;                  // weights_[0] is between layer 0 and layer 1. 

    VectorND<unsigned>    num_layer_acts_;           // The number of activation values of each layer. This includes bias.

	NeuralNetwork();
	NeuralNetwork(const int& _num_input, const int& _num_output, const int& _num_hidden_layers);

	void initialize(const int& _num_input, const int& _num_output, const int& _num_hidden_layers);
	void initialize(const VectorND<unsigned>& num_layer_acts_, const int& _num_hidden_layers);

	D getSigmoid(const D& x);
	D getRELU(const D& x);
	D getLRELU(const D& x);

	D getSigmoidGradFromY(const D& y);   // not from x. y = getSigmoid(x).
	D getRELUGradFromY(const D& x); // RELU Grad from X == RELU Grad from Y
	D getLRELUGradFromY(const D& x); // RELU Grad from X == RELU Grad from Y

	void applySigmoidToVector(VectorND<D>& vector);
	void applyRELUToVector(VectorND<D>& vector);
	void applyLRELUToVector(VectorND<D>& vector);

	void propForward();
	void propBackward(const VectorND<D>& target);
	void updateWeight(MatrixMN<D>& weight_matrix, VectorND<D>& next_layer_grad, VectorND<D>& prev_layer_act);

	void setInputVector(const VectorND<D>& input);
	void copyOutputVector(VectorND<D>& copy, bool copy_bias = false);
};