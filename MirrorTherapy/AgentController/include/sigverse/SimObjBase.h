/* $Id: SimObjBase.h,v 1.6 2011-12-21 09:19:52 okamoto Exp $ */ 
#ifndef SimObjBase_h
#define SimObjBase_h

#include <string>
#include <map>
#include <vector>

#include "systemdef.h"
#include "Attribute.h"
#include "Vector3d.h"
#include "Logger.h"


class Parts;
class Rotation;

/**
 * @brief シミュレーションエンティティ基本クラス
 */
class SimObjBase 
{
public:
	//! 例外基本クラス
	class Exception
	{
	protected:
		std::string m_msg;
	public:
		Exception() {}
		Exception(const char *msg_) : m_msg(msg_) {;}
		const char *msg() { return m_msg.c_str(); }
	};



	//! 必要な属性を保持していないときに発生する例外
	class NoAttributeException : public Exception
	{
	public:
		NoAttributeException(const char *attr) : Exception()
		{
			if (attr) {
				m_msg = "No attribute : \"";
				m_msg += attr;
				m_msg += "\"";
			}
		}
	};



	//! 読み込み専用属性の値を設定したときに発生する例外
	class AttributeReadOnlyException : public Exception
	{
	public:
		AttributeReadOnlyException(const char *attr) : Exception() {
			if (attr) {
				m_msg = "Read only attribute : \"";
				m_msg += attr;
				m_msg += "\"";
			}
		}
	};


	/**
	 * @brief 部品オブジェクトイテレータ
	 *
	 * 継承クラスで使用
	 */
	class PartsIterator {
	public:
		virtual ~PartsIterator() {}
		virtual Parts *next() = 0;
	};
#ifndef UNIT_TEST
protected:
#endif
	enum {
		OP_NOT_SET = 0x0,			//!< 操作なし
		OP_SET_POSITION = 0x1,		//!< 位置属性の修正操作
		OP_SET_ROTATION = 0x2,		//!< 回転属性の修正操作
		OP_SET_FORCE = 0x4,			//!< 力ベクトルの設定操作
		OP_SET_ACCEL = 0x8,			//!< 加速度の設定操作
		OP_SET_TORQUE = 0x10,		//!< トルクの設定操作
		OP_SET_VELOCITY = 0x20,		//!< 速度の設定操作
		OP_SET_ANGULAR_VELOCITY = 0x40,	//!< 角速度の設定操作
	};
	//! エンティティ操作型
	typedef unsigned Operation;
public:
	//! エンティティID型
	typedef unsigned Id;
protected:
	//! 文字列型
	typedef std::string S;
	//! 属性マップ型
	typedef std::map<S, Attribute*> AttrM;
protected:
	bool	m_attached;    //!< コントローラが割り当てられているか？
	//Id  	m_id;          //!< エンティティID。エンティティに割り振られる一意な値。
	int m_id;
	AttrM	m_attrs;       //!< 属性マップ
	Operation m_ops;       //!< 呼び出された操作
	std::vector<std::string>  m_files; //!< shapeファイル名
	std::vector<int>  m_ids;           //!< entityが持つカメラID

protected:
	//! コンストラクタ
	SimObjBase();
public:
	//! デストラクタ
	virtual ~SimObjBase();

	//! コピーメソッド
	void copy(const SimObjBase &o);

	//! 属性マップを取得する
	const AttrM &attrs() const { return m_attrs; }

	// shapeファイルを追加する	
	void addFile(const char* name){ m_files.push_back(name);}

	// shapeファイルの数を取得する
	int getFileNum(){ return m_files.size();}

	//! entityが持つカメラの数を取得する
	int getCameraNum(){return m_ids.size();}

	// entityが持つカメラIDを追加する
	void addCameraID(int id)
	{
	  int size = m_ids.size();
	  for(int i = 0; i < size; i++){
	    if(id == m_ids[i]) {
	      LOG_ERR(("Camera ID %d is already exist.",id));
	      return;	      
	    }
	  }
	  m_ids.push_back(id);
	}

	//! entityが持つカメラID(vector)を取得する
	std::vector<int> getCameraIDs(){return m_ids;}

	//! shapeファイル名を取得する
	std::string getFile(int num)
	{
	  if(num > m_files.size()){
	    return NULL;
	  }
	  else{
	    return m_files[num];
	  }
	}
	
	//! 属性を追加する
	void	push(Attribute *attr)
	{
		m_attrs[attr->name()] = attr;
	}
	/**
	 * @brief 属性値を文字列で設定する
	 *
	 * 値は文字列で与えるが、属性値の型に応じて適切にパースされる。
	 *
	 * @param name 属性名
	 * @param v    属性値
	 */
	void	setAttrValue(const char *name, const char *v)
	{
		getAttr(name).value().setString(v);
	}

	/**
	 * @brief 属性値を設定する
	 *
	 * @param name 属性名
	 * @param v    属性値
	 */
	void	setAttrValue(const char *name, const Value &v)
	{
		getAttr(name).value().copy(v);
	}

