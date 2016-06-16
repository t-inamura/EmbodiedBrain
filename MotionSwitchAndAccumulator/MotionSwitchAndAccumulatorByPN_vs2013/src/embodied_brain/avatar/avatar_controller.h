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
		std::string motionInfoStr;
	};

//	std::string floatToString(const float x);

	//SIGService
	sigverse::SIGService *m_srv = NULL;

	//SIGVerse送信用動作情報電文リスト
	std::list<MotionInfoTelegram> motionInfoTelegramList;

	std::string           messageHeader;
	PerceptionNeuronData  *perceptionNeuronData;

public:
	AvatarController(std::string messageHeader, PerceptionNeuronData *perceptionNeuronData);

	void connectSIGServer(const std::string &ipAddress, const int portNum);
	void disconnectFromAllController();
	void checkRecvSIGServiceData();

	void makeTelegramForAvatar(const std::list<PerceptionNeuronDAO::TimeSeries_t> &motionData);
	void sendMotionDataToSIGVerse();
	bool isConnectedToSIGServer();

	std::string latestMotionInfoStr;

	bool switched;
	bool replaying;
};

#endif //EMBODIED_BRAIN_AVATAR_CONTROLLER_H




