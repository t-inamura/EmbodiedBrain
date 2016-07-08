/*
 * アバター操作クラス
 */
#include <embodied_brain/common/param.h>
#include <embodied_brain/avatar/check_recv_sigservice_data.h>
#include <embodied_brain/avatar/avatar_controller.h>
#include <SIGService/SIGService.h>

#include <iostream>
#include <sstream>
#include <map>
#include <math.h>
#include <tchar.h>
#include <time.h>
#include <mmsystem.h>
#include <windows.h>

#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>


/*
 * コンストラクタ
 */
AvatarController::AvatarController(std::string messageHeader, PerceptionNeuronData *perceptionNeuronData)
{
	this->messageHeader        = messageHeader;
	this->perceptionNeuronData = perceptionNeuronData; // Shared data. Latest Perception Neuron data.
	this->reset();
}

/*
 * パラメータのリセット
 */
void AvatarController::reset()
{
	this->isSwitched  = false;
	this->isReplaying = true;
}

/*
 * SIGServerへの接続
 */
void AvatarController::connectSIGServer(const std::string &ipAddress, const int portNum)
{
	this->m_srv = new sigverse::SIGService(Param::getGeneralServiceName());
	
	this->m_srv->connect(ipAddress, portNum);
//	this->m_srv->connectToViewer();
//	this->m_srv->setAutoExitProc(true);
}

/*
 * SIGServerからの切断
 */
void AvatarController::disconnectFromAllController()
{
	if (this->m_srv == NULL){ return; }

	// 接続中の全コントローラと切断する
	if (this->isConnectedToSIGServer())
	{
		this->m_srv->disconnectFromAllController();
	}

	// SIGVerseサーバディスコネクト
	this->m_srv->disconnect();
}

/*
 * SIGServiceからのデータチェックを行う
 */
void AvatarController::checkRecvSIGServiceData()
{
	//データチェックは別スレッドで行う
	CheckRecvSIGServiceData checkRecvSIGServiceData;

	boost::thread thCheckRecvData(&CheckRecvSIGServiceData::run, &checkRecvSIGServiceData, this->m_srv);
}


//
//
///*
// * floatをstringに変換する
// */
//std::string AvatarController::floatToString(const float x)
//{
//	char tmp[32];
//	sprintf_s(tmp, 32, "%f", x);
//	std::string str;
//	str = std::string(tmp);
//	return str;
//}



/*
 * SIGVerse送信用動作情報電文を作成する
 */
void AvatarController::makeTelegramForAvatar(const std::list<PerceptionNeuronDAO::TimeSeries_t> &motionData)
{
	try
	{
		std::cout << "◆SIGVerse送信用動作情報作成　－開始－◆" << std::endl;

		std::list<PerceptionNeuronDAO::TimeSeries_t>::const_iterator it = motionData.begin();

		for (int i = 1; it != motionData.end(); i++)
		{
			PerceptionNeuronSensorData sensorData;

			sensorData.dataType = PerceptionNeuronSensorData::DataTypeEnum::BVH;
			
			sensorData.bvhData.avatarIndex   = 0;
			sensorData.bvhData.avatarName    = "PN01";
			sensorData.bvhData.withDisp      = false;
			sensorData.bvhData.withReference = false;
			sensorData.bvhData.frameIndex    = i;
			sensorData.bvhData.dataCount     = 180;
			sensorData.bvhData.data          = (float *)malloc(sensorData.bvhData.dataCount * sizeof(float));
	
			sensorData.bvhData.data[0] = (*it).hips_pos.x;
			sensorData.bvhData.data[1] = (*it).hips_pos.y;
			sensorData.bvhData.data[2] = (*it).hips_pos.z;

			for (int j = 0; j < NeuronBVH::BonesType::BonesTypeCount; j++)
			{
				sensorData.bvhData.data[3*(j+1)+0] = (*it).links[j].rotation.y;
				sensorData.bvhData.data[3*(j+1)+1] = (*it).links[j].rotation.x;
				sensorData.bvhData.data[3*(j+1)+2] = (*it).links[j].rotation.z;
			}

			MotionInfoTelegram motionInfoTelegram;
			motionInfoTelegram.elapsedTime   = (*it).elapsedTime;

			// Send message to SigServer.
			std::string sensorDataMessage = sensorData.encodeSensorData();

			motionInfoTelegram.motionInfoStr = this->messageHeader + sensorDataMessage;

			this->motionInfoTelegramList.push_back(motionInfoTelegram);

			free(sensorData.bvhData.data);

			it++;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}


/*
 * SIGVerseに動作情報電文を送信
 */
void AvatarController::sendMotionDataToSIGVerse()
{
	try
	{
//		std::cout << "◆描画　－開始－◆" << std::endl;

		if(this->motionInfoTelegramList.size()==0)
		{
			std::cout << "描画データが１件もありません。終了します。" << std::endl;
			return;
		}

		int startFakeElapsedTime, nextFakeElapsedTime;
		DWORD start_time, now_time, next_time;

		start_time = timeGetTime();

		std::list<MotionInfoTelegram>::iterator it = this->motionInfoTelegramList.begin();

		//最初のデータの経過時間を設定する
		startFakeElapsedTime = (*it).elapsedTime;

		int totalCnt = 0;
		int fakeCnt  = 0;
		std::string message;

		while (true)
		{
			//'Q'を押下されて強制終了された場合
			if (!this->isReplaying)
			{
				break;
			}

			if (!this->isSwitched)
			{
				PerceptionNeuronSensorData sensorData = this->perceptionNeuronData->getLatestSensorData();

				// Send message to SIGServer.
				std::string sensorDataMessage = sensorData.encodeSensorData();

				message = this->messageHeader + sensorDataMessage;
			}
			else
			{

				message = (*it).motionInfoStr;
			}

			// 接続中のコントローラの数
			int connectedNum = this->m_srv->getConnectedControllerNum();

			// 接続中のコントローラが存在する
			if (connectedNum > 0)
			{
				// 接続中の全エンティティに送信する
				std::vector<std::string> names = this->m_srv->getAllConnectedEntitiesName();

				for (int i = 0; i < connectedNum; i++)
				{
					this->m_srv->sendMsgToCtr(names[i].c_str(), message);
				}
			}

			totalCnt++;

			if (this->isSwitched)
			{
				fakeCnt++;

				//イテレータを次の送信データまで進める
				nextFakeElapsedTime = startFakeElapsedTime + Param::getSigAvatarDispInterval() * fakeCnt;

				while(it != this->motionInfoTelegramList.end() && (*it).elapsedTime < nextFakeElapsedTime)
				{
					it++;
				}

				//イテレータの最後に達したらループを抜ける
				if(it == this->motionInfoTelegramList.end()){ break; }
			}

			//次の送信時刻までsleepする
			next_time = start_time + Param::getSigAvatarDispInterval() * totalCnt;
			
			now_time = timeGetTime();

			DWORD sleepTime = next_time - now_time;

			if(sleepTime > 0 && sleepTime<1000000) //極端にsleepTimeが大きくなっているときは、next_time<now_time となっているため、その場合はsleepしない。
			{
				Sleep(sleepTime);
			}
		}

		this->isReplaying = false;

		this->motionInfoTelegramList.clear();

		std::cout << "◆描画　－終了－◆" << std::endl;

//		Sleep(100000);
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}


/*
 * SIGServerに接続しているか否かを返す
 */
bool AvatarController::isConnectedToSIGServer()
{
	if (this->m_srv == NULL){ return false; }

	return this->m_srv->getConnectedControllerNum() != 0;
}