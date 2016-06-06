/*
 * パラメータクラス
 */
#include <embodied_brain/common/param.h>

std::string Param::getDbHost()  { return dbHost; }
int         Param::getDbPort()  { return dbPort; }
std::string Param::getDbSchema(){ return dbSchema; }
std::string Param::getDbUser()  { return dbUser; }
std::string Param::getDbPass()  { return dbPass; }


/*
 * コンフィグファイルからパラメータを取得する
 */
void Param::readConfigFile()
{
	boost::property_tree::ptree pt;
	read_ini(PARAM_FILE_NAME, pt);

	dbHost   = pt.get<std::string>("db.host");
	dbPort   = pt.get<int>        ("db.port");
	dbSchema = pt.get<std::string>("db.schema");
	dbUser   = pt.get<std::string>("db.user");
	dbPass   = pt.get<std::string>("db.pass");

	//表示
	std::cout << "■ コンフィグ内容 ■" << std::endl;
	std::cout << "[db]host   =" << dbHost << std::endl;
	std::cout << "[db]port   =" << dbPort << std::endl;
	std::cout << "[db]schema =" << dbSchema << std::endl;
	std::cout << "[db]user   =" << dbUser << std::endl;
}
