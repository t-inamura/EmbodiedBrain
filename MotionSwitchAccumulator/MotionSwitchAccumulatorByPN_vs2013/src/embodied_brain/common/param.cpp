/*
 * パラメータクラス
 */
#include <boost/regex.hpp>
#include <embodied_brain/common/param.h>

std::string Param::getDbHost()   { return dbHost; }
int         Param::getDbPort()   { return dbPort; }
std::string Param::getDbSchema() { return dbSchema; }
std::string Param::getDbUser()   { return dbUser; }
std::string Param::getDbPass()   { return dbPass; }

std::string    Param::getGeneralServiceName()   { return generalServiceName; }
int            Param::getSigAvatarDispInterval(){ return sigAvatarDispInterval; }

int         Param::getSwitchAccumInterval()     { return switchAccumInterval; }
std::string Param::getSwitchMotionDataFilePath(){ return switchMotionDataFilePath; };

int         Param::getSwitchRecId()  { return switchRecId; }
int         Param::getSwitchUserId() { return switchUserId; }

int         Param::getSwitchFakeMaxTime(){ return switchFakeMaxTime; }

int         Param::getSwitchGroupId()  { return switchGroupId; }
int         Param::getSwitchFakeRecId(){ return switchFakeRecId; }

std::string Param::getSwitchDbPerceptionNeuronMemo() { return switchDbPerceptionNeuronMemo; }
std::string Param::getSwitchDbMswRecordingInfoMemo() { return switchDbMswRecordingInfoMemo; }

Param::Mode Param::getMode(){ return mode; }


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
	
	switchRecId   = pt.get<int>  ("switch.rec_id");
	switchUserId  = pt.get<int>  ("switch.user_id");

	switchFakeMaxTime = pt.get<int>  ("switch.fake_max_time");

	switchGroupId     = pt.get<int>  ("switch.group_id");
	switchFakeRecId   = pt.get<int>  ("switch.fake_rec_id");

	switchDbPerceptionNeuronMemo  = pt.get<std::string>("switch.db_perception_neuron_memo");
	switchDbMswRecordingInfoMemo  = pt.get<std::string>("switch.db_msw_recording_info_memo");


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
	}
	else if (switchMode == "Experiment")
	{
		Param::mode = Mode::Experiment;
		std::cout << "[switch]group_id      = " << switchGroupId << std::endl;
		std::cout << "[switch]fake_rec_id   = " << switchFakeRecId << std::endl;
	}
	else
	{
		std::cout << "illegal switch mode! mode=" << switchMode << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "[switch]db_perception_neuron_memo   = " << switchDbPerceptionNeuronMemo << std::endl;
	std::cout << "[switch]db_msw_recording_info_memo  = " << switchDbMswRecordingInfoMemo << std::endl;
}


///*
// * 文字列を区切り、整数リストを作成する
// */
//std::list<int> Param::splitStrIntoIntlist(const std::string &dataStr, const std::string &delimiter)
//{
//	/*
//	 * 文字列を区切り文字で分割し、文字列ベクタを作成
//	 */
//	boost::regex delimiterRegex(delimiter);
//	std::string dataStrTmp = dataStr; //regex_split用に別変数に格納
//
//	std::vector<std::string> dataVec;
//	boost::regex_split(back_inserter(dataVec), dataStrTmp, delimiterRegex);
//
//	/*
//	 * 文字列ベクタを整数リストに変換
//	 */
//	std::list<int> intList;
//
//	std::vector<std::string>::iterator it = dataVec.begin();
//
//	char *endp;
//
//	while (it != dataVec.end())
//	{
//		int intValue = std::strtol((*it).c_str(),&endp,10);
//
//		if(*endp!='\0'){ throw std::string("整数リストに分割できませんでした。元文字列=["+dataStr+"]"); }
//
//		intList.push_back(intValue);
//
//		it++;
//	}
//
//	return intList;
//}
