/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "LayerBase.h"
#include "ActivationFunctions.h"

void LayerBase::resize(const int& _dim)
{
	act_.initialize(_dim, true);
	grad_.initialize(_dim, true);

	act_[act_.num_dimension_ - 1] = 1.0;	// last element is bias
}

void LayerBase::initialize(const int& _dim, const ActType& _type)
{
	act_type_ = _type;

	resize(_dim);
}

void LayerBase::assignErrorToGrad(const VectorND<T>& target)
{
	assert(act_.num_dimension_ >= target.num_dimension_); // target may include bias or not
	assert(grad_.num_dimension_ >= target.num_dimension_);
	assert(act_.num_dimension_ == grad_.num_dimension_);

	for (int d = 0; d < act_.num_dimension_ - 1; d++)
	{
		grad_[d] = (target[d] - act_[d]);
	}
}

void LayerBase::activate()
{
	switch (act_type_)
	{
	case Sigmoid:
		ActFunc::applySigmoidToVector(1, act_);		// 1 is not to activate bias at the end of act_ vector
		break;

	case ReLU:
		ActFunc::applyRELUToVector(1, act_);		// 1 is not to activate bias at the end of act_ vector
		break;

	default:
		ActFunc::applyLRELUToVector(1, act_);		// 1 is not to activate bias at the end of act_ vector
	}
}

void LayerBase::multiplyActGradToGrad()
{
	switch (act_type_)
	{
	case Sigmoid:
		for (int d = 0; d < act_.num_dimension_ - 1; d++)   // skip last component (bias)
		{
			grad_[d] *= ActFunc::getSigmoidGradFromY(act_[d]);
		}
		break;
	case ReLU:
		for (int d = 0; d < act_.num_dimension_ - 1; d++)   // skip last component (bias)
		{
			grad_[d] *= ActFunc::getRELUGradFromY(act_[d]);
		}
		break;
	default:
		for (int d = 0; d < act_.num_dimension_ - 1; d++)   // skip last component (bias)
		{
			grad_[d] *= ActFunc::getLRELUGradFromY(act_[d]);
		}
	}
}

void LayerBase::setActType(const ActType& type)
{
	act_type_ = type;
}