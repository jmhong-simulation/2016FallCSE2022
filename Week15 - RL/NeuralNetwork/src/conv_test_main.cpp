/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "BreakoutGame.h"
#include "NeuralNetwork.h"
#include "CircularQueue.h"

#pragma comment(lib, "winmm.lib")

NeuralNetwork nn_;

void main()
{
	const int width = 10, height = 10;

	VectorND<T> input_image(width*height), output(2), desired(2);

	input_image.assignAllValues((T)0);

	nn_.initialize(width * height, output.num_dimension_, 2); // output is (x, y)

	nn_.layers_[0].act_type_ = LayerBase::ReLU;
	nn_.layers_[1].act_type_ = LayerBase::ReLU;
	nn_.layers_[2].act_type_ = LayerBase::ReLU;
	nn_.layers_[3].act_type_ = LayerBase::ReLU;

	nn_.eta_ = (T)1e-4;
	nn_.alpha_ = (T)0.9;

	nn_.layers_[1].initialize(width*height * 2 + 1, LayerBase::ReLU);
	nn_.layers_[2].initialize(output.num_dimension_ + 1, LayerBase::ReLU);
	nn_.setFullConnection(1, 0.1f, 0.01f);
	nn_.setFullConnection(2, 0.1f, 0.01f);

	{
		ConvFilter2D filter;
		filter.initialize(5, 5, 1, 1, 2, 2, 0.1, 0.01);
		ConvImage2D im;
		im.width_ = width;
		im.height_ = height;
		ConvImage2D om;
		om.width_ = width;
		om.height_ = height;

		ConvConnection2D *new_conn = nn_.setConvConnection2D(0);
		new_conn->channel_list_.push_back(new ConvChannel2D(filter, (T)0.1, (T)0.01, im, 0, om, 0));
		new_conn->channel_list_.push_back(new ConvChannel2D(filter, (T)0.1, (T)0.01, im, 0, om, width*height));
	}

	{
		ConvFilter2D filter;
		filter.initialize(width, height, 1, 1, 0, 0, 0.1, 0.01);
		ConvImage2D im;
		im.width_ = width;
		im.height_ = height;
		ConvImage2D om;
		om.width_ = 1;
		om.height_ = 1;

		ConvConnection2D *new_conn = nn_.setConvConnection2D(1);
		new_conn->channel_list_.push_back(new ConvChannel2D(filter, (T)0.1, (T)0.01, im, 0, om, 0));
		new_conn->channel_list_.push_back(new ConvChannel2D(filter, (T)0.1, (T)0.01, im, width * height, om, 1));
	}

	while (1)
	{
		T max_error = (T)0;

		for (int r = 0; r < width*height*100; r++)
		{
			const int rand_i = rand() % width;
			const int rand_j = rand() % height;

			//input_image.values_[rand_i - 1 + width* rand_j] = (T)1.0;
			input_image.values_[rand_i + width* rand_j] = (T)1.0;
			//input_image.values_[rand_i + 1 + width* rand_j] = (T)1.0;
			//input_image.values_[rand_i + width* (rand_j - 1)] = (T)1.0;
			//input_image.values_[rand_i + width* (rand_j + 1)] = (T)1.0;

			desired[0] = (T)rand_i / (T)width;
			desired[1] = (T)rand_j / (T)height;

			/*desired[0] = (T)rand_j / (T)width;*/

			nn_.setInputVector(input_image);
			nn_.feedForward();
			nn_.copyOutputVectorTo(false, output);

			const T linferror = nn_.getLinfNormError(desired);
			//std::cout << "F: " << dynamic_cast<ConvConnection2D*>(nn_.connections_[0])->channel_list_[0]->filter_.weights_ << std::endl;

			max_error = MAX2(linferror, max_error);

			//if (linferror < 1e-2)
			//{
			//	std::cout << linferror << std::endl;
			//	std::cout << output << std::endl;
			//	std::cout << "F: " << dynamic_cast<ConvConnection2D*>(nn_.connections_[0])->channel_list_[0]->filter_.weights_ << std::endl;
			//	//exit(1);
			//}

			nn_.propBackward(desired);

			input_image.values_[rand_i + width* rand_j] = (T)0.0;
		}

		std::cout << "Max error = " << max_error << std::endl;

		if (max_error < 0.00015)
		{
			//nn_.writeTXT("NN.txt");
			//return;
		
			//std::cout << dynamic_cast<ConvConnection2D*>(nn_.connections_[0])->channel_list_[0]->filter_.weights_ << std::endl;
			//std::cout << std::endl;
			////std::cout << dynamic_cast<ConvConnection2D*>(nn_.connections_[0])->channel_list_[1]->filter_.weights_ << std::endl;
			//////std::cout << nn_.layers_[1].act_ << std::endl;
			//dynamic_cast<FullConnection*>(nn_.connections_[1])->weights_.cout();
			//std::cout << std::endl;
			//exit(1);

			printf("\a");
		}
	}
}

