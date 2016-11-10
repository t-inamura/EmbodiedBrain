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
}

/*
 * パラメータのリセット
 */
void AvatarController::reset()
{
	this->isSwitched  = false;
	this->isReplaying = true;

	this->fakeMotions.clear();

	std::queue<PerceptionNeuronSensorData> empty;
	std::swap(this->tempMotionsForDelay, empty);

	this->motionDataBeforeSwitching = PerceptionNeuronSensorData();

	this->isFirstForSmoothingSubtractLastPosture = true;

	this->isFirstForSmoothingSubtractLastPostureWithRate = true;
	this->rateCoeffForSmoothingSubtractLastPostureWithRate = 1000 / Param::getSigAvatarDispInterval() - 1;
}

/*
 * SIGServerへの接続
 */
void AvatarController::connectSIGServer(const std::string &ipAddress, const int portNum)
{
	this->m_srv = new sigverse::SIGService(Param::getGeneralServiceName());
	
	bool connected = this->m_srv->connect(ipAddress, portNum);

	if (!connected)
	{
		throw std::string("SIGVerseに接続できませんでした。");
	}
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



///*
// * SIGVerse送信用動作情報電文を作成する
// */
//void AvatarController::makeTelegramForAvatar(const std::list<PerceptionNeuronDAO::TimeSeries_t> &motionData)
//{
//	try
//	{
//		std::cout << "◆SIGVerse送信用動作情報作成　－開始－◆" << std::endl;
//
//		std::list<PerceptionNeuronDAO::TimeSeries_t>::const_iterator it = motionData.begin();
//
//		for (int i = 1; it != motionData.end(); i++)
//		{
//			PerceptionNeuronSensorData sensorData;
//
//			sensorData.dataType = PerceptionNeuronSensorData::DataTypeEnum::BVH;
//			
//			sensorData.bvhData.avatarIndex   = 0;
//			sensorData.bvhData.avatarName    = "PN01";
//			sensorData.bvhData.withDisp      = false;
//			sensorData.bvhData.withReference = false;
//			sensorData.bvhData.frameIndex    = i;
//			sensorData.bvhData.dataCount     = 180;
//			sensorData.bvhData.data          = (float *)malloc(sensorData.bvhData.dataCount * sizeof(float));
//	
//			sensorData.bvhData.data[0] = (*it).hips_pos.x;
//			sensorData.bvhData.data[1] = (*it).hips_pos.y;
//			sensorData.bvhData.data[2] = (*it).hips_pos.z;
//
//			for (int j = 0; j < NeuronBVH::BonesType::BonesTypeCount; j++)
//			{
//				sensorData.bvhData.data[3*(j+1)+0] = (*it).links[j].rotation.y;
//				sensorData.bvhData.data[3*(j+1)+1] = (*it).links[j].rotation.x;
//				sensorData.bvhData.data[3*(j+1)+2] = (*it).links[j].rotation.z;
//			}
//
//			MotionInfoTelegram motionInfoTelegram;
//			motionInfoTelegram.elapsedTime   = (*it).elapsedTime;
//
//			// Send message to SigServer.
//			std::string sensorDataMessage = sensorData.encodeSensorData();
//
//			motionInfoTelegram.motionInfoStr = this->messageHeader + sensorDataMessage;
//
//			this->motionInfoTelegramList.push_back(motionInfoTelegram);
//
//			free(sensorData.bvhData.data);
//
//			it++;
//		}
//	}
//	catch (std::exception& ex)
//	{
//		std::cout << ex.what() << std::endl;
//	}
//}




/*
 * 偽データを共有メモリに保持する
 */
void AvatarController::setFakeData(const std::list<PerceptionNeuronDAO::TimeSeries_t> &motionData)
{
	try
	{
		std::cout << "◆SIGVerse送信用動作情報セット　－開始－◆" << std::endl;

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
			motionInfoTelegram.elapsedTime = (*it).elapsedTime;
			motionInfoTelegram.sensorData  = sensorData;

			this->fakeMotions.push_back(motionInfoTelegram);

			it++;
		}

		std::cout << "◆SIGVerse送信用動作情報セット　－終了－◆" << std::endl;
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
		std::cout << "◆SIGVerseに動作情報電文を送信するスレッド　－開始－◆" << std::endl;

		if(this->fakeMotions.size()==0)
		{
			std::cout << "描画データが１件もありません。終了します。" << std::endl;
			return;
		}

		int startFakeElapsedTime, nextFakeElapsedTime;
		DWORD start_time, now_time, next_time;

		start_time = timeGetTime();

		std::list<MotionInfoTelegram>::iterator it = this->fakeMotions.begin();

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

				PerceptionNeuronSensorData convertedSensorData = sensorData;
				
				// Delay
				convertedSensorData = this->getDelayedSensorData(convertedSensorData);

				// Invert left and right 
				if (Param::getSwitchInvertFlg())
				{
					convertedSensorData = this->getLeftRightInvertedSensorData(convertedSensorData);
				}

				// Save the motion data for after switching.
				this->motionDataBeforeSwitching = convertedSensorData;

				message = this->messageHeader + convertedSensorData.encodeSensorData();
			}
			else
			{
				PerceptionNeuronSensorData sensorData = (*it).sensorData;

				PerceptionNeuronSensorData convertedSensorData = sensorData;
				
				// Invert left and right for Fake motion.
				if (Param::getSwitchInvertFakeFlg())
				{
					convertedSensorData = this->getLeftRightInvertedSensorData(convertedSensorData);
				}

				// Invert left and right 
				if (Param::getSwitchInvertFlg())
				{
					convertedSensorData = this->getLeftRightInvertedSensorData(convertedSensorData);
				}

				// Smoothen
				convertedSensorData = this->getSmoothenedSensorData(convertedSensorData, this->motionDataBeforeSwitching);

				message = this->messageHeader + convertedSensorData.encodeSensorData();
			}


			// Send the message.
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

				while(it != this->fakeMotions.end() && (*it).elapsedTime < nextFakeElapsedTime)
				{
					it++;
				}

				//イテレータの最後に達したらループを抜ける
				if(it == this->fakeMotions.end()){ break; }
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

		this->fakeMotions.clear();

		std::cout << "◆SIGVerseに動作情報電文を送信するスレッド　－終了－◆" << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}


