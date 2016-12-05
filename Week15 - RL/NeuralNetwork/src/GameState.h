/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VectorND.h"
#include <vector>

class GameMemory // TODO: rename this to GameMemory
{
public:
	int num_elements_ = 0;
	int num_reserve_ = 10000;

	std::vector<VectorND<float> > state_vector_array_;
	std::vector<int> selected_array_;
	std::vector<float> reward_array_;
	std::vector<VectorND<float> > q_values_array_;

	GameMemory()
	{}

	void reserve(const int& num_reserve)
	{
		state_vector_array_.reserve(num_reserve);
		selected_array_.reserve(num_reserve);
		reward_array_.reserve(num_reserve);
		q_values_array_.reserve(num_reserve);

		num_reserve_ = num_reserve;
	}

	void reset()
	{
		num_elements_ = 0;

		state_vector_array_.clear();
		selected_array_.clear();
		reward_array_.clear();
		q_values_array_.clear();

		reserve(num_reserve_);
	}

	void append(const VectorND<float>& state_vector, const int& choice, const float& reward, const VectorND<float>& q_values)
	{
		assert(num_elements_ < num_reserve_);

		state_vector_array_.push_back(state_vector);
		selected_array_.push_back(choice);
		reward_array_.push_back(reward);
		q_values_array_.push_back(q_values);

		num_elements_++;
	}

	const VectorND<float>& getStateVectorFromLast(const int& ix_from_last) // ix_from_last = 0 returns last element, use -1, -2 ,...
	{
		return state_vector_array_[num_elements_ - 1 + ix_from_last];
	}

	const int& getSelectedIxFromLast(const int& ix_from_last)
	{
		return selected_array_[num_elements_ - 1 + ix_from_last];
	}

	const float& getRewardFromLast(const int& ix_from_last)
	{
		return reward_array_[num_elements_ - 1 + ix_from_last];
	}

	const VectorND<float>& getQValuesFromLast(const int& ix_from_last) // ix_from_last = 0 returns last element, use -1, -2 ,...
	{
		return q_values_array_[num_elements_ - 1 + ix_from_last];
	}

	/*GameMemory(const VectorND<float>& sv, const int& ch, const float& rew)
	{
		state_vector_ = sv;
		choice_ = ch;
		reward_ = rew;
	}*/

	/*GameState(const GameState& _gs)
		: state_vector_(_gs.state_vector_), choice_(_gs.choice_), reward_(_gs.reward_), Q_(_gs.Q_)
	{}*/

	/*void operator = (const GameState& from)
	{
		state_vector_ = from.state_vector_;
		choice_ = from.choice_;
		reward_ = from.reward_;
	}*/
};