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

	//typedef std::map<std::string, Quaternion> QMap;

	//bool diffVec(XnPoint3D &rvec, const XnSkeletonJointPosition &jvec, const XnSkeletonJointPosition &kvec);
	//Quaternion calcQuaternion(const XnPoint3D &kvec, const XnPoint3D &svec);
	//void rotVec(XnPoint3D &v, const Quaternion &q);
	//XnPoint3D getSigVec(int sigvec);
	std::string floatToString(const float x);
	//std::string getStringFromQuaternion(const std::string &jname, const Quaternion &q);
//	Quaternion multiQuaternion(const Quaternion &p, const Quaternion &q);

	//SIGService
	sigverse::SIGService *m_srv;

	//SIGVerse送信用動作情報電文リスト
	std::list<MotionInfoTelegram> motionInfoTelegramList;

public:

	void connectSIGServer(const std::string &ipAddress, const int portNum);
	void disconnectFromAllController();
	void checkRecvSIGServiceData();

	void makeTelegramForAvatar(const std::list<PerceptionNeuronDAO::TimeSeries_t> &motionData);
	void sendMotionDataToSIGVerse();
};

#endif //EMBODIED_BRAIN_AVATAR_CONTROLLER_H




