/* $Id: SimObj.h,v 1.21 2012-03-27 04:06:44 noma Exp $ */ 
#ifndef SimObj_h
#define SimObj_h

#include <map>
#include <iostream>
#include <vector>

#include "systemdef.h"
#include "SimObjBase.h"
#include "Attribute.h"
#include "CParts.h"
#include "Value.h"

class Vector3d;
#ifdef DEPRECATED
class Command;
#endif

class CommDataEncoder;
class JointForce;
class CommDataResult;

/**
 * @brief エンティティに対応するクラス(クライアントサイドで使用)
 * シミュレーションサーバから対応するエンティティの情報を取
 * 得し、クライアントサイドでそれらにアクセスできるようにす
 * る。SimObjに対する操作・属性等はシミュレーションサーバに
 * 転送され、その結果が反映される。
 *
 */
class SimObj : public SimObjBase
{
public:
	//! パケット送受信インターフェース
	class RequestSender {
	public:
		virtual ~RequestSender() {}
		virtual bool send(CommDataEncoder &) = 0;
		virtual CommDataResult * recv(int bufsize) = 0;
	};
	//! 属性マップ型
	typedef std::map<S, CParts*> PartsM;

	//! コンストラクタ
	SimObj();
	//! デストラクタ
	virtual ~SimObj();

	// added by kawamoto@tome (2011/04/15)
#ifdef WIN32
	//! パケット送受信実装クラスインスタンスを設定する
	void setRequestSener(RequestSender *s) {
		m_sender = s;
	}
#else
	//! this is request-- パケット送受信実装クラスインスタンスを設定する
	void setRequestSener(RequestSender *s);
	/* {
		m_sender = s;
	}*/
#endif

#ifdef WIN32
	// ++++++++++++++ for test only +++++++++++++++++
	// added by sekikawa(2007/11/30)
	void setId(int id);
	// +++++++ this should be deleted later +++++++++
#endif

	/**
	 * @brief エンティティを構成する部品を取得する
	 * @param name 部品名
	 */
	CParts  *getParts(const char *name);

	/**
	 * @brief Jointを持たないエンティティのメインパーツを取得する
	 */
	CParts  *getMainParts(){
	  return getParts("body");
	}

	// added by sekikaw(2007/11/30)
	std::map<std::string, CParts *>& getPartsCollection();

	//! 内部状態をコピーする
	void copy(const SimObj &o);

protected:
	//! 保持している動的メモリを解放する
	void	free_();
	RequestSender * m_sender;

private:
	typedef SimObjBase Super;
	PartsM	m_parts;

  
	class Iterator : public PartsIterator {
	private:
		typedef std::map<std::string, CParts*> M;
	private:
		M &m_map;
		M::iterator m_i;
	public:
		Iterator(M &m) : m_map(m) {
			m_i = m_map.begin();
		}
	private:
		Parts *next() {
			if (m_i == m_map.end()) { return NULL; }
			CParts *p = m_i->second;
			m_i++;
			return p;
		}
	};

public:

	/**
	 * パーツイテレータを作成します
	 */
	PartsIterator *getPartsIterator() {
		return new Iterator(m_parts);
	}

	void 	push(Attribute *attr) {
		Super::push(attr);
	}

	//! パーツを追加する
	void	push(CParts *p);

	int setBinary(char *data, int n);
	
	/**
	 * 車両のダイナミクスを利用するかどうか
	 */
	enum{
		MODE_NOT_USE_WHEEL = 0,	//	車両ダイナミクスを使わない
		MODE_USE_WHEEL		//	車両ダイナミクスを使用
	};
	/**
	 * 車両力学モードのフラグ
	 */
	int dynamicsMode;

	////// added by okamoto@tome(2012/08/02)
	/**
	 * @brief カメラが付属しているリンク名を得る
	 * @param camID 取得するするカメラID
	 * @return  リンク名
	 */
	std::string getCameraLinkName(int camID = 1);

