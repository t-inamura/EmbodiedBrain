/* $Id: CParts.h,v 1.4 2011-05-12 08:33:50 msi Exp $ */ 
#ifndef CParts_h
#define CParts_h

#include "systemdef.h"
#include "Parts.h"
//#include "SimObj.h"

/**
 * @brief エンティティの部品クラス(クライアントサイドで使用)
 *
 * @see SParts
 */
class CParts : public Parts
{
protected:
	/**
	 * @brief コンストラクタ
	 *
	 * @param t 部品タイプ
	 * @param name 部品名
	 * @param pos 部品の中心位置
	 */
	CParts(PartsType t, const char *name, const Position &pos)
		: Parts(t, name, pos) {}

	//! コピーコンストラクタ
	CParts(const CParts &o)
		: Parts(o) {}

  SOCKET m_sock;
  
  // パーツ所有オブジェクト名
  std::string m_owner;
private:
	void 	setQuaternion(dReal q0, dReal q1, dReal q2, dReal q3)
	{
		m_rot.setQuaternion(q0, q1, q2, q3);
	}
public:
	//! デストラクタ
	virtual ~CParts() {}
#ifdef CONTROLLER
	bool getPosition(Vector3d &v);
	Rotation & getRotation(Rotation &r);
#endif
	//! 同等の部品クラスオブジェクトを作成する
	virtual CParts * clone() = 0;

	const dReal * getPosition();
	const dReal * getRotation();
	const dReal * getQuaternion();


	//! サーバ送信用ソケット
	void setSocket(SOCKET sock)
	{
		m_sock = sock;
	}

	//! 所有者オブジェクト
	void setOwner(const char *name)
	{
	  m_owner = name;
	}

	//! オブジェクトを掴む
	bool graspObj(std::string name);

	//! オブジェクトを離す
	void releaseObj();

	//! 他のオブジェクトと衝突中かどうか取得する
	bool getCollisionState();
	

public:
#ifdef SIGVERSE_OGRE_CLIENT
	static CParts * decode(char *data, int aid);
#elif (defined IRWAS_OGRE_CLIENT)
	static CParts * decode(char *data, int aid);
#else
	/**
	 * @brief バイナリ化された部品データを復元する
	 */
	static CParts * decode(char *);
#endif	// SIGVERSE_OGRE_CLIENT
};

class BoxPartsCmpnt;

/**
 * @brief 箱形状の部品クラス(クライアントサイドで使用)
 */
class BoxParts : public CParts
{
private:
	BoxPartsCmpnt *m_cmpnt;
public:
	/**
	 * @brief コンストラクタ
	 *
	 * @param name 部品名
	 * @param pos 部品の中心位置
	 * @param sz  箱のサイズ(縦×横×高さ)
	 */
	BoxParts(const char *name, const Position &pos, const Size &sz);
	//! デストラクタ
	~BoxParts();

	// added by sekikawa(2007/11/30)
	void giveSize(double &x, double &y, double &z);
	void dump();
	
private:
	BoxParts(const BoxParts &o);

	PartsCmpnt * extdata();
private:
	CParts * clone() { return new BoxParts(*this); }

#ifdef IMPLEMENT_DRAWER
	void draw(DrawContext &c);
#endif	
};

class CylinderPartsCmpnt;

/**
 * @brief シリンダ形状の部品クラス(クライアントサイドで使用)
 */
class CylinderParts : public CParts
{
private:
	CylinderPartsCmpnt *m_cmpnt;
public:
	/**
	 * @brief コンストラクタ
	 *
	 * @param name 部品名
	 * @param pos 部品の中心位置
	 * @param rad  シリンダの円の半径
	 * @param len シリンダの長さ
	 */
	CylinderParts(const char *name,
		      const Position &pos, dReal rad, dReal len);
	//! デストラクタ
	~CylinderParts();

	// added by sekikawa(2007/11/30)
	void giveSize(double &radius, double &length);

	void dump();

private:
	CylinderParts(const CylinderParts &o);

private:
	CParts * clone() { return new CylinderParts(*this); }

	PartsCmpnt * extdata();
#ifdef IMPLEMENT_DRAWER
	void draw(DrawContext &c);
#endif

};

class SpherePartsCmpnt;

/**
 * @brief 球形状の部品クラス(クライアントサイドで使用)
 */
class SphereParts : public CParts
{
private:
	SpherePartsCmpnt *m_cmpnt;
public:
	/**
	 * @brief コンストラクタ
	 *
	 * @param name 部品名
	 * @param pos 部品の中心位置
	 * @param radius  球の半径
	 */
	SphereParts(const char *name, const Position &pos, double radius);
	
	//! デストラクタ
	~SphereParts();

private:
	SphereParts(const SphereParts &o);

//private:
public:
	// added by sekikawa(2007/11/30)
	void giveRadius(double &radius);
	void dump();

private:
	CParts * clone() { return new SphereParts(*this); }
	PartsCmpnt * extdata();
	
#ifdef IMPLEMENT_DRAWER
	void draw(DrawContext &c);
#endif
};

#endif // CParts_h
 
