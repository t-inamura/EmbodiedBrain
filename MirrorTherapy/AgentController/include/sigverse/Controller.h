/* $Id: Controller.h,v 1.19 2011-12-14 08:13:44 okamoto Exp $ */ 
#ifndef Controller_h
#define Controller_h

#include <string>
#include <vector>
#include <map>

#include "ControllerImpl.h"
#include "ViewImageInfo.h"
#include "SimObj.h"

//#include "DynamicsController.h"


class ViewImage;
class DepthImage;
class Text;
class RawSound;





class DynamicsController {
public:
	DynamicsController();
	/**
	 * @brief Webots互換のパラメータを設定します。変数の先頭にleftがつくものは左側車輪の設定、rightのつくものは右側車輪の設定
	 *
	 * @param leftWheelName          車輪のジョイント名
	 * @param leftMotorConsumption   車輪の消費電力(W)
	 * @param leftWheelMaxSpeed      車輪の最大角速度(rad/s)
	 * @param leftWheelSpeedUnit     wb_differential_wheels_set_speed で設定される速度の単位をセット(rad/s)
	 * @param leftSlipNoise          一様分布にしたがう車輪のスリップ(0.0 - 1.0)
	 * @param leftEncoderResolution  1[rad]あたりどの程度のスリップが生じるか
	 * @param leftMaxForce           最大トルク
	 * @param rightWheelName         車輪のジョイント名
	 * @param rightMotorConsumption  車輪の消費電力(W)
	 * @param rightWheelMaxSpeed     車輪の最大角速度(rad/s)
	 * @param rightWheelSpeedUnit    wb_differential_wheels_set_speed で設定される速度の単位をセット(rad/s)
	 * @param rightSlipNoise         一様分布にしたがう車輪のスリップ(0.0 - 1.0)
	 * @param rightEncoderResolution 1[rad]あたりどの程度のスリップが生じるか
	 * @param rightMaxForce          最大トルク
	 * @param axleLength             車輪間の距離
	 */
	void setWheelProperty(
		SimObj *my,
		char   *leftWheelName,
		double leftMotorConsumption,
		double leftWheelMaxSpeed,
		double leftWheelSpeedUnit,
		double leftSlipNoise,
		double leftEncoderResolution,
		double leftMaxForce,
		char   *rightWheelName,
		double rightMotorConsumption,
		double rightWheelMaxSpeed,
		double rightWheelSpeedUnit,
		double rightSlipNoise,
		double rightEncoderResolution,
		double rightMaxForce
	);

	/**
	 * 左右の車輪にそれぞれことなるスピードを与える
	 * @param left 左車輪の角速度
	 * @param right 右車輪の角速度
	 */
	void differentialWheelsSetSpeed(SimObj *my,double left,double right);

	/**
	 * 車軸の長さを取得します
	 */
	double getAxleLength();

	/**
	 * 左車輪の半径を取得します
	 */
	double getLeftWheelRadius(SimObj *my);

	/**
	 * 右車輪の半径を取得します
	 */
	double getRightWheelRadius(SimObj *my);

	/**
	 * 左車輪のノイズ蓄積量を取得します
	 */
	double getLeftEncoderNoise();

	/**
	 * 右車輪のノイズ蓄積量を取得します
	 */
	double getRightEncoderNoise();

	/**
	 * 左車輪のスリップを取得します
	 */
	double getLeftSlipNoise();

	/**
	 * 右車輪のスリップを取得します
	 */
	double getRightSlipNoise();

	/**
	 * 現在の左車輪のスピードを取得します
	 */
	double getCurrentLeftWheelSpeed();

	/**
	 * 現在の右車輪のスピードを取得します
	 */
	double getCurrentRightWheelSpeed();

private:

	#define ACCURACY (0.00000001)

