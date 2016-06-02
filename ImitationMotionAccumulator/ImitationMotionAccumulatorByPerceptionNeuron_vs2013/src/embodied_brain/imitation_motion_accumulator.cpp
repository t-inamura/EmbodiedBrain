/*
 * PSMove操作履歴蓄積プログラムは、人物モデルの動作情報を使用してSIGverse上のアバターを動かしつつ、ユーザがそのアバターの動作を見ながら操作した
 * PS Moveモーションコントローラの操作履歴をMySQLに蓄積する。
 * 
 *    ２つのプログラム（アバター描画処理と操作履歴蓄積処理）で構成され、それぞれをWindowsとUbuntuの2つのOS上で実行する。	
 *    Windowsを使用するのはSIGVerseを使用したアバター描画を行いたいためで、Ubuntuを使用するのはPS Move APIを使用したいため。
 *    描画処理プログラムと蓄積処理プログラムは平行して同時に実行しなければならないため、TCPソケット通信で同期を取り描画・蓄積処理を同時に開始する。
 *    描画処理プログラムをTCPサーバ側、蓄積処理プログラムをTCPクライアント側とする。そのため描画処理プログラムを起動してから、蓄積処理プログラムを起動する。
 *    
 *    本プログラムは、＜アバター描画側＞となる。
 *    描画処理プログラムは起動後、TCPクライアントからの接続要求を待ちうけ、メイン処理実行後は、再度接続要求待ち状態に戻る。
 *    描画にSIGVerseを使用しているため、SIGVerseのサービスプロバイダとして実装する。
 *    
 * ＜流れ＞
 * 1. SIGServerに接続する
 * 2. TCPサーバとして待機し、TCPクライアント(蓄積側)からの接続要求を待つ
 * 3. TCPクライアント(蓄積側)との接続後、TCPクライアント(蓄積側)とメッセージのやり取りを行いながら、描画データの準備と、蓄積側との処理同期を行う
 * 4. 人物モデル動作データをSIGVerseに送信し、SIGViewer上のアバターを動かす（同時に蓄積プログラムではデータの蓄積を行っている）
 * 5. 描画処理終了後、再度TCPサーバとして待機し、TCPクライアント(蓄積側)からの接続要求を待つ
 *
 * ＜引数＞
 * 第1引数：IPアドレス、第2引数：ポート番号
 * 
 * ＜その他＞
 * ・設定ファイル名は、ImitationMotionAccumulator.ini
 * ・処理終了後は、再びTCPクライアントからの接続要求の待ち受けを開始する
 * ・設定ファイルのcmn.motion_data_file_pathにファイルパスを設定した場合、データベースではなく当該ファイルからモデル動作情報を取得する
 */
#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>
#include <embodied_brain/imitation_motion_accumulator.h>

#include <embodied_brain/avatar/avatar_controller.h>
#include <embodied_brain/common/param.h>
#include <embodied_brain/file/file_manager.h>

#include <cstddef>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <sstream>
#include <conio.h>
#include <limits>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>

///@brief Parameter file name.
const std::string Param::PARAM_FILE_NAME = "ImitationMotionAccumulator.ini";

std::string Param::dbHost;
int         Param::dbPort;
std::string Param::dbSchema;
std::string Param::dbUser;
std::string Param::dbPass;

std::string Param::sigServiceName;
int         Param::sigAvatarDispInterval;

int         Param::imiAccumInterval;
std::string Param::imiMotionDataFilePath;

int         Param::imiRecId;
int         Param::imiOriginMaxTime;

int         Param::imiImitationGroupId;
int         Param::imiImitationRecType;
int         Param::imiUserId;
int         Param::imiImitationOriginRecId;

std::string Param::imiDbPerceptionNeuronMemo;
float       Param::imiDbImitationConditionPulsePower;
float       Param::imiDbImitationConditionPulseFrequency;
int         Param::imiDbImitationConditionPulseDuration;
int         Param::imiDbImitationConditionPulseInterval;
int         Param::imiDbImitationConditionPulseNumber;
std::string Param::imiDbImitationMemo;

