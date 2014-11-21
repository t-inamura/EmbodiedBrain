/* $Id: Vector3d.h,v 1.2 2011-03-31 08:15:58 okamoto Exp $ */ 
#ifndef Vector3d_h
#define Vector3d_h

#include <math.h>
#include "Logger.h"
#include "Position.h"
//#include "Rotation.h"

class Rotation;

/**
 * @brief 3次元ベクトルクラス
 */
class Vector3d
{
private:
	double	m_x, m_y, m_z;
public:
	//! コンストラクタ
	Vector3d() : m_x(0.0), m_y(0.0), m_z(0.0) {;}
	/**
	 * @brief コピーコンストラクタ
	 * @param x_ X軸要素
	 * @param y_ Y軸要素
	 * @param z_ Z軸要素
	 */
	Vector3d(double x_, double y_, double z_) : m_x(x_), m_y(y_), m_z(z_) {;}
	//! コピーコンストラクタ
	Vector3d(const Vector3d &o) : m_x(o.m_x), m_y(o.m_y), m_z(o.m_z) {;}

	//! 各要素を設定する
	void	set(double x_, double y_, double z_)
	{
		m_x = x_; m_y = y_; m_z = z_;
	}

	/**
	 * @brief ベクトルを平行移動させる
	 * @param v 並進ベクトル
	 */
	void	shift(const Vector3d &v)
	{
		m_x += v.x(); m_y += v.y(); m_z += v.z();
	}

	/**
	 * @brief ベクトルを平行移動させる
	 * @param x_ 並進ベクトルX軸要素
	 * @param y_ 並進ベクトルY軸要素
	 * @param z_ 並進ベクトルZ軸要素
	 */
	void	shift(double x_, double y_, double z_)
	{
		m_x += x_; m_y += y_; m_z += z_;
	}

	
	void	x(double v) { m_x = v; } //!< X軸要素を設定する
	void	y(double v) { m_y = v; } //!< Y軸要素を設定する
	void	z(double v) { m_z = v; } //!< Z軸要素を設定する

	
	double x() const { return m_x; } //!< X軸要素を取得する
	double y() const { return m_y; } //!< Y軸要素を取得する
	double z() const { return m_z; } //!< Z軸要素を取得する

	//! +=演算子(shiftと等価)
	Vector3d & operator+=(const Vector3d &o)
	{
		this->m_x += o.x();
		this->m_y += o.y();
		this->m_z += o.z();
		return *this;
	}

	//! -=演算子(shiftと等価)
	Vector3d & operator-=(const Vector3d &o)
	{
		this->m_x -= o.x();
		this->m_y -= o.y();
		this->m_z -= o.z();
		return *this;
	}
	//! +=演算子(shiftと等価)
	Vector3d & operator+=(const Position &o)
	{
		this->m_x += o.x();
		this->m_y += o.y();
		this->m_z += o.z();
		return *this;
	}

	//! -=演算子(shiftと等価)
	Vector3d & operator-=(const Position &o)
	{
		this->m_x -= o.x();
		this->m_y -= o.y();
		this->m_z -= o.z();
		return *this;
	}

	//! *=演算子(各要素に指定の値を掛ける)
	Vector3d & operator*=(double v)
	{
		this->m_x *= v;
		this->m_y *= v;
		this->m_z *= v;
		return *this;
	}

	//! /=演算子(各要素を指定の値で割る)
	Vector3d & operator/=(double v)
	{
		this->m_x /= v;
		this->m_y /= v;
		this->m_z /= v;
		return *this;
	}

	//! /=同じではない
	bool operator!=(const Vector3d &o)
	{
	  if(this->m_x != o.x() ||
	     this->m_y != o.y() ||
	     this->m_z != o.z())
	    {
	      return true;
	    }
	  else{ return false; }
	}

	//! ベクトルの長さ取得する
	double length() const { return sqrt(m_x*m_x + m_y*m_y + m_z*m_z); }

	/**
	 * 与えられたベクトルと成す角を得る
	 * @return cosθ(-1～1の範囲)
	 */
	double angle(const Vector3d &axis)
	{
		double prod = m_x*axis.m_x + m_y*axis.m_y + m_z*axis.m_z;
		double v = length() * axis.length();
		return prod/v;

	}

	//! ベクトルを規格化する(長さを1にする)
	void	normalize()
	{
		double l = length();
		m_x /=l; m_y/=l; m_z/=l;
	}

	/*
	  070809 yoshi
	  shared library で使用するとロードに失敗する 
	*/
	/**
	 * @brief ベクトルを回転する
	 * @param r 回転行列
	 * @return 回転後のベクトル
	 */
	Vector3d & rotate(const Rotation &r);

	// なぜかコントローラで使えない
	/*
	void rotateByQuaternion(double qw, double qx, double qy, double qz);
	*/
};


#endif // Vector3d_h
 