	/**
	 * @brief カメラの位置を取得する
	 * @param v カメラ設置されたリンクからの位置(リンク座標系）
	 * @param camID 取得するカメラID
	 * @param requestToServer サーバに問い合わせる場合はtrue
	 */
	bool getCamPos(Vector3d &pos, int camID = 1, bool requestToServer = true);

	/**
	 * @brief カメラの位置を設定する
	 * @param v カメラ設置されたリンクからの位置(リンク座標系）
	 * @param camID 設定するカメラID
	 */
	bool setCamPos(Vector3d pos, int camID = 1);

	/**
	 * @brief カメラの方向(リンク座標)を取得する
	 * @param v カメラの向きベクトル(リンク標系）
	 * @param camID 取得するカメラID
	 * @param requestToServer サーバに問い合わせる場合はtrue
	 */
	bool getCamDir(Vector3d &v, int camID = 1, bool requestToServer = true);

	/**
	 * @brief カメラのクオータニオンを取得する
	 * @param v カメラの向きベクトル(リンク標系）
	 * @param camID 取得するカメラID
	 */
	bool getCamQuaternion(double &qw,double &qx, double &qy, double &qz, int camID = 1);

	/**
	 * @brief カメラの方向(リンク座標)を設定する
	 * @param v カメラの向きベクトル(リンク標系）
	 * @param camID 設定するカメラID
	 */
	bool setCamDir(Vector3d v, int camID = 1);

	/**
	 * @brief  カメラのy方向視野角を取得する
	 * @param camID 取得するカメラID
	 * @return 視野角(degree)
	 */
	double getCamFOV(int camID = 1);

	/**
	 * @brief  カメラのy方向視野角を設定する
	 * @param  fov   視野角(degree)
	 * @param  camID カメラID
	 * @return 
	 */
	bool setCamFOV(double fov, int camID = 1);

	/**
	 * @brief  カメラのアスペクト比を取得する
	 * @return camID 設定するカメラID
	 */
	double getCamAS(int camID = 1);

	/**
	 * @brief  カメラのアスペクト比を設定する
	 * @return camID 設定するカメラID
	 */
	bool setCamAS(double as, int camID = 1);

	/**
	 * @brief 設置されたカメラのリンク名を取得する
	 * @return camID 取得するカメラID
	 */
	std::string getCamLink(int camID = 1);

	//[ToDo]
	// カメラのリンク名を設定する
	void setCamLink(std::string link, int camID = 1);

	//! 他のエンティティによりgraspされているかどうかを取得する
	bool getIsGrasped();

#ifdef CONTROLLER

public:

	void print();
	/*!
	 * @brief エンティティの位置を設定します
	 * @brief Set entity position.
	 * @param x X座標の位置
	 * @param y Y座標の位置
	 * @param z Z座標の位置
	 */
	void setPosition(double x, double y, double z);

	/*!
	 * @brief エンティティの位置を設定します
	 * @brief Set entity position.
	 * @param v 位置ベクトル
	 */
	void setPosition(const Vector3d &v);

	/*!
	 * @brief エンティティの向きを設定する
	 *
	 * @param ax 回転軸ベクトルのX成分
	 * @param ay 回転軸ベクトルのY成分
	 * @param az 回転軸ベクトルのZ成分
	 * @param angle 回転角度[rad]
	 */
	void setAxisAndAngle(double ax, double ay, double az, double angle);

	//for Okonomiyaki
        void setAxisAndAngle(double ax, double ay, double az, double angle, double direct);

	
	/*!
	 * エンティティの向きを設定する
	 * @param r 回転行列
	 */
	void setRotation(const Rotation &r);
	
	/*!
	 * @brief エンティティにかかる力を設定する(Dynamics ON でのみ使用可)
	 * @param fx X軸成分
	 * @param fy Y軸成分
	 * @param fz Z軸成分
	 */
	void setForce(double fx, double fy, double fz);

	/*!
	 * @brief エンティティに力を加える(Dynamics ON でのみ使用可)
	 * @brief Add force to a body using absolute coordinates. 
	 * @param fx 力のX軸成分
	 * @param fy 力のY軸成分
	 * @param fz 力のZ軸成分
	 */
	void addForce(double fx, double fy, double fz);