//PerceptionNeuronSensorData AvatarController::getDelayedSensorData(const PerceptionNeuronSensorData &sensorData)
//{
//	if (Param::getSwitchFramesNumberForDelay() <= 0)
//	{
//		return sensorData;
//	}
//	else
//	{
//		PerceptionNeuronSensorData delayedSensorData;
//
//		if (this->tempMotionsForDelay.size() > 0)
//		{
//			delayedSensorData = this->tempMotionsForDelay.front();
//
//			std::cout << "size1=" << this->tempMotionsForDelay.size() << ", data=" << delayedSensorData.bvhData.data[179] << ", arg=" << sensorData.bvhData.data[179] << std::endl;
//
//			if (this->tempMotionsForDelay.size() == Param::getSwitchFramesNumberForDelay())
//			{
//				this->tempMotionsForDelay.pop(); //delete
//			}
//		}
//		else
//		{
//			delayedSensorData = sensorData;
//
//			std::cout << "size0=" << this->tempMotionsForDelay.size() << ", data=" << delayedSensorData.bvhData.data[179] << std::endl;
//		}
//
//		PerceptionNeuronSensorData newSensorData;
//		newSensorData.dataType = PerceptionNeuronSensorData::DataTypeEnum::BVH;	
//		newSensorData.bvhData.avatarIndex   = sensorData.bvhData.avatarIndex;
//		newSensorData.bvhData.avatarName    = sensorData.bvhData.avatarName;
//		newSensorData.bvhData.withDisp      = sensorData.bvhData.withDisp;
//		newSensorData.bvhData.withReference = sensorData.bvhData.withReference;
//		newSensorData.bvhData.frameIndex    = sensorData.bvhData.frameIndex;
//		newSensorData.bvhData.dataCount     = sensorData.bvhData.dataCount;
//
//		newSensorData.bvhData.data = (float *)malloc(sensorData.bvhData.dataCount * sizeof(float));
//		memcpy(newSensorData.bvhData.data, sensorData.bvhData.data, sensorData.bvhData.dataCount * sizeof(float));
//
//		this->tempMotionsForDelay.push(newSensorData);
//
//		return delayedSensorData;
//	}
//}


