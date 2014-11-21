/* $Id: ControllerInf.h,v 1.2 2011-03-31 08:15:56 okamoto Exp $ */ 
#ifndef ControllerInf_h
#define ControllerInf_h

class InitEvent;
class ActionEvent;
class RecvTextEvent;
class RecvSoundEvent;
class RecvMessageEvent;
class RecvMsgEvent;
class CollisionEvent;

/**
 * @brief コントローラインターフェース
 */
class ControllerInf
{
public:
 
 ControllerInf(): m_startSim(false), m_excon(false) {;}

	//! デストラクタ
	virtual ~ControllerInf() {;}

	/**
	 * @brief コントローラの初期化コールバック関数
	 *
	 * シミュレーション開始時に呼び出される
	 */
	virtual void 	onInit(InitEvent &evt) {}
	
	/**
	 * @brief 行動コールバック関数
	 *
	 * エージェントの主な動きを実装する。通常、シミュレーション時間が進む毎に呼び出される。
	 *
	 * @return 次に呼び出されるまでの時間。
	 */
	virtual double 	onAction(ActionEvent &evt) { return 0.0; }

	//! テキスト受信コールバック関数
	virtual void 	onRecvText(RecvTextEvent &evt) {}
	//! 音声受信コールバック関数
	virtual void	onRecvSound(RecvSoundEvent &evt) {}
	//old
	virtual void	onRecvMessage(RecvMessageEvent &evt) {}
	//! メッセージ受信コールバック関数(2012/9/10追加)
	virtual void	onRecvMsg(RecvMsgEvent &evt) {}
	//! 衝突検出コールバック関数 
	virtual void 	onCollision(CollisionEvent &evt) {}

#ifndef UNIT_TEST
	//! コントローラが処理中であるか？
	virtual	bool	isProcessing() = 0;
	//! コールバック関数を呼び出す前の処理を実装する
	virtual void	onPreEvent() = 0;
	//! コールバック関数を呼び出した後の処理を実装する
	virtual void	onPostEvent() = 0;
#else
	virtual	bool	isProcessing() { return false; };
	virtual void	onPreEvent() {};
	virtual void	onPostEvent() {};
#endif


	//! シミュレーションが起動中かどうか取得する
	bool getSimState(){ return m_startSim; }

	//! シミュレーションが起動中かどうか設定する
	void setSimState(bool sim){ m_startSim = sim; }

	// 他のスレッドが関数を使用中かどうかを取得する
	bool getExCon(){ return m_excon; }

	// 関数を使用中であることを登録する
	void setExCon(bool excon){ m_excon = excon; }

 protected:
	//! シミュレーション起動中かどうか
	bool m_startSim;

	//! 排他制御用変数
	bool m_excon;
};


#endif // ControllerInf_h
 
 
