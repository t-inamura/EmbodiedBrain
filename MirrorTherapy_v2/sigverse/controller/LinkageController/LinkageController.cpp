/*
 * LinkageController.cpp
 *
 *  Created on: 2015/08/25
 *      Author: Yamada@tome
 */

#include <sigverse/common/device/SensorData.h>
#include <sigverse/controller/LinkageController/LinkageController.h>


#include <sys/time.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <cmath>
#include <algorithm>


const std::string LinkageController::parameterFileName = "LinkageController.ini";

const std::string LinkageController::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string LinkageController::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string LinkageController::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string LinkageController::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string LinkageController::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";

const std::string LinkageController::paramFileKeyOculusDK1ServiceName   = "OculusDK1.service_name";
const std::string LinkageController::paramFileKeyOculusDK1Devicetype    = "OculusDK1.device_type";
const std::string LinkageController::paramFileKeyOculusDK1DeviceUniqueID= "OculusDK1.device_unique_id";

const std::string LinkageController::paramFileValKinectV2SensorDataModeDefault = "QUATERNION";
const double      LinkageController::paramFileValKinectV2ScaleRatioDefault     = 10000.0;

const std::string LinkageController::paramFileKeyChangeModeGUIServiceName        = "ChangeModeGUI.service_name";
const std::string LinkageController::paramFileValChangeModeGUIServiceNameDefault = "SVC_CHANGE_MODE_GUI";

const std::string LinkageController::msgKeyLimbMode = "LIMB_MODE";
const std::string LinkageController::msgKeyReverse  = "REVERSE";

const std::string LinkageController::limbModes[LimbMode_Count] = { "HAND", "FOOT" };
const std::string LinkageController::reverseModes[ReverseMode_Count] = { "RIGHT", "LEFT", "NOREVERSE" };

const std::string LinkageController::rightLink4Hand = "RARM_LINK7";
const std::string LinkageController::leftLink4Hand  = "LARM_LINK7";
const std::string LinkageController::rightLink4Foot = "RLEG_LINK6";
const std::string LinkageController::leftLink4Foot  = "LLEG_LINK6";

const std::string LinkageController::chairName           = "chair";
const std::string LinkageController::tableName           = "table";
const std::string LinkageController::linkageObjName4Hand = "linkageObj4Hand";
const std::string LinkageController::linkageObjName4Foot = "linkageObj4Foot";

const double      LinkageController::distance4ReleaseJudgeOnHand = 12.0+1.0; // graspRadius + margin.
const double      LinkageController::distance4ReleaseJudgeOnFoot = 12.0+1.0; // graspRadius + margin.

const double      LinkageController::shiftDistanceForChangingLimb = 1000.0;



///@brief Initialize this controller.
void LinkageController::onInit(InitEvent &evt)
{
	this->readIniFileAndInitialize();

	SimObj *myself = getObj(myname());
	this->kinectV2DeviceManager.initPositionAndRotation(myself);

	SimObj *table = getObj(this->tableName.c_str());
	table->getPosition(this->tableIniPos);

	SimObj *linkageObj4Hand = getObj(this->linkageObjName4Hand.c_str());
	linkageObj4Hand->getPosition(this->linkageObjIniPos4Hand);

	SimObj *linkageObj4Foot = getObj(this->linkageObjName4Foot.c_str());
	linkageObj4Foot->getPosition(this->linkageObjIniPos4Foot);

	this->reverseMode = this->reverseModes[RIGHT]; // Set reverse mode.

	reset4Hand();

//	/* Adjustment of knee angles to sit on the chair */
//	/* Root of both legs */
//	myself->setJointAngle("RLEG_JOINT2", DEG2RAD(-90));
//	myself->setJointAngle("LLEG_JOINT2", DEG2RAD(-90));
//	/* Both knee */
//	myself->setJointAngle("RLEG_JOINT4", DEG2RAD( 90));
//	myself->setJointAngle("LLEG_JOINT4", DEG2RAD( 90));

	/* At the first, the robot does not grasp anything. */
	/* Do not set true. it results bug caused by developing mode settings */
	this->isGrasping = false;

	this->elapsedTimeSinceRelease = 0.0;

	this->usingOculus = false;

	this->guiService = NULL;

	this->myGraspingPartName = "";

	this->distanceAtGrasping.x(0.0);
	this->distanceAtGrasping.y(0.0);
	this->distanceAtGrasping.z(0.0);

	std::thread thCheckService(&LinkageController::checkServiceForThread, this);
	thCheckService.detach();
}