	/*!
	 * @brief エンティティに加わる力・作用点(グローバル座標)の設定(Dynamics ON でのみ使用可)
	 * @brief Add force to a entity using absolute coordinates at specified absolute position. The supplied position vector specifies the point at which the force is supplied in global coordinates. 
	 * @param fx 力のX軸成分
	 * @param fy 力のY軸成分
	 * @param fz 力のZ軸成分
	 * @param px 作用点x値  
	 * @param py 作用点y値  
	 * @param pz 作用点z値  
	 */
	void addForceAtPos(double fx, double fy, double fz, double px, double py, double pz);

	/*!
	 * @brief エンティティに加わる力・作用点(エンティティからの相対位置)の設定(Dynamics ON でのみ使用可)
	 * @brief Add force to a entity using absolute coordinates at specified relative position. The supplied position vector specifies the point at which the force is supplied in entity-relative coordinates. 
	 * @param fx 力のX軸成分
	 * @param fy 力のY軸成分
	 * @param fz 力のZ軸成分
	 * @param px 作用点x値  
	 * @param py 作用点y値  
	 * @param pz 作用点z値  
	 */
	void addForceAtRelPos(double fx, double fy, double fz, double px, double py, double pz);

	/*!
	 * @brief エンティティに力を加える(エンティティ座標系),(Dynamics ON でのみ使用可)
	 * @brief Add force to a entity using relative coordinates. This function takes a force vector that is relative to the entity's own frame of reference.  
	 * @param fx X軸成分 
	 * @param fy Y軸成分 
	 * @param fz Z軸成分 
	 */
	void addRelForce(double fx, double fy, double fz);
	
	/*!
	 * @brief エンティティに加わる力(エンティティ座標系)・作用点(グローバル座標)の設定(Dynamics ON でのみ使用可)
	 * @brief Add force to a entity using entity-relative coordinates at specified absolute position. The supplied position vector specifies the point at which the force is supplied in global coordinates. 
	 * @param fx 力のX軸成分 
	 * @param fy 力のY軸成分
	 * @param fz 力のZ軸成分
	 * @param px 作用点x値 
	 * @param py 作用点y値 
	 * @param pz 作用点z値 
	 */
	void addRelForceAtPos(double fx, double fy, double fz, double px, double py, double pz);

	/*!
	 * @brief エンティティに加わる力(エンティティ座標系)・作用点(エンティティ座標)の設定(Dynamics ON でのみ使用可)
	 * @brief Add force to a entity using entity-relative coordinates at specified relative position. The supplied position vector specifies the point at which the force is supplied in entity-relative coordinates. 
	 * @param fx 力のX軸成分
	 * @param fy 力のY軸成分
	 * @param fz 力のZ軸成分
	 * @param px 作用点x値
	 * @param py 作用点y値
	 * @param pz 作用点z値
	 */
	void addRelForceAtRelPos(double fx, double fy, double fz, double px, double py, double pz);

	/*!
	 * @brief エージェントのパーツに力を加える(Dynamics ON でのみ使用可)
	 * @brief Add force to a entity parts.
	 * @param parts パーツ名
	 * @param fx 力のX軸成分
	 * @param fy 力のY軸成分
	 * @param fz 力のZ軸成分
	 */
	void addForceToParts(const char* parts, double fx, double fy, double fz);

	/*!
	 * @brief 重力の影響を受けるかどうか設定する
	 * @brief Set Gravity mode.
	 * @param gravity 重力フラグ
	 */
	void setGravityMode(bool gravity);

	/*!
	 * @brief 重力の影響を受けるかどうかを取得する
	 * @brief Get Gravity mode.
	 * @retval 0 場合重力の影響を受けない。(Gravity mode on)
	 * @retval 1 場合重力の影響を受ける。(Gravity mode off)
	 * @retval -1 取得失敗。(Failed to get gravity mode)
	 */
	int getGravityMode();

	/*!
	 * @brief 動力学演算の有効または無効を設定
	 * @brief Enable or disalbe gravity mode.
	 * @param dynamics (trueの場合有効）
	 */
	void setDynamicsMode(bool dynamics);