	/**
	 * @brief 必要な属性があるか確認する
	 * シミュレーションを実行するのに最低限必要な属性があるか確認する
	 * @retval true  必要な属性を備えている
	 * @retval false 必要な属性が備えてない
	 */
	bool	checkAttrs();

private:
	Attribute* hasAttr(const char *name) const;
public:
	//! 指定の属性を得る
	Attribute & getAttr(const char *name) const;

	//! 指定の属性が存在するかどうか
	bool isAttr(const char *name);

	//! エンティティIDを得る
	Id    	id() const { return m_id; }
	//! コントローラが割り当てられているか？
	bool	isAttached() const { return m_attached; }

	//!エンティティ名を得る
	const char *name() const
	{
		return getAttr("name").value().getString();
	}

	//! クラス名を得る
	const char *classname()
	{
		return getAttr("class").value().getString();
	}

	// added by yahara@tome (2011/02/23)
#define DEFINE_ATTR_STRING(NAME, TOKEN)					\
		const char *NAME() const { return getAttr(TOKEN).value().getString(); } \
	void  NAME(const char *v) {						\
	  getAttr(TOKEN).value().setString(v);				\
	}
	//okamoto (2010/12/7)
#define DEFINE_ATTR_DOUBLE(NAME, TOKEN)					\
        double NAME() const { return getAttr(TOKEN).value().getDouble(); } \
	void  NAME(double v) {						\
	  getAttr(TOKEN).value().setDouble(v);				\
	}
	
#define DEFINE_ATTR_BOOL(NAME, TOKEN)				       \
        bool NAME() const { return getAttr(TOKEN).value().getBool(); } \
        void NAME(bool b) { getAttr(TOKEN).value().setBool(b); }

#include "SimObjBaseAttrs.h"

#undef DEFINE_ATTR_DOUBLE
#undef DEFINE_ATTR_BOOL
	// added by yahara@tome (2011/03/03)
#undef DEFINE_ATTR_STRING

	// Operation methods must be declared as virtual.
	/**
	 * @brief エンティティの位置を設定する
	 * @param v 3次元位置
	 */
	virtual void	setPosition(const Vector3d &v);
	/**
	 * @brief エンティティの位置を設定する
	 * @param x_ X座標値
	 * @param y_ Y座標値
	 * @param z_ Z座標値
	 */
	virtual void 	setPosition(double x_, double y_, double z_);
	/**
	 * @brief エンティティにかかる力を設定する
	 * @param fx X軸成分
	 * @param fy Y軸成分
	 * @param fz Z軸成分
	 */
	virtual void	setForce(double fx, double fy, double fz);

	/**
	 * @brief エンティティにかかるトルクを設定する
	 * @param x X軸成分
	 * @param y Y軸成分
	 * @param z Z軸成分
	 */
	virtual void	setTorque(double x, double y, double z);

	/**
	 * @brief エンティティにかかる速度を設定する
	 * @param vx_ X軸成分
	 * @param vy_ Y軸成分
	 * @param vz_ Z軸成分
	 */
	virtual void setVelocity(double vx_,double vy_,double vz_);

	/**
	 * @brief エンティティにかかる角速度を設定する
	 * @param x_ X軸成分[rad/s]
	 * @param y_ Y軸成分[rad/s]
	 * @param z_ Z軸成分[rad/s]
	 */
	virtual void setAngularVelocity(double x_,double y_,double z_);

	/**
	 * @brief エンティティの位置を取得する
	 */
	Vector3d &  getPosition(Vector3d &v)
	{
		v.set(x(), y(), z());
		return v;
	}
		
	/**
	 * @brief エンティティの向きを設定する
	 *
	 * @param ax 回転軸のX成分
	 * @param ay 回転軸のY成分
	 * @param az 回転軸のZ成分
	 * @param angle 回転角度
	 */
	virtual void setAxisAndAngle(double ax, double ay, double az, double angle);


        /*!
         * @brief It rotates for the specification of the relative angle.
         * @brief 相対角度指定による回転を行います
         * @param[in] X軸回転有無(四元数虚数部i)/x-axis rotation weather(i of quaternion complex part)
         * @param[in] Y軸回転有無(四元数虚数部j)/y-axis rotation weather(j of quaternion complex part)
         * @param[in] Z軸回転有無(四元数虚数部k)/z-axis rotation weather(k of quaternion complex part)
	 * @param[in] 指定フラグ(1.0=絶対角度指定,else=相対角度指定)                    
         */
	virtual void setAxisAndAngle(double ax, double ay, double az, double angle, double direct);

	/**
	 * @brief エンティティの向きを設定する
	 *
	 * @param r 回転行列
	 */
	virtual void setRotation(const Rotation &r);

private:
	void 	setQ(const dReal *q);
public:

	/**
	 * @brief エンティティデータをバイナリ化する
	 * @param バイナリデータサイズ
	 * @return バイナリデータ
	 */
	char *toBinary(int &n);

protected:
	//! パーツイテレータを取得する
	virtual PartsIterator * getPartsIterator() = 0;
private:
	void	free_();

public:
	void dump();
#ifdef UNIT_TEST
	Operation ops() { return m_ops; }
#endif	
#ifdef IMPLEMENT_DRAWER
	void draw(DrawContext &c);
#endif
};

#endif // SimObjBase_h
 
