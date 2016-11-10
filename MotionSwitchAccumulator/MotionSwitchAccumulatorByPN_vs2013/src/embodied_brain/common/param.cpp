/*
 * パラメータクラス
 */
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <embodied_brain/common/param.h>

std::string Param::getDbHost()   { return dbHost; }
int         Param::getDbPort()   { return dbPort; }
std::string Param::getDbSchema() { return dbSchema; }
std::string Param::getDbUser()   { return dbUser; }
std::string Param::getDbPass()   { return dbPass; }

std::string    Param::getGeneralServiceName()   { return generalServiceName; }
int            Param::getSigAvatarDispInterval(){ return sigAvatarDispInterval; }

int             Param::getSwitchAccumInterval()     { return switchAccumInterval; }
std::string     Param::getSwitchMotionDataFilePath(){ return switchMotionDataFilePath; };

int             Param::getSwitchRecId()  { return switchRecId; }
int             Param::getSwitchUserId() { return switchUserId; }

int             Param::getSwitchFakeMaxTime(){ return switchFakeMaxTime; }

int             Param::getSwitchNumberOfIterations(){ return switchNumberOfIterations; }
//std::list<int>  Param::getSwitchFakeRecIdList(){ return switchFakeRecIdList; }

std::string     Param::getSwitchDbPerceptionNeuronMemo() { return switchDbPerceptionNeuronMemo; }
std::string     Param::getSwitchDbMswRecordingInfoMemo() { return switchDbMswRecordingInfoMemo; }

int             Param::getSwitchFramesNumberForDelay(){ return switchFramesNumberForDelay; }

bool            Param::getSwitchInvertFlg(){ return switchInvertFlg; }
bool            Param::getSwitchInvertFakeFlg(){ return switchInvertFakeFlg; }

Param::Mode Param::getMode(){ return mode; }
Param::SmoothingType Param::getSmoothingType() { return smoothingType; }
Param::FakeMotionsSelectionMethod Param::getFakeMotionsSelectionMethod() { return fakeMotionSelectionMethod; }




/*
 * コンフィグファイルからパラメータを取得する
 */