	/*!
	 * @brief 動力学演算の有効または無効を設定
	 * @brief Enable or disalbe dynamics mode.
	 * @return trueの場合動力学演算有効
	 */
	bool getDynamicsMode();

	/*!
	 * @brief エンティティの質量を設定する
	 * @brief Set the mass of the entity
	 * @param mass 質量
	 */
	void setMass(double mass);

	/*!
	 * @brief エンティティの質量を取得する
	 * @briefGet the mass of the entity
	 * @return 質量
	 */
	double getMass();

	/*!
	 * エンティティに加速度を設定する
	 * @param ax 加速度X軸成分
	 * @param ay 加速度Y軸成分
	 * @param az 加速度Z軸成分
	 */
	void setAccel(double ax, double ay, double az);
	
	/*!
	 * @briefエンティティにかかるトルクを設定する
	 * @param x X軸成分
	 * @param y Y軸成分
	 * @param z Z軸成分
	 */
	void setTorque(double x, double y, double z);

	/**
	 * @brief エンティティの速度を設定する
	 * @param vx 速度ベクトルのX軸成分
	 * @param vy 速度ベクトルのY軸成分
	 * @param vz 速度ベクトルのZ軸成分
	 */
	void setVelocity(double vx,double vy,double vz);

	/**
	 * @brief エンティティの速度を取得する
	 * @param vec 速度ベクトル
	 */
	void getVelocity(Vector3d &vec);

	/**
	 * @brief エンティティの角速度を設定する(Dynamics ON でのみ使用可)
	 * @param x X軸成分[rad/s]
	 * @param y Y軸成分[rad/s]
	 * @param z Z軸成分[rad/s]
	 */
	void setAngularVelocity(double x,double y,double z);

	/**
	 * @brief エンティティの角速度を取得する(Dynamics ON でのみ使用可)
	 * @param vec 角速度ベクトル[rad/s]
	 */
	void getAngularVelocity(Vector3d &vec);

	/**
	 * 関節の角度を設定します(Dynamics OFF でのみ使用可)
	 * @param jointName 関節の名前
	 * @param angle 角度
	 */
	void setJointAngle(const char *jointName, double angle);

	//added by okamoto@tome (2011/2/17)
	/**
	 * @brief 関節のクオータニオンを設定する(Dynamics OFF でのみ使用可)
	 * @param jointName 関節名
	 * @param qw w成分
	 * @param qx x成分
	 * @param qy y成分
	 * @param qz z成分
	 * @param offset trueの場合肩,足の付け根などの関節の初期角度を設定できる
	 */
	void setJointQuaternion(const char *jointName, double qw, double qx, double qy, double qz , bool offset = false);

	//added by okamoto@tome (2011/3/3)
	/**
	 * @brief 関節にトルクを加える(Dynamics ON でのみ使用可)
	 * @param jointName 関節名
	 * @param t トルク
	 */
	void addJointTorque(const char *jointName, double t);

	//added by okamoto@tome (2011/3/9)
	/**
	 * @brief 関節の角速度を設定する(Dynamics ON でのみ使用可)
	 * @param jointName 関節名
	 * @param v 角速度 [rad/s]
	 * @param max 最大トルク 
	 */
	void setJointVelocity(const char *jointName, double v, double max);


	void setObjectVelocity(const char *objectName, double v, double max);

	//added by okamoto@tome (2011/3/9)
	/**
	 * @brief 関節の角度を取得する
	 * @param jointName 関節名
	 */
	double getJointAngle(const char *jointName);

	/**
	 * @brief 全関節の角度を取得する
	 * @return 関節名と角度 マップ
	 */
	std::map<std::string, double> getAllJointAngles();

	/**
	 * @brief 関節の位置を取得する
	 * @param position 位置
	 * @param jointName 関節名
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool getJointPosition(Vector3d &pos, const char *jointName);

	/**
	 * @brief パーツの位置を取得する
	 * @param position 位置
	 * @param partsName 関節名
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool getPartsPosition(Vector3d &pos, const char *partsName);
	
	/**
	 * @brief 関節にかかっている力を取得する(dynamics on で使用)
	 */
	bool getJointForce(const char *jointName, JointForce &jf1, JointForce &jf2);

