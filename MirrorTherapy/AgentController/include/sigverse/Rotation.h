/* $Id: Rotation.h,v 1.3 2011-07-27 07:12:28 okamoto Exp $ */ 
#ifndef Rotation_h
#define Rotation_h

#include "systemdef.h"
#include <string.h>

#ifdef USE_ODE
#include <ode/ode.h>
#endif

#include "Vector3d.h"

enum {
	ROTATION_TYPE_NOT_SET = -1,
	ROTATION_TYPE_QUATERNION,
	ROTATION_TYPE_NUM,
};

typedef short RotationType;

/**
 * @brief 回転行列クラス
 */
class Rotation
{
private:
	dQuaternion	m_q;
#ifdef USE_ODE
	dMatrix3	m_m;
#endif
private:
	void	makeMatrix()
	{
#ifdef USE_ODE
		dQtoR(m_q, m_m);
#endif
	}
public:
	//! コンストラクタ
	Rotation();
	//! コピーコンストラクタ
	Rotation(const Rotation &o);

	//! コンストラクタ
	Rotation(double w, double x, double y, double z){
	  setQuaternion(w, x, y, z);
	}
	//! クォータニオンを設定する
	void	setQuaternion(const dReal *q)
	{
	  setQuaternion(q[0], q[1], q[2], q[3]);
	}
	//! クォータニオンを設定する
	void	setQuaternion(dReal w, dReal x, dReal y, dReal z)
	{
		int i=0; 
		m_q[i] = w; i++;
		m_q[i] = x; i++;
		m_q[i] = y; i++;
		m_q[i] = z; i++;

		makeMatrix();
	}

	//! クォータニオンを取得する added by okamoto@tome 2011/2/22
	dReal	qw(){return m_q[0];}
	dReal	qx(){return m_q[1];}
	dReal	qy(){return m_q[2];}
	dReal	qz(){return m_q[3];}


	//! クォータニオンを取得する
	const 	dReal * q() const { return m_q; }

#ifdef USE_ODE
	//! 回転軸と角度を指定して回転行列を作成する
	void	setAxisAndAngle(double ax, double ay, double az, double angle)
	{
		dQFromAxisAndAngle(m_q, ax, ay, az, angle);
		makeMatrix();
	}

        /*!
         * @brief It rotates for the specification of the relative angle.
         * @brief 相対角度指定による回転を行います
         * @param[in] X軸回転有無(四元数虚数部i)/x-axis rotation weather(i of quaternion complex part)
         * @param[in] Y軸回転有無(四元数虚数部j)/y-axis rotation weather(j of quaternion complex part)
         * @param[in] Z軸回転有無(四元数虚数部k)/z-axis rotation weather(k of quaternion complex part)
         * @param[in] 絶対角度指定,相対角度指定フラグ(1.0=絶対角度指定,else=相対角度指定)
         */
        void    setAxisAndAngle(double ax, double ay, double az, double angle, double direct)
        {
	  if (direct == 1.0) {
	    // It rotates absolutely for the specification of the angle.
	    // 既存処理、絶対角度を適応します
	    dQFromAxisAndAngle(m_q, ax, ay, az, angle);
	    makeMatrix();
	  } else {
	    // It rotates for the specification of the relative angle.
	    // 新規処理、現在の角度に相対角度を適応します
	    dQuaternion qt1, qt2;
	    dQFromAxisAndAngle(qt1, ax, ay, az, angle);
	    dQMultiply0(qt2, q(), qt1); // 前回との差分を変動させる事に注意
	    setQuaternion(qt2[0], qt2[1], qt2[2], qt2[3]);
	  }
        }

	//! 回転行列を得る
	const dReal * matrix() const
	{
		return m_m;
	}
	/**
	 * @brief 回転行列を得る
	 *
	 * @param r 行番号
	 * @param c 列番号
	 */

	dReal operator()(int r, int c)  const {
		return m_m[r*4 + c];
	}

	/**
	 * @brief 指定の行に右からベクトルを掛けたときの値を算出する
         *
	 * @param row ベクトルを作用させる行
	 * @param v ベクトル
 	 */
	dReal	apply(int row, const Vector3d &v) const
	{
		const Rotation &r = *this;
		return r(row, 0)*v.x()  + r(row, 1)*v.y() + r(row, 2)*v.z();
	}
	
	/**
	 * @brief 右からベクトルを掛けたときのベクトルを算出する
         *
	 * @param v 作用させるベクトル
	 * @param o 結果用ベクトル
 	 */
	Vector3d & apply(const Vector3d &v, Vector3d &o) const
	{
		o.x(apply(0, v));
		o.y(apply(1, v));
		o.z(apply(2, v));
		return o;
	}

	/**
	 * @brief 右からベクトルを掛けたときのベクトルを算出する
	 *
	 * 結果は引数で与えたベクトルに格納する。
         *
	 * @param v 作用させるベクトル
 	 */
	Vector3d & apply(Vector3d &v) const
	{
		Vector3d v_;
		v_.x(apply(0, v));
		v_.y(apply(1, v));
		v_.z(apply(2, v));
		v = v_;
		return v;
	}

	//! *演算子(行列同士のかけ算)
	Rotation & operator*=(const Rotation &o);
	//! コピー演算子
	Rotation & operator=(const Rotation &o);

	//! !=演算子(一致しない場合)
	bool operator!=(Rotation &o)
	{
	  if(this->m_q[0] != o.qw() ||
	     this->m_q[1] != o.qx() ||
	     this->m_q[2] != o.qy() || 
	     this->m_q[3] != o.qz())
	    {
	      return true;
	    }
	  else{ return false; }
	}


#endif
};


#endif // Rotation_h
 