	/** 左側の車輪の名前 */
	char   *leftWheelName;
	/** 左車輪の電力[W] */
	double leftMotorConsumption;
	/** 左車輪の最大角速度[rad/s] */
	double leftWheelMaxSpeed;
	/** 左車輪の速度単位設定 */
	double leftWheelSpeedUnit;
	/** 左車輪のすべり */
	double leftSlipNoise;
	/** 左車輪のスリップノイズの蓄積量 */
	//double leftCumulativeNoise;
	/** 1[rad]あたりどの程度のスリップが生じるか */
	double leftEncoderResolution;
	/** 最大トルク */
	double leftMaxForce;
	/** 右側の車輪の名前 */
	char   *rightWheelName;
	/** 右側のモータの消費電力 */
	double rightMotorConsumption;
	/** 右側のホイールの最大角速度 */
	double rightWheelMaxSpeed;
	/** 右車輪の速度単位設定 */
	double rightWheelSpeedUnit;
	double rightSlipNoise;
	//double rightCumulativeNoise;
	double rightEncoderResolution;
	double rightMaxForce;


	/** 車輪間の距離 */
	double axleLength;
	/** 左車輪の半径[m] */
	double leftWheelRadius;
	/** 右側のホイールの半径 */
	double rightWheelRadius;
	/** 左車輪のノイズの蓄積 */
	double leftEncoderNoise;
	/** 右車輪のノイズの蓄積 */
	double rightEncoderNoise;

	/** 現在の左車輪の速度 */
	double currentLeftWheelSpeed;
	/** 現在の右車輪の速度 */
	double currentRightWheelSpeed;

};





/**
 * @brief ユーザ定義コントローラ基本クラス
 *
 * エージェントコントローラはこのクラスを継承したクラスを作
 * 成し、そこにエージェントの振舞いを記述する。
 */
class Controller : public ControllerImpl, public SimObj::RequestSender
{
private:
	typedef ControllerImpl Super;
	typedef std::string S;
	typedef std::map<S, SimObj*> M;
	
private:
	M		m_objs;
	bool		m_in;
private:
	SimObj *	find(const char *name)
	{
	  return m_objs[name];
	}
 public:
	void	updateObjs();
	void	clearObjs();
private:
	bool			send(CommDataEncoder &);
	CommDataResult *	recv(int bufsize);
	SOCKET getControllerSocket() { SOCKET sock; return sock; }

protected:
	//! コンストラクタ
	Controller();

	/**
	 * 0 - 1までの一様分布の乱数を生成します
	 */
	double getRand();

	/**
	 * @brief Webots互換のパラメータを設定します。変数の先頭にleftがつくものは左側車輪の設定、rightのつくものは右側車輪の設定
	 *
	 * @param leftWheelName          車輪のジョイント名
	 * @param leftMotorConsumption   車輪の消費電力(W)
	 * @param leftWheelRadius        車輪の半径(m)
	 * @param leftWheelMaxSpeed      車輪の最大角速度(rad/s)
	 * @param leftWheelSpeedUnit     wb_differential_wheels_set_speed で設定される速度の単位をセット(rad/s)
	 * @param leftSlipNoise          一様分布にしたがう車輪のスリップ(0.0 - 1.0)
	 * @param leftEncoderNoise       スリップノイズの蓄積
	 * @param leftEncoderResolution  1[rad]あたりどの程度のスリップが生じるか
	 * @param leftMaxForce           最大トルク
	 * @param rightWheelName         車輪のジョイント名
	 * @param rightMotorConsumption  車輪の消費電力(W)
	 * @param rightWheelRadius       車輪の半径(m)
	 * @param rightWheelMaxSpeed     車輪の最大角速度(rad/s)
	 * @param rightWheelSpeedUnit    wb_differential_wheels_set_speed で設定される速度の単位をセット(rad/s)
	 * @param rightSlipNoise         一様分布にしたがう車輪のスリップ(0.0 - 1.0)
	 * @param rightEncoderNoise      スリップノイズの蓄積
	 * @param rightEncoderResolution 1[rad]あたりどの程度のスリップが生じるか
	 * @param rightMaxForce          最大トルク
	 */
	void setWheelProperty(
		char   *leftWheelName,
		double leftMotorConsumption,
		double leftWheelMaxSpeed,
		double leftWheelSpeedUnit,
		double leftSlipNoise,
		double leftEncoderResolution,
		double leftMaxForce,
		char   *rightWheelName,
		double rightMotorConsumption,
		double rightWheelMaxSpeed,
		double rightWheelSpeedUnit,
		double rightSlipNoise,
		double rightEncoderResolution,
		double rightMaxForce
	);

