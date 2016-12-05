/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <assert.h>
#include <iostream>

#include "ConventionalMacros.h"
template<class TT>
class VectorND
{
public:
	int num_dimension_;
	TT *values_;

public:
	VectorND(void)
		: num_dimension_(0), values_(nullptr)
	{};

	VectorND(const int& num)
	{
		values_ = nullptr;

		initialize(num);
	}

	VectorND(const VectorND<TT>& vector_)
	{
		values_ = nullptr;

		initialize(vector_.num_dimension_, false);

		for(int i=0; i < num_dimension_; i ++)	values_[i] = vector_[i];
	}

	~VectorND(void)
	{
		if(values_ != 0) delete [] values_;
		num_dimension_ = 0;
	}

public:
	void initialize(const int& num_, const bool initialize = false)
	{
		assert(num_ >= 0);

		if (num_ < 1)
		{
			num_dimension_ = num_;

			SAFE_DELETE_ARRAY(values_);
		}
		else
		{
			if (num_dimension_ != num_)
			{
				SAFE_DELETE_ARRAY(values_);
				values_ = new TT[num_];
			}

			num_dimension_ = num_;

			if (initialize == true) for (int i = 0; i < num_dimension_; i++) values_[i] = TT();
		}
	}

public:
	void operator = (const VectorND<TT>& from)
	{
        if (from.num_dimension_ != num_dimension_)
        {
            num_dimension_ = from.num_dimension_;

            SAFE_DELETE(values_);

            values_ = new TT[num_dimension_];
        }

		for(int i = 0; i < num_dimension_; i++) values_[i] = from[i];
	}

	TT& operator[](const int& i) const
	{
        assert(i >= 0);
        assert(i < num_dimension_);

		return values_[i];
	}

    TT& operator()(const int& i) const
    {
        assert(i >= 0);
        assert(i < num_dimension_);

        return values_[i];
    }

	VectorND<TT> operator+(const VectorND & vector) const
	{
		assert(num_dimension_ == vector.num_dimension_);

		VectorND<TT> result(num_dimension_);

		for(int i=0;i<num_dimension_;i++) result[i] = values_[i] + vector.values_[i];

		return result;
	}

	VectorND<TT> operator-(const VectorND & vector)const
	{
		assert(num_dimension_ == vector.num_dimension_);

		VectorND<TT> result(num_dimension_);

		for(int i=0;i<num_dimension_;i++) result[i] = values_[i] - vector.values_[i];

		return result;
	}

	//T SqrMagnitude() const
 //   {
	//	T sum(0);
	//	for(int i=0;i<num_dimension_;i++)
	//		sum+=SQUARE(values_[i]);
	//	return sum;
	//}

	//T MaxAbs() const
	//{
	//	T max_abs(0);
	//	for(int i=0;i<num_dimension_;i++) max_abs = max(abs(values_[i]), max_abs);
	//	return max_abs;
	//}

	void operator += (const TT& s)
	{
		for(int i = 0; i < num_dimension_; i++) values_[i] += s;
	}

	void operator -= (const TT& s)
	{
		for(int i = 0; i < num_dimension_; i++) values_[i] -= s;
	}

	void operator *= (const int& s)
	{
		for(int i = 0; i < num_dimension_; i++) values_[i] *= s;
	}

	void operator *= (const TT& s)
	{
		for(int i = 0; i < num_dimension_; i++) values_[i] *= s;
	}

	void operator /= (const TT& s)
	{
		for(int i = 0; i < num_dimension_; i++) values_[i] /= s;
	}

	void operator += (const VectorND<TT>& s)
	{
		assert(num_dimension_ == s.num_dimension_);

		for(int i = 0; i < num_dimension_; i++) values_[i] += s.values_[i];
	}

	void operator -= (const VectorND<TT>& s)
	{
		assert(num_dimension_ == s.num_dimension_);
		
		for(int i = 0; i < num_dimension_; i++) values_[i] -= s.values_[i];
	}

	void operator *= (const VectorND<TT>& s)
	{
		assert(num_dimension_ == s.num_dimension_);

		for(int i = 0; i < num_dimension_; i++) values_[i] *= s.values_[i];
	}

	void operator /= (const VectorND<TT>& s)
	{
		assert(num_dimension_ == s.num_dimension_);

		for(int i = 0; i < num_dimension_; i++) values_[i] /= s.values_[i];
	}

