/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VectorND.h"
#include <assert.h>
#include <fstream>

template<class T>
class MatrixMN
{
public:
    int num_rows_;  // m_
    int num_cols_;  // n_
    T *values_;

    MatrixMN()
        : values_(nullptr), num_rows_(0), num_cols_(0)
    {}

    void initialize(const int& _m, const int& _n, const bool init = true)
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

	void assignRandom(const T& scale, const T& min)
	{
		const int num_all = num_rows_ * num_cols_;
		for (int i = 0; i < num_all; i++)
			values_[i] = (T)rand() / (T)RAND_MAX * scale + min;
	}

	void assignAll(const T& v)
	{
		const int num_all = num_rows_ * num_cols_;
		for (int i = 0; i < num_all; i++)
			values_[i] = v;
	}

    void multiply(const VectorND<T>& vector, VectorND<T>& result) const;
    void multiplyTransposed(const VectorND<T>& vector, VectorND<T>& result) const;

    int get1DIndex(const int& row, const int& column) const
    {
        assert(row >= 0);
        assert(column >= 0);
        assert(row < num_rows_);
        assert(row < num_cols_);

        // column = i, row = j
        return column + row * num_cols_;        // data structure is for faster dot product of a row vector and VectorND input.
    }

    T& getValue(const int& row, const int& column) const
    {
        return values_[get1DIndex(row, column)];
    }

    //T multiplyRowAndVectorWithBias(const int& row, const VectorND<T>& vector, const T& bias)    // (v0, v1, ..., vn-1, bias)
    //{
    //    assert(num_cols_ == vector.num_dimension_ + 1); // +1 is for bias
    //    
    //    T dot = (T)0;
    //    for (int col = 0; col < num_cols_ - 1; col++)   // num_cols_ - 1 : don't operate on bias now.
    //    {
    //        dot += getValue(row, col) * vector[col];
    //    }

    //    dot += getValue(row, num_cols_ - 1) * bias;     // last column value is the weight of bias

    //    return dot;
    //}

    //void multiplyVectorWithBias(const VectorND<T>& vector, const T& bias, VectorND<T>& result)
    //{
    //    assert(num_cols_ == (vector.num_dimension_ + 1));
    //    assert(num_rows_ == result.num_dimension_);

    //    for (int row = 0; row < num_rows_; row++)
    //    {
    //        result[row] = multiplyRowAndVectorWithBias(row, vector, bias);
    //    }
    //}

    //void multiplyTransWithBias(const VectorND<T>& vector, VectorND<T>& result)
    //{
    //    assert(num_rows_ <= vector.num_dimension_); // don't multiply last bias component
    //    assert(num_cols_ == result.num_dimension_);

    //    for (int col = 0; col < num_cols_; col++)
    //    {
    //        result[col] = (T)0;

    //        for (int row = 0; row < num_rows_; row++)
    //        {
    //            result[col] += getValue(row, col) * vector[row];
    //        }
    //    }
    //}

    void getTransposed(MatrixMN<T>& m_tr)
    {
        m_tr.initialize(num_cols_, num_rows_);

#pragma omp parallel for
        for (int row = 0; row < num_rows_; row++)
        {
            int ix_from = row * num_cols_;
            int ix_to = row;

            for (int col = 0; col < num_cols_; col++, ix_from++, ix_to += num_rows_)
            {
                  m_tr.values_[ix_to] = values_[ix_from];
            }
        }

        // test codes
        /*MatrixMN<double> mat;
        mat.initialize(2, 3);
        mat.getValue(0, 0) = 1;
        mat.getValue(0, 1) = 2;
        mat.getValue(0, 2) = 3;

        mat.getValue(1, 0) = 4;
        mat.getValue(1, 1) = 5;
        mat.getValue(1, 2) = 6;

        mat.cout();

        MatrixMN<double> mat2;

        mat.getTransposed(mat2);
        mat2.cout();*/
    }

	void setDiagonal()
	{
		const int num = MIN2(num_cols_, num_rows_);

		for (int i = 0; i < num_cols_ * num_rows_; i++) values_[i] = 0.0;

		for (int i = 0; i < num; i++) getValue(i, i) = 1.0;
	}

    void cout()
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

	void normalizeAllRows(const T& row_sum_min)
	{
		for (int row = 0; row < num_rows_; row++)
			normalizeRow(row, row_sum_min);
	}

	void normalizeRow(const int& row, const T& row_sum_min)
	{
		T row_sum = (T)0;

		for (int col = 0; col < num_cols_-1; col++) // TODO normalize bias option
		{
			row_sum += getValue(row, col);
		}

		if (row_sum > row_sum_min)
		{
			for (int col = 0; col < num_cols_-1; col++)// TODO normalize bias option
			{
				getValue(row, col) /= row_sum;
			}
		}
	}

	void writeTXT(std::ofstream& of) const
	{
		of << num_rows_ << " " << num_cols_ << std::endl;
		for (int i = 0; i < num_rows_ * num_cols_; i++)
		{
			if (i != 0 && i % num_cols_ == 0) of << std::endl;

			of << values_[i];

			if (i != num_rows_ * num_cols_ - 1)
				of << " ";			
		}
		of << std::endl;
	}

	void check() const
	{
		for (int i = 0; i < num_rows_ * num_cols_; i++)
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
};
