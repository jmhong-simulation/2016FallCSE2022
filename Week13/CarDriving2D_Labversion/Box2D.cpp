#include "Box2D.h"

template<class TT>
bool Box2D<TT>::isInside(const TT& x, const TT& y)
{
	if(x < x_min_) return false;
	else if(x > x_max_) return false;

	if(y < y_min_) return false;
	else if(y > y_max_) return false;

	return true;
}

template class Box2D<int>;
template class Box2D<float>;
template class Box2D<double>;