	VectorND <TT> operator*(const TT& s) const
	{
		VectorND <TT> V(num_dimension_);

		for(int i=0; i < num_dimension_; i++) V.values_[i]=values_[i]*s;

		return V;
	}

	void assignAllValues(const TT& input)
	{
		for (int i = 0; i < num_dimension_; i++) values_[i] = input;
	}

    void copyPartial(const VectorND<TT>& source, const int& start_ix_this, const int& start_ix_source, const int& num)
    {
        assert(start_ix_this >= 0);
        assert(start_ix_this + num <= num_dimension_);

        assert(start_ix_source >= 0);
        assert(start_ix_source + num <= source.num_dimension_);

        for (int i = 0; i < num; i++)
        {
            values_[start_ix_this + i] = source.values_[start_ix_source + i];
        }
    }

	void write(std::ofstream& os) const
	{
		os.write((char*)&i_start_, sizeof(i_start_));
		os.write((char*)&j_start_, sizeof(j_start_));
		os.write((char*)&k_start_, sizeof(k_start_));

		os.write((char*)&i_res_, sizeof(i_res_));
		os.write((char*)&j_res_, sizeof(j_res_));
		os.write((char*)&k_res_, sizeof(k_res_));

		for (int p = 0; p < num_dimension_; p++)
			os.write((char*)&values_[p], sizeof(TT));
	}

	void writeTXT(std::ofstream& of) const
	{
		of << num_dimension_ << std::endl;
		for (int i = 0; i < num_dimension_; i++)
		{
			of << values_[i];

			if (i != num_dimension_ - 1)
				of << " ";
		}
		of << std::endl;
	}

	void check() const
	{
		for (int i = 0; i < num_dimension_; i++)
		{
			if (std::isnan(values_[i]))
			{
				std::cout << "Nan " << values_[i] << std::endl;

				assert(false);

				exit(1);
			}

			if (std::isinf(values_[i]))
			{
				std::cout << "inf " << values_[i] << std::endl;

				assert(false);

				exit(1);
			}
		}
	}

	TT getMaxValue()
	{
		assert(num_dimension_ > 0);

		TT max = values_[0];
		
		int ix = 0;

		for (int d = 1; d < num_dimension_; d++)
		{
			if (max < values_[d])
			{
				max = values_[d];
				ix = d;
			}
		}

		return max;
	}
};

template<class TT>
void dotProduct(const VectorND<TT>& v1, const VectorND<TT>& v2, TT& sum)
{
	assert(v1.num_dimension_ == v2.num_dimension_);

	sum = 0;

	for (int i = 0; i < v1.num_dimension_; i++)
		sum += v1.values_[i] * v2.values_[i];	
}
//
//
//inline static T dotProduct(const VectorND<T>& v1,const VectorND<T>& v2)
//{
//	assert(v1.num_dimension_ == v2.num_dimension_);
//
//	T sum=0;
//
//	for(int i=0; i<v1.num_dimension_; i++)
//		sum += v1.values_[i]*v2.values_[i];
//
//	return sum;
//}
//
//inline static T dotProduct(const VectorND<T>& v1, const VectorND<T>& v2, const int& start_ix, const int& end_ix)
//{
//	assert(v1.num_dimension_ == v2.num_dimension_);
//	
//	T *v1_values = v1.values_, *v2_values = v2.values_;
//
//	T sum(0);
//
//	for(int i = start_ix; i <= end_ix; i++) sum += v1_values[i] * v2_values[i];
//
//	return sum;
//}
//
//template<class TT>
//TT dotProduct(MT* mt, const int& thread_id, const VectorND<TT>& v1, const VectorND<TT>& v2)
//{
//	assert(v1.num_dimension_ == v2.num_dimension_);
//
//	TT *v1_values = v1.values_, *v2_values = v2.values_;
//
//	const int start_ix(mt->start_ix_1D_[thread_id]), end_ix(mt->end_ix_1D_[thread_id]);
//
//	TT dot_product = (TT)0;
//
//	for(int i = start_ix; i <= end_ix; i++) dot_product += v1_values[i] * v2_values[i];
//
//	dot_product = mt->syncSum(thread_id, dot_product);
//
//	return dot_product;
//}

template<class TT> 
std::ostream& operator<<(std::ostream& output, const VectorND<TT>& v)
{
	for(int i=0; i<v.num_dimension_; i++) output<<v.values_[i]<<" ";
    output << std::flush;
//	output << std::endl;
	return output;
}
