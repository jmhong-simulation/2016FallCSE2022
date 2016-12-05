/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "MatrixMN.h"

#define SAFE_DELETE_ARRAY(pointer) if(pointer != nullptr){delete [] pointer; pointer=nullptr;}

template<class T>
void MatrixMN<T>::initialize(const int& _m, const int& _n, const bool init = true)
{
	const int num_all_old = num_rows_ * num_cols_;

	num_rows_ = _m;
	num_cols_ = _n;

	SAFE_DELETE_ARRAY(values_);

	const int num_all = num_rows_ * num_cols_;

	if (num_all_old != num_all) // allocate memory if num_all is changed
	{
		// check if the matrix is too large
		assert((double)num_rows_ * (double)num_cols_ <= (double)INT_MAX);

		values_ = new T[num_all];

		if (init == true)
			for (int i = 0; i < num_all; i++)
				values_[i] = (T)0;
	}
}

template<class T>
void MatrixMN<T>::multiply(const VectorND<T>& vector, VectorND<T>& result) const
{
    assert(num_rows_ <= result.num_dimension_);
    assert(num_cols_ <= vector.num_dimension_);

    for (int row = 0; row < num_rows_; row++)
    {
        result.values_[row] = (T)0;

        int ix = row*num_cols_;
        T temp;

        for (int col = 0; col < num_cols_; col++, ix++)
        {
            temp = values_[ix];
            temp *= vector.values_[col];

            result.values_[row] += temp;
        }
    }
}

template<class T>
void MatrixMN<T>::multiplyTransposed(const VectorND<T>& vector, VectorND<T>& result) const 
{
    assert(num_rows_ <= vector.num_dimension_);
    assert(num_cols_ <= result.num_dimension_);

    for (int col = 0; col < num_cols_; col++)
    {
        result.values_[col] = (T)0;

        for (int row = 0, ix = col; row < num_rows_; row++, ix += num_cols_)
        {
            result.values_[col] += values_[ix] * vector.values_[row];
        }
    }

	//Note: You may transpose matrix and then multiply for better performance.
	//See Eigen library. http://eigen.tuxfamily.org/index.php?title=Main_Page
}

template<class T>
void MatrixMN<T>::cout()
{
	for (int row = 0; row < num_rows_; row++)
	{
		for (int col = 0; col < num_cols_; col++)
		{
			std::cout << getValue(row, col) << " ";
		}

		std::cout << std::endl;
	}
}

template<class T>
int MatrixMN<T>::get1DIndex(const int& row, const int& column) const
{
	assert(row >= 0);
	assert(column >= 0);
	assert(row < num_rows_);
	assert(row < num_cols_);

	// column = i, row = j
	return column + row * num_cols_;        // data structure is for faster dot product of a row vector and VectorND input.
}

template<class T>
T& MatrixMN<T>::getValue(const int& row, const int& column) const
{
	return values_[get1DIndex(row, column)];
}

template class MatrixMN<float>;
template class MatrixMN<double>;

//template void MatrixMN<float>::multiply(const VectorND<float>& vector, VectorND<float>& result) const;
//template void MatrixMN<float>::multiplyTransposed(const VectorND<float>& vector, VectorND<float>& result) const;
//
//template void MatrixMN<double>::multiply(const VectorND<double>& vector, VectorND<double>& result) const;
//template void MatrixMN<double>::multiplyTransposed(const VectorND<double>& vector, VectorND<double>& result) const;