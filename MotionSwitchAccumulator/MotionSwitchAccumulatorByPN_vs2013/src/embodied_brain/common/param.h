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

	enum SmoothingType
	{
		None,
		SubtractLastPosture,
		SubtractLastPostureWithRate,
	};

	enum FakeMotionsSelectionMethod
	{
		Random,
		Sequentially,
	};


	//パラメータファイル名
	static const std::string PARAM_FILE_NAME;

private:
	static std::string dbHost;
	static int         dbPort;
	static std::string dbSchema;
	static std::string dbUser;
	static std::string dbPass;

	static std::string generalServiceName;
	static int         sigAvatarDispInterval;

	static int            switchAccumInterval;
	static std::string    switchMotionDataFilePath;

	static int            switchRecId;
	static int            switchUserId;

	static int            switchFakeMaxTime;

	static int            switchNumberOfIterations;

	static std::list<int> switchFakeRecIdList;

	static std::string    switchDbPerceptionNeuronMemo;
	static std::string    switchDbMswRecordingInfoMemo;

	static int            switchFramesNumberForDelay;

	static bool           switchInvertFlg;
	static bool           switchInvertFakeFlg;

	static Mode mode;
	static SmoothingType smoothingType;
	static FakeMotionsSelectionMethod fakeMotionSelectionMethod;

public:

	static std::string getDbHost();
	static int         getDbPort();
	static std::string getDbSchema();
	static std::string getDbUser();
	static std::string getDbPass();

	static std::string getGeneralServiceName();

	static int         getSigAvatarDispInterval();

	static int             getSwitchAccumInterval();
	static std::string     getSwitchMotionDataFilePath();

	static int             getSwitchRecId();
	static int             getSwitchUserId();

	static int             getSwitchFakeMaxTime();

	static int             getSwitchNumberOfIterations();

	static std::list<int>  getSwitchFakeRecIdList();

	static std::string     getSwitchDbPerceptionNeuronMemo();
	static std::string     getSwitchDbMswRecordingInfoMemo();

	static int             getSwitchFramesNumberForDelay();

	static bool            getSwitchInvertFlg();
	static bool            getSwitchInvertFakeFlg();

	static Mode  getMode();
	static SmoothingType  getSmoothingType();
	static FakeMotionsSelectionMethod  getFakeMotionsSelectionMethod();

	static void readConfigFile();


	static std::list<int> splitStrIntoIntList(const std::string &dataStr, const std::string &delimiter);
};

#endif //EMBODIED_BRAIN_PARAM_H
