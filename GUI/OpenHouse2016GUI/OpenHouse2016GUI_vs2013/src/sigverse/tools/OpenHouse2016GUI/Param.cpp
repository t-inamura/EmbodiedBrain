/*
 * パラメータクラス
 */
#include <sigverse/tools/OpenHouse2016GUI/Param.h>

std::string Param::getSigServiceName() { return sigServiceName; }

/*
 * コンフィグファイルからパラメータを取得する
 */
void Param::readConfigFile()
{
	boost::property_tree::ptree pt;
	read_ini(PARAM_FILE_NAME, pt);

	//SIGVerse関連
	sigServiceName = pt.get<std::string>("sigverse.service_name");


	//表示
	std::cout << "■ コンフィグ内容 ■" << std::endl;
	std::cout << "[sigverse]service_name=" << sigServiceName << std::endl;
}