///@brief Movement of the robot.
double LinkageController::onAction(ActionEvent &evt)
{
	const double interval = 0.1;

	try
	{
		/* Keep grasping object*/
		SimObj *myself = getObj(myname());
		SimObj *target = getObj(this->linkageObjName.c_str());

		if (!myself || !target){ return interval; }

		if (myself->dynamics() || target->dynamics()){ return interval; }

		Vector3d targetPos;
		target->getPosition(targetPos);

		if (this->isGrasping)
		{
			/* Keep adjusting the position of object while grasping*/
			CParts *myParts  = myself->getParts(this->myGraspingPartName.c_str());

			/* Get name and position of parts grasped */
			Vector3d myPartsPos;
			myParts->getPosition(myPartsPos);

			Vector3d newTargetPos;
			newTargetPos.x(targetPos.x());
			newTargetPos.y(myPartsPos.y()+this->distanceAtGrasping.y());
			newTargetPos.z(myPartsPos.z()+this->distanceAtGrasping.z());

			//Reposition after landing.
			if(newTargetPos.y() < this->minOfLinkageObj.y())
			{
				newTargetPos.y(this->minOfLinkageObj.y());
				if(newTargetPos.z() < this->minOfLinkageObj.z()){ newTargetPos.z(this->minOfLinkageObj.z()); }
				if(newTargetPos.z() > this->maxOfLinkageObj.z()){ newTargetPos.z(this->maxOfLinkageObj.z()); }
			}
//			std::cout << "onAction 1:" << grasped_pos.y() << std::endl;

			//Recalc.
			this->distanceAtGrasping.x(newTargetPos.x()-myPartsPos.x());
			this->distanceAtGrasping.y(newTargetPos.y()-myPartsPos.y());
			this->distanceAtGrasping.z(newTargetPos.z()-myPartsPos.z());

			target->setPosition(newTargetPos);

			if (this->distanceAtGrasping.length() > this->distance4Releasejudge)
			{
				this->isGrasping = false;
				this->elapsedTimeSinceRelease = 0.0;

				LOG_MSG(("Release %s.", linkageObjName.c_str()));
			}
		}
		else
		{
			if(targetPos.y() > this->minOfLinkageObj.y())
			{
				this->elapsedTimeSinceRelease+=interval;
				double gravity = -980.665;
				double distanceOfFreeFall = interval*gravity*(2.0*this->elapsedTimeSinceRelease-interval)/2.0;
				double newYpos = targetPos.y()+distanceOfFreeFall;

				Vector3d newTargetPos;
				newTargetPos.x(targetPos.x());
				newTargetPos.y(newYpos);
				newTargetPos.z(targetPos.z());

				//Reposition after landing.
				if(newTargetPos.y() < this->minOfLinkageObj.y())
				{
					newTargetPos.y(this->minOfLinkageObj.y());
					if(newTargetPos.z() < this->minOfLinkageObj.z()){ newTargetPos.z(this->minOfLinkageObj.z()); }
					if(newTargetPos.z() > this->maxOfLinkageObj.z()){ newTargetPos.z(this->maxOfLinkageObj.z()); }
				}

				target->setPosition(newTargetPos);
			}
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

	return interval;
}


void LinkageController::checkServiceForThread()
{
	LOG_MSG(("Start checkServiceForThread method."));

	try
	{
		while(true)
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

			bool oculusDK1Available = checkService(this->oculusDK1DeviceManager.serviceName);

			if (oculusDK1Available && this->oculusDK1DeviceManager.service == NULL)
			{
				this->oculusDK1DeviceManager.service = connectToService(this->oculusDK1DeviceManager.serviceName);
			}
			else if (!oculusDK1Available && this->oculusDK1DeviceManager.service != NULL)
			{
				this->oculusDK1DeviceManager.service = NULL;
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

			//sleep 3 sec.
			sleep(3);
		}
	}
	catch(...)
	{
		LOG_MSG(("Exception occurred in checkServiceForThread."));
	}
}

///@brief Message from device.
void LinkageController::onRecvMsg(RecvMsgEvent &evt)
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

				// If using oculus, set invalid head quaternion.
				//   (When all values are zero, head quaternion will be invalid.)
				if (this->usingOculus)
				{
					posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::HEAD_JOINT1].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
				}

				// Invalidate body rotation.
				posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
				posture.joint[ManNiiPosture::WAIST_JOINT1].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);

				this->makeInvertPosture(posture);

				// Set SIGVerse quaternions and positions.
				SimObj *obj = getObj(myname());
				//this->kinectV2DeviceManager.setRootPosition(obj, sensorData.rootPosition); // Don't move.
				// Set joint quaternions to avatar.
				KinectV2DeviceManager::setJointQuaternions2ManNii(obj, posture, sensorData);
			}
			/*
			 * Oculus DK1
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


void LinkageController::makeInvertPosture(ManNiiPosture &posture)
{
	// Left hand motion affects right hand.
	if (this->limbMode ==limbModes[HAND] && this->reverseMode == reverseModes[RIGHT])
	{
		// Get original quaternions.
		double w2, x2, y2, z2, w3, x3, y3, z3;
		posture.joint[ManNiiPosture::LARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
		posture.joint[ManNiiPosture::LARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);

		// Set reverse quaternions.
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

		// Set reverse quaternions.
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

		// Set reverse quaternions.
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

		// Set reverse quaternions.
		posture.joint[ManNiiPosture::LLEG_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
		posture.joint[ManNiiPosture::LLEG_JOINT4].quaternion.setQuaternion(w4, x4, -y4, -z4);
	}
}


void LinkageController::changeMode(const std::map<std::string, std::vector<std::string> > &map)
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
			this->reset4Hand();
		}
		else if(it->second[0]==limbModes[FOOT])
		{
			this->reset4Foot();
		}
	}

	if (map.find(msgKeyReverse) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyReverse);
		this->reverseMode = it->second[0];

		std::cout << "Set reverse mode:" << this->reverseMode << std::endl;
	}
}


void LinkageController::reset4Hand()
{
	// Set Linkage parameter.
	this->limbMode         = limbModes[HAND];     // Set limb mode.
	this->linkageObjName   = this->linkageObjName4Hand;
	this->distance4Releasejudge  = this->distance4ReleaseJudgeOnHand;

	this->rightLink = this->rightLink4Hand;
	this->leftLink  = this->leftLink4Hand;

	this->minOfLinkageObj.x(-10000.0);                          // No limit.
	this->minOfLinkageObj.y(this->linkageObjIniPos4Hand.y());   // Same as initial position.
	this->minOfLinkageObj.z(this->linkageObjIniPos4Hand.z()-8); // -8 is fixed value.

	this->maxOfLinkageObj.x(+10000.0);                          // No limit.
	this->maxOfLinkageObj.y(+10000.0);                          // No limit.
	this->maxOfLinkageObj.z(this->linkageObjIniPos4Hand.z()+8); // +8 is fixed value.

	// Set table position.
	SimObj *table = getObj(this->tableName.c_str());
	table->setPosition(this->tableIniPos);

	// Set linkageObj4Hand position.
	SimObj *linkageObj4Hand = getObj(this->linkageObjName4Hand.c_str());
	linkageObj4Hand->setPosition(this->linkageObjIniPos4Hand);

	// Shift linkageObj4Foot position.
	SimObj *linkageObj4Foot = getObj(this->linkageObjName4Foot.c_str());
	linkageObj4Foot->setPosition(this->linkageObjIniPos4Foot.x(), this->linkageObjIniPos4Foot.y(), this->linkageObjIniPos4Foot.z()+this->shiftDistanceForChangingLimb);
}

void LinkageController::reset4Foot()
{
	// Set Linkage parameter.
	this->limbMode         = limbModes[FOOT];     // Set limb mode.
	this->linkageObjName   = this->linkageObjName4Foot;
	this->distance4Releasejudge  = this->distance4ReleaseJudgeOnFoot;

	this->rightLink = this->rightLink4Foot;
	this->leftLink  = this->leftLink4Foot;

	this->minOfLinkageObj.x(-10000.0);                          // No limit.
	this->minOfLinkageObj.y(this->linkageObjIniPos4Foot.y());   // Same as initial position.
	this->minOfLinkageObj.z(this->linkageObjIniPos4Foot.z()-8); // -8 is fixed value.

	this->maxOfLinkageObj.x(+10000.0);                          // No limit.
	this->maxOfLinkageObj.y(+10000.0);                          // No limit.
	this->maxOfLinkageObj.z(this->linkageObjIniPos4Foot.z()+8); // +8 is fixed value.

	// Shift table position.
	SimObj *table = getObj(this->tableName.c_str());
	table->setPosition(this->tableIniPos.x(), this->tableIniPos.y(), this->tableIniPos.z()+this->shiftDistanceForChangingLimb);

	// Shift linkageObj4Hand position.
	SimObj *linkageObj4Hand = getObj(this->linkageObjName4Hand.c_str());
	linkageObj4Hand->setPosition(this->linkageObjIniPos4Hand.x(), this->linkageObjIniPos4Hand.y(), this->linkageObjIniPos4Hand.z()+this->shiftDistanceForChangingLimb);

	// Set linkageObj4Foot position.
	SimObj *linkageObj4Foot = getObj(this->linkageObjName4Foot.c_str());
	linkageObj4Foot->setPosition(this->linkageObjIniPos4Foot);
}


/*!
 * @brief Grasp LinkageObject when left hand touches it
 */
