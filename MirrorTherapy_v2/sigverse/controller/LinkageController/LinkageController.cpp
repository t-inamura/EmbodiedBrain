/*
 * LinkageController.cpp
 *
 *  Created on: 2015/08/25
 *      Author: Yamada@tome
 */

#include <sigverse/common/device/SensorData.h>
#include <sigverse/controller/LinkageController/LinkageController.h>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <cmath>
#include <ctime>
#include <algorithm>


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

	// Save initial position of Table.
	SimObj *table = getObj(this->tableName.c_str());
	table->getPosition(this->tableIniPos);

	if(this->limbMode==this->limbModes[HAND])
	{
		// Set variables for Hand mode.
		this->resetVariables4Hand();
	}
	else
	{
		// Set variables for Foot mode.
		this->resetVariables4Foot();
	}

	// At the first, the robot does not grasp anything.
	this->isGrasping = false;

	this->usingOculus = false;

	this->guiService = NULL;

	this->myGraspingPartName = "";

	this->distanceOfAvatarAndGraspedObject.x(1000.0); // Long enough.
	this->distanceOfAvatarAndGraspedObject.y(1000.0); // Long enough.
	this->distanceOfAvatarAndGraspedObject.z(1000.0); // Long enough.
}


/*
 * Movement of the avatar.
 */
double LinkageController::onAction(ActionEvent &evt)
{
	const double interval = 0.1; // 0.1[s]

	try
	{
		SimObj *myself = getObj(myname());
		SimObj *target = getObj(this->linkageObjName.c_str());

		if (!myself || !target){ return interval; }
		if (myself->dynamics() || target->dynamics()){ return interval; }

		Vector3d targetPos;
		target->getPosition(targetPos);

		// When the avatar is grasping object.
		if (this->isGrasping)
		{
			/*
			 * Calculate position of grasped object. And set position of grasped object.
			 */
			CParts *myParts  = myself->getParts(this->myGraspingPartName.c_str());

			Vector3d myPartsPos;
			myParts->getPosition(myPartsPos);

			/*
			 * Calculate distance of avatar and grasped object.
			 */
			this->distanceOfAvatarAndGraspedObject.x(targetPos.x()-myPartsPos.x());
			this->distanceOfAvatarAndGraspedObject.y(targetPos.y()-myPartsPos.y());
			this->distanceOfAvatarAndGraspedObject.z(targetPos.z()-myPartsPos.z());

			/*
			 * Release object if distance is long.
			 */
			if (this->distanceOfAvatarAndGraspedObject.length() > this->distance4Releasejudge)
			{
				this->isGrasping = false;
				myParts->releaseObj();

				LOG_MSG(("Release %s.", linkageObjName.c_str()));
			}

			Vector3d newTargetPos = myPartsPos + distanceOfAvatarAndGraspedObject;

			std::stringstream msgStr;
			msgStr << "SET_POSITION:" << newTargetPos.x() << ":" << newTargetPos.y() << ":" << newTargetPos.z();
			this->sendMsg(this->linkageObjName, msgStr.str());
		}
	}
	catch(SimObj::NoAttributeException &err)
	{
		LOG_MSG(("NoAttributeException: %s", err.msg()));
	}
	catch(SimObj::AttributeReadOnlyException &err)
	{
		LOG_MSG(("AttributeReadOnlyException: %s", err.msg()));
	}
	catch(SimObj::Exception &err)
	{
		LOG_MSG(("Exception: %s", err.msg()));
	}

	return checkServiceForOnAction(interval);
}


/*
 * Check service provider in onAction method.
 *
 * Return a new interval of onAction.
 */