PerceptionNeuronSensorData AvatarController::getDelayedSensorData(const PerceptionNeuronSensorData &sensorData)
{
	if (Param::getSwitchFramesNumberForDelay() <= 0)
	{
		return sensorData;
	}
	else
	{
		// delete
		if ((int)this->tempMotionsForDelay.size() > Param::getSwitchFramesNumberForDelay())
		{
			this->tempMotionsForDelay.pop(); //delete
		}

		// insert
		this->tempMotionsForDelay.push(sensorData);

		// get
		PerceptionNeuronSensorData delayedSensorData = this->tempMotionsForDelay.front();


		if ((int)this->tempMotionsForDelay.size() > Param::getSwitchFramesNumberForDelay())
		{
//			std::cout << "size1=" << this->tempMotionsForDelay.size() << ", data=" << delayedSensorData.bvhData.data[179] << ", arg=" << sensorData.bvhData.data[179] << std::endl;

			return delayedSensorData;
		}
		else
		{
			PerceptionNeuronSensorData copiedSensorData;
			copiedSensorData.dataType = PerceptionNeuronSensorData::DataTypeEnum::BVH;	
			copiedSensorData.bvhData.avatarIndex   = delayedSensorData.bvhData.avatarIndex;
			copiedSensorData.bvhData.avatarName    = delayedSensorData.bvhData.avatarName;
			copiedSensorData.bvhData.withDisp      = delayedSensorData.bvhData.withDisp;
			copiedSensorData.bvhData.withReference = delayedSensorData.bvhData.withReference;
			copiedSensorData.bvhData.frameIndex    = delayedSensorData.bvhData.frameIndex;
			copiedSensorData.bvhData.dataCount     = delayedSensorData.bvhData.dataCount;

			copiedSensorData.bvhData.data = (float *)malloc(delayedSensorData.bvhData.dataCount * sizeof(float));
			memcpy(copiedSensorData.bvhData.data, delayedSensorData.bvhData.data, delayedSensorData.bvhData.dataCount * sizeof(float));

//			std::cout << "size0=" << this->tempMotionsForDelay.size() << ", data=" << copiedSensorData.bvhData.data[179] << ", arg=" << sensorData.bvhData.data[179] << std::endl;

			return copiedSensorData;
		}
	}
}


PerceptionNeuronSensorData AvatarController::getSmoothenedSensorData(PerceptionNeuronSensorData &sensorData, const PerceptionNeuronSensorData &motionDataBeforeSwitching)
{
	switch (Param::getSmoothingType())
	{
		case Param::SmoothingType::None:
		{
			break;
		}
		case Param::SmoothingType::SubtractLastPosture:
		{
			this->smoothingSubtractLastPosture(sensorData, motionDataBeforeSwitching);
			break;
		}
		case Param::SmoothingType::SubtractLastPostureWithRate:
		{
			this->smoothingSubtractLastPostureWithRate(sensorData, motionDataBeforeSwitching);
			break;
		}
		default:
		{
			std::cout << "incorrect SmoothingType!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	return sensorData;
}


/*
 * スムージング（姿勢減算）
 *   切替直前の姿勢との差分を、偽動作から引き続ける
 */
void AvatarController::smoothingSubtractLastPosture(PerceptionNeuronSensorData &sensorData, const PerceptionNeuronSensorData &motionDataBeforeSwitching)
{
	//ダミー引く本当を、ダミーから引けばいい
	if (this->isFirstForSmoothingSubtractLastPosture)
	{
		this->diffSensorDataForSmoothingSubtractLastPosture.dataType = PerceptionNeuronSensorData::DataTypeEnum::BVH;
			
		//this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.avatarIndex   = 0;
		//this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.avatarName    = "PN01";
		//this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.withDisp      = false;
		//this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.withReference = false;
		//this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.frameIndex    = i;
		this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.dataCount     = 180;
		this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.data          = (float *)malloc(this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.dataCount * sizeof(float));
	
		this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.data[0] = sensorData.bvhData.data[0] - motionDataBeforeSwitching.bvhData.data[0];
		this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.data[1] = sensorData.bvhData.data[1] - motionDataBeforeSwitching.bvhData.data[1];
		this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.data[2] = sensorData.bvhData.data[2] - motionDataBeforeSwitching.bvhData.data[2];

		for (int j = 0; j < NeuronBVH::BonesType::BonesTypeCount; j++)
		{
			this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.data[3*(j+1)+0] = sensorData.bvhData.data[3*(j+1)+0] - motionDataBeforeSwitching.bvhData.data[3*(j+1)+0];
			this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.data[3*(j+1)+1] = sensorData.bvhData.data[3*(j+1)+1] - motionDataBeforeSwitching.bvhData.data[3*(j+1)+1];
			this->diffSensorDataForSmoothingSubtractLastPosture.bvhData.data[3*(j+1)+2] = sensorData.bvhData.data[3*(j+1)+2] - motionDataBeforeSwitching.bvhData.data[3*(j+1)+2];
		}

		this->isFirstForSmoothingSubtractLastPosture = false;
	}

	float rate = 1.0;

	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHand, rate);
	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandThumb1, rate);
	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandThumb2, rate);
	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandThumb3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightInHandIndex, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandIndex1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandIndex2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandIndex3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightInHandMiddle, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandMiddle1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandMiddle2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandMiddle3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightInHandRing, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandRing1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandRing2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandRing3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightInHandPinky, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandPinky1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandPinky2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::RightHandPinky3, rate);

	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHand, rate);
	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandThumb1, rate);
	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandThumb2, rate);
	this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandThumb3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftInHandIndex, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandIndex1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandIndex2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandIndex3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftInHandMiddle, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandMiddle1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandMiddle2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandMiddle3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftInHandRing, rate;
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandRing1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandRing2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandRing3, rate);
//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftInHandPinky, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandPinky1, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandPinky2, rate);
	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPosture, NeuronBVH::BonesType::LeftHandPinky3, rate);
}


