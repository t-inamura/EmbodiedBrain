#ifndef SIGVERSE_LINKAGE_GRASP_GUI_PARAM_H
#define SIGVERSE_LINKAGE_GRASP_GUI_PARAM_H

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

/*
 * Manage parameter file.
 */
class Param
{
private:
	static const std::string PARAM_FILE_NAME;

	static std::string sigServiceName;

public:

	static std::string getSigServiceName();

	static void readConfigFile();
};

#endif // SIGVERSE_LINKAGE_GRASP_GUI_PARAM_H