void Param::readConfigFile()
{
	boost::property_tree::ptree pt;
	read_ini(Param::PARAM_FILE_NAME, pt);

	//データベース接続関連
	dbHost   = pt.get<std::string>("db.host");
	dbPort   = pt.get<int>        ("db.port");
	dbSchema = pt.get<std::string>("db.schema");
	dbUser   = pt.get<std::string>("db.user");
	dbPass   = pt.get<std::string>("db.pass");

	//SIGService名
	generalServiceName = pt.get<std::string>("General.service_name");
	//アバター表示更新間隔
	sigAvatarDispInterval = pt.get<int> ("sigverse.avatar_disp_interval");

	switchAccumInterval           = pt.get<int>  ("switch.accum_interval");
	//動作データファイルパス(動作データをファイル入力する場合のみ指定)
	switchMotionDataFilePath = pt.get<std::string>("switch.motion_data_file_path");

	std::string switchMode  = pt.get<std::string>("switch.mode");
	
	std::string switchRecIdStr = pt.get<std::string>("switch.rec_id");
	if (switchRecIdStr == ""){ switchRecIdStr = "0"; }
	switchRecId = std::atoi(switchRecIdStr.c_str());

	std::string switchUserIdStr = pt.get<std::string>("switch.user_id");
	if (switchUserIdStr == ""){ switchUserIdStr = "0"; }
	switchUserId = std::atoi(switchUserIdStr.c_str());

	switchFakeMaxTime = pt.get<int>  ("switch.fake_max_time");

	switchNumberOfIterations = pt.get<int>("switch.number_of_iterations");

	std::string switchFakeMotionsSelectionMethodStr = pt.get<std::string>("switch.fake_motions_selection_method");

	if (switchFakeMotionsSelectionMethodStr == "Random")
	{
		Param::fakeMotionSelectionMethod = FakeMotionsSelectionMethod::Random;
	}
	else if (switchFakeMotionsSelectionMethodStr == "Sequentially")
	{
		Param::fakeMotionSelectionMethod = FakeMotionsSelectionMethod::Sequentially;
	}
	else
	{
		std::cout << "illegal fake_motions_selection_method! :" << switchFakeMotionsSelectionMethodStr << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string switchFakeRecIdListStr = pt.get<std::string>("switch.fake_rec_id_list");
	Param::switchFakeRecIdList = Param::splitStrIntoIntList(switchFakeRecIdListStr, ",");


	switchDbPerceptionNeuronMemo  = pt.get<std::string>("switch.db_perception_neuron_memo");
	switchDbMswRecordingInfoMemo  = pt.get<std::string>("switch.db_msw_recording_info_memo");

	std::string switchSmoothingType = pt.get<std::string>("switch.smoothing_type");

	switchFramesNumberForDelay =  pt.get<int> ("switch.frames_number_for_delay");

	switchInvertFlg     = pt.get<bool> ("switch.invert_flg");
	switchInvertFakeFlg = pt.get<bool> ("switch.invert_fake_flg");

	//表示
	std::cout << "◆ コンフィグ内容 ◆" << std::endl;
	std::cout << "[db]host            = " << dbHost << std::endl;
	std::cout << "[db]port            = " << dbPort << std::endl;
	std::cout << "[db]schema          = " << dbSchema << std::endl;
	std::cout << "[db]user            = " << dbUser << std::endl;

	std::cout << "[General]service_name          = " << generalServiceName << std::endl;

	std::cout << "[sigverse]avatar_disp_interval = " << sigAvatarDispInterval << std::endl;

	std::cout << "[switch]accum_interval         = " << switchAccumInterval << std::endl;
	std::cout << "[switch]motion_data_file_path  = " << switchMotionDataFilePath << std::endl;

	std::cout << "[switch]mode                   = " << switchMode << std::endl;
	std::cout << "[switch]rec_id                 = " << switchRecId << std::endl;
	std::cout << "[switch]user_id                = " << switchUserId << std::endl;

	if (switchMode=="RecFake")
	{
		Param::mode = Mode::RecFake;
		std::cout << "[switch]fake_max_time = " << switchFakeMaxTime << std::endl;

		switchNumberOfIterations = 1;
	}
	else if (switchMode == "Experiment")
	{
		Param::mode = Mode::Experiment;
		std::cout << "[switch]fake_motions_selection_method = " << switchFakeMotionsSelectionMethodStr << std::endl;
		std::cout << "[switch]fake_rec_id_list = " << switchFakeRecIdListStr << std::endl;
	}
	else
	{
		std::cout << "illegal switch mode! mode=" << switchMode << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "[switch]number_of_iterations   = " << switchNumberOfIterations << std::endl;

	std::cout << "[switch]db_perception_neuron_memo   = " << switchDbPerceptionNeuronMemo << std::endl;
	std::cout << "[switch]db_msw_recording_info_memo  = " << switchDbMswRecordingInfoMemo << std::endl;
	std::cout << "[switch]smoothing_type  = " << switchSmoothingType << std::endl;

	if (switchSmoothingType == "None")
	{
		Param::smoothingType = SmoothingType::None;
	}
	else if (switchSmoothingType == "SubtractLastPosture")
	{
		Param::smoothingType = SmoothingType::SubtractLastPosture;
	}
	else if (switchSmoothingType == "SubtractLastPostureWithRate")
	{
		Param::smoothingType = SmoothingType::SubtractLastPostureWithRate;
	}
	else
	{
		std::cout << "illegal smoothing type! type=" << switchSmoothingType << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "[switch]frames_number_for_delay  = " << switchFramesNumberForDelay << std::endl;

	std::cout << "[switch]invert_flg      = " << switchInvertFlg << std::endl;
	std::cout << "[switch]invert_fake_flg = " << switchInvertFakeFlg << std::endl;
}


/*
 * 文字列を区切り、整数リストを作成する
 */
std::list<int> Param::splitStrIntoIntList(const std::string &dataStr, const std::string &delimiter)
{
	/*
	 * 文字列を区切り文字で分割し、文字列ベクタを作成
	 */
	boost::regex delimiterRegex(delimiter);
	std::string dataStrTmp = dataStr; //regex_split用に別変数に格納

	boost::algorithm::trim(dataStrTmp);
	std::vector<std::string> dataVec;
	boost::regex_split(back_inserter(dataVec), dataStrTmp, delimiterRegex);

	/*
	 * 文字列ベクタを整数リストに変換
	 */
	std::vector<std::string>::iterator it = dataVec.begin();

	std::list<int> intList;
	char *endp;

//	std::cout << "[DebugLog]文字列分割：";

	while (it != dataVec.end())
	{
		int intValue = std::strtol((*it).c_str(),&endp,10);

		if(*endp!='\0'){ throw std::string("整数リストに分割できませんでした。元文字列=["+dataStr+"]"); }

		intList.push_back(intValue);

//		std::cout << "[" << intValue << "]";

		it++;
	}

//	std::cout << std::endl;

	return intList;
}