double LinkageController::checkServiceForOnAction(const double intervalOfOnAction)
{
	try
	{
		static time_t startTime = 0;
		time_t now;

		// Only the first time.
		if(startTime==0)
		{
			std::time( &startTime );

			if(this->intervalOfCheckingService < intervalOfOnAction)
			{
				LOG_MSG(("A checking service interval is shorter than onAction interval. So repeat onAction using a checking service interval."));
			}
		}

		std::time( &now );

		// Get elapsed time from last checking.
		double elapsedTime = std::difftime( now, startTime );

		// If enough time has elapsed, check service provider.
		if(elapsedTime > this->intervalOfCheckingService)
		{
			// Reset startTime;
			startTime = now;

			// Check Kinect v2 service provider.
			bool kinectV2Available = checkService(this->kinectV2DeviceManager.serviceName);

			if (kinectV2Available && this->kinectV2DeviceManager.service == NULL)
			{
				this->kinectV2DeviceManager.service = connectToService(this->kinectV2DeviceManager.serviceName);
			}
			else if (!kinectV2Available && this->kinectV2DeviceManager.service != NULL)
			{
				this->kinectV2DeviceManager.service = NULL;
			}

			// Check Oculus Rift DK1 service provider.
			bool oculusDK1Available = checkService(this->oculusDK1DeviceManager.serviceName);

			if (oculusDK1Available && this->oculusDK1DeviceManager.service == NULL)
			{
				this->oculusDK1DeviceManager.service = connectToService(this->oculusDK1DeviceManager.serviceName);
			}
			else if (!oculusDK1Available && this->oculusDK1DeviceManager.service != NULL)
			{
				this->oculusDK1DeviceManager.service = NULL;
			}

			// Check GUI service provider.
			bool guiAvailable = checkService(this->guiServiceName);

			if (guiAvailable && this->guiService == NULL)
			{
				this->guiService = connectToService(this->guiServiceName);
			}
			else if (!guiAvailable && this->guiService != NULL)
			{
				this->guiService = NULL;
			}
		}
	}
	catch(...)
	{
		LOG_MSG(("Exception occurred in checkServiceForThread."));
	}

	// Change interval of onAction in interval of checking service.
	if(this->intervalOfCheckingService < intervalOfOnAction)
	{
		return this->intervalOfCheckingService;
	}

	return intervalOfOnAction;
}

/*
 * Receive message.
 */
void LinkageController::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		const std::string allMsg = evt.getMsg();

//		std::cout << "msg=" << allMsg << std::endl;

		// Get device type from message.
		std::map<std::string, std::vector<std::string> > sensorDataMap = SensorData::decodeSensorData(allMsg);

		/*
		 * Device
		 */
		if (sensorDataMap.find(MSG_KEY_DEV_TYPE) != sensorDataMap.end())
		{
			std::string deviceTypeValue = sensorDataMap[MSG_KEY_DEV_TYPE][0];
			std::string deviceUniqueId  = sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0];

			/*
			 * Kinect V2
			 */
			if (deviceTypeValue == this->kinectV2DeviceManager.deviceType && deviceUniqueId == this->kinectV2DeviceManager.deviceUniqueID)
			{
				// Decode message to sensor data of kinect v2.
				KinectV2SensorData sensorData;
				sensorData.setSensorData(sensorDataMap);

				// Convert kinect v2 quaternions(orientations) to man-nii posture(sigverse quaternion format).
				ManNiiPosture posture = KinectV2DeviceManager::convertSensorData2ManNiiPosture(sensorData);

				// When using oculus, set invalid head quaternion. (When all values are zero, it means invalid.)
				if (this->usingOculus)
				{
					posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::HEAD_JOINT1].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
				}

				// Invalidate body rotation. (When all values are zero, it means invalid.)
				if(this->isWaistFixed)
				{
					posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::WAIST_JOINT1].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
				}

				// Invert posture.
				this->makeInvertPosture(posture);

				// Set SIGVerse quaternions and positions.
				SimObj *obj = getObj(myname());

				//this->kinectV2DeviceManager.setRootPosition(obj, sensorData.rootPosition); // Don't move.

				// Set joint quaternions to avatar.
				KinectV2DeviceManager::setJointQuaternions2ManNii(obj, posture, sensorData);
			}
			/*
			 * Oculus Rift DK1
			 */
			else if (deviceTypeValue == this->oculusDK1DeviceManager.deviceType && deviceUniqueId ==  this->oculusDK1DeviceManager.deviceUniqueID)
			{
				if(!this->usingOculus){ this->usingOculus = true; }

				OculusRiftDK1SensorData sensorData;
				sensorData.setSensorData(sensorDataMap);

				ManNiiPosture posture = OculusDK1DeviceManager::convertEulerAngle2ManNiiPosture(sensorData.getEulerAngle());

				SimObj *obj = getObj(myname());
				OculusDK1DeviceManager::setJointQuaternions2ManNii(obj, posture);
			}
		}
		// Not a device.(Message from Change mode GUI)
		else
		{
			this->changeMode(sensorDataMap);
		}
	}
	catch(...)
	{
		LOG_MSG(("SIGViewer message is invalid."));
	}
}


/*
 * Make invert posture from current posture.
 */
