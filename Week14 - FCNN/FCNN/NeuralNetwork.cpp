/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork()
{}

NeuralNetwork::NeuralNetwork(const int& _num_input, const int& _num_output, const int& _num_hidden_layers)
{
	initialize(_num_input, _num_output, _num_hidden_layers);
}

void NeuralNetwork::initialize(const int& _num_input, const int& _num_output, const int& _num_hidden_layers)
{
	num_layer_acts_.initialize(_num_hidden_layers + 2);

	num_layer_acts_[0] = _num_input + 1;                        // layer 0 is input layer, +1 is for bias
	for (int l = 1; l < _num_hidden_layers + 1; ++l)
	{
		num_layer_acts_[l] = _num_input + 1;                    // default value
	}

	num_layer_acts_[_num_hidden_layers + 1] = _num_output + 1;   // last layer is output layer. Add +1 for bias as well in case this NN is combined with others.

	initialize(num_layer_acts_, _num_hidden_layers);
}

void NeuralNetwork::initialize(const VectorND<unsigned>& num_layer_acts_, const int& _num_hidden_layers)
{
	num_input_ = num_layer_acts_[0] - 1;                         // -1 is for bias
	num_output_ = num_layer_acts_[_num_hidden_layers + 1] - 1;   // -1 is for bias
	num_all_layers_ = _num_hidden_layers + 2; // hidden layers + 1 input layer + 1 output layer

	bias_ = 1;
	alpha_ = 0.15;

	//layer_type_act_.initialize(num_all_layers_, true);         // activation type: sigmoid, ReLU, LReLU, etc.

	// initialize all layers
	layer_neuron_act_.initialize(num_all_layers_);
	for (int l = 0; l < num_all_layers_; ++l)
	{
		layer_neuron_act_[l].initialize(num_layer_acts_[l], true);
		layer_neuron_act_[l][num_layer_acts_[l] - 1] = bias_;     // bias
	}

	// initialize to store gradient of layers
	layer_neuron_grad_.initialize(num_all_layers_);
	for (int l = 0; l < num_all_layers_; ++l)
		layer_neuron_grad_[l].initialize(num_layer_acts_[l], true);

	// initialize weight matrix between layers
	weights_.initialize(num_all_layers_ - 1);                  // Note -1. Weight matrices are between layers.
	for (int l = 0; l < weights_.num_elements_; l++)
	{
		// row x column = (dimension of next layer  - 1 for bias) x  (dimension of prev layer - this includes bias)
		weights_[l].initialize(layer_neuron_act_[l + 1].num_dimension_ - 1, layer_neuron_act_[l].num_dimension_);// -1 is for bias. y = W [x b]^T. Don't subtract 1 if you want [y b]^T = W [x b]^T.

		// random initialization
		for (int ix = 0; ix < weights_[l].num_rows_ * weights_[l].num_cols_; ix++)
			weights_[l].values_[ix] = (D)rand() / RAND_MAX * 0.1;
	}

	//TODO: Temporary array to store weight matrices from previous step for momentum term.
}

D NeuralNetwork::getSigmoid(const D& x)
{
	return 1.0 / (1.0 + exp(-x));
}

D NeuralNetwork::getSigmoidGradFromY(const D& y)   // not from x. y = getSigmoid(x).
{
	return (1.0 - y) * y;
}

D NeuralNetwork::getRELU(const D& x)
{
	return MAX2(0.0, x);
}

D NeuralNetwork::getRELUGradFromY(const D& x) // RELU Grad from X == RELU Grad from Y
{
	if (x > 0.0) return 1.0;
	else return 0.0;
}

D NeuralNetwork::getLRELU(const D& x)
{
	return x > 0.0 ? x : 0.01*x;
}

D NeuralNetwork::getLRELUGradFromY(const D& x) // RELU Grad from X == RELU Grad from Y
{
	if (x > 0.0) return 1.0;
	else return 0.01;
}

