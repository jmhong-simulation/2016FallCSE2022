/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VectorND.h"

template<class T>
class MatrixMN
{
public:
    int num_rows_;  // m_
    int num_cols_;  // n_
    T   *values_;

    MatrixMN()
        : values_(nullptr), num_rows_(0), num_cols_(0)
    {}

	void initialize(const int& _m, const int& _n, const bool init = true);

	void multiply(const VectorND<T>& vector, VectorND<T>& result) const;
    void multiplyTransposed(const VectorND<T>& vector, VectorND<T>& result) const;

	int get1DIndex(const int& row, const int& column) const;
	T&  getValue(const int& row, const int& column) const;

	void cout();
};