void LinkageController::makeInvertPosture(ManNiiPosture &posture)
{
	// Left hand motion affects right hand.
	if (this->limbMode ==limbModes[HAND] && this->reverseMode == reverseModes[RIGHT])
	{
		// Get original quaternions.
		double w2, x2, y2, z2, w3, x3, y3, z3;
		posture.joint[ManNiiPosture::LARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
		posture.joint[ManNiiPosture::LARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);

		// Set a reversed quaternions.
		posture.joint[ManNiiPosture::RARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
		posture.joint[ManNiiPosture::RARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
	}
	// Right hand motion affects left hand.
	else if (this->limbMode ==limbModes[HAND] && this->reverseMode == reverseModes[LEFT])
	{
		// Get original quaternions.
		double w2, x2, y2, z2, w3, x3, y3, z3;
		posture.joint[ManNiiPosture::RARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
		posture.joint[ManNiiPosture::RARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);

		// Set a reversed quaternions.
		posture.joint[ManNiiPosture::LARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
		posture.joint[ManNiiPosture::LARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
	}
	// Left foot motion affects right foot.
	else if (this->limbMode ==limbModes[FOOT] && this->reverseMode == reverseModes[RIGHT])
	{
		// Get original quaternions.
		double w2, x2, y2, z2, w4, x4, y4, z4;
		posture.joint[ManNiiPosture::LLEG_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
		posture.joint[ManNiiPosture::LLEG_JOINT4].quaternion.getQuaternion(w4, x4, y4, z4);

		// Set a reversed quaternions.
		posture.joint[ManNiiPosture::RLEG_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
		posture.joint[ManNiiPosture::RLEG_JOINT4].quaternion.setQuaternion(w4, x4, -y4, -z4);
	}
	// Right foot motion affects left foot.
	else if (this->limbMode ==limbModes[FOOT] && this->reverseMode == reverseModes[LEFT])
	{
		// Get original quaternions.
		double w2, x2, y2, z2, w4, x4, y4, z4;
		posture.joint[ManNiiPosture::RLEG_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
		posture.joint[ManNiiPosture::RLEG_JOINT4].quaternion.getQuaternion(w4, x4, y4, z4);

		// Set a reversed quaternions.
		posture.joint[ManNiiPosture::LLEG_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
		posture.joint[ManNiiPosture::LLEG_JOINT4].quaternion.setQuaternion(w4, x4, -y4, -z4);
	}
}


/*
 * Change mode. (Limb mode or Reverse mode)
 */
void LinkageController::changeMode(const std::map<std::string, std::vector<std::string> > &map)
{
	// Change Limb mode.
	this->changeLimbMode(map);

	// Change Reverse mode.
	this->changeReverseMode(map);
}

/*
 * Change Limb mode.
 */
void LinkageController::changeLimbMode(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyLimbMode) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyLimbMode);

		if(this->limbMode == it->second[0])
		{
			std::cout << "It is already " << this->limbMode << " mode." << std::endl;
			return;
		}

		if(it->second[0]==limbModes[HAND])
		{
			this->resetVariables4Hand();
		}
		else if(it->second[0]==limbModes[FOOT])
		{
			this->resetVariables4Foot();
		}
		else
		{
			std::cout << "It is invalid value. LIMB_MODE=" << it->second[0] << std::endl;
			return;
		}

		std::cout << "Set limb mode:" << this->limbMode << std::endl;
	}
}

/*
 * Change Reverse mode.
 */
void LinkageController::changeReverseMode(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyReverseMode) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyReverseMode);

		if(this->reverseMode == it->second[0])
		{
			std::cout << "It is already " << this->reverseMode << " mode." << std::endl;
			return;
		}

		if(it->second[0]==reverseModes[RIGHT] || it->second[0]==reverseModes[LEFT] || it->second[0]==reverseModes[NOREVERSE])
		{
			this->reverseMode = it->second[0];
		}
		else
		{
			std::cout << "It is invalid value. REVERSE_MODE=" << it->second[0] << std::endl;
			return;
		}

		this->reverseMode = it->second[0];

		std::cout << "Set reverse mode:" << this->reverseMode << std::endl;
	}
}


/*
 * Reset variables for Hand mode.
 */
void LinkageController::resetVariables4Hand()
{
	// Set some variables.
	this->limbMode         = limbModes[HAND];
	this->linkageObjName   = this->linkageObjName4Hand;
	this->distance4Releasejudge  = this->distance4ReleaseJudgeInHand;

	this->rightLink = this->rightLink4Hand;
	this->leftLink  = this->leftLink4Hand;

	// Reset position of the table.
	SimObj *table = getObj(this->tableName.c_str());
	table->setPosition(this->tableIniPos);

	this->sendMsg(this->linkageObjName, "RESTART:FOOT");
}

/*
 * Reset variables for Foot mode.
 */
void LinkageController::resetVariables4Foot()
{
	// Set some variables.
	this->limbMode         = limbModes[FOOT];
	this->linkageObjName   = this->linkageObjName4Foot;
	this->distance4Releasejudge  = this->distance4ReleaseJudgeInFoot;

	this->rightLink = this->rightLink4Foot;
	this->leftLink  = this->leftLink4Foot;

	// Shift position of the table.
	SimObj *table = getObj(this->tableName.c_str());
	table->setPosition(this->tableIniPos.x(), this->tableIniPos.y(), this->tableIniPos.z()+1000.0); // Long enough shift.

	this->sendMsg(this->linkageObjName, "RESTART:FOOT");
}


/*
 * Collision detection.
 */
void LinkageController::onCollision(CollisionEvent &evt)
{
	try
	{
		// When the avatar is not grasping object.
		if (isGrasping == false)
		{
			/*
			 * Check collision with the linkage object.
			 */
			const std::vector<std::string> & collidedObj = evt.getWith();

			if(std::find(collidedObj.begin(), collidedObj.end(), this->linkageObjName)==collidedObj.end()){ return; }

			/*
			 * Check collided part. And save part name.
			 */
			const std::vector<std::string> & myCollidedParts = evt.getMyParts();

			if(std::find(myCollidedParts.begin(), myCollidedParts.end(), this->rightLink)!=myCollidedParts.end())
			{
				this->myGraspingPartName = this->rightLink;
			}
			else if(std::find(myCollidedParts.begin(), myCollidedParts.end(), this->leftLink)!=myCollidedParts.end())
			{
				this->myGraspingPartName = this->leftLink;
			}
			else
			{
				return;
			}

			/*
			 * Calculate distance of avatar and grasped object.
			 * And set the grasping state.
			 */
			SimObj *myself = getObj(myname());
			SimObj *target = getObj(this->linkageObjName.c_str());

			if (!myself || !target){ return; }
			if (myself->dynamics() || target->dynamics()){ return; }

			Vector3d myPartsPos, targetPos;
			target->getPosition(targetPos);
			CParts *cparts = myself->getParts(this->myGraspingPartName.c_str());
			cparts->getPosition(myPartsPos);
			cparts->graspObj(this->linkageObjName);

			this->distanceOfAvatarAndGraspedObject.x(targetPos.x()-myPartsPos.x());
			this->distanceOfAvatarAndGraspedObject.y(targetPos.y()-myPartsPos.y());
			this->distanceOfAvatarAndGraspedObject.z(targetPos.z()-myPartsPos.z());

			this->isGrasping = true;

			LOG_MSG(("Grasped the %s.", this->linkageObjName.c_str()));
		}
	}
	catch(SimObj::NoAttributeException &err)
	{
		LOG_MSG(("NoAttributeException: %s", err.msg()));
	}
	catch(SimObj::AttributeReadOnlyException &err)
	{
		LOG_MSG(("AttributeReadOnlyException: %s", err.msg()));
	}
	catch(SimObj::Exception &err)
	{
		LOG_MSG(("Exception: %s", err.msg()));
	}
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

	// If parameter file is "not" exists.
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
		this->graspWithBoth = paramFileValLinkageGraspGraspWithBothDefault;

		this->isWaistFixed = paramFileValLinkageGraspIsWaistFixedDefault;
	}
	// if parameter file is exists.
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
			this->reverseMode   = pt.get<std::string>(paramFileKeyLinkageGraspReverseMode);
			this->graspWithBoth = pt.get<bool>(paramFileKeyLinkageGraspGraspWithBoth);

			this->isWaistFixed = pt.get<bool>(paramFileKeyLinkageGraspIsWaistFixed);
		}
		catch (boost::exception &ex)
		{
			std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		}
	}

	// Print the initialized parameter.
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
	std::cout << paramFileKeyLinkageGraspReverseMode   << ":" << this->reverseMode  << std::endl;
	std::cout << paramFileKeyLinkageGraspGraspWithBoth << ":" << this->graspWithBoth << std::endl;

	std::cout << paramFileKeyLinkageGraspIsWaistFixed  << ":" << this->isWaistFixed  << std::endl;

	this->kinectV2DeviceManager = KinectV2DeviceManager(kinectV2ServiceName, kinectV2DeviceType, kinectV2DeviceUniqueID, scaleRatio);

	// Set a sensor data mode of Kinect v2.
	KinectV2SensorData::setSensorDataMode(sensorDataModeStr);

	this->oculusDK1DeviceManager = OculusDK1DeviceManager(oculusDK1ServiceName, oculusDK1DeviceType, oculusDK1DeviceUniqueID);
}


/*
 * Create and return instance of this controller.
 */
extern "C" Controller * createController()
{
	return new LinkageController;
}

