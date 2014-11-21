/* $Id: Size.h,v 1.2 2011-03-31 08:15:57 okamoto Exp $ */ 
#ifndef Size_h
#define Size_h

#include "Position.h"

//! シミュレーション世界内における3次元直方体の大きさを表すクラス
class Size : public Position
{
	typedef Position Super;
public:
	//! コンストラクタ
	Size() : Super() {;}
	/**
	 * @brief コンストラクタ
	 * @param x X方向の辺の長さ
	 * @param y Y方向の辺の長さ
	 * @param z Z方向の辺の長さ
	 */
	Size(dReal x_, dReal y_, dReal z_) : Super(x_, y_, z_) {;}
	//! コピーコンストラクタ
	Size(const Size &s) : Super(s) {;}
};


#endif // Size_h
 
