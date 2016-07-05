/*
 * 真似動作収録プログラムは、事前に収録した動作情報を使用してSIGverse上のアバターを動かしつつ、ユーザがそのアバターの動作を真似し、その真似動作をMySQLに蓄積する。
 * 
 *    ・動作情報取得には Perception Neuron を使用する。
 *    ・以下の２つの収録モードを用意する。
 *    　　・手本動作収録モード      ：ユーザに見せるための手本動作を収録するためのモード
 *    　　・実験(真似動作収録)モード：手本を見ながら真似した動作を収録するためのモード
 *    ・手本動作収録モード時はPerception Neuronのみ使用し、実験(真似動作収録)モード時は、SIGVerse、Oculus Rift DK2、Perception Neuronを使用する。
 *    
 *    
 * ＜手本動作収録の流れ＞
 *  1. Axis Neuronを起動し、Perceptoin Neuronを接続しておく。
 *  2. コマンドプロンプトから本プログラムを起動する。
 *  3. 収録を行うかの問いに"y"を押下する。
 *  4. 10秒後にPerception Neuronから動作収録を開始する。
 *  5. 指定した最大収録時間に達するか、"q"をキー入力したときに取得を終了する。
 *  6. コンソールに、取得したデータをデータベースに蓄積するかの問いに、"y"を押下すると取得データをデータベースに蓄積する。
 *  7. コンソールに、取得したデータをファイル出力するかの問いに、"y"を押下すると取得データをファイル出力する。
 *  8. 上記で１収録が終了し、再度収録を行うかの問いが表示される。
 *     このとき設定ファイルを修正して"y"を押下すれば、再び収録が可能で、連続収録ができる。
 * 
 *   ※ 設定ファイルを修正すれば、連続収録が可能だが、途中で実験(真似動作収録)モードに切り替えることは現状不可能。プログラムの再起動が必要。
 *   ※ AXIS NEURON上で動作を保存してから、AXIS NEURON上でその動作を再生・確認し、その後それを再生させつつ動作収録プログラムを実行することも可能。
 * 
 * ＜真似動作収録の流れ＞
 *  1. Axis Neuronを起動し、Perceptoin Neuronを接続する。
 *  2. Oculus Rift DK2も接続する。
 *  3. (Ubuntu側)世界ファイルを使用してSIGServerを起動する。
 *  4. SIGViewerを起動、[START]ボタンを押下し、OculusRiftDK2_vs2010.sigサービスプロバイダを起動する。
 *  5. SIGViewer上のカメラをアバター視点に変更し、SIGViewer画面をOculus Riftのモニターに移動する。
 *  6. コマンドプロンプトから本プログラムを起動する。（第一引数：SIGServerのIPアドレス、第二引数：SIGServerのポート番号）
 *  7. 収録を行うかの問いに"y"を押下する。
 *  8. 10秒後に手本動作の配信が行われるため、ユーザはその動作を真似する。
 *     手本動作配信と同時にPerception Neuronから動作収録が開始される。
 *  9. 手本動作の再生が終了すると、収録も終了する。
 * 10. コンソールに、取得したデータをデータベースに蓄積するかの問いに、"y"を押下すると取得データをデータベースに蓄積する。
 * 11. コンソールに、取得したデータをファイル出力するかの問いに、"y"を押下すると取得データをファイル出力する。
 * 12. 上記で１収録が終了し、再度収録を行うかの問いが表示される。
 *     このとき設定ファイルを修正して"y"を押下すれば、再び収録が可能で、連続収録ができる。
 *  
 *   ※ 設定ファイルを修正すれば、連続収録が可能だが、途中で手本動作収録モードに切り替えることは現状不可能。プログラムの再起動が必要。
 *   ※ 現状、プログラムを終了すると、SIGVerse環境の再起動も必要。
 *
 * ＜引数＞
 * ・手本動作収録の場合：無し
 * ・真似動作収録の場合：第1引数：SIGServer IPアドレス、第2引数：SIGServer ポート番号
 * 
 * ＜その他＞
 * ・設定ファイル名は、ImitationMotionAccumulator.ini
 * ・収録は基本的に連続で行うことが出来るが、収録モードの変更は不可能なので、プログラムを再起動する必要がある。
 * ・設定ファイルのimitation.motion_data_file_pathにファイルパスを設定した場合、データベースではなく当該ファイルから手本動作を取得する
 */
#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>
#include <embodied_brain/imitation_motion_accumulator.h>

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
#include <boost/bind.hpp>

