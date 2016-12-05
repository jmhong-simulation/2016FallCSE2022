/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include "Array1D.h"
#include "MatrixMN.h"
#include "ConnectionBase.h"
#include "FullConnection.h"
#include "LayerBase.h"
#include "ConvConnection2D.h"

#define MIN2(a, b)							((a) > (b) ? (b) : (a))
#define MAX2(a, b)							((a) > (b) ? (a) : (b))

typedef float T;

class NeuralNetwork
{
public:
    int num_input_;
    int num_output_;
    int num_all_layers_; // num_all_layers_ = num_hidden_layers_ + 2

    T   bias_;  // constant bias
    T   eta_;   // learning rate
    T   alpha_; // momentum term coefficient

	Array1D<LayerBase> layers_; // layers_[0] = input_layer, layers_[num_all_layers_-1] = output_layer

    Array1D<ConnectionBase*>   connections_; // connections_[0] connects layer 0 and layer 1

    NeuralNetwork();
    NeuralNetwork(const int& _num_input, const int& _num_output, const int& _num_hidden_layers);
	~NeuralNetwork();

    void initialize(const int& _num_input, const int& _num_output, const int& _num_hidden_layers);
 
	void setFullConnection(const int& con_ix, const T& rand_scale, const T& rand_min);
	ConvConnection2D* setConvConnection2D(const int& con_ix);

    // forward propagation
    void feedForward();
    void setInputVector(const VectorND<T>& input);

    // back propagation
    void propBackward(const VectorND<T>& target);    // backward propagation
	void propBackwardError(const VectorND<T>& target);
	void updateConnectionWeights();

    // output access
    int getOutputIXEpsilonGreedy(const T& epsilon);
    int getOutputIXMaxComponent();
    int getOutputIXProbability();
    T   getOutputValueMaxComponent();
	T   getL2NormError(const VectorND<T>& desired);
	T   getLinfNormError(const VectorND<T>& desired); // http://lmp.ime.eb.br/intro_analise_de_erro.pdf
	T   getOutput(const int& ix);

    void copyOutputVectorTo(const bool& copy_bias, VectorND<T>& copy);

	void readTXT(const std::string& filename);
	void writeTXT(const std::string& filename) const;

	void check() const
	{
		for (int i = 0; i < layers_.num_elements_; i++)
			layers_[i].check();

		for (int i = 0; i < connections_.num_elements_; i++)
			connections_[i]->check();
	}
};