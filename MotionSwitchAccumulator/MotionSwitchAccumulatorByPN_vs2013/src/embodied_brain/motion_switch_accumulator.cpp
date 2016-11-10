/*
 * 動作切替・収録プログラムは、最初は自分自身の動作をSIGverseアバターに反映し、'S'キーを押下した瞬間に事前収録した偽動作をSIGVerseアバターに反映する。
 * 動作切替の前の動作と、後の動作をMySQLに蓄積する。
 * 
 *    ・動作情報取得には Perception Neuron を使用する。
 *    ・以下の２つの収録モードを用意する。
 *    　　・偽動作収録モード　　：ユーザに見せるための偽動作を収録するためのモード
 *    　　・動作切替・収録モード：最初は自分自身の動作をSIGverseアバターに反映し、'S'キーを押下した瞬間に事前収録した偽動作をSIGVerseアバターに反映するモード
 *    ・偽動作収録モード時はPerception Neuronのみ使用し、動作切替・収録モード時は、SIGVerse、Oculus Rift DK2、Perception Neuronを使用する。
 *    ・動作切替・収録モード時は、同じパラメータを使用して反復実験＆連続収録可能。同一被験者に同様の実験を連続で何回もしていただいて、効率的に実験結果を蓄積することが目的。
 *    　（反復収録した場合、収録IDは変えず、シリアル番号を振る）
 *    
 * ＜偽動作収録モードの流れ＞
 *  1. Axis Neuronを起動し、Perceptoin Neuronを接続しておく。
 *  2. コマンドプロンプトから本プログラムを起動する。
 *  3. 一部のパラメータ(収録ID、ユーザID、偽収録IDリストなど)はコンフィグファイルにブランクが設定されている場合、コマンドプロンプトから手入力する。
 *  4. 収録を行うかの問いに"y"を押下する。
 *  5. 3秒後にPerception Neuronから動作収録を開始する。
 *  6. 指定した最大収録時間に達するか、"q"をキー入力したときに取得を終了する。
 *  7. コンソールに、取得したデータをデータベースに蓄積するかの問いに、"y"を押下すると取得データをデータベースに蓄積する。
 *  8. コンソールに、取得したデータをファイル出力するかの問いに、"y"を押下すると取得データをファイル出力する。
 *  9. 上記で１収録が終了し、再度収録を行うかの問いが表示される。
 *     このとき設定ファイルを修正して"y"を押下すれば、再び収録が可能で、連続収録ができる。
 * 
 *   ※ 設定ファイルを修正すれば、連続収録が可能だが、途中で動作切替・収録モードに切り替えることは現状不可能。プログラムの再起動が必要。
 *   ※ AXIS NEURON上で動作を保存してから、AXIS NEURON上でその動作を再生・確認し、その後それを再生させつつ動作収録プログラムを実行することも可能。
 * 
 * ＜動作切替・収録モードの流れ＞
 *  1. Axis Neuronを起動し、Perceptoin Neuronを接続する。
 *  2. Oculus Rift DK2も接続する。
 *  3. (Ubuntu側)世界ファイルを使用してSIGServerを起動する。
 *  4. SIGViewerを起動、[START]ボタンを押下し、OculusRiftDK2_vs2010.sigサービスプロバイダを起動する。
 *  5. SIGViewer上のカメラをアバター視点に変更し、SIGViewer画面をOculus Riftのモニターに移動する。
 *  6. コマンドプロンプトから本プログラムを起動する。（第一引数：SIGServerのIPアドレス、第二引数：SIGServerのポート番号）
 *  7. 一部のパラメータ(収録ID、ユーザID、偽収録IDリストなど)はコンフィグファイルにブランクが設定されている場合、コマンドプロンプトから手入力する。
 *  8. 収録を行うかの問いに"y"を押下する。
 *  9. 3秒後に自分自身の動作がSIGVerseアバターに反映されるため、ユーザはそれを見ながら何らかの動作を行う。
 *     同時にPerception Neuron動作（切替前動作）の収録が開始される。
 * 10. 操作者は任意のタイミングで"s"を押下する。するとそのタイミングで、事前収録した偽動作がSIGVerseアバターに反映される。
 *     このとき切替前動作の収録が終了し、更に切替後動作の収録が開始される。
 * 11. 偽動作の再生が終わるか、"q"キー押下で切替後動作の収録が終了する。
 * 12. コンソールに、取得したデータをデータベースに蓄積するかの問いに、"y"を押下すると取得データをデータベースに蓄積する。
 * 13. コンソールに、取得したデータをファイル出力するかの問いに、"y"を押下すると取得データをファイル出力する。
 * 14. 上記で１収録が終了し、再度収録を行うかの問いが表示される。
 *     (但し、コンフィグパラメータの実験反復回数に2以上が設定されていた場合、手順9に戻り同様の収録がその回数分繰り返される)
 *     このとき設定ファイルを修正して"y"を押下すれば、再び収録が可能で、連続収録ができる。
 *  
 *   ※ 設定ファイルを修正すれば、連続収録が可能だが、途中で偽動作収録モードに切り替えることは現状不可能。プログラムの再起動が必要。
 *   ※ 現状、プログラムを終了すると、SIGVerse環境の再起動も必要。
 *
 * ＜引数＞
 * ・偽動作収録の場合：無し
 * ・動作切替・収録の場合：第1引数：SIGServer IPアドレス、第2引数：SIGServer ポート番号
 * 
 * ＜その他＞
 * ・設定ファイル名は、MotionSwitchAccumulator.ini
 * ・収録は基本的に連続で行うことが出来るが、収録モードの変更は不可能で、プログラムを再起動する必要がある。
 * ・設定ファイルのswitch.motion_data_file_pathにファイルパスを設定した場合、データベースではなく当該ファイルから偽動作を取得する
 */