// position guess test
//void main()
//{
//	const int width = 12, height = 8;
//
//	VectorND<T> input_image(width*height), output(2), desired(2);
//
//	input_image.assignAllValues((T)0);
//
//	nn_.initialize(width * height, 2, 1); // output is (x, y)
//
//	nn_.layers_[0].act_type_ = LayerBase::LReLU;
//	nn_.layers_[1].act_type_ = LayerBase::LReLU;
//	nn_.layers_[2].act_type_ = LayerBase::LReLU;
//	//nn_.layers_[3].act_type_ = LayerBase::LReLU;
//
//	nn_.eta_ = (T)1e-4;
//	nn_.alpha_ = (T)0.9;
//
//	ConvFilter2D filter;
//	filter.initialize(3, 3, 1, 1, 1, 1, 0.1, 0.01);
//	ConvImage2D im;
//	im.width_ = width;
//	im.height_ = height;
//	ConvImage2D om;
//	om.width_ = width;
//	om.height_ = height;
//	nn_.setConvConnection2D(0, filter, 0.1, 0.01, im, om);
//
//	//nn_.writeTXT("NN.txt");
//	//nn_.readTXT("NN.txt");
//	//exit(1);
//
//	bool is_training = true;
//
//	while (true)
//	{
//		T max_error = (T)0.00;
//		for (int r = 0; r < width * height; r++)
//		{
//			const int rand_i = rand() % (width - 2) + 1;
//			const int rand_j = rand() % (height - 2) + 1;
//
//			input_image.values_[rand_i - 1 + width* rand_j] = (T)1.0;
//			input_image.values_[rand_i + width* rand_j] = (T)1.0;
//			input_image.values_[rand_i + 1 + width* rand_j] = (T)1.0;
//			input_image.values_[rand_i + width* (rand_j - 1)] = (T)1.0;
//			input_image.values_[rand_i + width* (rand_j + 1)] = (T)1.0;
//
//			nn_.setInputVector(input_image);
//			nn_.feedForward();
//			nn_.copyOutputVectorTo(false, output);
//
//			desired[0] = (T)rand_i / (T)width;
//			desired[1] = (T)rand_j / (T)height;
//
//			//if (is_training == false)
//			//{
//			//	std::cout <<rand_i <<" "<<rand_j<< " Desired = " << desired << " Output = " << output << std::endl;
//			//}
//
//			const T error = sqrt(POW2(output[0] - desired[0]) + POW2(output[1] - desired[1]));
//
//			max_error = MAX2(max_error, error);
//
//			nn_.propBackward(desired);
//
//			input_image.values_[rand_i - 1 + width* rand_j] = (T)0.0;
//			input_image.values_[rand_i + width* rand_j] = (T)0.0;
//			input_image.values_[rand_i + 1 + width* rand_j] = (T)0.0;
//			input_image.values_[rand_i + width* (rand_j - 1)] = (T)0.0;
//			input_image.values_[rand_i + width* (rand_j + 1)] = (T)0.0;
//		}
//
//		std::cout << "Max error = " << max_error << std::endl;
//
//		if (max_error < 0.0001)
//		{
//			//nn_.writeTXT("NN.txt");
//			//return;
//			is_training = false;
//
//			//std::cout << dynamic_cast<ConvConnection2D*>(nn_.connections_[0])->filter_.weights_ << std::endl;
//			std::cout << nn_.layers_[1].act_ << std::endl;
//		}
//
//	}
//}