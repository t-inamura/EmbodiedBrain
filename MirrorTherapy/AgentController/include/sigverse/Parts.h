/* $Id: Parts.h,v 1.3 2011-05-12 08:33:51 msi Exp $ */
#ifndef Parts_h
#define Parts_h

#include "systemdef.h"

#ifdef USE_ODE
#include <ode/ode.h>
#endif

#include <string>
#include <stdio.h>

#include "Position.h"
#include "Size.h"
#include "Rotation.h"

class ODEObj;
class ODEWorld;
class SimObjBase;

enum {
	PARTS_TYPE_NOT_SET = -1,
	PARTS_TYPE_BOX,
	PARTS_TYPE_CYLINDER,
	PARTS_TYPE_SPHERE,
	PARTS_TYPE_NUM,
};

typedef short PartsType;

/**
 * @brief パーツを構成する要素インターフェース
 */
class PartsCmpnt
{
public:
	//! デストラクタ
	virtual ~PartsCmpnt() {};

	//! バイナリ化する際のサイズを取得する
	virtual int datalen() = 0;

	//! バイナリ化データを取得する
	virtual char *dataBinary() = 0;

	//! 保持している情報（サイズ）の表示
	virtual void dump() = 0;	// FIX20110421(ExpSS)
};

/**
 * @brief エンティティを構成する部品クラス
 */
class Parts
{
public:
	typedef unsigned Id;
private:
	static	Id	s_cnt;
public:
	static void	initCounter() { s_cnt = 0; }
private:
	typedef std::string S;
protected:
	PartsType	m_type;
	S		m_name;

	Position	m_pos;
	Rotation	m_rot;

	Id		m_id;
	bool		m_blind;

	char * 		m_buf;
	int		m_bufsize;
private:
	void	free_();

protected:
	Parts(PartsType t, const char *name, const Position &pos)
		: m_type(t), m_name(name), m_pos(pos),
		  m_id(0), m_blind(false), m_buf(NULL), m_bufsize(0) {}

	Parts(const Parts &o) :
		m_type(o.m_type), m_name(o.m_name),
		m_pos(o.m_pos), m_rot(o.m_rot),
		m_id(o.m_id), m_blind(o.m_blind),
		m_buf(NULL), m_bufsize(0) {}
public:
	//! デストラクタ
	virtual ~Parts() {	free_(); }
public:
	/**
	 * @brief IDの付与
	 *
	 * IDはパーツについての一意の番号。
	 */
	void	setId(Id id) { m_id = id; }

	/**
	 * @brief IDの自動付与
	 *
	 * IDはパーツについての一意の番号。
	 */
	void	addId() {
		s_cnt++;
		m_id = s_cnt;
	}
	//! IDの取得
	Id	id() { return m_id; }

	//! パーツの種類の取得
	PartsType getType() { return m_type; }

	//! パーツ名の取得
	const char *name(){ return m_name.c_str(); }

	//! body(エンティティの中心を構成する要素)であるか？
	bool	isBody() const {
		return strcmp(m_name.c_str(), "body") == 0? true: false;
	}

	//! 不可視のパーツであるか？
	bool	isBlind() const { return m_blind; }

protected:
	void	setBlind(bool b) { m_blind = b; }

public:
	//! クォータニオンの設定
	virtual void	setQuaternion(dReal q0, dReal q1, dReal q2, dReal q3) = 0;

	//! 位置の取得
	virtual const dReal * getPosition() = 0;

	//! 回転行列の取得
	virtual const dReal * getRotation() = 0;

	//! クォータニオンの取得
	virtual const dReal * getQuaternion() = 0;

	//! 位置の取得
	void givePosition(double &x, double &y, double &z);

	//! クォータニオンの取得
	void giveQuaternion(double &qw, double &qx, double &qy, double &qz);

	/**
	 * @brief パーツオブジェクトのバイナリ化
	 *
	 * @param n バイナリデータサイズ
	 * @return	バイナリデータ
	 */
	char *	toBinary(int &n);

public:
	virtual PartsCmpnt * extdata() = 0;

	// ダンプメソッド(デバッグ用)
	virtual void	dump();

#ifdef IMPLEMENT_DRAWER
	virtual void draw(DrawContext &c) = 0;
#endif

};

#endif // Parts_h

