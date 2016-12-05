/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CircularQueue.h"
#include "NeuralNetwork.h"
#include "GameState.h"

class ReinforcementLearning
{
public:
	int num_exp_replay_;		// histories for experience replay training
	int num_input_histories_;	// input to nn
	int num_state_variables_;   // dimension of the state variables of the game
	int num_game_actions_;			// num outputs of the game

	float gamma_;

	NeuralNetwork nn_;
	GameMemory memory_;
	//CircularQueue<int>				history_selected_;

	VectorND<float> old_input_vector_, reward_vector_, next_input_vector_;

	void initialize()
	{
		//assert(num_exp_replay_ >= num_input_histories_);

		// initialize neural network
		const int num_hidden_layers = 1;

		nn_.initialize(num_state_variables_ * num_input_histories_, num_game_actions_, num_hidden_layers);

		for (int i = 0; i <= num_hidden_layers + 1; i++)
			nn_.layers_[i].act_type_ = LayerBase::ReLU;

		nn_.eta_ = 1e-5;
		nn_.alpha_ = 0.9;

		gamma_ = 0.9f;

		// initialize replay memory
		memory_.reserve(1e5);

		old_input_vector_.initialize(nn_.num_input_, true);
		next_input_vector_.initialize(nn_.num_input_, true);

		//TODO: don't train if histories are not enough
		//for (int h = 0; h < num_input_histories_; h++)
		//	history_.pushBack(game_.getStateBuffer());

		// initialize NN for good training results (default output is 1.0 to 0.5 for all)
		//old_input_vector_.assignAllValues(1.0f);
		//VectorND<T> temp, temp2(nn_.num_output_);
		//temp2.assignAllValues(0.5f);
		//while (true)
		//{
		//	nn_.setInputVector(old_input_vector_);
		//	nn_.feedForward();
		//	nn_.copyOutputVectorTo(false, temp);
		//	bool converged = true;
		//	T max_abs_error = 0;

		//	for (int i = 0; i < temp.num_dimension_; i++)
		//	{
		//		const T error = ABS(temp[i] - 0.5f);

		//		max_abs_error = MAX2(error, max_abs_error);

		//		if (error >= 0.1f) converged = false;
		//	}

		//	//std::cout << "Max abs error pre training " << max_abs_error << std::endl;
		//	if (converged == true)
		//		break;

		//	nn_.propBackward(temp2);
		//}

		//std::cout << "Pre training end" << std::endl;

	}

	void initializeConv2D()
	{
		int scr_width_ = 20;
		int scr_height_ = 20;

		const int num_channels = 4;

		assert(num_exp_replay_ >= num_input_histories_);

		nn_.initialize(num_state_variables_ * num_input_histories_, num_game_actions_, 2);

		nn_.layers_[0].act_type_ = LayerBase::LReLU;
		nn_.layers_[1].act_type_ = LayerBase::LReLU;
		nn_.layers_[2].act_type_ = LayerBase::LReLU;

		nn_.eta_ = 1e-4;
		nn_.alpha_ = 0.9;

		nn_.layers_[0].act_type_ = LayerBase::LReLU;
		//conn 0 : filter
		nn_.layers_[1].act_type_ = LayerBase::LReLU;
		//conn 1 : averaging
		nn_.layers_[2].act_type_ = LayerBase::LReLU;
		//conn 2 : full
		nn_.layers_[3].act_type_ = LayerBase::LReLU;

		nn_.layers_[1].initialize(num_input_histories_ *num_state_variables_ * num_channels + 1, LayerBase::LReLU);
		nn_.layers_[2].initialize(num_input_histories_ * num_channels + 1, LayerBase::LReLU);
		nn_.setFullConnection(1, 0.1f, 0.01f);
		nn_.setFullConnection(2, 0.1f, 0.01f);

		{
			ConvFilter2D filter;
			filter.initialize(5, 5, 1, 1, 2, 2, 0.1, 0.01);
			ConvImage2D im;
			im.width_ = scr_width_;
			im.height_ = scr_height_;;
			ConvImage2D om;
			om.width_ = scr_width_;
			om.height_ = scr_height_;

			ConvConnection2D *new_conv = nn_.setConvConnection2D(0);

			int in_count = 0;
			int out_count = 0;
			for (int h = 0; h < num_input_histories_; h++)
			{
				for (int ch = 0; ch < num_channels; ch++)
				{
					new_conv->channel_list_.push_back(new ConvChannel2D(filter, (T)0.1, (T)0.01, im, in_count, om, out_count));

					out_count += num_state_variables_;
				}

				in_count += num_state_variables_;
			}
		}

		{
			ConvFilter2D filter;
			filter.initialize(scr_width_, scr_height_, 1, 1, 0, 0, 0.1, 0.01);
			ConvImage2D im;
			im.width_ = scr_width_;
			im.height_ = scr_height_;
			ConvImage2D om;
			om.width_ = 1;
			om.height_ = 1;

			int in_count = 0;
			int out_count = 0;

			ConvConnection2D *new_conv = nn_.setConvConnection2D(1);

			for (int h = 0; h < num_input_histories_; h++)
			{
				for (int ch = 0; ch < num_channels; ch++)
				{
					new_conv->channel_list_.push_back(new ConvChannel2D(filter, (T)0.1, (T)0.01, im, in_count, om, out_count));

					in_count += num_state_variables_;
					out_count++;
				}
			}
		}
			

		//history_.initialize(num_exp_replay_ + num_input_histories_);
		memory_.reserve(1e5);

		gamma_ = 0.95f;

		old_input_vector_.initialize(nn_.num_input_, true);
		next_input_vector_.initialize(nn_.num_input_, true);

		//TODO: don't train if histories are not enough
		//for (int h = 0; h < num_input_histories_; h++)
		//	history_.pushBack(game_.getStateBuffer());
	}

