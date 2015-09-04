/*
 * LinkageControllerOnInit.cpp
 *
 *  Created on: 2015/09/04
 *      Author: Yamada@tome
 */

#include "LinkageController.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>


/*
 * Initialize this controller.
 */
void LinkageController::onInit(InitEvent &evt)
{
	// Read parameter file and initialize.
	this->readParamFileAndInitialize();

	// Initialize Kinect v2 device manager.
	SimObj *myself = getObj(myname());
	this->kinectV2DeviceManager.initPositionAndRotation(myself);

	this->guiService = NULL;

	// Save initial position of Table.
	SimObj *table = getObj(Cmn::tableName.c_str());
	table->getPosition(this->tableIniPos);

	if(this->limbMode==Cmn::limbModes[CmnLMT::HAND])
	{
		// Set variables for Hand mode.
		this->resetVariables4Hand();
	}
	else
	{
		// Set variables for Foot mode.
		this->resetVariables4Foot();
	}

	// Set variables for grasp mode.
	this->resetVariables4GraspMode(this->graspMode);

	this->distanceOfAvatarAndGraspedObject.x(1000.0); // Long enough.
	this->distanceOfAvatarAndGraspedObject.y(1000.0); // Long enough.
	this->distanceOfAvatarAndGraspedObject.z(1000.0); // Long enough.

	this->distanceBetweenBothLinks = 1000.0;
	this->isRightHandClosed = false;
	this->isLeftHandClosed  = false;

	// At the first, the robot does not grasp anything.
	this->isGrasping = false;

	this->usingOculus = false;
}


/*
 * Read parameter file and initialize.
 */
void LinkageController::readParamFileAndInitialize()
{
	std::ifstream ifs(parameterFileName.c_str());

	std::string kinectV2ServiceName;
	std::string kinectV2DeviceType;
	std::string kinectV2DeviceUniqueID;
	double      scaleRatio;
	std::string sensorDataModeStr;

	std::string oculusDK1ServiceName;
	std::string oculusDK1DeviceType;
	std::string oculusDK1DeviceUniqueID;

	std::string linkObjNameList4HandTmp;
	std::string linkObjNameList4FootTmp;

	/*
	 * If parameter file is "not" exists.
	 */
	if (ifs.fail())
	{
		// Set a default parameter.
		std::cout << "Not exist : " << parameterFileName << std::endl;
		std::cout << "Use default parameter." << std::endl;

		kinectV2ServiceName    = SERVICE_NAME_KINECT_V2;
		kinectV2DeviceType     = DEV_TYPE_KINECT_V2;
		kinectV2DeviceUniqueID = DEV_UNIQUE_ID_0;

		sensorDataModeStr = paramFileValKinectV2SensorDataModeDefault;
		scaleRatio        = paramFileValKinectV2ScaleRatioDefault;

		oculusDK1ServiceName    = SERVICE_NAME_OCULUS_DK1;
		oculusDK1DeviceType     = DEV_TYPE_OCULUS_DK1;
		oculusDK1DeviceUniqueID = DEV_UNIQUE_ID_0;

		this->guiServiceName = paramFileValLinkageGraspGUIServiceNameDefault;

		this->limbMode      = paramFileValLinkageGraspLimbModeDefault;
		this->reverseMode   = paramFileValLinkageGraspReverseModeDefault;

		linkObjNameList4HandTmp = paramFileValLinkageGraspLinkObjList4HandDefault;
		linkObjNameList4FootTmp = paramFileValLinkageGraspLinkObjList4FootDefault;
		// split by ",".
		boost::split(this->linkObjNameList4Hand, linkObjNameList4HandTmp, boost::is_any_of(","));
		boost::split(this->linkObjNameList4Foot, linkObjNameList4FootTmp, boost::is_any_of(","));

		this->isWaistFixed    = paramFileValLinkageGraspFixedWaistDefault;
		this->correctionAngle = paramFileValLinkageGraspCorrectionAngleDefault;
	}
	/*
	 * If parameter file is exists.
	 */
	else
	{
		// Get a parameter from parameter file, then set.
		try
		{
			std::cout << "Read " << parameterFileName << std::endl;
			boost::property_tree::ptree pt;
			boost::property_tree::read_ini(parameterFileName, pt);

			kinectV2ServiceName    = pt.get<std::string>(paramFileKeyKinectV2ServiceName);
			kinectV2DeviceType     = pt.get<std::string>(paramFileKeyKinectV2Devicetype);
			kinectV2DeviceUniqueID = pt.get<std::string>(paramFileKeyKinectV2DeviceUniqueID);

			sensorDataModeStr = pt.get<std::string>(paramFileKeyKinectV2SensorDataMode);
			scaleRatio        = pt.get<double>     (paramFileKeyKinectV2ScaleRatio);

			oculusDK1ServiceName    = pt.get<std::string>(paramFileKeyOculusDK1ServiceName);
			oculusDK1DeviceType     = pt.get<std::string>(paramFileKeyOculusDK1Devicetype);
			oculusDK1DeviceUniqueID = pt.get<std::string>(paramFileKeyOculusDK1DeviceUniqueID);

			this->guiServiceName = pt.get<std::string>(paramFileKeyLinkageGraspGUIServiceName);

			this->limbMode      = pt.get<std::string>(paramFileKeyLinkageGraspLimbMode);
			this->graspMode     = pt.get<std::string>(paramFileKeyLinkageGraspGraspMode);
			this->reverseMode   = pt.get<std::string>(paramFileKeyLinkageGraspReverseMode);

			linkObjNameList4HandTmp = pt.get<std::string>(paramFileKeyLinkageGraspLinkObjList4Hand);
			linkObjNameList4FootTmp = pt.get<std::string>(paramFileKeyLinkageGraspLinkObjList4Foot);
			// split by ",".
			boost::split(this->linkObjNameList4Hand, linkObjNameList4HandTmp, boost::is_any_of(","));
			boost::split(this->linkObjNameList4Foot, linkObjNameList4FootTmp, boost::is_any_of(","));

			this->isWaistFixed    = pt.get<bool>  (paramFileKeyLinkageGraspFixedWaist);
			this->correctionAngle = pt.get<double>(paramFileKeyLinkageGraspCorrectionAngle);
		}
		catch (boost::exception &ex)
		{
			std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		}
	}

	/*
	 * Print the initialized parameter.
	 */
	std::cout << paramFileKeyKinectV2ServiceName    << ":" << kinectV2ServiceName    << std::endl;
	std::cout << paramFileKeyKinectV2Devicetype     << ":" << kinectV2DeviceType     << std::endl;
	std::cout << paramFileKeyKinectV2DeviceUniqueID << ":" << kinectV2DeviceUniqueID << std::endl;

	std::cout << paramFileKeyKinectV2SensorDataMode << ":" << sensorDataModeStr << std::endl;
	std::cout << paramFileKeyKinectV2ScaleRatio     << ":" << scaleRatio << std::endl;

	std::cout << paramFileKeyOculusDK1ServiceName    << ":" << oculusDK1ServiceName    << std::endl;
	std::cout << paramFileKeyOculusDK1Devicetype     << ":" << oculusDK1DeviceType     << std::endl;
	std::cout << paramFileKeyOculusDK1DeviceUniqueID << ":" << oculusDK1DeviceUniqueID << std::endl;

	std::cout << paramFileKeyLinkageGraspGUIServiceName << ":" << this->guiServiceName << std::endl;

	std::cout << paramFileKeyLinkageGraspLimbMode      << ":" << this->limbMode     << std::endl;
	std::cout << paramFileKeyLinkageGraspGraspMode     << ":" << this->graspMode    << std::endl;
	std::cout << paramFileKeyLinkageGraspReverseMode   << ":" << this->reverseMode  << std::endl;

	std::cout << paramFileKeyLinkageGraspLinkObjList4Hand  << ":" << linkObjNameList4HandTmp << std::endl;
	std::cout << paramFileKeyLinkageGraspLinkObjList4Foot  << ":" << linkObjNameList4FootTmp << std::endl;

	std::cout << paramFileKeyLinkageGraspFixedWaist       << ":" << this->isWaistFixed    << std::endl;
	std::cout << paramFileKeyLinkageGraspCorrectionAngle  << ":" << this->correctionAngle << std::endl;

	this->kinectV2DeviceManager = KinectV2DeviceManager(kinectV2ServiceName, kinectV2DeviceType, kinectV2DeviceUniqueID, scaleRatio);

	// Set a sensor data mode of Kinect v2.
	KinectV2SensorData::setSensorDataMode(sensorDataModeStr);

	this->oculusDK1DeviceManager = OculusDK1DeviceManager(oculusDK1ServiceName, oculusDK1DeviceType, oculusDK1DeviceUniqueID);
}