void NeuralNetwork::applySigmoidToVector(VectorND<D>& vector)
{
	for (int d = 0; d < vector.num_dimension_ - 1; d++) // don't apply activation function to bias
		vector[d] = getSigmoid(vector[d]);
}

void NeuralNetwork::applyRELUToVector(VectorND<D>& vector)
{
	for (int d = 0; d < vector.num_dimension_ - 1; d++) // don't apply activation function to bias
		vector[d] = getRELU(vector[d]);
}

void NeuralNetwork::applyLRELUToVector(VectorND<D>& vector)
{
	for (int d = 0; d < vector.num_dimension_ - 1; d++) // don't apply activation function to bias
		vector[d] = getLRELU(vector[d]);
}

void NeuralNetwork::propForward()
{
	for (int l = 0; l < weights_.num_elements_; l++)
	{
		// The last component of layer_neuron_act_[l + 1], bias, shouldn't be updated. 
		weights_[l].multiply(layer_neuron_act_[l], layer_neuron_act_[l + 1]);

		// activate 
		applyRELUToVector(layer_neuron_act_[l + 1]);
	}
}

// backward propagation
void NeuralNetwork::propBackward(const VectorND<D>& target)
{
	// calculate gradients of output layer
	{const int l = layer_neuron_grad_.num_elements_ - 1;

	for (int d = 0; d < layer_neuron_grad_[l].num_dimension_ - 1; d++)  // skip last component (bias)
	{
		const D &output_value(layer_neuron_act_[l][d]);
		layer_neuron_grad_[l][d] = (target[d] - output_value) * getRELUGradFromY(output_value);
	}}

	// calculate gradients of hidden layers
	for (int l = weights_.num_elements_ - 1; l >= 0; l--)
	{
		weights_[l].multiplyTransposed(layer_neuron_grad_[l + 1], layer_neuron_grad_[l]);

		for (int d = 0; d < layer_neuron_act_[l].num_dimension_ - 1; d++)   // skip last component (bias)
		{
			layer_neuron_grad_[l][d] *= getRELUGradFromY(layer_neuron_act_[l][d]);
		}
	}

	// update weights after all gradients are calculated
	for (int l = weights_.num_elements_ - 1; l >= 0; l--)
	{
		// correct weight values of matrix from layer l + 1 to l
		updateWeight(weights_[l], layer_neuron_grad_[l + 1], layer_neuron_act_[l]);
	}
}

void NeuralNetwork::updateWeight(MatrixMN<D>& weight_matrix, VectorND<D>& next_layer_grad, VectorND<D>& prev_layer_act)
{
	for (int row = 0; row < weight_matrix.num_rows_; row++)
	{
		for (int col = 0; col < weight_matrix.num_cols_; col++)
		{
			const D delta_w = alpha_ * next_layer_grad[row] * prev_layer_act[col];

			weight_matrix.getValue(row, col) += delta_w;

			//TODO: update momentum term
		}
	}
}

void NeuralNetwork::setInputVector(const VectorND<D>& input)
{
	// use num_input_ in case input vector doesn't include bias

	if (input.num_dimension_ < num_input_)
		std::cout << "Input dimension is wrong" << std::endl;

	for (int d = 0; d < num_input_; d++)
		layer_neuron_act_[0][d] = input[d];
}

void NeuralNetwork::copyOutputVector(VectorND<D>& copy, bool copy_bias)
{
	const VectorND<D>& output_layer_act(layer_neuron_act_[layer_neuron_act_.num_elements_ - 1]);

	if (copy_bias == false)
	{
		copy.initialize(num_output_, false);

		for (int d = 0; d < num_output_; d++)
			copy[d] = output_layer_act[d];
	}
	else
	{
		copy.initialize(num_output_ + 1, false);

		for (int d = 0; d < num_output_ + 1; d++)
			copy[d] = output_layer_act[d];
	}
}