	void trainReward()
	{
		/*for (int ix_from_end = 0; ix_from_end > -num_exp_replay_; ix_from_end--)
			trainReward(ix_from_end);*/

		trainReward(0);	// train with last memory
		// train precious experience only
		//if (history_.getValue(-2).reward_ > 0.5)
		//	for (int i = 0; i < 10; i++)
		//		trainReward(0);
		//else
		//	trainReward(0);
	}

	void trainRewardMemory()
	{
		for (int ix_from_end = 0; ix_from_end > -(memory_.num_elements_ - num_input_histories_); ix_from_end--)
			trainReward(ix_from_end);
	}

	void trainReward(const int ix_from_end)
	{
		// guess next Q value
		makeInputVectorFromHistory(ix_from_end, next_input_vector_);

		//const float reward_ix = history_.getValue(history_.array_.num_elements_ - 1 - 0 + ix_from_end).reward_;
		const float reward_ix = memory_.getRewardFromLast(ix_from_end);

		nn_.setInputVector(next_input_vector_);
		nn_.feedForward();

		const float next_Q = reward_ix < 0.0f ? 0.0f : nn_.getOutputValueMaxComponent(); // final status test
		//const float next_Q = nn_.getOutputValueMaxComponent();

		//const int selected_dir = history_.getValue(history_.getLastIndex() -1 + ix_from_end).choice_; // last history is in one step future 
		const int selected_dir = memory_.getSelectedIxFromLast(ix_from_end);
		//TODO: default training direction!

		makeInputVectorFromHistory(ix_from_end-1, old_input_vector_);

		//const float high_reward_th = 0.8;
		const float high_reward_tr_ep = 0.1f;

		//if (high_reward_th <= reward_ix || reward_ix < 0.1)
		{
			//std::cout << "High reward training start" << std::endl;

			int count = 0;
			while (true)			
//			for(int r = 0; r < 1; r ++)
			{
				nn_.setInputVector(old_input_vector_); // old input
				nn_.feedForward();
				nn_.copyOutputVectorTo(false, reward_vector_);
				
				const float target = reward_ix + gamma_ * next_Q;
				const float error = ABS(reward_vector_[selected_dir] - target);
				
				reward_vector_[selected_dir] = reward_ix + gamma_ * next_Q;

				nn_.propBackward(reward_vector_);

				nn_.check();

				if (error < high_reward_tr_ep || count > 10000)
				{
					//std::cout << "High reward training error " << error << " " << reward_vector_[selected_dir] << " " << (reward_ix + gamma_ * next_Q) << std::endl;

					break;
				}

				count++;
			}
			//std::cout << "High reward training end" << std::endl;
		}
	}

	void forward()
	{
		makeInputVectorFromHistory(0, old_input_vector_);
		nn_.setInputVector(old_input_vector_);
		nn_.feedForward();
	}

	// push back this to history
	void recordHistory(const VectorND<float>& state_vector, const float& reward, const int& choice, const VectorND<float>& q_values)
	{
		//const GameState gstemp(state_vector, choice, reward);

		//history_.pushBack(gstemp);
		//TODO: this makes slower
		memory_.append(state_vector, choice, reward, q_values);
	}

	void makeInputVectorFromHistory(const int& ix_from_end, VectorND<float>& input)
	{
		for (int r = 0, count = 0; r < num_input_histories_; r++, count += num_state_variables_)
		{
			const VectorND<float> &state_vector =
				memory_.getStateVectorFromLast(ix_from_end - r);
				//history_.getValue(history_.getLastIndex() - r + ix_from_end).state_vector_;

			input.copyPartial(state_vector, count, 0, num_state_variables_);
		}
	}

};