///@brief Parameter file name.
const std::string Param::PARAM_FILE_NAME = "ImitationMotionAccumulator.ini";

std::string Param::dbHost;
int         Param::dbPort;
std::string Param::dbSchema;
std::string Param::dbUser;
std::string Param::dbPass;

std::string Param::generalServiceName;
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
	/*
	 * Perception Neuron関連の初期化
	 */
	std::cout << "■ Perception Neuron関連のコンフィグファイル読込 開始 ■" << std::endl;

	// Read the initialize file.
	this->readIniFile(Param::PARAM_FILE_NAME);

	this->latestSensorData.dataType     = PerceptionNeuronSensorData::DataTypeEnum::BVH;
	this->latestSensorData.bvhData.data = (float *)calloc(10000, sizeof(float));  //多めに領域確保

	std::cout << "■ Perception Neuronへ接続 開始 ■" << std::endl;
	// connect to Perception Neuron
	SOCKET_REF sockRefBvh  = NULL;

	this->connect4Bvh(sockRefBvh);

	// Receive BVH data by the callback function.
	BRRegisterFrameDataCallback(this, ImitationMotionAccumulator::bvhFrameDataReceived);

	// Receive Socket status by the callback function.
	BRRegisterSocketStatusCallback(this, ImitationMotionAccumulator::socketStatusChanged);


	AvatarController avatarController;

	// ★★★★ メインループ 開始 ★★★★
	std::string inputKey;

	while (true)
	{
		// 収録の開始要否をユーザに問う
		inputKey = "";

		if (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
		{
			std::cout << std::endl << "★★ 収録を行いますか？(y/n)★★ ："; std::cin >> inputKey;
		}

		if (inputKey.compare("y") == 0)
		{
			// 処理続行
		}
		else if (inputKey.compare("n") == 0)
		{
			break;
		}
		else
		{
			continue;
		}

		/*
		 * コンフィグファイル読み込み
		 *   （収録の度にコンフィグファイルを変更し、読みなおすことが可能。但し現状、収録モードは変更不可。再起動が必要）
		 */
		std::cout << "■ コンフィグファイル読込 開始 ■" << std::endl;
		Param::readConfigFile();

		char argSigAddr[128]; //SIGVerse IPアドレス
		int  argSigPortNum;   //SIGVerse ポート番号

		if (Param::getMode()==Param::Experiment)
		{
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
				return(EXIT_FAILURE);
			}
		}

		std::cout << "■ データベースの重複チェック (PerceptionNeuron関連) 開始 ■" << std::endl;
		//データベースのID重複チェック
		if (!PerceptionNeuronDAO::duplicationCheck(Param::getImiRecId())){ continue; }


		// 真似動作収録時は、手本動作をDBから取得したり、SIGServerに接続する必要がある
		if (Param::getMode() == Param::Mode::Experiment)
		{
			std::cout << "■ データベースの重複チェック (PMS真似情報関連) 開始 ■" << std::endl;
			//データベースのID重複チェック
			if (!PmsImitationDAO::duplicationCheck(Param::getImiImitationGroupId(), Param::getImiImitationRecType())){ continue; }

			/*
			 * 手本の動作情報を取得
			 */
			std::list<PerceptionNeuronDAO::TimeSeries_t> motionData = this->getMotionDataFromDBorFile(std::to_string(Param::getImiImitationOriginRecId()));

			//DBから取得した手本動作情報を使用してSIGVerseへ送信する電文リストを作成する
			avatarController.makeTelegramForAvatar(motionData, this->generateMessageHeader());

			//不要になった動作情報リストの全要素を削除
			motionData.clear();

			std::cout << "■ SIGVerseへの接続 開始 ■" << std::endl;
			/*
			 * SIGVerseに接続する
			 */
			if (!avatarController.isConnectedToSIGServer())
			{
				//接続
				avatarController.connectSIGServer(argSigAddr, argSigPortNum);

				//SIGVerseからの定期データ受信スレッド実行
				avatarController.checkRecvSIGServiceData();
			}
		}


		std::cout << "■ 収録 開始 ■" << std::endl;

		/*
		 * カウントダウンを表示
		 */
		int showCnt = 10;

		while (showCnt > 0)
		{
			std::cout << "収録開始 " << showCnt << " 秒前" << std::endl;

			Sleep(1000); //１秒スリープ

			showCnt--;
		}

		std::cout << "収録開始！" << std::endl;

		if (Param::getMode() == Param::Mode::RecOrigin)
		{
			std::cout << "('q'キー押下で終了)" << std::endl;
		}
		else
		{
			std::cout << "('q'キー押下で終了することも可能)" << std::endl;
		}


		/*
		 * 真似動作収録時は、別スレッドでSIGVerseアバターに手本動作を送信する
		 */
		boost::thread thSendMotionDataToSIGVerse;

		if (Param::getMode() == Param::Mode::Experiment)
		{
			avatarController.replaying = true;

			//アバターに電文を送って操作
			thSendMotionDataToSIGVerse = boost::thread(&AvatarController::sendMotionDataToSIGVerse, &avatarController);
		}

		/*
		 * 動作情報取得、データベースへの蓄積、ファイル出力処理の開始
		 */
		this->accumulateMotionData(avatarController);

		if (Param::getMode() == Param::Mode::Experiment){ thSendMotionDataToSIGVerse.join(); }

		std::cout << "■ 収録終了 ■" << std::endl;
	}

	// ★★★★ メインループ 終了 ★★★★

	std::cout << "★★ プログラム終了 ★★" << std::endl << std::endl;

	//SIGServerからの切断
	if (Param::getMode() == Param::Mode::Experiment) { avatarController.disconnectFromAllController(); }

	// close socket
	BRCloseSocket(sockRefBvh);

	std::free(this->latestSensorData.bvhData.data);

	// Windows での終了設定
	WSACleanup();

	return EXIT_SUCCESS;
}


