/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "MatrixMN.h"
//#include <atomic>
#include <xmmintrin.h>

template<class T>
void MatrixMN<T>::multiply(const VectorND<T>& vector, VectorND<T>& result) const
{
    assert(num_rows_ <= result.num_dimension_);
    assert(num_cols_ <= vector.num_dimension_);

    // default implementation
    //for (int row = 0; row < num_rows_; row++)
    //{
    //    result[row] = (T)0;

    //    for (int col = 0; col < num_cols_; col++)
    //    {
    //        result[row] += getValue(row, col) * vector[col];

    //        //if (std::isnan(result[row]) == true)
    //        //{
    //        //    std::cout << "NAN" << std::endl;
    //        //    exit(1);
    //        //}
    //    }
    //}

    //for (int ix = 0; ix < rho4.ijk_res_; ix += 4)
    //{
    //    __m128 *r1 = (__m128*) &rho1.values_[ix];
    //    __m128 *r2 = (__m128*) &rho2.values_[ix];
    //    __m128 *r4 = (__m128*) &rho4.values_[ix];

    //    *r4 = _mm_mul_ps(*r1, *r2);
    //}

    // float SSE2 implementation
//#pragma omp parallel for
//    for (int row = 0; row < num_rows_; row++)
//    {
//        int ix = row*num_cols_, col = 0;
//        int loop_end = num_cols_ - num_cols_ % 4;
//
//        __m128 r1, r2, r3;
//        __m128 r4 = _mm_setzero_ps();
//
//        for (; col < loop_end; col += 4, ix += 4)
//        {
//            r1 = _mm_loadu_ps(&values_[ix]);
//            r2 = _mm_loadu_ps(&vector.values_[col]);
//            r3 = _mm_mul_ps(r1, r2);
//            r4 = _mm_add_ps(r4, r3);
//        }
//
//        float temp[4];
//        _mm_storeu_ps(temp, r4);
//
//        result.values_[row] = temp[0];
//        result.values_[row] += temp[1];
//        result.values_[row] += temp[2];
//        result.values_[row] += temp[3];
//
//        for (; col < num_cols_; col++, ix++)
//        {
//             result.values_[row] += values_[ix] * vector.values_[col];
//        }
//    }

#pragma omp parallel for
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

    // default implementation
    //for (int col = 0; col < num_cols_; col++)
    //{
    //    result[col] = (T)0;

    //    for (int row = 0; row < num_rows_; row++)
    //    {
    //        result[col] += getValue(row, col) * vector[row];
    //    }
    //}

    //TODO: remove racing problem (use local memory and add)
//  std::atomic_int *result_values = (std::atomic_int*)result.values_;
//#pragma omp parallel for
//    for (int col = 0; col < num_cols_; col++)
//        result.values_[col] = (T)0;
//
//#pragma omp parallel for
//    for (int row = 0; row < num_rows_; row++)
//    {
//        int ix = row*num_cols_;
//        const T temp = vector.values_[row];
//        T temp2;
//
//        for (int col = 0; col < num_cols_; col++, ix++)
//        {
//            temp2 = temp;
//            temp2 *= values_[ix];
//
//            //result.values_[col] += temp2;
//            result_values[col] += temp2;
//        }
//    }

    //const int two = 2 * num_cols_;
    //const int three = 3 * num_cols_;
    //const int four = 3 * num_cols_;

    // slower
//#pragma omp parallel for
//    for (int col = 0; col < num_cols_; col++)
//    {
//        float temp[4];
//
//        int row = 0, ix = col;
//        int loop_end = num_rows_ - num_rows_ % 4;
//
//        __m128 r1, r2, r3;
//        __m128 r4 = _mm_setzero_ps();
//
//        for (; row < loop_end; row += 4, ix += four)
//        {
//            r1 = _mm_set_ps(values_[ix], values_[ix + num_cols_], values_[ix + two], values_[ix + three]);
//            //__m128 r2 = _mm_set_ps(vector.values_[row], vector.values_[row + 1], vector.values_[row + 2], vector.values_[row + 3]);
//            r2 = _mm_loadu_ps(&vector.values_[row]);
//            r3 = _mm_mul_ps(r1, r2);
//            r4 = _mm_add_ps(r4, r3);
//        }
//
//        _mm_storeu_ps(temp, r4);
//
//        result.values_[col] = temp[0];
//        result.values_[col] += temp[1];
//        result.values_[col] += temp[2];
//        result.values_[col] += temp[3];
//
//        for (; row < num_rows_; row++, ix += num_cols_)
//        {
//            result.values_[col] += values_[ix] * vector.values_[row];
//        }
//    }

#pragma omp parallel for
    for (int col = 0; col < num_cols_; col++)
    {
        result.values_[col] = (T)0;

        for (int row = 0, ix = col; row < num_rows_; row++, ix += num_cols_)
        {
            result.values_[col] += values_[ix] * vector.values_[row];
        }
    }
}

template void MatrixMN<float>::multiply(const VectorND<float>& vector, VectorND<float>& result) const;
template void MatrixMN<float>::multiplyTransposed(const VectorND<float>& vector, VectorND<float>& result) const;

//template void MatrixMN<double>::multiply(const VectorND<double>& vector, VectorND<double>& result) const;
//template void MatrixMN<double>::multiplyTransposed(const VectorND<double>& vector, VectorND<double>& result) const;