	/**
	 * @brief 衝突判定を行うかどうか設定する
	 */
	void setCollisionEnable(bool flag);


	void	connectJoint(const char *jointName, const char *targetName)
	{
		connectJoint(jointName, NULL, targetName, NULL);
	}

	void	connectJoint(const char *jointName, const char *myParts,
			     const char *targetName, const char *targetParts);
	void	releaseJoint(const char *jointName);

	//! 位置取得
	Vector3d & getPosition(Vector3d &v);

	//! エンティティの回転を取得する
	Rotation & getRotation(Rotation &r);

	// fx, fy, fz
	Vector3d & getForce(Vector3d &v);

	Vector3d & getAccel(Vector3d &v);

	// tqx, tqy, tqz
	Vector3d & getTorque(Vector3d &v);

	// lepx, lepy, lepz
	//Vector3d & getLeftViewPoint(Vector3d &v);
	// repx, repy, repz
	//Vector3d & getRightViewPoint(Vector3d &v);


	////////
	////// added by okamoto@tome(2012/01/05)
	/**
	 * @brief カメラの位置を設定する(before v2.1.0)
	 * @param v カメラ設置されたリンクからの位置（リンク座標系）
	 * @param camID 設定するカメラID
	 */
	void setCameraViewPoint(Vector3d v, int camID = 1);

	/**
	 * @brief カメラの位置を取得する(before v2.1.0)
	 * @param v カメラ設置されたリンクからの位置（リンク座標系）
	 * @param camID 取得するカメラID
	 */
	Vector3d & getCameraViewPoint(Vector3d &v, int camID = 1);

	/**
	 * @brief カメラの向きを設定する(before v2.1.0)
	 * @param v カメラの方向ベクトル
	 * @param camID 設定するカメラID
	 */
	void setCameraViewVector(Vector3d v, int camID = 1);

	/**
	 * @brief カメラの向きを取得する(before v2.1.0)
	 * @param v カメラの方向ベクトル
	 * @param camID　取得するカメラID
	 */
	Vector3d & getCameraViewVector(Vector3d &v, int camID = 1);

	// カメラの数を増やす added by yahara@tome (2011/02/14)
	// epx1, epy1, epz1
	Vector3d & getCamera1ViewPoint(Vector3d &v);
	// epx2, epy2, epz2
	Vector3d & getCamera2ViewPoint(Vector3d &v);
	// epx3, epy3, epz3
	Vector3d & getCamera3ViewPoint(Vector3d &v);
	// epx4, epy4, epz4
	Vector3d & getCamera4ViewPoint(Vector3d &v);
	// epx5, epy5, epz5
	Vector3d & getCamera5ViewPoint(Vector3d &v);
	// epx6, epy6, epz6
	Vector3d & getCamera6ViewPoint(Vector3d &v);
	// epx7, epy7, epz7
	Vector3d & getCamera7ViewPoint(Vector3d &v);
	// epx8, epy8, epz8
	Vector3d & getCamera8ViewPoint(Vector3d &v);
	// epx9, epy9, epz9
	Vector3d & getCamera9ViewPoint(Vector3d &v);


	// levx, levy, levz
	//Vector3d & getLeftViewVector(Vector3d &v);
	// revx, revy, revz
	//Vector3d & getRightViewVector(Vector3d &v);
	// カメラの数を増やす added by yahara@tome (2011/02/14)
	// evx1, evy1, evz1
	Vector3d & getCamera1ViewVector(Vector3d &v);
	// evx2, evy2, evz2
	Vector3d & getCamera2ViewVector(Vector3d &v);
	// evx3, evy3, evz3
	Vector3d & getCamera3ViewVector(Vector3d &v);
	// evx4, evy4, evz4
	Vector3d & getCamera4ViewVector(Vector3d &v);
	// evx5, evy5, evz5
	Vector3d & getCamera5ViewVector(Vector3d &v);
	// evx6, evy6, evz6
	Vector3d & getCamera6ViewVector(Vector3d &v);
	// evx7, evy7, evz7
	Vector3d & getCamera7ViewVector(Vector3d &v);
	// evx8, evy8, evz8
	Vector3d & getCamera8ViewVector(Vector3d &v);
	// evx9, evy9, evz9
	Vector3d & getCamera9ViewVector(Vector3d &v);