/*
 * Perception Neuronから動作情報を受信する、コールバック関数
 */
void __stdcall ImitationMotionAccumulator::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	ImitationMotionAccumulator* pthis = (ImitationMotionAccumulator*)customedObj;

	pthis->updateBvhData(customedObj, sender, header, data);
}


/*
 * Perception Neuronから受信した最新の動作情報を、共有メモリに保存する
 */
void ImitationMotionAccumulator::updateBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	ImitationMotionAccumulator* pthis = (ImitationMotionAccumulator*)customedObj;

	// 排他制御
	std::lock_guard<std::mutex> lock(pthis->mtx4LatestSensorData);

	pthis->latestSensorData.bvhData.avatarIndex   = header->AvatarIndex;
	pthis->latestSensorData.bvhData.avatarName    = std::string((char*)header->AvatarName);
	pthis->latestSensorData.bvhData.withDisp      = (header->WithDisp==1);
	pthis->latestSensorData.bvhData.withReference = (header->WithReference==1);
	pthis->latestSensorData.bvhData.frameIndex    = header->FrameIndex;
	pthis->latestSensorData.bvhData.dataCount     = header->DataCount;

	memcpy(pthis->latestSensorData.bvhData.data, data, header->DataCount * sizeof(float));
}



/*
 * 手本動作情報をDB (or ファイル) から取得する
 */
std::list<PerceptionNeuronDAO::TimeSeries_t> ImitationMotionAccumulator::getMotionDataFromDBorFile(const std::string &recIdStr)
{
	std::list<PerceptionNeuronDAO::TimeSeries_t> motionInfo;
	int motionDataNum;

	if(Param::getImiMotionDataFilePath()=="")
	{
		//データベースから動作情報を取得
		motionDataNum = PerceptionNeuronDAO::selectMotionData(motionInfo, recIdStr);
	}
	else
	{
		//ファイルから動作情報を取得
		FileManager fileManager;
		motionDataNum = fileManager.getMotionData(motionInfo, Param::getImiMotionDataFilePath(), recIdStr);
	}
	if(motionDataNum==0)
	{
		std::cout << "データが１件も無いため終了します。" << std::endl;
		exit(EXIT_FAILURE);
	}

	return motionInfo;
}


/*
 * Perception Neuronから動作情報を取得・蓄積し、データベースへの追加とファイル出力を行う
 */