Param::Mode Param::mode;


/*
 * メイン処理
 */
int ImitationMotionAccumulator::run(int argc, char **argv)
{
	char argSigAddr[128]; //SIGVerse IPアドレス
	int argSigPortNum;    //SIGVerse ポート番号

	if(argc == 3) 
	{
		sprintf_s(argSigAddr, 128, "%s",argv[1]);
		argSigPortNum  = atoi(argv[2]);

		std::cout << "Connect to server [" <<argSigAddr << "]\n";
		std::cout << "Portnumber [" << argSigPortNum << "]\n\n";
	}
	else
	{
		std::cout << "引数不正です" << std::endl;
		return(-1);
	}

	//コンフィグファイル読み込み
	Param::readConfigFile();


	/*
	 * Perception Neuron関連の初期化
	 */
	// Read the initialize file.
	this->readIniFile(Param::PARAM_FILE_NAME);

	// connect to Perception Neuron
	SOCKET_REF sockRefBvh  = NULL;

	this->connect4Bvh(sockRefBvh);

	// Receive BVH data by the callback function.
	BRRegisterFrameDataCallback(this, ImitationMotionAccumulator::bvhFrameDataReceived);

	// Receive Socket status by the callback function.
	BRRegisterSocketStatusCallback(this, this->socketStatusChanged);



	//データベースのID重複チェック（重複があった場合、exceptionをthrowする）
	PerceptionNeuronDAO::duplicationCheck(Param::getImiUserId());


	AvatarController avatarController; 

	if (Param::getMode()==Param::Mode::Imitation)
	{
		//データベースのID重複チェック（重複があった場合、exceptionをthrowする）
		PmsImitationDAO::duplicationCheck(Param::getImiImitationGroupId(), Param::getImiImitationRecType());

		/*
		 * 手本モーションIDのモデル動作情報を取得
		 */
		std::list<PerceptionNeuronDAO::TimeSeries_t> motionData = this->getMotionDataFromDBorFile(std::to_string(Param::getImiImitationOriginRecId()));

		/*
		 * SIGVerseに接続
		 *
		 * ・DBから取得したモデル動作情報からSIGVerseへ送信する電文リストを作成する
		 * ・SIGVerseに接続する
		 */
		//接続
		avatarController.connectSIGServer(argSigAddr, argSigPortNum);

		//SIGVerseからの定期データ受信スレッド実行
		avatarController.checkRecvSIGServiceData();

		//電文作成
		avatarController.makeTelegramForAvatar(motionData);

		//不要になった動作情報リストの全要素を削除
		motionData.clear();
	}


	//カウントダウンを描画
	int showCnt = 10;

	while (showCnt > 0)
	{
		std::cout << "収録開始 " << showCnt << " 秒前" << std::endl;

		Sleep(1000); //１秒スリープ

		showCnt--;
	}

	std::cout << "収録開始！('q'キー押下で終了)" << std::endl;

	//人物モーション情報取得、データベース蓄積、ファイル出力処理の開始
	this->accumulateMotionData();

	if (Param::getMode() == Param::Mode::Imitation)
	{
		//アバターに電文を送って操作
		avatarController.sendMotionDataToSIGVerse();

		//SIGServerからの切断
		avatarController.disconnectFromAllController();
	}

	// close socket
	BRCloseSocket(sockRefBvh);

	// Windows での終了設定
	WSACleanup();

	std::cout << "収録終了" << std::endl;

	return EXIT_SUCCESS;
}

///*
// * 初期化全般
// */
//void ImitationMotionAccumulator::initialize(const int argRecId, const std::string &memo, const int argMaxRecordTime)
//{
//	this->recId = argRecId;
//
//
//}




void __stdcall ImitationMotionAccumulator::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	ImitationMotionAccumulator* pthis = (ImitationMotionAccumulator*)customedObj;

	pthis->updateBvhData(customedObj, sender, header, data);
}


