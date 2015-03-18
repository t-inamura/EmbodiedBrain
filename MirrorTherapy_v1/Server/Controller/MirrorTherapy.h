
#ifndef _QUATERNION_UTIL_
#define _QUATERNION_UTIL_s

#include <math.h>

char right_hand[][16] =
{
                "RARM"
};

char left_hand[][16] =
{
                "LARM"
};

char right_leg[][16] =
{
                "RLEG"
};

char left_leg[][16] =
{
                "LLEG"
};

void getConjugateQuaternion(double* w,double* x,double* y,double* z){
	//w* = *w;
	*x = -*x;
	*y = -*y;
	*z = -*z;
}


#endif    //QUATERNION