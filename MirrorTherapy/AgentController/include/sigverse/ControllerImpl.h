/* $Id: ControllerImpl.h,v 1.6 2012-03-27 04:07:53 noma Exp $ */
#ifndef ControllerImpl_h
#define ControllerImpl_h

#include "ControllerInf.h"
#include "RawSound.h"
#include "systemdef.h"
#include "Logger.h"
#include "ViewImage.h"
#include <vector>
#include <map>
#include <string>


class Command;
class CTSimObj;
class ControllerImpl;

namespace SocketUtil {
  bool sendData(SOCKET sock, const char* msg, int size);
  bool recvData(SOCKET sock, char* msg, int size);
}
//! サービスプロバイダとデータの送受信を行うクラス
class BaseService 
{
 public:
  BaseService(){;} 
  
 BaseService(std::string name, unsigned short port, SOCKET sock) 
   : m_name(name), m_port(port), m_sock(sock), m_getData(false) {;}
  ~BaseService(){
    /*
    if(m_image != NULL) {
      delete m_image;
      m_image = NULL;
    }
    */
  }
  
  //! データ受信ループを開始する
  bool startServiceLoop(ControllerImpl *con);

  //! データ受信ループを終了する
  void endServiceLoop();
  
  //! サービスプロバイダの名前取得   
  std::string getName() { return m_name; }

  //! サービスプロバイダ受付ソケットを取得する
  SOCKET getSocket() { return m_sock; }

  //! サービスプロバイダ接続ソケットを取得する
  SOCKET getClientSocket() { return m_clientSock; }

  //! サービスプロバイダ接続ソケットの設定
  void setClientSocket(SOCKET sock) { m_clientSock = sock; }

  //! メッセージ送信
  bool sendMsgToSrv(std::string msg);

  //! データ受信したことを知らせる
  void setGetData(bool get){ m_getData = get; }

  //! 検出したオブジェクト名を設定する
  void setDetectedNames(std::vector<std::string> names) { m_detectedNames = names; }
  /*
  ViewImage* getImage() {
    return m_image;
  }
  */
  //! サービスを利用するエンティティの名前取得
  std::string getEntityName() { 
    return m_entname; 
  }

  //! サービスを利用するエンティティの名前取得
  void setEntityName(std::string name) {
    m_entname = name;
  }

 protected:
  // 接続先のサービス名
  std::string m_name;

  // port number(コントローラ)
  unsigned short m_port;

  // socket
  SOCKET m_sock;

  // クライアント側ソケット
  SOCKET m_clientSock;

  //! サービスがデータを受信しているか
  bool m_getData;

  // サービスを利用するエンティティ名
  std::string m_entname;

  // 画像データ
  std::vector<ViewImage*> m_images;

  // 距離データ
  unsigned char         m_distance;

  // detectEntitiesの結果
  std::vector<std::string> m_detectedNames;
};

class ViewService : public BaseService
{


 public:

  ViewService(){;} 
  
 ViewService(std::string name, unsigned short port, SOCKET sock) 
   : BaseService(name, port, sock) {;}
  
  /**
   * @brief 視野内に存在するエンティティの検出(サービスプロバイダ機能使用)
   *
   * @param v エンティティ名を格納するコンテナ
   * @param id カメラID
   * @retval true 成功
   * @retval false 失敗
   */
  bool  detectEntities(std::vector<std::string> &v, int id = 1);

  /**
   * @brief エージェント視点画像を取得する
   * @param camID カメラID
   * @param ctype 画像タイプ
   * @param size  画像サイズ
   * @retval != NULL 画像データ
   * @retval NULL    失敗
   */
  ViewImage *captureView(int camID = 1, ColorBitType ctype = COLORBIT_24, ImageDataSize size = IMAGE_320X240);
  
  /**
   * @brief カメラ視線方向にあるオブジェクトまでの距離を得る
   * @param start 最小距離(戻り値が0となる距離)
   * @param end   最大距離(戻り値が255となる距離)
   * @param ctype ビット深度
   * @param camID カメラID
   * @retval 0~255 距離データ
   */
  unsigned char distanceSensor(double start = 0.0, double end = 255.0, int camID =1, ColorBitType ctype = DEPTHBIT_8);

  /**
   * @brief カメラ視野内にあるオブジェクトまでの1次元距離データを得る
   * @param start 最小距離(戻り値が0となる距離)
   * @param end   最大距離(戻り値が255となる距離)
   * @param camID カメラID
   * @param ctype 画像タイプ
   * @param size  距離データサイズ
   */
  ViewImage *distanceSensor1D(double start = 0.0, double end = 255.0, int camID = 1, ColorBitType ctype = DEPTHBIT_8, ImageDataSize size = IMAGE_320X1);

