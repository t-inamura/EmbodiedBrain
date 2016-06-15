/*
 * HeadController.cpp
 *
 *  Created on: 2016/04/06
 *      Author: Yamada
 */
#include "HeadController.h"

#include <sys/time.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <math.h>
#include <sigverse/plugin/common/sensor/SensorData.h>


///@brief Initialize this controller.
void HeadController::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();

	//SimObj *myself =
	getObj(myname());

	//Set default avatar
	this->bodyAvatarName = this->mirrorTherapyManName;

	this->guiService = NULL;
}

///@brief Movement of the robot.
double HeadController::onAction(ActionEvent &evt)
{
	static int serviceCheckCnt = 0;
	serviceCheckCnt++;

	/*
	 * Move the head to the main body.
	 */
	Vector3d position;
	dReal quaternion[4];
	SimObj *bodyAvatarObj = getObj(HeadController::bodyAvatarName.c_str());
	bodyAvatarObj->getPartsPosition(position, "HEAD_LINK");
	bodyAvatarObj->getPartsQuaternion(quaternion[0], quaternion[1], quaternion[2], quaternion[3], "HEAD_LINK");

	SimObj *myself = getObj(myname());
	myself->setPosition(position);
	myself->setEntityQuaternion(quaternion, true);


	/*
	 * Check Service.
	 */
	if(serviceCheckCnt==100)
	{
		bool kinectV2Available = checkService(this->kinectV2DeviceManager.serviceName);

		if (kinectV2Available && this->kinectV2DeviceManager.service == NULL)
		{
			this->kinectV2DeviceManager.service = connectToService(this->kinectV2DeviceManager.serviceName);
		}
		else if (!kinectV2Available && this->kinectV2DeviceManager.service != NULL)
		{
			this->kinectV2DeviceManager.service = NULL;
		}

		bool oculusDK2Available = checkService(this->oculusDK2DeviceManager.serviceName);

		if (oculusDK2Available && this->oculusDK2DeviceManager.service == NULL)
		{
			this->oculusDK2DeviceManager.service = connectToService(this->oculusDK2DeviceManager.serviceName);
		}
		else if (!oculusDK2Available && this->oculusDK2DeviceManager.service != NULL)
		{
			this->oculusDK2DeviceManager.service = NULL;
		}

		bool guiAvailable = checkService(this->guiServiceName);

		if (guiAvailable && this->guiService == NULL)
		{
			this->guiService = connectToService(this->guiServiceName);
		}
		else if (!guiAvailable && this->guiService != NULL)
		{
			this->guiService = NULL;
		}

		serviceCheckCnt = 0;
	}

	return 0.01;
}


///@brief Message from device.
void HeadController::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		// Receive message.
		const std::string allMsg = evt.getMsg();

//		std::cout << "msg=" << allMsg << std::endl;

		// Get device type from message.
		std::map<std::string, std::vector<std::string> > sensorDataMap = SensorData::decodeSensorData(allMsg);

		/*
		 * Device
		 */
		if (sensorDataMap.find(MSG_KEY_DEV_TYPE) != sensorDataMap.end())
		{
			sendMsg(this->bodyAvatarName, allMsg);
		}
		// Not a device.(Message from Change Avatar GUI)
		else
		{
			this->changeAvatar(sensorDataMap);

			sendMsg(this->mirrorTherapyManName,       allMsg);
			sendMsg(this->mirrorArmLongManName,       allMsg);
			sendMsg(this->mirrorArmShortManName,      allMsg);
			sendMsg(this->mirrorRightArmRobotManName, allMsg);
			sendMsg(this->linkageManName,             allMsg);
			sendMsg(this->sosArmMiddleManName,        allMsg);
			sendMsg(this->sosArmLongManName,          allMsg);
			sendMsg(this->sosArmShortManName,         allMsg);
		}
	}
	catch(...)
	{
		LOG_MSG(("SIGViewer message is invalid."));
	}
}




