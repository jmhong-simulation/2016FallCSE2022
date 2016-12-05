/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef PI
	#define PI 3.14159265358979323846
#endif

#define ABS(a) ((a) > 0 ? (a) : -(a))

#define MIN2(a, b)							((a) > (b) ? (b) : (a))
#define MIN3(a, b, c)						(MIN2(MIN2(a, b), (c)))
#define MIN4(a, b, c, d)					(MIN2(MIN3(a, b, c), (d)))
#define MIN5(a, b, c, d, e)					(MIN2(MIN4(a, b, c, d), (e)))
#define MIN7(a, b, c, d, e, f, g)			(MIN2(MIN3(a, b, c), MIN4(d, e, f, g)))
#define MIN8(a, b, c, d, e, f, g, h)		(MIN2(MIN7(a, b, c, d, e, f, g), h))

#define MAX2(a, b)							((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)						(MAX2(MAX2(a, b), (c)))
#define MAX4(a, b, c, d)					(MAX2(MAX3(a, b, c), (d)))
#define MAX5(a, b, c, d, e)					(MAX2(MAX4(a, b, c, d), (e)))
#define MAX7(a, b, c, d, e, f, g)			(MAX2(MAX3(a, b, c), MAX4(d, e, f, g)))
#define MAX8(a, b, c, d, e, f, g, h)		(MAX2(MAX7(a, b, c, d, e, f, g), h))

#define MIN_ABS2(a, b)						(ABS(a) > ABS(b) ? (b) : (a))
#define MAX_ABS2(a, b)						(ABS(a) > ABS(b) ? (a) : (b))

#define CLAMP(v, min, max)		((v) > (max) ? (max) : ((v) < (min) ? (min) : (v)))

#define SQUARE(a)				((a)*(a))
#define POW3(a)					((a)*(a)*(a))

#define SWAP(a,b,type)			{type __temp__ = a; a = b; b = __temp__;}

inline float POW2(const float& a){return a*a;}
inline float POW4(const float& a){const float a2 = a*a; return a2*a2;}
inline float POW5(const float& a){const float a2 = a*a; return a2*a2*a;}
inline float POW6(const float& a){const float a3 = a*a*a; return a3*a3;}
inline float POW7(const float& a){const float a3 = a*a*a; return a3*a3*a;}
inline float POW8(const float& a){const float a2 = a*a; const float a4 = a2*a2; return a4*a4;}

inline double POW2(const double& a){return a*a;}
inline double POW4(const double& a){const double a2 = a*a; return a2*a2;}
inline double POW5(const double& a){const double a2 = a*a; return a2*a2*a;}
inline double POW6(const double& a){const double a3 = a*a*a; return a3*a3;}
inline double POW7(const double& a){const double a3 = a*a*a; return a3*a3*a;}
inline double POW8(const double& a){const double a2 = a*a; const double a4 = a2*a2; return a4*a4;}

#define SIGN(a) (a > 0 ? 1 : -1) //Note: a == 0 returns negative because phi <= 0 means inside.

#define SAFE_DELETE(pointer) if(pointer != nullptr){delete pointer; pointer=nullptr;}
#define SAFE_DELETE_ARRAY(pointer) if(pointer != nullptr){delete [] pointer; pointer=nullptr;}

// sort a, b, c -> a1 < a2 < a3	
#define INCREASING_SORT3(a, b, c, a1, a2, a3)		if(a <= b){										\
														if(b <= c){a1 = a;a2 = b;a3 = c;}			\
														else if(a <= c){a1 = a;a2 = c;a3 = b;}		\
														else{a1 = c;a2 = a;a3 = b;}}				\
													 else{											\
														if(a <= c){a1 = b;a2 = a;a3 = c;}			\
														else if(b <= c){a1 = b;a2 = c;a3 = a;}		\
														else{a1 = c;a2 = b;a3 = a;}}

#define INCREASING_SORT2(a, b, a1, a2)				if(a <= b){a1 = a; a2 = b;} \
													else{a1 = b; a2 = a;}													 

// debugging helper macros
#define PRINT_NAME_AND_VALUE(variable)			{std::cout<<#variable<<" = "<<variable<<std::endl;}
#define PRINT_NAME_AND_VALUE_EXIT(variable)  	{std::cout<<#variable<<" = "<<variable<<std::endl; exit(1);}
#define PRINT_AND_EXIT(variable)  				{std::cout<<variable<<std::endl; exit(1);}
#define EXIT_IF(expression)						if(expression) exit(1);
#define COUT(words)                             {BEGIN_HEAD_THREAD_WORK{std::cout<<words<<std::endl;}END_HEAD_THREAD_WORK}

const int POW_OF_TWO(const int& n);
const int POW_OF_EIGHT(const int& n);