#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>
#include <embodied_brain/motion_switch_accumulator.h>

#include <embodied_brain/common/param.h>
#include <embodied_brain/file/file_manager.h>

#include <cstddef>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <sstream>
#include <conio.h>
#include <limits>
#include <ctime>
#include <random>
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
const std::string Param::PARAM_FILE_NAME = "MotionSwitchAccumulator.ini";

std::string Param::dbHost;
int         Param::dbPort;
std::string Param::dbSchema;
std::string Param::dbUser;
std::string Param::dbPass;

std::string Param::generalServiceName;
int         Param::sigAvatarDispInterval;

int             Param::switchAccumInterval;
std::string     Param::switchMotionDataFilePath;

int             Param::switchRecId;
int             Param::switchUserId;

int             Param::switchFakeMaxTime;

int             Param::switchNumberOfIterations;
std::list<int>  Param::switchFakeRecIdList;

std::string     Param::switchDbPerceptionNeuronMemo;
std::string     Param::switchDbMswRecordingInfoMemo;

int             Param::switchFramesNumberForDelay;

bool            Param::switchInvertFlg;
bool            Param::switchInvertFakeFlg;

Param::Mode Param::mode;
Param::SmoothingType Param::smoothingType;
Param::FakeMotionsSelectionMethod Param::fakeMotionSelectionMethod;


/*
 * メイン処理
 */