	// levx, levy, levz
	//void setLeftViewVector(const Vector3d &v);
	// revx, revy, revz
	//void setRightViewVector(const Vector3d &v);
	// カメラの数を増やす added by yahara@tome (2011/02/14)
	// evx1, evy1, evz1



	void setCamera1ViewVector(const Vector3d &v);
	// evx2, evy2, evz2
	void setCamera2ViewVector(const Vector3d &v);
	// evx3, evy3, evz3
	void setCamera3ViewVector(const Vector3d &v);
	// evx4, evy4, evz4
	void setCamera4ViewVector(const Vector3d &v);
	// evx5, evy5, evz5
	void setCamera5ViewVector(const Vector3d &v);
	// evx6, evy6, evz6
	void setCamera6ViewVector(const Vector3d &v);
	// evx7, evy7, evz7
	void setCamera7ViewVector(const Vector3d &v);
	// evx8, evy8, evz8
	void setCamera8ViewVector(const Vector3d &v);
	// evx9, evy9, evz9
	void setCamera9ViewVector(const Vector3d &v);
	

	////// added by noma@tome(2012/02/20)
	/**
	 * @brief 指差されたエンティティの名前を得る
	 * @param speakerName 発話者の名前
	 * @param lrFlag 左(0)か右(1)か
	 * @param lineID 線種の設定 1なら目を始点,2なら肘を始点とし手首を終点とする方向ベクトル
	 * @param typicalType 0 エンティティのサイズ考慮なし
	 *                    1 エンティティのメインパーツの外接球半径
	 *                    2 エンティティのメインパーツの体積の3乗根
	 */
	std::vector<const char*> getPointedObject(const char* speakerName, int lrFlag, int lineID, int typicalType=1);

	////// added by okamoto@tome(2012/11/14)
	/**
	 * @brief 2つの関節をつなぐベクトルを取得する
	 * @param joint1 始点関節 
	 * @param joint2 終点関節 
	 */
	bool getPointingVector(Vector3d &vec, const char *joint1, const char *joint2);

	////// added by okamoto@tome(2012/11/14)
	/**
	 * @brief 肘を始点とし手首を終点とする方向ベクトル(絶対座標系)を取得する
	 * @param lrFlag 左(0)か右(1)か
	 */
	bool getPointingVector(Vector3d &vec, int lrFlag = 0);

 private:
	//! エンティティの回転をサーバに送る(abs == trueのとき絶対角度)
	bool sendEntityQuaternion(const dReal *rot, bool abs);

	bool sendRequest(std::string name, int requestNum);

 public:

	
#ifdef DEPRECATED
	Command * createJointControlCommand();
#endif // DEPRECATED

#endif // CONTROLLER


#ifdef DEPRECATED
private:
	typedef std::map<S, double> JointValueM;
	JointValueM	m_jointValues;
#endif // DEPRECATED


};



/**
 * @brief ロボット用オブジェクト
 *
 */
class RobotObj : public SimObj
{
 public:

  //! コンストラクタ
 RobotObj() : 
  SimObj(), 
    m_wheelRadius(0.0),
    m_wheelDistance(0.0)
      {}


  /**
   * @brief 車輪の設定(Dynamics OFF でのみ使用)
   * @param wheelRadius    車輪の半径
   * @param wheelDistance  2つの車輪の間隔
   */
  bool setWheel(double wheelRadius, double wheelDistance);

  /**
   * @brief 車輪の角速度設定(Dynamics OFF でのみ使用)
   * @param leftWheel    左車輪の角速度
   * @param rightWheel   右車輪の角速度
   */
  bool setWheelVelocity(double leftWheel, double rightWheel);
  
  

 private:

  double m_wheelRadius;
  double m_wheelDistance;
  
};
#endif // SimObj_h
 