  /**
   * @brief カメラ視野内にあるオブジェクトまでの距離データの２次データを得る
   * @param start 最小距離(戻り値が0となる距離)
   * @param end   最大距離(戻り値が255となる距離)
   * @param camID カメラID
   * @param ctype 画像タイプ
   * @param size  画像サイズ
   */
  ViewImage *distanceSensor2D(double start = 0.0, double end = 255.0, int camID = 1, ColorBitType ctype = DEPTHBIT_8, ImageDataSize size = IMAGE_320X240);

 private:
  /**
  * @brief distanceSensorリクエストをサービスプロバイダに送信する
  * @param type リクエストタイプ（取得するデータの次元）
  * @param start 最小距離(戻り値が0となる距離)
  * @param end   最大距離(戻り値が255となる距離)
  * @param camID カメラID
  * @param ctype 画像タイプ
  * @param size  画像サイズ
  */
  bool sendDSRequest(int type, double start = 0.0, double end = 255.0, int camID = 1, ColorBitType ctype = DEPTHBIT_8, ImageDataSize size = IMAGE_320X240);

};

/**
 * @brief ネットワーク通信コントローラクラス
 *
 * シミュレーションサーバとの通信を行なうコントローラクラス
 */
class ControllerImpl : public ControllerInf
{
 private:
  typedef ControllerInf Super;

 private:
  std::string 	m_server;
  int		m_port;
  std::string 	m_myname;
  
  bool m_isAttached;	// added by sekikawa(2007/10/15)


 protected:
  SOCKET		m_cmdSock;
  SOCKET	       m_dataSock;
  SOCKET                m_srvSock; //サービスプロバイダ用
  unsigned short        m_srvPort; //サービスプロバイダ用ポート番号
  SOCKET                m_tmpSock; // tmpSock

  // サービスプロバイダごとのソケット保存
  std::map<std::string, SOCKET> m_srvSocks; 

  // サービスプロバイダ接続成功か
  bool m_connected;



  // サービスプロバイダに接続成功したかどうか
  bool           m_connectService;

  // サービスプロバイダ用ソケット<サービス名、ソケット>
  std::vector<BaseService*> m_services;

  // changed by sekikawa (2007/10/16)
  // (********* this is only experiment. need to recover later *********)
  //private:
 private:
  CTSimObj * m_ctSimObj;
 protected:
  CTSimObj & getCTSimObj();

  /**
   * @brief サービスプロバイダに接続する(ポート番号はメインポート+1)
   *        
   * @param name	  サービスプロバイダ名
   * @retval BaseService  サービス
   * @retval NULL 　　　　失敗
   */
  BaseService* connectToService(std::string name);

  /**
   * @brief サービスプロバイダに接続する(ポート番号指定)
   *        
   * @param name	  サービスプロバイダ名
   * @param port          サーバ側で使用するポート番号
   * @retval BaseService  サービス
   * @retval NULL 　　　　失敗
   */
  BaseService* connectToService(std::string name, unsigned short port);

  /**
   * @brief サービスプロバイダと切断する
   *        
   * @param name	  サービスプロバイダ名
   */
  void disconnectToService(std::string name);

  /**
   * @brief  サービスプロバイダが使用可能かサーバに問い合わせる
   * @param  name  調べたいサービス名  
   * @retval true  サービス使用可能
   * @retval false サービス使用不可
   */
  bool checkService(std::string name);
  
 private:
  // bool recvData(SOCKET sock, char *msg, int size);


 public:
  static void *serviceThread(void *pParam);


 protected:
  void	close_();
	
 protected:
  //! コンストラクタ
 ControllerImpl()
   : Super(),
    m_port(-1), 
    m_isAttached(false), 
    m_cmdSock(-1), 
    m_dataSock(-1),
    m_srvSock(-1), 
    m_tmpSock(-1), 
    m_ctSimObj(NULL), 
    m_connectService(false),
    m_connected(false){;}

  //! デストラクタ
  ~ControllerImpl() {
    close_();
  }

 public:
  // サービスプロバイダに接続中かどうか
  bool connected() {return m_connected;}

  // tmpソケット設定
  void setTmpSock(SOCKET sock) {m_tmpSock = sock;}

  //! 接続中のサービスとソケットのマップ取得
  std::map<std::string, SOCKET> getSrvSocks() { return m_srvSocks; }

  // サービスプロバイダ接続状態設定
  void setConnected(bool connected) {m_connected = connected;}

  //! サーバ名を取得する
  const char * 	server() { return m_server.c_str(); }

  //! サーバポート番号を取得する
  int		port() { return m_port; }

  //! アタッチするエージェント名を取得する
  const char *	myname() { return m_myname.c_str(); }

  //! サーバ内のエージェントにアタッチしているか？
  bool	isAttached() { return m_isAttached; }	// added by sekikawa(2007/10/15)

  /** サーバ内の指定のエージェントにコントローラを割り
   * 当てる
   *
   * @param server シミュレーションサーバ名
   * @param port	 シミュレーションサーバポート番号
   * @param myname アタッチするエージェント名
   */
  bool	attach(const char *server, int port, const char *myname);

