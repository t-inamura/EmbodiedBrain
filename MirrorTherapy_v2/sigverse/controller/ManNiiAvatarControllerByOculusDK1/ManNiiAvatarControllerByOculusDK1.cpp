/*
 * ManNiiAvatarControllerByOculusDK1.cpp
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */

#include <sigverse/common/device/OculusRiftDK1SensorData.h>
#include <sigverse/controller/ManNiiAvatarControllerByOculusDK1/ManNiiAvatarControllerByOculusDK1.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <cmath>

///@brief Parameter file name.
const std::string ManNiiAvatarControllerByOculusDK1::parameterFileName = "OculusRiftDK1.ini";


///@brief Initialize this controller.
void ManNiiAvatarControllerByOculusDK1::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();
//
//	SimObj *my = getObj(myname());
}


///@brief Movement of the robot.
double ManNiiAvatarControllerByOculusDK1::onAction(ActionEvent &evt)
{
	bool oculusDK1Available = checkService(this->oculusDK1DeviceManager.serviceName);

	if (oculusDK1Available && this->oculusDK1DeviceManager.service == NULL)
	{
		this->oculusDK1DeviceManager.service = connectToService(this->oculusDK1DeviceManager.serviceName);
	}
	else if (!oculusDK1Available && this->oculusDK1DeviceManager.service != NULL)
	{
		this->oculusDK1DeviceManager.service = NULL;
	}

	return 1.0;
}

void ManNiiAvatarControllerByOculusDK1::onRecvMsg(RecvMsgEvent &evt)
{
	const std::string allMsg = evt.getMsg();

	//std::cout << allMsg << std::endl;

	std::map<std::string, std::vector<std::string> > sensorDataMap = OculusRiftDK1SensorData::decodeSensorData(allMsg);

	if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

	if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->oculusDK1DeviceManager.deviceType    ){ return; }
	if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->oculusDK1DeviceManager.deviceUniqueID){ return; }

	OculusRiftDK1SensorData sensorData;
	sensorData.setSensorData(sensorDataMap);

	ManNiiPosture manNiiPosture = this->oculusDK1DeviceManager.convertEulerAngle2ManNiiPosture(sensorData.getEulerAngle());

	SimObj *obj = getObj(myname());
	this->oculusDK1DeviceManager.setJointQuaternions2ManNii(obj, manNiiPosture);
}



///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void ManNiiAvatarControllerByOculusDK1::readIniFileAndInitialize()
{
	std::ifstream ifs(this->parameterFileName.c_str());

	std::string oculusDK1ServiceName;
	std::string oculusDK1DeviceType;
	std::string oculusDK1DeviceUniqueID;

	// Parameter file is "not" exists.
	if (ifs.fail())
	{
		std::cout << "Not exist : " << this->parameterFileName << std::endl;
		std::cout << "Use default parameter." << std::endl;

		oculusDK1ServiceName    = SERVICE_NAME_OCULUS_DK1;
		oculusDK1DeviceType     = DEV_TYPE_OCULUS_DK1;
		oculusDK1DeviceUniqueID = DEV_UNIQUE_ID_0;
	}
	// Parameter file is exists.
	else
	{
		try
		{
			std::cout << "Read " << this->parameterFileName << std::endl;
			boost::property_tree::ptree pt;
			boost::property_tree::read_ini(this->parameterFileName, pt);

			oculusDK1ServiceName    = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME);
			oculusDK1DeviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE);
			oculusDK1DeviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID);
		}
		catch (boost::exception &ex)
		{
			std::cout << this->parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		}
	}

	std::cout << PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME     << ":" << oculusDK1ServiceName    << std::endl;
	std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE      << ":" << oculusDK1DeviceType     << std::endl;
	std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID << ":" << oculusDK1DeviceUniqueID << std::endl;

	this->oculusDK1DeviceManager = OculusDK1DeviceManager(oculusDK1ServiceName, oculusDK1DeviceType, oculusDK1DeviceUniqueID);
}


extern "C" Controller * createController()
{
	return new ManNiiAvatarControllerByOculusDK1;
}


