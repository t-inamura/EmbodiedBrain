#include <sigverse/tools/LinkageGraspGUI/Param.h>

/*
 * Read parameter from configuration file.
 */
void Param::readConfigFile()
{
	boost::property_tree::ptree pt;
	read_ini(PARAM_FILE_NAME, pt);

	//For SIGVerse
	sigServiceName = pt.get<std::string>("sigverse.service_name");

	//Display parameter list.
	std::cout << "## configuration file ##" << std::endl;
	std::cout << "[sigverse]service_name=" << sigServiceName << std::endl;
}


std::string Param::getSigServiceName()
{
	return sigServiceName;
}