void ImitationMotionAccumulator::accumulateMotionData(AvatarController &avatarController)
{
	PerceptionNeuronDAO::DataSet motionSet;

	motionSet.summary.recId       = Param::getImiRecId();
	motionSet.summary.userId      = Param::getImiUserId();
	motionSet.summary.recInterval = Param::getImiAccumInterval();
	motionSet.summary.memo        = Param::getImiDbPerceptionNeuronMemo();

	double maxTime;

	if (Param::getMode() == Param::Mode::RecOrigin)
	{
		maxTime = (double)(Param::getImiOriginMaxTime()*1000.0); //単位はms
	}
	else if (Param::getMode() == Param::Mode::Experiment)
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
	 * 特定の条件が満たされるまで、Perception Neuronの動作情報をメモリに蓄積し続ける
	 */
	while (true)
	{
		QueryPerformanceCounter(&nowLi);

		//経過時間
		double elapsedTime = (double)(nowLi.QuadPart - startLi.QuadPart) / freqMilli; //ms単位

		//データ蓄積
		if (elapsedTime >= accumNextTime)
		{
			//動作情報をメモリに蓄積
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

		//最大収録時間を経過するか、手本動作の送信が終了したら、蓄積終了
		if (elapsedTime > maxTime || !avatarController.replaying)
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

	while (it != this->accumulatingSensorDataMap.end())
	{
		PerceptionNeuronDAO::TimeSeries_t posture;
		posture.recId       = Param::getImiRecId();
		posture.elapsedTime = (*it).first;
		posture.hips_pos.x  = ((*it).second).bvhData.data[0];
		posture.hips_pos.y  = ((*it).second).bvhData.data[1];
		posture.hips_pos.z  = ((*it).second).bvhData.data[2];

		for (int i = 0; i < NeuronBVH::BonesType::BonesTypeCount; i++)
		{
			posture.links[i].rotation.y  = ((*it).second).bvhData.data[3*(i+1)+0];
			posture.links[i].rotation.x  = ((*it).second).bvhData.data[3*(i+1)+1];
			posture.links[i].rotation.z  = ((*it).second).bvhData.data[3*(i+1)+2];
		}
		motionSet.timeSeries.push_back(posture);

		free(((*it).second).bvhData.data);

		it++;
	}

	/*
	 * 真似情報作成
	 */
	PmsImitationDAO::DataSet imitationInfo;

	if (Param::getMode() == Param::Mode::Experiment)
	{
		imitationInfo.groupId = Param::getImiImitationGroupId();
		imitationInfo.recType = Param::getImiImitationRecType();
		imitationInfo.recId = Param::getImiRecId();
		imitationInfo.originalRecId = Param::getImiImitationOriginRecId();
		imitationInfo.conditionPulsePower = Param::getImiDbImitationConditionPulsePower();
		imitationInfo.conditionPulseFrequency = Param::getImiDbImitationConditionPulseFrequency();
		imitationInfo.conditionPulseDuration = Param::getImiDbImitationConditionPulseDuration();
		imitationInfo.conditionPulseInterval = Param::getImiDbImitationConditionPulseInterval();
		imitationInfo.conditionPulseNumber = Param::getImiDbImitationConditionPulseNumber();
		imitationInfo.memo = Param::getImiDbImitationMemo();
	}

	// 少しだけキー入力待機
	Sleep(500);


	/*
	 * 取得したPerception Neuronの動作情報をデータベースに追加
	 */
	std::string inputKey;

	while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
	{
		std::cout << "収録データをデータベースに蓄積しますか？(y/n)："; std::cin >> inputKey;
	}

	if (inputKey.compare("y") == 0)
	{
		// Perception Neuron動作関連
		PerceptionNeuronDAO::insertDatabaseExec(motionSet);

		if (Param::getMode() == Param::Mode::Experiment)
		{
			// 真似動作関連
			PmsImitationDAO::insertDatabaseExec(imitationInfo);
		}
	}
	else
	{
		std::cout << "◆データベースに蓄積しません◆" << std::endl << std::endl;
	}


	/*
	 * 取得したPerception Neuronの動作情報をファイル出力
	 */
	FileManager fileManager;
	fileManager.outputDataFile(motionSet, imitationInfo);

	this->accumulatingSensorDataMap.clear();
}


/*
 * 最新の動作情報を共有メモリに保存
 */
void ImitationMotionAccumulator::accumulate(const int elapsedTime)
{
	// スレッド排他制御
	std::lock_guard<std::mutex> lock(this->mtx4LatestSensorData);

	PerceptionNeuronSensorData sensorData;
	sensorData.bvhData.data = (float *)malloc(this->latestSensorData.bvhData.dataCount * sizeof(float));

	sensorData.bvhData.dataCount = this->latestSensorData.bvhData.dataCount;
	memcpy(sensorData.bvhData.data, this->latestSensorData.bvhData.data, this->latestSensorData.bvhData.dataCount * sizeof(float));

	//最新の動作情報を共有メモリのマップに保持する
	this->accumulatingSensorDataMap.insert( make_pair(elapsedTime, sensorData));
}