	/**
	 * @brief Webots互換のパラメータを設定します。変数の先頭にleftがつくものは左側車輪の設定、rightのつくものは右側車輪の設定
	 *
	 * @param objectName             エージェントの名前
	 * @param leftWheelName          車輪のジョイント名
	 * @param leftMotorConsumption   車輪の消費電力(W)
	 * @param leftWheelRadius        車輪の半径(m)
	 * @param leftWheelMaxSpeed      車輪の最大角速度(rad/s)
	 * @param leftWheelSpeedUnit     wb_differential_wheels_set_speed で設定される速度の単位をセット(rad/s)
	 * @param leftSlipNoise          一様分布にしたがう車輪のスリップ(0.0 - 1.0)
	 * @param leftEncoderNoise       スリップノイズの蓄積
	 * @param leftEncoderResolution  1[rad]あたりどの程度のスリップが生じるか
	 * @param leftMaxForce           最大トルク
	 * @param rightWheelName         車輪のジョイント名
	 * @param rightMotorConsumption  車輪の消費電力(W)
	 * @param rightWheelRadius       車輪の半径(m)
	 * @param rightWheelMaxSpeed     車輪の最大角速度(rad/s)
	 * @param rightWheelSpeedUnit    wb_differential_wheels_set_speed で設定される速度の単位をセット(rad/s)
	 * @param rightSlipNoise         一様分布にしたがう車輪のスリップ(0.0 - 1.0)
	 * @param rightEncoderNoise      スリップノイズの蓄積
	 * @param rightEncoderResolution 1[rad]あたりどの程度のスリップが生じるか
	 * @param rightMaxForce          最大トルク
	 */
	void setWheelProperty(
		const char   *objectName,
		char   *leftWheelName,
		double leftMotorConsumption,
		double leftWheelMaxSpeed,
		double leftWheelSpeedUnit,
		double leftSlipNoise,
		double leftEncoderResolution,
		double leftMaxForce,
		char   *rightWheelName,
		double rightMotorConsumption,
		double rightWheelMaxSpeed,
		double rightWheelSpeedUnit,
		double rightSlipNoise,
		double rightEncoderResolution,
		double rightMaxForce
	);

	/**
	 * 自分自身の車軸の長さを取得します
	 */
	double getAxleLength();

	/**
	 * 車軸の長さを取得します
	 */
	double getAxleLength(const char *simObjName);

	/**
	 * 自分自身の左車輪の半径を取得します
	 */
	double getLeftWheelRadius();

	/**
	 * 左車輪の半径を取得します
	 */
	double getLeftWheelRadius(const char *simObjName);

	/**
	 * 自分自身の右車輪の半径を取得します
	 */
	double getRightWheelRadius();

	/**
	 * 右車輪の半径を取得します
	 */
	double getRightWheelRadius(const char *simObjName);

	/**
	 * 自分自身の左車輪のエンコーダのノイズ蓄積量を取得します
	 */
	double getLeftEncoderNoise();

	/**
	 * 左車輪のエンコーダのノイズ蓄積量を取得します
	 */
	double getLeftEncoderNoise(const char *simObjName);

	/**
	 * 自分自身の右車輪のエンコーダのノイズ蓄積量を取得します
	 */
	double getRightEncoderNoise();

	/**
	 * 右車輪のエンコーダのノイズ蓄積量を取得します
	 */
	double getRightEncoderNoise(const char *simObjName);

	/**
	 * 自分自身の左右の車輪にそれぞれことなるスピードを与える
	 * @param left 左車輪の角速度
	 * @param right 右車輪の角速度
	 */
	void differentialWheelsSetSpeed(double left,double right);

	/**
	 * 左右の車輪にそれぞれことなるスピードを与える
	 * @param simObjName SimObjの名前
	 * @param left 左車輪の角速度
	 * @param right 右車輪の角速度
	 */
	void differentialWheelsSetSpeed(const char *simObjName,double left,double right);

	/**
	 * @brief 指定のエンティティを取得する
	 * @param name エンティティ名
	 */
	SimObj *	getObj(const char *name);