/*
 * スムージング（姿勢減算）
 *   切替直前の姿勢との差分を偽動作から引くが、徐々にその比率を下げ、１秒後位には偽動作そのものを使用する
 */
void AvatarController::smoothingSubtractLastPostureWithRate(PerceptionNeuronSensorData &sensorData, const PerceptionNeuronSensorData &motionDataBeforeSwitching)
{
	//ダミー引く本当を、ダミーから引けばいい
	if (this->isFirstForSmoothingSubtractLastPostureWithRate)
	{
		this->diffSensorDataForSmoothingSubtractLastPostureWithRate.dataType = PerceptionNeuronSensorData::DataTypeEnum::BVH;
			
		//this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.avatarIndex   = 0;
		//this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.avatarName    = "PN01";
		//this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.withDisp      = false;
		//this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.withReference = false;
		//this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.frameIndex    = i;
		this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.dataCount     = 180;
		this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.data          = (float *)malloc(this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.dataCount * sizeof(float));
	
		this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.data[0] = sensorData.bvhData.data[0] - motionDataBeforeSwitching.bvhData.data[0];
		this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.data[1] = sensorData.bvhData.data[1] - motionDataBeforeSwitching.bvhData.data[1];
		this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.data[2] = sensorData.bvhData.data[2] - motionDataBeforeSwitching.bvhData.data[2];

		for (int j = 0; j < NeuronBVH::BonesType::BonesTypeCount; j++)
		{
			this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.data[3*(j+1)+0] = sensorData.bvhData.data[3*(j+1)+0] - motionDataBeforeSwitching.bvhData.data[3*(j+1)+0];
			this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.data[3*(j+1)+1] = sensorData.bvhData.data[3*(j+1)+1] - motionDataBeforeSwitching.bvhData.data[3*(j+1)+1];
			this->diffSensorDataForSmoothingSubtractLastPostureWithRate.bvhData.data[3*(j+1)+2] = sensorData.bvhData.data[3*(j+1)+2] - motionDataBeforeSwitching.bvhData.data[3*(j+1)+2];
		}

		this->isFirstForSmoothingSubtractLastPostureWithRate = false;
	}


	if (this->rateCoeffForSmoothingSubtractLastPostureWithRate > 0)
	{
		float rate = this->rateCoeffForSmoothingSubtractLastPostureWithRate * Param::getSigAvatarDispInterval() / 1000.0f;

		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHand, rate);
		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandThumb1, rate);
		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandThumb2, rate);
		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandThumb3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightInHandIndex, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandIndex1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandIndex2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandIndex3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightInHandMiddle, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandMiddle1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandMiddle2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandMiddle3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightInHandRing, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandRing1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandRing2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandRing3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightInHandPinky, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandPinky1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandPinky2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::RightHandPinky3, rate);

		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHand, rate);
		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandThumb1, rate);
		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandThumb2, rate);
		this->smoothingAddPosture1Link (sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandThumb3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftInHandIndex, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandIndex1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandIndex2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandIndex3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftInHandMiddle, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandMiddle1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandMiddle2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandMiddle3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftInHandRing, rate;
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandRing1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandRing2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandRing3, rate);
	//	this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftInHandPinky, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandPinky1, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandPinky2, rate);
		this->smoothingAddPosture1LinkZ(sensorData, this->diffSensorDataForSmoothingSubtractLastPostureWithRate, NeuronBVH::BonesType::LeftHandPinky3, rate);

		this->rateCoeffForSmoothingSubtractLastPostureWithRate--;
	}
}



void AvatarController::smoothingAddPosture1Link(PerceptionNeuronSensorData &sensorData, const PerceptionNeuronSensorData &diffSensorData, NeuronBVH::BonesType bonesType, float rate)
{
	sensorData.bvhData.data[3*(bonesType+1)+0] -= diffSensorData.bvhData.data[3*(bonesType+1)+0] * rate;
	sensorData.bvhData.data[3*(bonesType+1)+1] -= diffSensorData.bvhData.data[3*(bonesType+1)+1] * rate;
	sensorData.bvhData.data[3*(bonesType+1)+2] -= diffSensorData.bvhData.data[3*(bonesType+1)+2] * rate;
}