void HeadController::changeAvatar(const std::map<std::string, std::vector<std::string> > &map)
{
	std::string roomName;

	if (map.find(msgKeyRoom) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyRoom);
		roomName = it->second[0];
	}
	else
	{
		return;
	}

	if (map.find(msgKeyAvatar) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyAvatar);
		this->bodyAvatarName = it->second[0];
		std::cout << "Change Avatar:" << this->bodyAvatarName << std::endl;

		SimObj *myself = getObj(HeadController::audienceName.c_str());
		Vector3d pos;
		myself->getPosition(pos);

		if(roomName==msgValueRoomMirrorTherapy)
		{
			pos.x(0.0);
			myself->setPosition(pos);
		}
		else if(roomName==msgValueRoomLinkage)
		{
			pos.x(2000.0);
			myself->setPosition(pos);
		}
		else if(roomName==msgValueRoomSenseOfSelf)
		{
			pos.x(4000.0);
			myself->setPosition(pos);
		}
	}
}

///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void HeadController::readIniFileAndInitialize()
{
	try
	{
		std::ifstream ifs(parameterFileName.c_str());

		std::string kinectV2ServiceName;
		std::string kinectV2DeviceType;
		std::string kinectV2DeviceUniqueID;
		double      scaleRatio;
		std::string sensorDataModeStr;

		std::string oculusDK2ServiceName;
		std::string oculusDK2DeviceType;
		std::string oculusDK2DeviceUniqueID;

		// Parameter file is "not" exists.
		if (ifs.fail())
		{
			std::cout << "Not exist : " << parameterFileName << std::endl;
			exit(-1);
		}

		// Parameter file is exists.
		std::cout << "Read " << parameterFileName << std::endl;
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(parameterFileName, pt);

		kinectV2ServiceName    = pt.get<std::string>(paramFileKeyKinectV2ServiceName);
		kinectV2DeviceType     = pt.get<std::string>(paramFileKeyKinectV2Devicetype);
		kinectV2DeviceUniqueID = pt.get<std::string>(paramFileKeyKinectV2DeviceUniqueID);

		sensorDataModeStr = pt.get<std::string>(paramFileKeyKinectV2SensorDataMode);
		scaleRatio        = pt.get<double>     (paramFileKeyKinectV2ScaleRatio);
		this->correctionAngle= pt.get<double>  (paramFileKeyKinectV2CorrectionAngle);

		oculusDK2ServiceName    = pt.get<std::string>(paramFileKeyOculusDK2ServiceName);
		oculusDK2DeviceType     = pt.get<std::string>(paramFileKeyOculusDK2Devicetype);
		oculusDK2DeviceUniqueID = pt.get<std::string>(paramFileKeyOculusDK2DeviceUniqueID);

		this->guiServiceName = pt.get<std::string>(paramFileKeyChangeAvatarGUIServiceName);

		std::cout << paramFileKeyKinectV2ServiceName    << ":" << kinectV2ServiceName    << std::endl;
		std::cout << paramFileKeyKinectV2Devicetype     << ":" << kinectV2DeviceType     << std::endl;
		std::cout << paramFileKeyKinectV2DeviceUniqueID << ":" << kinectV2DeviceUniqueID << std::endl;

		std::cout << paramFileKeyKinectV2SensorDataMode << ":" << sensorDataModeStr << std::endl;
		std::cout << paramFileKeyKinectV2ScaleRatio     << ":" << scaleRatio << std::endl;

		std::cout << paramFileKeyOculusDK2ServiceName    << ":" << oculusDK2ServiceName    << std::endl;
		std::cout << paramFileKeyOculusDK2Devicetype     << ":" << oculusDK2DeviceType     << std::endl;
		std::cout << paramFileKeyOculusDK2DeviceUniqueID << ":" << oculusDK2DeviceUniqueID << std::endl;

		std::cout << paramFileKeyChangeAvatarGUIServiceName << ":" << this->guiServiceName << std::endl;


		this->kinectV2DeviceManager = KinectV2DeviceManager(kinectV2ServiceName, kinectV2DeviceType, kinectV2DeviceUniqueID, scaleRatio);

		// Set sensor data mode.
		KinectV2SensorData::setSensorDataMode(sensorDataModeStr);

		this->oculusDK2DeviceManager = OculusDK2DeviceManager(oculusDK2ServiceName, oculusDK2DeviceType, oculusDK2DeviceUniqueID);
	}
	catch (boost::exception &ex)
	{
		std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
	}
}



extern "C" Controller * createController()
{
	return new HeadController;
}