	/**
	 * @brief シミュレーション時間を取得する
	 */
	double          getSimulationTime();

	/**
	 * @brief robotエンティティを取得する
	 * @param name エンティティ名
	 */
	RobotObj *	getRobotObj(const char *name);

	// old
	ViewImage * 	captureView(ColorBitType cbtype, ImageDataSize size);

	// old
	ViewImage * 	captureView(ColorBitType cbtype, ImageDataSize size, int id);

	// old
	bool	       	detectEntities(std::vector<std::string> &v);

	// old
	bool	       	detectEntities(std::vector<std::string> &v, int id);

	/**
	 * @brief SIGVerse世界を1ステップ更新する
	 * @param stepsize ステップ幅(sec)
	 */
	void            worldStep(double stepsize);

	/**
	 * @brief SIGVerse世界を1ステップ更新する(worldStepに比べて速度が早いが精度は低い)
	 * @param stepsize ステップ幅(sec)
	 */
	void            worldQuickStep(double stepsize);

	/**

	 * @brief 音声データを音声認識処理に掛ける
	 *
	 * 音声認識サービスプロバイダに音声データを転送し、音声認識結果(テキスト)を得る。
	 *
	 * @param sound  recvSoundメソッドにより取得した音声データ
	 *
	 * @retval !=NULL 成功
	 * @retval NULL 失敗
	 */
	Text *		getText(RawSound &sound);

	/**
	 * @brief シミュレーション世界内に存在するすべてのエンティティの名前を取得する
	 * 
	 * @param v エンティティ名を格納するコンテナ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool		getAllEntities(std::vector<std::string> &v);

	/**
	 * @brief  接続するサービス名
	 * 
	 * @param  name 接続するサービス名
	 * @retval true 成功
	 * @retval false 失敗
	 */
	//bool		connectToService(std::string name);


	bool	isProcessing()
	{
		return m_in;
	}
	void	onPreEvent()
	{
		m_in = true;
	}
	void	onPostEvent()
	{
	  //updateObjs();
	  //clearObjs();
	  m_in = false;
	}

private:
	typedef std::map<std::string, DynamicsController*> DYNAMICS_CONTROLLER_LIST;
	DYNAMICS_CONTROLLER_LIST dynamicsDataList;


	void	add(SimObj *obj);

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

	void slipWheel();

	//
	// 左車輪をスリップさせます。
	// スリップ量はleftSlipNoiseから算出される。
	//
	//void slipLeftWheel();

	//
	// 右車輪をスリップさせます。
	// スリップ量はleftSlipNoiseから算出される。
	//
	//void slipRightWheel();

	//
	// 角速度と電力から、トルクを算出する。
	// モータの計算式は下記の式に従うとする。
	//             T
	// P = F・rω = ーー ・ rω = 2πTf
	//             r
	// P : 消費電力[W]
	// F : タイヤにかかる力[N]
	// r : タイヤの半径[m]
	//ω : 角周波数[rad/sec]
	// T : トルク[N・m]
	// f : 周波数(回転数)[Hz]
	//
	// @param motorConsumption モータの電力[W]
	// @param radius 車輪の半径[m]
	// @param wheelSpeed 車輪の角速度[rad/sec]
	// @return 車輪にかかるトルク[N・m]
	//
	//double getTorqueFromMotor(double motorConsumption,double radius,double wheelSpeed);

	//
	// @brief モータを動かします
	// @param jointName        ジョイント名
	// @param motorConsumption 電力[W]
	///
	//void moveMotor(char *jointName,double motorConsumption);

public:
	/**
	 * @brief エージェントにコントローラを割り当てる
	 *
	 * シミュレーションサーバに接続し、指定したエージェントに当コントローラを割り当てる
	 *
	 * @param server  シミュレーションサーバ(IPアドレスまたはホスト名)
	 * @param port    シミュレーションサーバポート番号
	 * @param myname  エージェント名
	 * 
	 */
	bool	attach(const char *server, int port, const char *myname);
	//! コントローラループ関数
	void	loopMain();

	//!! コントローラ初期化関数
	static void init();
};


#endif // Controller_h
 
