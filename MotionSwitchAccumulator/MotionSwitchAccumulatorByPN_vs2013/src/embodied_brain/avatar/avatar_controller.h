#ifndef EMBODIED_BRAIN_AVATAR_CONTROLLER_H
#define EMBODIED_BRAIN_AVATAR_CONTROLLER_H

#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <math.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <queue>

#include <SIGService/SIGService.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>
#include <embodied_brain/common/perception_neuron_data.h>


/*
 * アバター操作クラス
 */
class AvatarController
{
private:
	//動作情報電文データ構造
	struct MotionInfoTelegram
	{
		int elapsedTime;
//		std::string motionInfoStr;
		PerceptionNeuronSensorData sensorData;
	};

	//SIGService
	sigverse::SIGService *m_srv = NULL;

	//偽動作時系列データ
	std::list<MotionInfoTelegram> fakeMotions;

	std::queue<PerceptionNeuronSensorData> tempMotionsForDelay;

	PerceptionNeuronSensorData motionDataBeforeSwitching;

	std::string           messageHeader;
	PerceptionNeuronData  *perceptionNeuronData;

public:
	AvatarController(std::string messageHeader, PerceptionNeuronData *perceptionNeuronData);
	void reset();

	void connectSIGServer(const std::string &ipAddress, const int portNum);
	void disconnectFromAllController();
	void checkRecvSIGServiceData();

	void makeTelegramForAvatar(const std::list<PerceptionNeuronDAO::TimeSeries_t> &motionData);
	void setFakeData(const std::list<PerceptionNeuronDAO::TimeSeries_t> &motionData);

	void sendMotionDataToSIGVerse();
	bool isConnectedToSIGServer();

	std::string latestMotionInfoStr;

	PerceptionNeuronSensorData getDelayedSensorData(PerceptionNeuronSensorData sensorData);

	PerceptionNeuronSensorData getSmoothenedSensorData(PerceptionNeuronSensorData sensorData, PerceptionNeuronSensorData motionDataBeforeSwitching);
		void smoothingSubtractLastPostureWithRate(PerceptionNeuronSensorData &sensorData, PerceptionNeuronSensorData motionDataBeforeSwitching);
		void smoothingSubtractLastPosture(PerceptionNeuronSensorData &sensorData, PerceptionNeuronSensorData motionDataBeforeSwitching);

	void smoothingAddPosture1Link(PerceptionNeuronSensorData &sensorData, PerceptionNeuronSensorData diffSensorData, NeuronBVH::BonesType bonesType, float rate);
	void smoothingAddPosture1LinkZ(PerceptionNeuronSensorData &sensorData, PerceptionNeuronSensorData diffSensorData, NeuronBVH::BonesType bonesType, float rate);

	PerceptionNeuronSensorData getLeftRightInvertedSensorData(PerceptionNeuronSensorData &sensorData);
	void invertPosture1Link(PerceptionNeuronSensorData &sensorData, NeuronBVH::BonesType bonesType1, NeuronBVH::BonesType bonesType2);
	void invertPosture1LinkZ(PerceptionNeuronSensorData &sensorData, NeuronBVH::BonesType bonesType1, NeuronBVH::BonesType bonesType2);

	bool isSwitched;
	bool isReplaying;
};

#endif //EMBODIED_BRAIN_AVATAR_CONTROLLER_H