void AvatarController::smoothingAddPosture1LinkZ(PerceptionNeuronSensorData &sensorData, const PerceptionNeuronSensorData &diffSensorData, NeuronBVH::BonesType bonesType, float rate)
{
	sensorData.bvhData.data[3*(bonesType+1)+2] -= diffSensorData.bvhData.data[3*(bonesType+1)+2] * rate;
}


/*
 * 左右反転させる
 */
PerceptionNeuronSensorData AvatarController::getLeftRightInvertedSensorData(PerceptionNeuronSensorData &sensorData)
{
	this->invertPosture1Link (sensorData, NeuronBVH::BonesType::RightHand,         NeuronBVH::BonesType::LeftHand);
	this->invertPosture1Link (sensorData, NeuronBVH::BonesType::RightHandThumb1,   NeuronBVH::BonesType::LeftHandThumb1);
	this->invertPosture1Link (sensorData, NeuronBVH::BonesType::RightHandThumb2,   NeuronBVH::BonesType::LeftHandThumb2);
	this->invertPosture1Link (sensorData, NeuronBVH::BonesType::RightHandThumb3,   NeuronBVH::BonesType::LeftHandThumb3);
//	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightInHandIndex,  NeuronBVH::BonesType::LeftInHandIndex); //
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandIndex1,   NeuronBVH::BonesType::LeftHandIndex1);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandIndex2,   NeuronBVH::BonesType::LeftHandIndex2);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandIndex3,   NeuronBVH::BonesType::LeftHandIndex3);
//	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightInHandMiddle, NeuronBVH::BonesType::LeftInHandMiddle); //
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandMiddle1,  NeuronBVH::BonesType::LeftHandMiddle1);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandMiddle2,  NeuronBVH::BonesType::LeftHandMiddle2);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandMiddle3,  NeuronBVH::BonesType::LeftHandMiddle3);
//	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightInHandRing,   NeuronBVH::BonesType::LeftInHandRing); //
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandRing1,    NeuronBVH::BonesType::LeftHandRing1);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandRing2,    NeuronBVH::BonesType::LeftHandRing2);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandRing3,    NeuronBVH::BonesType::LeftHandRing3);
//	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightInHandPinky,  NeuronBVH::BonesType::LeftInHandPinky); //
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandPinky1,   NeuronBVH::BonesType::LeftHandPinky1);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandPinky2,   NeuronBVH::BonesType::LeftHandPinky2);
	this->invertPosture1LinkZ(sensorData, NeuronBVH::BonesType::RightHandPinky3,   NeuronBVH::BonesType::LeftHandPinky3);

	return sensorData;
}


void AvatarController::invertPosture1Link(PerceptionNeuronSensorData &sensorData, NeuronBVH::BonesType bonesType1, NeuronBVH::BonesType bonesType2)
{
	float tempy = sensorData.bvhData.data[3*(bonesType1+1)+0];
	float tempx = sensorData.bvhData.data[3*(bonesType1+1)+1];
	float tempz = sensorData.bvhData.data[3*(bonesType1+1)+2];

	sensorData.bvhData.data[3*(bonesType1+1)+0] = -sensorData.bvhData.data[3*(bonesType2+1)+0];
	sensorData.bvhData.data[3*(bonesType1+1)+1] = +sensorData.bvhData.data[3*(bonesType2+1)+1];
	sensorData.bvhData.data[3*(bonesType1+1)+2] = -sensorData.bvhData.data[3*(bonesType2+1)+2];

	sensorData.bvhData.data[3*(bonesType2+1)+0] = -tempy;
	sensorData.bvhData.data[3*(bonesType2+1)+1] = +tempx;
	sensorData.bvhData.data[3*(bonesType2+1)+2] = -tempz;
}

void AvatarController::invertPosture1LinkZ(PerceptionNeuronSensorData &sensorData, NeuronBVH::BonesType bonesType1, NeuronBVH::BonesType bonesType2)
{
	float tempz = sensorData.bvhData.data[3*(bonesType1+1)+2];

	sensorData.bvhData.data[3*(bonesType1+1)+2] = -sensorData.bvhData.data[3*(bonesType2+1)+2];

	sensorData.bvhData.data[3*(bonesType2+1)+2] = -tempz;
}


/*
 * SIGServerに接続しているか否かを返す
 */
bool AvatarController::isConnectedToSIGServer()
{
	if (this->m_srv == NULL){ return false; }

	return this->m_srv->getConnectedControllerNum() != 0;
}

