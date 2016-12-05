/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////


#pragma once

#include <assert.h>
#include <fstream>
#include <iostream>

#define SAFE_DELETE_ARRAY(pointer) if(pointer != nullptr){delete [] pointer; pointer=nullptr;}

template<class TT>
class Array1D
{
public:
	int num_elements_; // number of all elements, i.e. thus the index of last element is num_element - 1.
	TT *values_;

public:
	Array1D()
		: num_elements_(0), values_(nullptr)
	{}

	Array1D(const int& num_elements_input)
		: num_elements_(0), values_(nullptr)
	{
		initialize(num_elements_input);
	}
	
	Array1D(const int& num_elements_input, const TT& values_input)
		: num_elements_(0), values_(nullptr)
	{
		initialize(num_elements_input, values_input);
	}

	Array1D(const Array1D<TT>& array_input)
		: num_elements_(0), values_(nullptr)
	{
		initialize(array_input);
	}
	
	~Array1D()
	{
		SAFE_DELETE_ARRAY(values_);

		num_elements_ = 0;
	}
	
	void initialize(const int& num_elements_input)
	{
		num_elements_ = num_elements_input;

		SAFE_DELETE_ARRAY(values_);

		if (num_elements_ > 0)
			values_ = new TT[num_elements_];
	}
	
	void initialize(const int& num_elements_input, const TT& values_input)
	{
		num_elements_ = num_elements_input;

		SAFE_DELETE_ARRAY(values_);

		if (num_elements_ > 0)
		{
			values_ = new TT[num_elements_];

			assignAllValues(values_input);
		}
	}
	
	void initialize(const Array1D<TT>& array_input)
	{
		initialize(array_input.num_elements_);

		copyFrom(array_input);
	}
	
	void assignAllValues(const TT& constant)
	{
		for (int w = 0; w < num_elements_; w++) values_[w] = constant;
	}
	
	void assignValues(const int& start_ix, const int& end_ix, const TT& constant)
	{
		for (int w = start_ix; w <= end_ix; w++) values_[w] = constant;
	}
	
	void assignMin(const int& ix, const TT& value)
	{
		TT &temp = *(values_ + ix);
		temp = MIN2(value, temp);
	}

	void copyFrom(const Array1D<TT>& from)
	{
		assert(num_elements_ == from.num_elements_);

		TT *from_val = from.values_;

		for (int w = 0; w < num_elements_; w++) values_[w] = from_val[w];
	}

	void resize(const int new_size)
	{
		TT *new_values = new TT[new_size];

		for (int i = 0; i < MIN2(new_size, num_elements_); i++)
		{
			new_values[i] = values_[i];
		}

		SWAP(values_, new_values, TT*);

		SAFE_DELETE_ARRAY(new_values);

		num_elements_ = new_size;
	}

	void freeMemory()
	{
		num_elements_ = 0;

		SAFE_DELETE_ARRAY(values_);
	}
	
	TT& operator [] (const int& i) const
	{
		assert(i >= 0);
#ifndef NDEBUG

		if (!(i < num_elements_))
		{
			const int do_something = (int)3.141592;
		}

#endif
		assert(i < num_elements_);

		return values_[i];
	}
	
	const int getSizeOfData() const
	{
		return num_elements_*sizeof(TT);
	}
	
	const int getSizeOfType() const
	{
		return sizeof(TT);
	}
	
	void compactArray(Array1D<int>& flag_array, const int remove_flag)
	{
		// count num of valid elements
		int new_num_elements = 0;
		for (int i = 0; i < num_elements_; i++)
		{
			if (flag_array[i] != remove_flag) flag_array[i] = new_num_elements++;
		}

		static Array1D<TT> temp;
		temp.initialize(new_num_elements);		// for multi-threading

		// copy valid data
		for (int i = 0; i < num_elements_; i++)
		{
			const int new_ix = flag_array[i];

			if (new_ix == remove_flag) continue;

			temp[new_ix] = values_[i];
		}

		// swap
		num_elements_ = temp.num_elements_;
		delete[] values_;
		values_ = temp.values_;

		temp.num_elements_ = 0;
		temp.values_ = nullptr;
	}

	void copyToPartialArray(Array1D<TT>& dest, const int start, const int end) {

		dest.initialize(end - start + 1);

		
		for (int ix = 0, i = start; i <= end; i++, ix++) {
			
			dest.values_[ix] = values_[i];

		}

	}

	void append(const Array1D<TT>& source)
	{
		const int num_temp = num_elements_;

		resize(num_elements_ + source.num_elements_);

		for (int i = num_temp; i < num_elements_; i++)
		{
			values_[i] = source.values_[i - num_temp];
		}
	}

	int searchIncremental(const TT& v)
	{
		// perform domain search when values are sorted incrementally 

		for (int i = 0; i < num_elements_; i++)
		{
			if (v < values_[i]) return i - 1;
		}

		return num_elements_;
	}

	void accumulateFromLeft()
	{
		TT acc = (TT)0;
		for (int i = 0; i < num_elements_; i++)
		{
			values_[i] += acc;

			acc = values_[i];
		}
	}

	friend std::ostream& operator<< (std::ostream& stream, const Array1D<TT>& arr)
	{
		for(int i = 0; i < arr.num_elements_; i ++)
			stream << arr[i]<<" ";

		return stream;
	}

	void read(std::ifstream& is)
	{
		int num_elements;

		is.read((char*)&num_elements, sizeof(num_elements));

		initialize(num_elements);

		for (int i = 0; i < num_elements; i++)
			is.read((char*)&values_[i], sizeof(TT));
	}

	void write(std::ofstream& os) const
	{
		os.write((char*)&num_elements_, sizeof(num_elements_));

		for (int i = 0; i < num_elements_; i++)
			os.write((char*)&values_[i], sizeof(TT));
	}

	void operator *= (const TT& constant)
	{
		for (int w = 0; w < num_elements_; w++) values_[w] *= constant;
	}

	void operator += (const TT& constant)
	{
		for (int w = 0; w < num_elements_; w++) values_[w] += constant;
	}

	void operator -= (const TT& constant)
	{
		for (int w = 0; w < num_elements_; w++) values_[w] -= constant;
	}


	/*
	//TODO not yet tested
	
	std::ofstream& write(std::ofstream& os)
	{
	os.write((char*)&num_elements_, sizeof(num_elements_));

	for (int i = 0; i < num_elements_; i++)
	{
	os.write((char*)&values_[i], sizeof(TT));
	}
	return os;
	}
	*/

	/*
	//TODO not yet tested
	
	std::ifstream& read(std::ifstream& is)
	{
	is.read((char*)&num_elements_, sizeof(num_elements_));

	for (int i = 0; i < num_elements_; i++)
	{
	is.read((char*)&values_[i], sizeof(TT));
	}

	return is;
	}
	*/
};
