/* $Id: Position.h,v 1.2 2011-03-31 08:15:57 okamoto Exp $ */ 
#ifndef Position_h
#define Position_h

#include "systemdef.h"

// added by sekikawa (2009/3/24)
// compile error occurred (memcpy() not found) when USE_ODE not defined.
#include <string.h>

#ifdef USE_ODE
#include "ode/ode.h"
#endif

#ifdef UNIT_TEST
#include <math.h>
#include <float.h>
#endif

/**
 * @brief 3次元位置クラス
 *
 * シミュレーション世界内の位置を表す。
 * 位置を表す変数の型は、ODEを使用する場合ODEに従う。ODEを使
 * 用しない場合はdouble型。
 * 
 */
class Position
{
private:
	enum { DIMENSION = 3,};
private:
	dReal m_values[DIMENSION];
public:
	//! コンストラクタ
	Position() {
		set(0.0, 0.0, 0.0);
	}
	
	/**
	 * @brief コンストラクタ
	 * @param x X座標値
	 * @param y Y座標値
	 * @param z Z座標値
	 */
	Position(dReal x_, dReal y_, dReal z_) 
	{
		set(x_, y_, z_);
	}
	//! コピーコンストラクタ
	Position(const Position &o) 
	{
		copy(o);
	}
	/**
	 * @brief 各座標値を設定する
	 * @param x X座標値
	 * @param y Y座標値
	 * @param z Z座標値
	 */
	void	set(dReal x_, dReal y_, dReal z_)
	{
		m_values[0] = x_;
		m_values[1] = y_;
		m_values[2] = z_;
	}
	//! 座標値を取得する
	const dReal * values() { return m_values; }

	//! X座標値を取得する
	dReal x() const { return m_values[0]; }
	//! Y座標値を取得する
	dReal y() const { return m_values[1]; }
	//! Z座標値を取得する
	dReal z() const { return m_values[2]; }

	//! X座標値を設定する
	void x(dReal v) { m_values[0] = v; }
	//! Y座標値を設定する
	void y(dReal v) { m_values[1] = v; }
	//! Z座標値を設定する
	void z(dReal v) { m_values[2] = v; }

#ifdef IRWAS_TEST_CLIENT
	dReal glx() const { return x(); }
	dReal gly() const { return y(); }
	dReal glz() const { return z(); }
#endif

	//! コピー演算子
	Position &operator=(const Position &o) {
		copy(o);
		return *this;
	}

private:
	void copy(const Position &o){
		memcpy(m_values, o.m_values, sizeof(m_values[0])*DIMENSION);
	}

public:
#ifdef UNIT_TEST
	bool operator==(const Position &o) {
		for (int i=0; i<DIMENSION; i++) {
			if (fabs(m_values[i] - o.m_values[i]) > DBL_EPSILON) {
				return false;
			}
		}
		return true;
	}
#endif
};


#endif // Position_h
 