void ImitationMotionAccumulator::updateBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	ImitationMotionAccumulator* pthis = (ImitationMotionAccumulator*)customedObj;

	PerceptionNeuronSensorData sensorData;

	sensorData.dataType = PerceptionNeuronSensorData::DataTypeEnum::BVH;

	sensorData.bvhData.avatarIndex   = header->AvatarIndex;
	sensorData.bvhData.avatarName    = std::string((char*)header->AvatarName);
	sensorData.bvhData.withDisp      = (header->WithDisp==1);
	sensorData.bvhData.withReference = (header->WithReference==1);
	sensorData.bvhData.frameIndex    = header->FrameIndex;
	sensorData.bvhData.dataCount     = header->DataCount;
	sensorData.bvhData.data          = data;

	// 排他制御
	std::lock_guard<std::mutex> lock(pthis->mtx4LatestSensorData);

	pthis->latestSensorData = sensorData;
}


/*
 * 動作情報を取得する
 */
std::list<PerceptionNeuronDAO::TimeSeries_t> ImitationMotionAccumulator::getMotionDataFromDBorFile(const std::string &recIdStr)
{
	std::list<PerceptionNeuronDAO::TimeSeries_t> motionInfo;
	int motionDataNum;

	if(Param::getImiMotionDataFilePath()=="")
	{
		//データベースからモデル動作情報を取得
		motionDataNum = PerceptionNeuronDAO::selectMotionData(motionInfo, recIdStr);
	}
	else
	{
		//ファイルからモデル動作情報を取得
		FileManager fileManager;
		motionDataNum = fileManager.getMotionData(motionInfo, Param::getImiMotionDataFilePath(), recIdStr);
	}
	if(motionDataNum==0)
	{
		std::cout << "データが１件も無いため終了します。" << std::endl;
		exit(-1);
	}

	return motionInfo;
}




/*
 * Perception Neuronから人物モーション情報を取得し、データベースへの追加とファイル出力を行う
 */