/*
 * Reset variables for Hand state in Limb mode.
 */
void LinkageController::resetVariables4Hand()
{
	// Set some variables.
	this->limbMode         = Cmn::limbModes[CmnLMT::HAND];
	this->linkObjNameList  = this->linkObjNameList4Hand;

	this->rightLink = this->rightLink4Hand;
	this->leftLink  = this->leftLink4Hand;

	// Reset position of the table.
	SimObj *table = getObj(Cmn::tableName.c_str());
	table->setPosition(this->tableIniPos);

	// Reset position of the object for HAND.
	for(int i=0; i<this->linkObjNameList4Hand.size(); i++)
	{
		this->sendMsg(this->linkObjNameList4Hand[i], "RESTART:"+this->limbMode);
	}
	// Reset position of the object for FOOT.
	for(int i=0; i<this->linkObjNameList4Foot.size(); i++)
	{
		this->sendMsg(this->linkObjNameList4Foot[i], "RESTART:"+this->limbMode);
	}
}

/*
 * Reset variables for Foot state in Limb mode.
 */
void LinkageController::resetVariables4Foot()
{
	// Set some variables.
	this->limbMode         = Cmn::limbModes[CmnLMT::FOOT];
	this->linkObjNameList  = this->linkObjNameList4Foot;

	this->rightLink = this->rightLink4Foot;
	this->leftLink  = this->leftLink4Foot;

	// Shift position of the table.
	SimObj *table = getObj(Cmn::tableName.c_str());
	table->setPosition(this->tableIniPos.x(), this->tableIniPos.y(), this->tableIniPos.z()+1000.0); // Long enough shift.

	// Reset position of the object for HAND.
	for(int i=0; i<this->linkObjNameList4Hand.size(); i++)
	{
		this->sendMsg(this->linkObjNameList4Hand[i], "RESTART:"+this->limbMode);
	}
	// Reset position of the object for FOOT.
	for(int i=0; i<this->linkObjNameList4Foot.size(); i++)
	{
		this->sendMsg(this->linkObjNameList4Foot[i], "RESTART:"+this->limbMode);
	}
}


/*
 * Reset variables for Grasp mode.
 */
void LinkageController::resetVariables4GraspMode(std::string graspMode)
{
	this->graspMode = graspMode;
	this->isGrasping = false;
	this->distanceBetweenBothLinks = 1000.0;
	this->isRightHandClosed = false;
	this->isLeftHandClosed = false;
}

