/*
 * SigCmn.h
 *
 *  Created on: 2015/07/01
 *      Author: tome-yamada
 */

#ifndef __SIG_CMN_H__
#define __SIG_CMN_H__

#include <string>
#include <sstream>
#include <cmath>


/*!
 * @brief Translate degree to radian.
 */
#define DEG2RAD(DEG) ( std::M_PI * (DEG) / 180.0 )


class SigCmn
{
public:

	///@brief For expression position.
	typedef struct _Vector3
	{
		float x;
		float y;
		float z;
	} Vector3;

	///@brief For expression position.
	typedef struct _Vector4
	{
		float w;
		float x;
		float y;
		float z;
	} Vector4;

	///@brief For expression position.
	typedef struct
	{
		float yaw;
		float pitch;
		float roll;
	} EulerAngleType;

	/*
	 * Calculate Vector from 2 position.
	 */
	static bool diffVec(SigCmn::Vector3 &rvec, const SigCmn::Vector3 &jvec, const SigCmn::Vector3 &kvec);
};

#endif /* __SIG_CMN_H__ */