int MotionSwitchAccumulator::run(int argc, char **argv)
{
	/*
	 * Perception Neuron関連の初期化
	 */
	std::cout << "■ Perception Neuron関連のコンフィグファイル読込 開始 ■" << std::endl;

	// Read the initialize file.
	this->readIniFile(Param::PARAM_FILE_NAME);

	this->perceptionNeuronData = new PerceptionNeuronData();

	std::cout << "■ Perception Neuronへ接続 開始 ■" << std::endl;
	// connect to Perception Neuron
	SOCKET_REF sockRefBvh  = NULL;

	this->connect4Bvh(sockRefBvh);

	// Receive BVH data by the callback function.
	BRRegisterFrameDataCallback(this, MotionSwitchAccumulator::bvhFrameDataReceived);

	// Receive Socket status by the callback function.
	BRRegisterSocketStatusCallback(this, MotionSwitchAccumulator::socketStatusChanged);


	AvatarController avatarController(this->generateMessageHeader(), this->perceptionNeuronData);

	// ★★★★ メインループ 開始 ★★★★
	std::string inputKey;

	while (true)
	{
		// 収録の開始要否をユーザに問う
		inputKey = "";

		if (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
		{
			std::cout << std::endl << "★★★ 収録を行いますか？(y/n)★★★ ："; std::cin >> inputKey;
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

		// ユーザID、収録ID、偽収録IDリストを入力
		this->inputValues();


		if (Param::getSwitchRecId() > MotionSwitchAccumulator::additionalId4before || Param::getSwitchRecId()==0)
		{
			std::cout << std::endl << "RecIdは、" << MotionSwitchAccumulator::additionalId4before << " 以下にしてください。あと０も禁止。" << std::endl;
			continue;
		}

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
		if (!PerceptionNeuronDAO::duplicationCheck(Param::getSwitchRecId())){ continue; }
		if (!PerceptionNeuronDAO::duplicationCheck(Param::getSwitchRecId()+MotionSwitchAccumulator::additionalId4before)){ continue; } //切替前ID

		// 偽動作IDの再生順配列動的確保
		int *fakeIdPlayOrderList = new int[Param::getSwitchNumberOfIterations()];

		// 真似動作収録時は、SIGServerに接続する必要がある
		if (Param::getMode() == Param::Mode::Experiment)
		{
			std::cout << "■ データベースの重複チェック (動作切替・収録情報関連) 開始 ■" << std::endl;
			//データベースのID重複チェック
			if (!MswRecordingInfoDAO::duplicationCheck(Param::getSwitchRecId())){ continue; }

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

			//偽動作IDの再生順配列作成
			this->initFakeIdPlayOrderList(fakeIdPlayOrderList);
		}

		std::cout << "★★ 反復収録　開始 (反復回数="<< Param::getSwitchNumberOfIterations() << ")★★" << std::endl;
			
		// 反復収録
		for (int serialNumber = 1; serialNumber<=Param::getSwitchNumberOfIterations(); serialNumber++)
		{
			std::cout << "■ 収録開始 (" << serialNumber << "回目)■" << std::endl;

			int fakeRecId;

			// 真似動作収録時は、偽動作をDBから取得する必要がある
			if (Param::getMode() == Param::Mode::Experiment)
			{
				avatarController.reset();

				fakeRecId = fakeIdPlayOrderList[serialNumber-1];

				/*
				 * 偽の動作情報を取得
				 */
				std::list<PerceptionNeuronDAO::TimeSeries_t> motionData = this->getMotionDataFromDBorFile(fakeRecId, MotionSwitchAccumulator::fakeMotionSerialNumber);

				//DBから取得した偽動作情報を使用してSIGVerseへ送信する電文リストを作成する
				//			avatarController.makeTelegramForAvatar(motionData);
				avatarController.setFakeData(motionData);

				//不要になった動作情報リストの全要素を削除
				motionData.clear();
			}

			/*
			 * カウントダウンを表示
			 */
			int showCnt = 3;

			while (showCnt > 0)
			{
				std::cout << "収録開始 " << showCnt << " 秒前" << std::endl;

				Sleep(1000); //１秒スリープ

				showCnt--;
			}

			if (Param::getMode() == Param::Mode::RecFake)
			{
				std::cout << "('q'キー押下で終了)" << std::endl;
			}
			else
			{
				std::cout << "('s'キー押下で 偽動作にSwitch)(Switch後なら 'q'キー押下で終了可能)" << std::endl;
			}


			/*
			 * 真似動作収録時は、別スレッドでSIGVerseアバターに偽動作を送信する
			 */
			boost::thread thSendMotionDataToSIGVerse;

			if (Param::getMode() == Param::Mode::Experiment)
			{
				//アバターに電文を送って操作
				thSendMotionDataToSIGVerse = boost::thread(&AvatarController::sendMotionDataToSIGVerse, &avatarController);
			}

			/*
			 * 動作情報取得、データベースへの蓄積、ファイル出力処理の開始
			 */
			if (Param::getMode() == Param::Mode::RecFake)
			{
				this->accumulateMotionData4RecFake(MotionSwitchAccumulator::fakeMotionSerialNumber);
			}
			else
			{
				this->accumulateMotionData4Experiment(avatarController, serialNumber, fakeRecId);
			}

			if (Param::getMode() == Param::Mode::Experiment){ thSendMotionDataToSIGVerse.join(); }

			std::cout << "■ 収録終了 (" << serialNumber << "回目)■" << std::endl << std::endl << std::endl;
		}

		delete[] fakeIdPlayOrderList;

		std::cout << "★★ 反復収録　終了★★" << std::endl << std::endl << std::endl;
	}

	// ★★★★ メインループ 終了 ★★★★

	std::cout << "★★★ プログラム終了 ★★★" << std::endl << std::endl;

	//SIGServerからの切断
	if (Param::getMode() == Param::Mode::Experiment) { avatarController.disconnectFromAllController(); }

	// close socket
	BRCloseSocket(sockRefBvh);

	delete this->perceptionNeuronData;

	// Windows での終了設定
	WSACleanup();

	return EXIT_SUCCESS;
}


/*
 * 偽動作IDの再生順配列作成
 */
void MotionSwitchAccumulator::initFakeIdPlayOrderList(int *fakeIdPlayOrderList)
{
	// 偽動作IDの再生順配列初期化
	for (int i = 0; i<Param::getSwitchNumberOfIterations(); i++)
	{
		fakeIdPlayOrderList[i] = 0;
	}

	// 偽動作IDの再生順配列作成
	switch (Param::getFakeMotionsSelectionMethod())
	{
		case Param::FakeMotionsSelectionMethod::Random:
		{
			std::srand((unsigned int)time(NULL));
				
//			std::cout << "size=" << Param::switchFakeRecIdList.size() << std::endl;

			std::list<int>::iterator it = Param::switchFakeRecIdList.begin();

			for (int i = 0; i<Param::getSwitchNumberOfIterations(); i++)
			{
				while (true)
				{
					int j = std::rand() % Param::getSwitchNumberOfIterations();

					if (fakeIdPlayOrderList[j] == 0)
					{
//						std::cout << "順序生成 j=" << j << ", val=" << *it << std::endl;

						fakeIdPlayOrderList[j] = (int)(*it);
						it++;
						break;
					}
				}

				if (it == Param::switchFakeRecIdList.end())
				{
					it = Param::switchFakeRecIdList.begin();
				}
			}

			break;
		}
		case Param::FakeMotionsSelectionMethod::Sequentially:
		{
			if (Param::getSwitchNumberOfIterations()!=Param::switchFakeRecIdList.size())
			{
				std::cout << "Sequentiallyの場合は、実験反復回数と偽収録IDリストの数を同じにしてください。" << std::endl;
				exit(EXIT_FAILURE);
			}
			std::list<int>::iterator it = Param::switchFakeRecIdList.begin();

			for (int i = 0; i<Param::getSwitchNumberOfIterations(); i++)
			{
				fakeIdPlayOrderList[i] = (int)(*it);
				it++;
			}

			break;
		}
		default:
		{
			std::cout << "incorrect FakeMotionsSelectionMethod!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// 偽動作IDの再生順表示
	std::cout << "偽動作IDの再生順" << std::endl;
			
	for (int i = 0; i<Param::getSwitchNumberOfIterations(); i++)
	{
		std::cout << fakeIdPlayOrderList[i] << std::endl;
	}
}


/*
 * 幾つかのパラメータに関しては、手入力可能とする
 */
void MotionSwitchAccumulator::inputValues()
{
	const int maxLine = 10;
	char inputLineAry[maxLine];

	boost::regex regexId("[0-9]{1,9}");

	// ユーザIDの入力
	std::cout << std::endl;
	
	std::cin.getline(inputLineAry, maxLine);

	// 少しだけキー入力待機
	Sleep(500);

	if (Param::getSwitchUserId()!=0)
	{
		std::cout << "> ユーザID (user_id) は設定ファイルのものを使用します。" << std::endl;
	}
	else
	{
		while (true)
		{
			std::cout << "> ユーザID (user_id) を入力してください" << std::endl;
			std::cout << "> "; std::cin.getline(inputLineAry, maxLine);

			std::string inputLine = inputLineAry;
			boost::algorithm::trim(inputLine);

			if (regex_match(inputLine, regexId))
			{
				Param::switchUserId = std::atoi(inputLine.c_str());

				std::cout << Param::getSwitchUserId() << "に設定しました" << std::endl;
				break;
			}
			else
			{
				std::cout << "不正な値です。" << std::endl;
			}
		}
	}

	// 収録IDの入力
	std::cout << std::endl;
	
	// 少しだけキー入力待機
	Sleep(500);

	if (Param::getSwitchRecId()!=0)
	{
		std::cout << "> 収録ID (rec_id) は設定ファイルのものを使用します。"<< std::endl;
	}
	else
	{
		while (true)
		{
			std::cout << "> 収録ID (rec_id) を入力してください" << std::endl;
			std::cout << "> "; std::cin.getline(inputLineAry, maxLine);

			std::string inputLine = inputLineAry;
			boost::algorithm::trim(inputLine);

			if (regex_match(inputLine, regexId))
			{
				Param::switchRecId = std::atoi(inputLine.c_str());

				std::cout << Param::getSwitchRecId() << "に設定しました" << std::endl;
				break;
			}
			else
			{
				std::cout << "不正な値です。" << std::endl;
			}
		}
	}

	// 偽収録IDリスト(カンマ区切り)の入力
	if (Param::getMode() == Param::Mode::Experiment)
	{
		std::cout << std::endl;
	
		// 少しだけキー入力待機
		Sleep(500);

		if (!(Param::switchFakeRecIdList.size()==0))
		{
			std::cout << "> 偽収録IDリスト(fake_rec_id_list) は設定ファイルのものを使用します。" << std::endl;
		}
		else
		{
			while (true)
			{
				std::cout << "> 偽収録IDリスト(fake_rec_id_list) をカンマ区切りで入力してください" << std::endl;
				std::cout << "> "; std::cin.getline(inputLineAry, maxLine);

				std::string inputLine = inputLineAry;
				boost::algorithm::trim(inputLine);

				try
				{
					Param::switchFakeRecIdList = Param::splitStrIntoIntList(inputLine, ",");

					std::cout << inputLine << "に設定しました" << std::endl;
					break;
				}
				catch (std::string str)
				{
					std::cout << str << std::endl;
				}
			}
		}
	}
}


/*
 * Perception Neuronから動作情報を受信する、コールバック関数
 */
void __stdcall MotionSwitchAccumulator::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	MotionSwitchAccumulator* pthis = (MotionSwitchAccumulator*)customedObj;

	pthis->perceptionNeuronData->updateBvhData(header, data);
}


/*
 * 偽動作情報をDB (or ファイル) から取得する
 */
std::list<PerceptionNeuronDAO::TimeSeries_t> MotionSwitchAccumulator::getMotionDataFromDBorFile(const int fakeRecId, const int serialNumber)
{
	std::list<PerceptionNeuronDAO::TimeSeries_t> motionInfo;
	int motionDataNum;

	if(Param::getSwitchMotionDataFilePath()=="")
	{
		//データベースから動作情報を取得
		motionDataNum = PerceptionNeuronDAO::selectMotionData(motionInfo, fakeRecId, serialNumber);
	}
	else
	{
		//ファイルから動作情報を取得
		FileManager fileManager;
		motionDataNum = fileManager.getMotionData(motionInfo, Param::getSwitchMotionDataFilePath(), fakeRecId, serialNumber);
	}
	if(motionDataNum==0)
	{
		std::cout << "データが１件も無いため終了します。" << std::endl;
		exit(EXIT_FAILURE);
	}

	return motionInfo;
}


/*
 * Perception Neuronから動作情報を取得・蓄積し、データベースへの追加とファイル出力を行う（偽動作収録モード用）
 */
void MotionSwitchAccumulator::accumulateMotionData4RecFake(const int serialNumber)
{
	AvatarController dummyAvatarController(this->generateMessageHeader(), this->perceptionNeuronData);

	//時系列モーションデータを共有メモリに保持しつつ、サマリ情報を作成
	PerceptionNeuronDAO::DataSet pnDataSet = this->accumulateMotionDataAfterSwitching(dummyAvatarController, serialNumber);

	std::cout << "収録を終了します" << std::endl;

	//時系列モーションデータを、DataSet型に追加する
	this->setMotionDataAfterSwitching(pnDataSet,  this->accumulatedDataMapAfterSwitching);

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
		try
		{
			// Perception Neuron動作関連
			PerceptionNeuronDAO::insertDatabaseExec(pnDataSet);
		}
		catch (std::string &ex)
		{
			std::cout << " ERR :" << ex << std::endl;
			std::cout << "Inserting data to database is failed." << std::endl;
		}
		catch (boost::exception &ex) 
		{
			std::cout << " ERR :" << boost::diagnostic_information_what(ex) << std::endl;
			std::cout << "Inserting data to database is failed." << std::endl;
		}
		catch (std::exception ex)
		{
			std::cout << " ERR :" << ex.what() << std::endl;
			std::cout << "Inserting data to database is failed." << std::endl;
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
	fileManager.outputDataFile(pnDataSet);

	this->accumulatedDataMapAfterSwitching.clear();
}


/*
 * Perception Neuronから動作情報を取得・蓄積し、データベースへの追加とファイル出力を行う（実験(動作切替・収録)モード用）
 */
void MotionSwitchAccumulator::accumulateMotionData4Experiment(AvatarController &avatarController, const int serialNumber, const int fakeRecId)
{
	//切替前後の時系列モーションデータを共有メモリに保持しつつ、サマリ情報を作成
	PerceptionNeuronDAO::DataSet pnDataSetBeforeSwitching = this->accumulateMotionDataBeforeSwitching(avatarController, serialNumber);
	PerceptionNeuronDAO::DataSet pnDataSetAfterSwitching  = this->accumulateMotionDataAfterSwitching(avatarController, serialNumber);

	std::cout << "収録を終了します" << std::endl;

	//切替前後の時系列モーションデータを、DataSet型に追加する
	this->setMotionDataBeforeSwitching(pnDataSetBeforeSwitching, this->accumulatedDataMapBeforeSwitching);
	this->setMotionDataAfterSwitching (pnDataSetAfterSwitching,  this->accumulatedDataMapAfterSwitching);

	/*
	 * 動作切替情報作成
	 */
	MswRecordingInfoDAO::DataSet recordingInfo;

	recordingInfo.afterSwitchingRecId     = Param::getSwitchRecId();
	recordingInfo.seialNumber             = serialNumber;
	recordingInfo.beforeSwitchingRecId    = Param::getSwitchRecId() + MotionSwitchAccumulator::additionalId4before;
	recordingInfo.fakeRecId               = fakeRecId;
	recordingInfo.memo                    = Param::getSwitchDbMswRecordingInfoMemo();
	recordingInfo.numberOfIterations      = Param::getSwitchNumberOfIterations();
	recordingInfo.smoothingType           = (int)(Param::getSmoothingType());
	recordingInfo.framesNumberForDelay    = Param::getSwitchFramesNumberForDelay();
	recordingInfo.invertFlg               = Param::getSwitchInvertFlg();
	recordingInfo.invertFakeFlg           = Param::getSwitchInvertFakeFlg();

	// 少しだけキー入力待機
	Sleep(500);


	/*
	 * 取得したPerception Neuronの動作情報と動作切替情報をデータベースに追加
	 */
	std::string inputKey;

	while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
	{
		std::cout << "収録データをデータベースに蓄積しますか？(y/n)："; std::cin >> inputKey;
	}

	if (inputKey.compare("y") == 0)
	{
		// Perception Neuron動作関連
		PerceptionNeuronDAO::insertDatabaseExec(pnDataSetBeforeSwitching);
		PerceptionNeuronDAO::insertDatabaseExec(pnDataSetAfterSwitching);

		// 動作切替関連
		MswRecordingInfoDAO::insertDatabaseExec(recordingInfo);
	}
	else
	{
		std::cout << "◆データベースに蓄積しません◆" << std::endl << std::endl;
	}

	/*
	 * 取得したPerception Neuronの動作情報と動作切替情報をファイル出力
	 */
	FileManager fileManager;
	fileManager.outputDataFile(pnDataSetBeforeSwitching, pnDataSetAfterSwitching, recordingInfo);

	this->accumulatedDataMapBeforeSwitching.clear();
	this->accumulatedDataMapAfterSwitching.clear();
}


/*
 * 切替前のモーションデータを共有メモリに保持する（キー"s"が入力されるまでループを繰り返す）
 */
PerceptionNeuronDAO::DataSet MotionSwitchAccumulator::accumulateMotionDataBeforeSwitching(AvatarController &avatarController, const int serialNumber)
{
	PerceptionNeuronDAO::DataSet motionSet;

	motionSet.summary.recId        = Param::getSwitchRecId()+MotionSwitchAccumulator::additionalId4before;
	motionSet.summary.serialNumber = serialNumber;
	motionSet.summary.userId       = Param::getSwitchUserId();
	motionSet.summary.recInterval  = Param::getSwitchAccumInterval();
	motionSet.summary.memo         = Param::getSwitchDbPerceptionNeuronMemo();

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
	double accumNextTime = (double)(Param::getSwitchAccumInterval() * accumCnt);
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
			//最新の動作情報を共有メモリのマップに保持する
			this->accumulatedDataMapBeforeSwitching.insert( make_pair((int)elapsedTime, this->perceptionNeuronData->getLatestSensorData()));

			//次回データ蓄積時間を算出
			while (elapsedTime >= accumNextTime)
			{
				accumCnt++;
				accumNextTime = (double)(Param::getSwitchAccumInterval() * accumCnt);
			}
		}

		//100msに1回キー入力待ちする
		if (elapsedTime >= waitKeyNextTime)
		{
			//キー"s"が入力されたら、偽動作にSwitch
			if (GetAsyncKeyState('S'))
			{
				while (_kbhit() != 0){ _getch(); } //キー入力をクリア
				avatarController.isSwitched = true;

				std::cout << "切り替え！" << std::endl;
				break;
			}

			//次回キー入力待ち時間を算出
			while (elapsedTime >= waitKeyNextTime)
			{
				waitKeyCnt++;
				waitKeyNextTime = (double)(waitKeyInterval  * waitKeyCnt);
			}
		}
	}

	time(&end_time);

	//収録時間をセットする
	motionSet.summary.recTotalTime = (int)(difftime(end_time, start_time)); //秒

	return motionSet;
}


/*
 * 切替後の時系列モーションデータを共有メモリに保持しつつ、サマリ情報を作成
 */
PerceptionNeuronDAO::DataSet MotionSwitchAccumulator::accumulateMotionDataAfterSwitching(AvatarController &avatarController, const int serialNumber)
{
	PerceptionNeuronDAO::DataSet motionSet;

	motionSet.summary.recId        = Param::getSwitchRecId();
	motionSet.summary.serialNumber = serialNumber;
	motionSet.summary.userId       = Param::getSwitchUserId();
	motionSet.summary.recInterval  = Param::getSwitchAccumInterval();
	motionSet.summary.memo         = Param::getSwitchDbPerceptionNeuronMemo();

	double maxTime;

	if (Param::getMode() == Param::Mode::RecFake)
	{
		maxTime = (double)(Param::getSwitchFakeMaxTime()*1000.0); //単位はms
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
	double accumNextTime = (double)(Param::getSwitchAccumInterval() * accumCnt);
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
			//最新の動作情報を共有メモリのマップに保持する
			this->accumulatedDataMapAfterSwitching.insert( make_pair((int)elapsedTime, this->perceptionNeuronData->getLatestSensorData()));

			//次回データ蓄積時間を算出
			while (elapsedTime >= accumNextTime)
			{
				accumCnt++;
				accumNextTime = (double)(Param::getSwitchAccumInterval() * accumCnt);
			}
		}

		//100msに1回キー入力待ちする
		if (elapsedTime >= waitKeyNextTime)
		{
			//キー"q"が入力されたら処理終了
			if (GetAsyncKeyState('Q'))
			{
				while (_kbhit() != 0){ _getch(); } //キー入力をクリア
				avatarController.isReplaying = false;
				break;
			}

			//次回キー入力待ち時間を算出
			while (elapsedTime >= waitKeyNextTime)
			{
				waitKeyCnt++;
				waitKeyNextTime = (double)(waitKeyInterval  * waitKeyCnt);
			}
		}

		//最大収録時間を経過するか、偽動作の送信が終了したら、蓄積終了
		if (elapsedTime > maxTime || !avatarController.isReplaying)
		{
			break;
		}
	}

	time(&end_time);

	//収録時間をセットする
	motionSet.summary.recTotalTime = (int)(difftime(end_time, start_time)); //秒

	return motionSet;
}


/*
 * 切替前のモーションデータを、DataSet型に入れなおす
 * （切替前のモーションは直近10秒間だけ収録するものとする）
 */
void MotionSwitchAccumulator::setMotionDataBeforeSwitching(PerceptionNeuronDAO::DataSet &motionSet, const std::map<int, PerceptionNeuronSensorData> &accumulatedDataMap)
{
	// 10秒を超えていなければ、すべての動作を収録する
	int maxTimeBeforeSwitching = 10; //10 s

	if (motionSet.summary.recTotalTime <= maxTimeBeforeSwitching)
	{
		this->setMotionDataAfterSwitching(motionSet, accumulatedDataMap);
		return;
	}

	/*
	 * 10秒を超えていた場合、直近10秒間だけ収録する
	 */
	motionSet.summary.recTotalTime = maxTimeBeforeSwitching;

	int startPointOfElapsedTime = -1;

	std::map<int, PerceptionNeuronSensorData>::const_reverse_iterator it = accumulatedDataMap.rbegin();

	// 逆順でデータを取得
	while (it != accumulatedDataMap.rend())
	{
		PerceptionNeuronDAO::TimeSeries_t posture;
		posture.recId        = motionSet.summary.recId;
		posture.serialNumber = motionSet.summary.serialNumber;
		posture.elapsedTime  = (*it).first;
		posture.hips_pos.x   = ((*it).second).bvhData.data[0];
		posture.hips_pos.y   = ((*it).second).bvhData.data[1];
		posture.hips_pos.z   = ((*it).second).bvhData.data[2];

		// 切替直前の時刻の10秒前（直近10秒の開始時刻）を算出する
		if (startPointOfElapsedTime < 0)
		{
			startPointOfElapsedTime = posture.elapsedTime - maxTimeBeforeSwitching * 1000;
		}

		// 直近10秒に該当する場合は、データを収録する
		if (posture.elapsedTime - startPointOfElapsedTime >= 0)
		{
			for (int i = 0; i < NeuronBVH::BonesType::BonesTypeCount; i++)
			{
				posture.links[i].rotation.y  = ((*it).second).bvhData.data[3*(i+1)+0];
				posture.links[i].rotation.x  = ((*it).second).bvhData.data[3*(i+1)+1];
				posture.links[i].rotation.z  = ((*it).second).bvhData.data[3*(i+1)+2];
			}
			motionSet.timeSeries.push_front(posture);
		}

		free(((*it).second).bvhData.data);

		it++;
	}
}

/*
 * 切替後のモーションデータを、DataSet型に入れなおす
 */
void MotionSwitchAccumulator::setMotionDataAfterSwitching(PerceptionNeuronDAO::DataSet &motionSet, const std::map<int, PerceptionNeuronSensorData> &accumulatedDataMap)
{
	std::map<int, PerceptionNeuronSensorData>::const_iterator it = accumulatedDataMap.begin();

	while (it != accumulatedDataMap.end())
	{
		PerceptionNeuronDAO::TimeSeries_t posture;
		posture.recId        = motionSet.summary.recId;
		posture.serialNumber = motionSet.summary.serialNumber;
		posture.elapsedTime  = (*it).first;
		posture.hips_pos.x   = ((*it).second).bvhData.data[0];
		posture.hips_pos.y   = ((*it).second).bvhData.data[1];
		posture.hips_pos.z   = ((*it).second).bvhData.data[2];

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
}