void ImitationMotionAccumulator::accumulateMotionData()
{
	PerceptionNeuronDAO::DataSet motionSet;

	motionSet.summary.recId       = Param::getImiRecId();
	motionSet.summary.userId      = Param::getImiUserId();
	motionSet.summary.recInterval = Param::getImiAccumInterval();
	motionSet.summary.memo        = Param::getImiDbPerceptionNeuronMemo();

	double maxTime;

	if (Param::getMode() == Param::Mode::Origin)
	{
		maxTime = (double)(Param::getImiOriginMaxTime()*1000.0); //単位はms
	}
	else if (Param::getMode() == Param::Mode::Imitation)
	{
		maxTime = (std::numeric_limits<double>::max)();
	}


	//時間計測関連変数
	LARGE_INTEGER startLi, nowLi, freqLi;
	double freq, freqMilli;

	QueryPerformanceFrequency(&freqLi);
	freq = (double)freqLi.QuadPart;
	freqMilli = (double)freqLi.QuadPart / 1000.0;

	//キー入力待ち間隔[ms]
	int waitKeyInterval = 100;

	//データ蓄積カウンタ
	int accumCnt = 0;
	//キー入力待ちカウンタ
	int waitKeyCnt = 1;

	//次回データ蓄積時間
	double accumNextTime = (double)(Param::getImiAccumInterval() * accumCnt);
	//次回キー入力待ち時間
	double waitKeyNextTime = (double)(waitKeyInterval * waitKeyCnt);

	//収録開始時間をセットする
	time_t timet = time(NULL);
	localtime_s(&motionSet.summary.recStartTime, &timet);

	time_t start_time, end_time;

	time(&start_time);

	QueryPerformanceCounter(&startLi);

	/*
	 * 特定の条件が満たされるまで、Perception Neuronの人物モーション情報をメモリに蓄積し続ける
	 */
	while (true)
	{
		QueryPerformanceCounter(&nowLi);

		//経過時間
		double elapsedTime = (double)(nowLi.QuadPart - startLi.QuadPart) / freqMilli; //ms単位

		//データ蓄積
		if (elapsedTime >= accumNextTime)
		{
			//人物モーション情報をメモリに蓄積
			this->accumulate((int)elapsedTime);

			//次回データ蓄積時間を算出
			while (elapsedTime >= accumNextTime)
			{
				accumCnt++;
				accumNextTime = (double)(Param::getImiAccumInterval() * accumCnt);
			}
		}

		//100msに1回キー入力待ちする
		if (elapsedTime >= waitKeyNextTime)
		{
			//キー"q"が入力されたら処理終了
			if (GetAsyncKeyState('Q'))
			{
				while (_kbhit() != 0){ _getch(); } //キー入力をクリア
				break;
			}

			//次回キー入力待ち時間を算出
			while (elapsedTime >= waitKeyNextTime)
			{
				waitKeyCnt++;
				waitKeyNextTime = (double)(waitKeyInterval  * waitKeyCnt);
			}
		}

		//最大収録時間を経過したら処理終了
		if (elapsedTime > maxTime)
		{
			break;
		}
	}

	time(&end_time);

	//収録時間をセットする
	motionSet.summary.recTotalTime = (int)(difftime(end_time, start_time)); //秒

	std::cout << "収録を終了します" << std::endl;

	/*
	 * 取得したモーションデータを、DataSet型に入れなおす
	 */
	std::map<int, PerceptionNeuronSensorData>::iterator it = this->accumulatingSensorDataMap.begin();

	//PreparedStatementを使用して複数件のレコードをINSERT
	while (it != this->accumulatingSensorDataMap.end())
	{
		std::string telegram = (*it).second.encodeSensorData();
		std::map<std::string, std::vector<std::string> > sensorMap = SensorData::decodeSensorData(telegram);

		((*it).second).setSensorData(sensorMap);

		PerceptionNeuronDAO::TimeSeries_t posture;
		posture.recId       = Param::getImiRecId();
		posture.elapsedTime = (*it).first;
		posture.hips_pos    = ((*it).second).rootPosition;

		for (int i = 0; i < NeuronBVH::BonesType::BonesTypeCount; i++)
		{
			posture.links[i]    = ((*it).second).bvhJoints[i];
		}
		motionSet.timeSeries.push_back(posture);

		it++;
	}

	PmsImitationDAO::DataSet imitationInfo;
	imitationInfo.groupId = Param::getImiImitationGroupId();
	imitationInfo.recType = Param::getImiImitationRecType();
	imitationInfo.recId = Param::getImiRecId();
	imitationInfo.originalRecId = Param::getImiImitationOriginRecId();
	imitationInfo.conditionPulsePower     = Param::getImiDbImitationConditionPulsePower();
	imitationInfo.conditionPulseFrequency = Param::getImiDbImitationConditionPulseFrequency();
	imitationInfo.conditionPulseDuration  = Param::getImiDbImitationConditionPulseDuration();
	imitationInfo.conditionPulseInterval  = Param::getImiDbImitationConditionPulseInterval();
	imitationInfo.conditionPulseNumber    = Param::getImiDbImitationConditionPulseNumber();
	imitationInfo.memo                    = Param::getImiDbImitationMemo();


	/*
	 * 取得したPerception Neuronの人物モーション情報をデータベースに追加
	 */
	PerceptionNeuronDAO::insertDatabaseExec(motionSet);

	PmsImitationDAO::insertDatabaseExec(imitationInfo);


	/*
	 * 取得したPerception Neuronの人物モーション情報をファイル出力
	 */
	FileManager fileManager;
	fileManager.outputDataFile(motionSet, imitationInfo);
}


/*
 * 人物モーション情報をメモリに蓄積
 */
void ImitationMotionAccumulator::accumulate(const int elapsedTime)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(this->mtx4LatestSensorData);

	//モーション情報をマップに保持する
	this->accumulatingSensorDataMap.insert( make_pair(elapsedTime, this->latestSensorData));

	this->latestSensorData = PerceptionNeuronSensorData();
}
