/*
 * ファイル管理クラス
 */
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <time.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <embodied_brain/file/file_manager.h>

/*
 * ファイルから動作データを取得する
 */
int  FileManager::getMotionData(std::list<PerceptionNeuronDAO::TimeSeries_t> &timeSeries, const std::string &fileName, const int recId, const int serialNumber)
{
	try
	{
		DWORD startTime, endTime;
		double diffTime;

		boost::regex delimiter("\t");
		int jointDataNum = 3;

		std::cout << "◆動作データ取得　－開始－◆" << std::endl;

		/*
		 * ファイルから動作データ取得
		 */
		startTime = timeGetTime();

		std::ifstream ifs(fileName);
		std::string lineStr, buf;

		while(std::getline(ifs,lineStr))
		{
			std::vector<std::string> results;
			boost::regex_split(back_inserter(results), lineStr, delimiter);

			//１件でも動作IDが異なるデータが存在した場合は、強制終了する
			if(std::to_string(recId)!=results[0] && std::to_string(serialNumber)!=results[1])
			{
				std::cout << "ファイル内に、引数で指定した動作IDと異なる動作IDが存在したため終了します。" << std::endl;
				exit(EXIT_FAILURE);
			}

			PerceptionNeuronDAO::TimeSeries_t motion;

			motion.recId        = std::atoi(results[0].c_str());
			motion.serialNumber = std::atoi(results[1].c_str());
			motion.elapsedTime  = std::atoi(results[2].c_str());

			motion.hips_pos.x   = (float)atof(results[3].c_str());
			motion.hips_pos.y   = (float)atof(results[4].c_str());
			motion.hips_pos.z   = (float)atof(results[5].c_str());

			int index = 6;

			this->setJointPosition(motion.links[NeuronBVH::BonesType::Hips],              NeuronBVH::BonesType::Hips,              results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightUpLeg],        NeuronBVH::BonesType::RightUpLeg,        results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightLeg],          NeuronBVH::BonesType::RightLeg,          results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightFoot],         NeuronBVH::BonesType::RightFoot,         results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftUpLeg],         NeuronBVH::BonesType::LeftUpLeg,         results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftLeg],           NeuronBVH::BonesType::LeftLeg,           results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftFoot],          NeuronBVH::BonesType::LeftFoot,          results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::Spine],             NeuronBVH::BonesType::Spine,             results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::Spine1],            NeuronBVH::BonesType::Spine1,            results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::Spine2],            NeuronBVH::BonesType::Spine2,            results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::Spine3],            NeuronBVH::BonesType::Spine3,            results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::Neck],              NeuronBVH::BonesType::Neck,              results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::Head],              NeuronBVH::BonesType::Head,              results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightShoulder],     NeuronBVH::BonesType::RightShoulder,     results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightArm],          NeuronBVH::BonesType::RightArm,          results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightForeArm],      NeuronBVH::BonesType::RightForeArm,      results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHand],         NeuronBVH::BonesType::RightHand,         results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandThumb1],   NeuronBVH::BonesType::RightHandThumb1,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandThumb2],   NeuronBVH::BonesType::RightHandThumb2,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandThumb3],   NeuronBVH::BonesType::RightHandThumb3,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandIndex],  NeuronBVH::BonesType::RightInHandIndex,  results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandIndex1],   NeuronBVH::BonesType::RightHandIndex1,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandIndex2],   NeuronBVH::BonesType::RightHandIndex2,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandIndex3],   NeuronBVH::BonesType::RightHandIndex3,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandMiddle], NeuronBVH::BonesType::RightInHandMiddle, results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandMiddle1],  NeuronBVH::BonesType::RightHandMiddle1,  results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandMiddle2],  NeuronBVH::BonesType::RightHandMiddle2,  results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandMiddle3],  NeuronBVH::BonesType::RightHandMiddle3,  results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandRing],   NeuronBVH::BonesType::RightInHandRing,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandRing1],    NeuronBVH::BonesType::RightHandRing1,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandRing2],    NeuronBVH::BonesType::RightHandRing2,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandRing3],    NeuronBVH::BonesType::RightHandRing3,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandPinky],  NeuronBVH::BonesType::RightInHandPinky,  results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandPinky1],   NeuronBVH::BonesType::RightHandPinky1,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandPinky2],   NeuronBVH::BonesType::RightHandPinky2,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::RightHandPinky3],   NeuronBVH::BonesType::RightHandPinky3,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftShoulder],      NeuronBVH::BonesType::LeftShoulder,      results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftArm],           NeuronBVH::BonesType::LeftArm,           results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftForeArm],       NeuronBVH::BonesType::LeftForeArm,       results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHand],          NeuronBVH::BonesType::LeftHand,          results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandThumb1],    NeuronBVH::BonesType::LeftHandThumb1,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandThumb2],    NeuronBVH::BonesType::LeftHandThumb2,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandThumb3],    NeuronBVH::BonesType::LeftHandThumb3,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandIndex],   NeuronBVH::BonesType::LeftInHandIndex,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandIndex1],    NeuronBVH::BonesType::LeftHandIndex1,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandIndex2],    NeuronBVH::BonesType::LeftHandIndex2,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandIndex3],    NeuronBVH::BonesType::LeftHandIndex3,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandMiddle],  NeuronBVH::BonesType::LeftInHandMiddle,  results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandMiddle1],   NeuronBVH::BonesType::LeftHandMiddle1,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandMiddle2],   NeuronBVH::BonesType::LeftHandMiddle2,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandMiddle3],   NeuronBVH::BonesType::LeftHandMiddle3,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandRing],    NeuronBVH::BonesType::LeftInHandRing,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandRing1],     NeuronBVH::BonesType::LeftHandRing1,     results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandRing2],     NeuronBVH::BonesType::LeftHandRing2,     results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandRing3],     NeuronBVH::BonesType::LeftHandRing3,     results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandPinky],   NeuronBVH::BonesType::LeftInHandPinky,   results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandPinky1],    NeuronBVH::BonesType::LeftHandPinky1,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandPinky2],    NeuronBVH::BonesType::LeftHandPinky2,    results, index); index+=jointDataNum;
			this->setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandPinky3],    NeuronBVH::BonesType::LeftHandPinky3,    results, index); index+=jointDataNum;

			//動作情報リストに追加
			timeSeries.push_back(motion);
		}

		endTime = timeGetTime();

		diffTime = (endTime-startTime)/1000.0;

		std::cout << "レコード数＝" << timeSeries.size() << std::endl;

		if (timeSeries.size() == 0)
		{
			std::cout << "データがありません。ファイルパスも確認してください。" << std::endl;
		}

		fprintf(stdout, "ファイル入力にかかった時間： %8.3f[ms] \n", diffTime);

		std::cout << "◆モデル動作データ取得　－終了－◆" << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return timeSeries.size();
}