void LinkageController::onCollision(CollisionEvent &evt)
{
	try
	{
		if (isGrasping == false)
		{
			/* Get the name grasping object */
			const std::vector<std::string> & collidedObj = evt.getWith();

			if(std::find(collidedObj.begin(), collidedObj.end(), this->linkageObjName)==collidedObj.end()){ return; }

			/* Get one's parts colliding. */
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

			SimObj *myself = getObj(myname());
			SimObj *target = getObj(this->linkageObjName.c_str());

			if (!myself || !target){ return; }
			if (myself->dynamics() || target->dynamics()){ return; }

			Vector3d myPartsPos, targetPos;
			target->getPosition(targetPos);
			myself->getParts(this->myGraspingPartName.c_str())->getPosition(myPartsPos);

			this->distanceAtGrasping.x(targetPos.x()-myPartsPos.x());
			this->distanceAtGrasping.y(targetPos.y()-myPartsPos.y());
			this->distanceAtGrasping.z(targetPos.z()-myPartsPos.z());

			this->isGrasping = true;
			this->elapsedTimeSinceRelease = 0.0;

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


///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void LinkageController::readIniFileAndInitialize()
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

	// Parameter file is "not" exists.
	if (ifs.fail())
	{
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

		this->guiServiceName = paramFileValChangeModeGUIServiceNameDefault;
	}
	// Parameter file is exists.
	else
	{
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

			this->guiServiceName = pt.get<std::string>(paramFileKeyChangeModeGUIServiceName);
		}
		catch (boost::exception &ex)
		{
			std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		}
	}

	std::cout << paramFileKeyKinectV2ServiceName    << ":" << kinectV2ServiceName    << std::endl;
	std::cout << paramFileKeyKinectV2Devicetype     << ":" << kinectV2DeviceType     << std::endl;
	std::cout << paramFileKeyKinectV2DeviceUniqueID << ":" << kinectV2DeviceUniqueID << std::endl;

	std::cout << paramFileKeyKinectV2SensorDataMode << ":" << sensorDataModeStr << std::endl;
	std::cout << paramFileKeyKinectV2ScaleRatio     << ":" << scaleRatio << std::endl;

	std::cout << paramFileKeyOculusDK1ServiceName    << ":" << oculusDK1ServiceName    << std::endl;
	std::cout << paramFileKeyOculusDK1Devicetype     << ":" << oculusDK1DeviceType     << std::endl;
	std::cout << paramFileKeyOculusDK1DeviceUniqueID << ":" << oculusDK1DeviceUniqueID << std::endl;

	std::cout << paramFileKeyChangeModeGUIServiceName << ":" << this->guiServiceName << std::endl;


	this->kinectV2DeviceManager = KinectV2DeviceManager(kinectV2ServiceName, kinectV2DeviceType, kinectV2DeviceUniqueID, scaleRatio);

	// Set sensor data mode.
	KinectV2SensorData::setSensorDataMode(sensorDataModeStr);

	this->oculusDK1DeviceManager = OculusDK1DeviceManager(oculusDK1ServiceName, oculusDK1DeviceType, oculusDK1DeviceUniqueID);
}



extern "C" Controller * createController()
{
	return new LinkageController;
}

