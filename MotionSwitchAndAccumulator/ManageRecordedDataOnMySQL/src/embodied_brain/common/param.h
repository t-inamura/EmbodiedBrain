#ifndef EMBODIED_BRAIN_PARAM_H
#define EMBODIED_BRAIN_PARAM_H

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

/*
 * パラメータクラス
 */
class Param
{
private:
	static const std::string PARAM_FILE_NAME;

	static std::string dbHost;
	static int         dbPort;
	static std::string dbSchema;
	static std::string dbUser;
	static std::string dbPass;

public:

	static std::string getDbHost();
	static int         getDbPort();
	static std::string getDbSchema();
	static std::string getDbUser();
	static std::string getDbPass();

	static void readConfigFile();
};

#endif //EMBODIED_BRAIN_PARAM_H
