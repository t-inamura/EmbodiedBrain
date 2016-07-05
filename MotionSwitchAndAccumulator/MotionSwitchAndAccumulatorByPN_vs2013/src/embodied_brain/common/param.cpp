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

std::string    Param::getGeneralServiceName()       { return generalServiceName; }
int            Param::getSigAvatarDispInterval(){ return sigAvatarDispInterval; }
//double         Param::getSigAvatarMoveSpeed()   { return sigAvatarMoveSpeed; }

int         Param::getImiAccumInterval()     { return switchAccumInterval; }
std::string Param::getImiMotionDataFilePath(){ return switchMotionDataFilePath; };

int         Param::getImiRecId()  { return switchRecId; }
int         Param::getImiUserId() { return switchUserId; }

int         Param::getImiOriginMaxTime(){ return switchFakeMaxTime; }

int         Param::getImiImitationGroupId()    { return switchGroupId; }
int         Param::getImiImitationRecType()    { return imiImitationRecType; }
int         Param::getImiImitationOriginRecId(){ return switchFakeRecId; }

std::string Param::getImiDbPerceptionNeuronMemo()            { return switchDbPerceptionNeuronMemo; }
float       Param::getImiDbImitationConditionPulsePower()    { return imiDbImitationConditionPulsePower; }
float       Param::getImiDbImitationConditionPulseFrequency(){ return imiDbImitationConditionPulseFrequency; }
int         Param::getImiDbImitationConditionPulseDuration() { return imiDbImitationConditionPulseDuration; }
int         Param::getImiDbImitationConditionPulseInterval() { return imiDbImitationConditionPulseInterval; }
int         Param::getImiDbImitationConditionPulseNumber()   { return imiDbImitationConditionPulseNumber; }
std::string Param::getImiDbImitationMemo()                   { return switchDbMswRecordingInfoMemo; }

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
	////アバター移動速度 (1.0で通常速度)
	//sigAvatarMoveSpeed = pt.get<double> ("sigverse.avatar_move_speed");

	switchAccumInterval           = pt.get<int>  ("imitation.accum_interval");
	//動作データファイルパス(動作データをファイル入力する場合のみ指定)
	switchMotionDataFilePath = pt.get<std::string>("imitation.motion_data_file_path");

	std::string imiMode  = pt.get<std::string>("imitation.mode");
	
	switchRecId   = pt.get<int>  ("imitation.rec_id");
	switchUserId  = pt.get<int>  ("imitation.user_id");

	switchFakeMaxTime = pt.get<int>  ("imitation.origin_max_time");

	switchGroupId                   = pt.get<int>  ("imitation.imitation_group_id");
	imiImitationRecType                   = pt.get<int>  ("imitation.imitation_rec_type");
	switchFakeRecId               = pt.get<int>  ("imitation.imitation_origin_rec_id");

	switchDbPerceptionNeuronMemo             = pt.get<std::string>("imitation.db_perception_neuron_memo");
	imiDbImitationConditionPulsePower     = pt.get<float>      ("imitation.db_imitation_condition_pulse_power");
	imiDbImitationConditionPulseFrequency = pt.get<float>      ("imitation.db_imitation_condition_pulse_frequency");
	imiDbImitationConditionPulseDuration  = pt.get<int>        ("imitation.db_imitation_condition_pulse_duration");
	imiDbImitationConditionPulseInterval  = pt.get<int>        ("imitation.db_imitation_condition_pulse_interval");
	imiDbImitationConditionPulseNumber    = pt.get<int>        ("imitation.db_imitation_condition_pulse_number");
	switchDbMswRecordingInfoMemo                    = pt.get<std::string>("imitation.db_imitation_memo");


	//表示
	std::cout << "◆ コンフィグ内容 ◆" << std::endl;
	std::cout << "[db]host            = " << dbHost << std::endl;
	std::cout << "[db]port            = " << dbPort << std::endl;
	std::cout << "[db]schema          = " << dbSchema << std::endl;
	std::cout << "[db]user            = " << dbUser << std::endl;

	std::cout << "[General]service_name         = "         << generalServiceName << std::endl;

	std::cout << "[sigverse]avatar_disp_interval = " << sigAvatarDispInterval << std::endl;

	std::cout << "[imitation]accum_interval            = " << switchAccumInterval << std::endl;
	std::cout << "[imitation]motion_data_file_path     = " << switchMotionDataFilePath << std::endl;

	std::cout << "[imitation]mode                      = " << imiMode << std::endl;
	std::cout << "[imitation]rec_id                    = " << switchRecId << std::endl;
	std::cout << "[imitation]user_id                   = " << switchUserId << std::endl;

	if (imiMode=="origin")
	{
		Param::mode = Mode::RecFake;
		std::cout << "[imitation]origin_max_time           = " << switchFakeMaxTime << std::endl;
	}
	else if (imiMode == "imitation")
	{
		Param::mode = Mode::Experiment;
		std::cout << "[imitation]imitation_group_id        = " << switchGroupId << std::endl;
		std::cout << "[imitation]imitation_rec_type        = " << imiImitationRecType << std::endl;
		std::cout << "[imitation]imitation_origin_rec_id   = " << switchFakeRecId << std::endl;
	}
	else
	{
		std::cout << "illegal imitation mode! mode=" << imiMode << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "[imitation]db_perception_neuron_memo              = " << switchDbPerceptionNeuronMemo << std::endl;
	std::cout << "[imitation]db_imitation_condition_pulse_power     = " << imiDbImitationConditionPulsePower << std::endl;
	std::cout << "[imitation]db_imitation_condition_pulse_frequency = " << imiDbImitationConditionPulseFrequency << std::endl;
	std::cout << "[imitation]db_imitation_condition_pulse_duration  = " << imiDbImitationConditionPulseDuration << std::endl;
	std::cout << "[imitation]db_imitation_condition_pulse_interval  = " << imiDbImitationConditionPulseInterval << std::endl;
	std::cout << "[imitation]db_imitation_condition_pulse_number    = " << imiDbImitationConditionPulseNumber << std::endl;
	std::cout << "[imitation]db_imitation_memo                      = " << switchDbMswRecordingInfoMemo << std::endl;
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