  /**
   * @brief エージェントからコントローラを切離す
   *
   * 切離し後、サーバとの通信を切断する
   */
  void	detach() { close_(); }

  /**
   * @brief エンティティデータ送受信用ソケットを得る
   *
   */
  SOCKET  getDataSock() { return m_dataSock; }

  /**
   * @brief サービスプロバイダ受け付け用ソケットを得る
   *
   */
  SOCKET  getSrvSock() { return m_srvSock; }

  // old
  void	sendText(double t, const char *to, const char *text, double reachRadius);

  // old
  void	sendText(double t, const char *to, const char *text);

  void	sendDisplayText(double t, const char *to, const char *text, int fs, const char *color, double reachRadius = -1.0);

  void	displayText(const char *text, int fs, const char *color, double dummy = -1.0);

  void	sendMessage(const char *to, int argc, char **argv);

  /**
   * @brief メッセージを送信する(メッセージを受け取ったエンティティはonRecvMsgが呼び出される)
   *
   * @param to		  送信先エージェント名
   * @param msg		  メッセージ
   * @param distance	  メッセージが届く距離(指定しなければ必ず届く)
   */
  bool	sendMsg(std::string to, std::string msg, double distance = -1.0);

  /**
   * @brief 複数のエンティティにメッセージを送信する
   *        (メッセージを受け取ったエンティティはonRecvMsgが呼び出される)
   *
   * @param to		  送信先エージェント名(Vecotr)
   * @param msg		  メッセージ
   * @param distance	  メッセージが届く距離(指定しなければ必ず届く)
   */
  bool	sendMsg(std::vector<std::string> to, std::string msg, double distance = -1.0);

  void	sendDisplayMessage(const char *to, int argc, char **argv, int fs, const char *color);

  /**
   * @brief 接続中の全サービスプロバイダにメッセージを送信する
   * 
   * @param msg		  メッセージ
   */
  bool	broadcastMsgToSrv(std::string msg);

  /**
   * @brief 接続中のコントローラを持つ全エンティティにメッセージを送信する
   * 
   * @param msg		  メッセージ
   * @param distance	  メッセージが届く距離(指定しなければ必ず届く)
   */
  bool	broadcastMsgToCtl(std::string msg, double distance = -1.0);

  // old
  void	broadcastMessage(int argc, char **argv);

  /**
   * @brief メッセージをブロードキャストする
   *
   * @param msg		  メッセージ
   * @param distance	  メッセージが届く距離（指定しなければ全ユーザーに届く)	  
   */
  bool	broadcastMsg(std::string msg, double distance = -1.0);

  // old
  void	broadcastDisplayMessage(int argc, char **argv, int fs, const char *color);

  // begin(FIX20110401)
  /**
   * @brief 他のエージェントに音声データを送信する
   *
   * @param t			  現在の時刻
   * @param to		  送信先(NULLの場合すべてのエージェントに送信する)
   * @param text		  音声データ
   */
  void sendSound(double t, const char *to, RawSound &sound);
  // end(FIX20110401)

  //! データを送信する
  bool sendData(SOCKET sock, const char *msg, int size);

#ifdef DEPRECATED
  void	send(Command &cmd);
  void	moveTo(double x, double z, double velocity);
#endif


  /**
   * @brief メッセージをブロードキャストする
   *
   * @param msg		  メッセージ
   * @param distance	  メッセージが届く距離(-1.0の場合は必ず届く)	  
   * @param to              -1の場合はすべて、-2の場合はサービスプロバイダ、-3の場合はコントローラ
   */
  bool broadcast(std::string msg, double distance, int to);

  // 最後に接続したサービスを削除
  void deleteLastService() {
    BaseService* srv = m_services.back();
    delete srv;
    m_services.pop_back();
  }

  // 接続中の指定したサービスを削除
  void deleteService(std::string sname) {
    std::vector<BaseService*>::iterator it;
    it = m_services.begin();
    while(it != m_services.end()) {
      std::string name = (*it)->getName();
      if(name == sname) {
	delete *it;
	m_services.erase(it);
	break;
      }
      it++;
    }

    // サービス名とソケットのマップから削除
    std::map<std::string, SOCKET>::iterator mit;
    mit = m_srvSocks.begin();
    while(mit != m_srvSocks.end()) {
      if((*mit).first == sname) {
	m_srvSocks.erase(mit);
	return;
      }
      mit++;
    }
    LOG_ERR(("deleteService: cannot find %s", sname.c_str()));
  }

  //! 最後に接続したサービスを取得する
  BaseService* getLastService() {
    return m_services.back();
  }

  //! 接続中の指定したサービスを取得する
  BaseService* getService(std::string sname) {
    std::vector<BaseService*>::iterator it;
    it = m_services.begin();
   
    while(it != m_services.end()) {
      std::string name = (*it)->getName();
      if(name == sname) {
	return (*it);
      }
      it++;
    }
    LOG_ERR(("getService: cannot find %s", sname.c_str()));
    return NULL;
  }


};



#endif // ControllerImpl_h
