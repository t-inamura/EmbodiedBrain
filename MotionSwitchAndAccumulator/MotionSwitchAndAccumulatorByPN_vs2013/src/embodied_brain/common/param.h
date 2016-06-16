#ifndef EMBODIED_BRAIN_PARAM_H
#define EMBODIED_BRAIN_PARAM_H

#include <iostream>
#include <string>
#include <list>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

/*
 * パラメータクラス
 */
class Param
{
public :
	enum Mode
	{
		RecFake,
		Experiment,
	};

	//パラメータファイル名
	static const std::string PARAM_FILE_NAME;

private:
	static std::string dbHost;
	static int         dbPort;
	static std::string dbSchema;
	static std::string dbUser;
	static std::string dbPass;

	static std::string    generalServiceName;
	static int            sigAvatarDispInterval;

	static int         imiAccumInterval;
	static std::string imiMotionDataFilePath;

	static int         imiRecId;
	static int         imiUserId;

	static int         imiOriginMaxTime;

	static int         imiImitationGroupId;
	static int         imiImitationRecType;
	static int         imiImitationOriginRecId;

	static std::string imiDbPerceptionNeuronMemo;
	static float       imiDbImitationConditionPulsePower;
	static float       imiDbImitationConditionPulseFrequency;
	static int         imiDbImitationConditionPulseDuration;
	static int         imiDbImitationConditionPulseInterval;
	static int         imiDbImitationConditionPulseNumber;
	static std::string imiDbImitationMemo;

	static Mode mode;

public:

	static std::string getDbHost();
	static int         getDbPort();
	static std::string getDbSchema();
	static std::string getDbUser();
	static std::string getDbPass();

	static std::string      getGeneralServiceName();

	static int              getSigAvatarDispInterval();

	static int         getImiAccumInterval();
	static std::string getImiMotionDataFilePath();

	static int         getImiRecId();
	static int         getImiUserId();

	static int         getImiOriginMaxTime();

	static int         getImiImitationGroupId();
	static int         getImiImitationRecType();
	static int         getImiImitationOriginRecId();

	static std::string getImiDbPerceptionNeuronMemo();
	static float       getImiDbImitationConditionPulsePower();
	static float       getImiDbImitationConditionPulseFrequency();
	static int         getImiDbImitationConditionPulseDuration();
	static int         getImiDbImitationConditionPulseInterval();
	static int         getImiDbImitationConditionPulseNumber();
	static std::string getImiDbImitationMemo();

	static Mode getMode();

	static void readConfigFile();
};

#endif //EMBODIED_BRAIN_PARAM_H