/*
 * MySQLから取得した関節座標データ(x,y,z)を、SIGVerse送信用関節オブジェクトに設定する
 */
void FileManager::setJointPosition(PerceptionNeuronSensorData::PerceptionNeuronBVHJoint &joint, const NeuronBVH::BonesType &bonesType, const std::vector<std::string> &vecStr, const int index)
{
	joint.jointType = bonesType;

	joint.rotation.x = (float)atof(vecStr[index+0].c_str());
	joint.rotation.y = (float)atof(vecStr[index+1].c_str());
	joint.rotation.z = (float)atof(vecStr[index+2].c_str());
}


/*
 * 動作情報をファイル出力（実験(動作切替・収録)モード用）
 */
void FileManager::outputDataFile(const PerceptionNeuronDAO::DataSet &motionDataBeforeSwitching, const PerceptionNeuronDAO::DataSet &motionDataAfterSwitching, const MswRecordingInfoDAO::DataSet &recordingInfo)
{
	try
	{
		std::string inputKey;

		while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
		{
			std::cout << "収録データをファイル出力しますか？(y/n)："; std::cin >> inputKey;
		}

		if (inputKey.compare("y") == 0)
		{
			std::cout << "◆ファイル出力　－開始－◆" << std::endl;

			//dataディレクトリの作成
			if (!boost::filesystem::exists("data"))
			{
				if (!boost::filesystem::create_directory("data"))
				{
					std::cout << "data ディレクトリの作成に失敗しました。ファイル出力を中止します。" << std::endl;
					return;
				}
			}

			this->outputDataFilePerceptionNeuron(motionDataBeforeSwitching);
			this->outputDataFilePerceptionNeuron(motionDataAfterSwitching);

			this->outputDataFileSwitching(recordingInfo);

			std::cout << "◆ファイル出力　－終了－◆" << std::endl << std::endl;
		}
		else
		{
			std::cout << "◆ファイル出力しません◆" << std::endl << std::endl;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}


/*
 * 動作情報をファイル出力（偽動作収録モード用）
 */
void FileManager::outputDataFile(const PerceptionNeuronDAO::DataSet &motionData)
{
	try
	{
		std::string inputKey;

		while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
		{
			std::cout << "収録データをファイル出力しますか？(y/n)："; std::cin >> inputKey;
		}

		if (inputKey.compare("y") == 0)
		{
			std::cout << "◆ファイル出力　－開始－◆" << std::endl;

			//dataディレクトリの作成
			if (!boost::filesystem::exists("data"))
			{
				if (!boost::filesystem::create_directory("data"))
				{
					std::cout << "data ディレクトリの作成に失敗しました。ファイル出力を中止します。" << std::endl;
					return;
				}
			}

			this->outputDataFilePerceptionNeuron(motionData);

			std::cout << "◆ファイル出力　－終了－◆" << std::endl << std::endl;
		}
		else
		{
			std::cout << "◆ファイル出力しません◆" << std::endl << std::endl;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}



/*
 * PerceptionNeuron情報をファイル出力
 */
void FileManager::outputDataFilePerceptionNeuron(const PerceptionNeuronDAO::DataSet &motionData)
{
	//出力ファイルパスを作成
	struct tm nowTm;
	time_t timet;
	char filePath[250];

	timet = time(NULL);
	localtime_s(&nowTm, &timet);

	std::ofstream ofs;

	/*
	 * Perception Neuron動作サマリファイル出力
	 */
	strftime(filePath, sizeof(filePath), "data\\perception_neuron_motions_summary_%Y%m%d_%H%M%S.dat", &nowTm);

	ofs.open(filePath);

	ofs << motionData.summary.recId                << "\t"
		<< motionData.summary.serialNumber         << "\t"
		<< motionData.summary.userId               << "\t"
		<< motionData.summary.recTotalTime         << "\t"
		<< motionData.summary.getRecStartTimeStr() << "\t"
		<< motionData.summary.recInterval          << "\t"
		<< "'" << motionData.summary.memo << "'"
		<< std::endl;

	ofs.flush();
	ofs.close();
	ofs.clear();

	std::cout << "　Perception Neuron動作サマリファイル出力 完了(" << filePath << ")" << std::endl;

	/*
	 * Perception Neuron動作時系列ファイル出力
	 */
	strftime(filePath, sizeof(filePath), "data\\perception_neuron_motions_time_series_%Y%m%d_%H%M%S.dat", &nowTm);

	ofs.open(filePath);

	std::list<PerceptionNeuronDAO::TimeSeries_t>::const_iterator it = motionData.timeSeries.begin();

	while (it != motionData.timeSeries.end())
	{
		ofs << (*it).recId << "\t" << (*it).elapsedTime << "\t"
			<< (*it).hips_pos.x << "\t" << (*it).hips_pos.y << "\t" << (*it).hips_pos.z;

		std::string linksStr;

		for (int i = 0; i < NeuronBVH::BonesType::BonesTypeCount; i++)
		{
			linksStr += this->getJointStr((*it).links[i].rotation);
		}

		ofs << linksStr << std::endl;

		it++;
	}

	ofs.flush();
	ofs.close();
	ofs.clear();

	std::cout << "　Perception Neuron動作時系列出力 完了(" << filePath << ")" << std::endl;
}

/*
 * 切替動作・収録情報をファイル出力
 */
void FileManager::outputDataFileSwitching(const MswRecordingInfoDAO::DataSet &recordingInfo)
{
	std::cout << "◆ファイル出力　－開始－◆" << std::endl;

	//出力ファイルパスを作成
	struct tm nowTm;
	time_t timet;
	char filePath[250];

	timet = time(NULL);
	localtime_s(&nowTm, &timet);

	std::ofstream ofs;

	/*
	 * 動作切替実験_収録情報ファイル出力
	 */
	strftime(filePath, sizeof(filePath), "data\\motion_switching_recording_info_%Y%m%d_%H%M%S.dat", &nowTm);

	ofs.open(filePath);

	ofs << recordingInfo.afterSwitchingRecId << "\t"
		<< recordingInfo.seialNumber << "\t"
		<< recordingInfo.beforeSwitchingRecId << "\t"
		<< recordingInfo.fakeRecId << "\t"
		<< "'" << recordingInfo.memo << "'"
		<< std::endl;

	ofs.flush();
	ofs.close();
	ofs.clear();

	std::cout << "　動作切替実験_収録情報ファイル出力 完了(" << filePath << ")" << std::endl;
}


/*
 * ファイル出力用Joint情報文字列を返す
 */
std::string FileManager::getJointStr(const SigCmn::Vector3 &rotation)
{
	return "\t" + std::to_string(rotation.x) + "\t" + std::to_string(rotation.y) + "\t" + std::to_string(rotation.z);
}
