/*
 * MirrorTherapyController.cpp
 *
 *  Created on: 2016/04/26
 *      Author: Yamada
 */
#include "MirrorTherapyController.h"

#include <sys/time.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <math.h>
#include <sigverse/plugin/common/sensor/SensorData.h>


///@brief Initialize this controller.
void MirrorTherapyController::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();

	SimObj *myself = getObj(myname());

	this->kinectV2DeviceManager.initPositionAndRotation(myself);


	// For Mirror therapy initialize.
	this->reverseMode = reverseModes[LEFTHAND]; // Set reverse mode.

	// Set delay variables.
	gettimeofday(&this->initTimeVal, NULL);
	this->frameNumber = 0;
	this->targetDelayTime = defaultDelayTime;
	this->pastTimeAndPostures = std::vector<TimeAndPosture>(timeSeriesBufferSize);

//	if(std::string(myname())==headName)
//	{
//		this->isHead = true;
//	}
	this->rootJointQuaternion.w = 1.0;
	this->rootJointQuaternion.x = 0.0;
	this->rootJointQuaternion.y = 0.0;
	this->rootJointQuaternion.z = 0.0;
	this->waistJointQuaternion.w = 1.0;
	this->waistJointQuaternion.x = 0.0;
	this->waistJointQuaternion.y = 0.0;
	this->waistJointQuaternion.z = 0.0;

	//Set default avatar
	this->bodyAvatarName = this->middleArmManName;

	this->guiService = NULL;
}

///@brief Movement of the robot.
double MirrorTherapyController::onAction(ActionEvent &evt)
{
	return 10.0;
}


///@brief Message from device.
void MirrorTherapyController::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
//		if(!this->isHead && std::string(evt.getSender())!=headName){ return; }

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

				if (this->usingOculus)
				{
					// (When all values are zero, it means invalid.)
					posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
//					posture.joint[ManNiiPosture::HEAD_JOINT1].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
				}

				// Correct the angle of x-direction slope for human avatar.
				double radx = this->correctionAngle * M_PI / 180.0;
				Quaternion correctQuaternion(std::cos(radx/2.0),std::sin(radx/2.0),0.0,0.0);
				Quaternion correctedRootQuaternion = Quaternion::calcCrossProduct(posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion, correctQuaternion);
				posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion = correctedRootQuaternion;

				if(std::string(myname())==middleArmManName)
				{
					// Store current posture and time stamp.
					int currentIndex = this->storeCurrentPosture(posture);

					// Search nearest neighbor of target delay time in stored time stamps.
					//   last 500 postures is in this->pastPostures.
					//   find nearest index.
					ManNiiPosture delayedPosture = this->getDelayedPosture(currentIndex);

					this->invertPosture(posture, delayedPosture);
				}

				this->rootJointQuaternion  = posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion;
				this->waistJointQuaternion = posture.joint[ManNiiPosture::WAIST_JOINT1].quaternion;

				// Set SIGVerse quaternions and positions
				SimObj *obj = getObj(myname());
				this->kinectV2DeviceManager.setRootPosition(obj, sensorData.rootPosition);
				KinectV2DeviceManager::setJointQuaternions2ManNii(obj, posture, sensorData);
			}
			/*
			 * Oculus DK2
			 */
			else if (deviceTypeValue == this->oculusDK2DeviceManager.deviceType && deviceUniqueId ==  this->oculusDK2DeviceManager.deviceUniqueID)
			{
				if(!this->usingOculus){ this->usingOculus = true; }

				OculusRiftDK2SensorData sensorData;
				sensorData.setSensorData(sensorDataMap);

				ManNiiPosture posture = OculusDK2DeviceManager::convertQuaternion2ManNiiPosture(sensorData.getQuaternion());

				Quaternion correctedQuaternion = Quaternion(posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion);

				correctedQuaternion = Quaternion::multiplyNI(this->waistJointQuaternion, correctedQuaternion);
				correctedQuaternion = Quaternion::multiplyNI(this->rootJointQuaternion,  correctedQuaternion);

				posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion = correctedQuaternion;

				SimObj *obj = getObj(myname());
				OculusDK2DeviceManager::setJointQuaternions2ManNii(obj, posture);
			}
		}
		// Not a device.(Message from Change Avatar GUI)
		else
		{
			this->changeAvatar(sensorDataMap);

			if(std::string(myname())==middleArmManName)
			{
				this->setReverseModeAndDelayTime(sensorDataMap);
			}
		}
	}
	catch(...)
	{
		LOG_MSG(("SIGViewer message is invalid."));
	}
}


const double MirrorTherapyController::generateCurrentTimeStamp()
{
	// Get current timestamp.
	struct timeval currentTimeVal;
	gettimeofday(&currentTimeVal, NULL);
	const double currentTimeStamp = (currentTimeVal.tv_sec - this->initTimeVal.tv_sec) * 1000.0 + (currentTimeVal.tv_usec - this->initTimeVal.tv_usec) / 1000.0;
	return currentTimeStamp;
}


int MirrorTherapyController::storeCurrentPosture(const ManNiiPosture &posture)
{
	TimeAndPosture current;
	current.timeStamp = this->generateCurrentTimeStamp();

	for (int i = 0; i < ManNiiPosture::ManNiiJointType_Count; i++)
	{
		current.posture.joint[i] = posture.joint[i];
	}

	int index = this->frameNumber % timeSeriesBufferSize;
	this->pastTimeAndPostures[index] = current;

	this->frameNumber++;

	return index;
}


ManNiiPosture MirrorTherapyController::getDelayedPosture(const int currentIndex)
{
	// If targetDelayTime <= 0, don't do anything.
	if (this->targetDelayTime <= 0.0){ return this->pastTimeAndPostures[currentIndex].posture; }

	// If elapsed time < targetDelayTime, don't do anything.
	if(this->pastTimeAndPostures[currentIndex].timeStamp < this->targetDelayTime){ return this->pastTimeAndPostures[currentIndex].posture; }


	const double idealPastTime = this->pastTimeAndPostures[currentIndex].timeStamp - this->targetDelayTime;

	int delayedPostureIndex = 0;
	double nearestTimeDistance = DBL_MAX;

	// Search nearest index to the delay time.
	for (int i = timeSeriesBufferSize-1; i >= 0; i--)
	{
		double tmpTimeDistance = abs(idealPastTime - this->pastTimeAndPostures[i].timeStamp);

		if (tmpTimeDistance < nearestTimeDistance)
		{
			nearestTimeDistance = tmpTimeDistance;
			delayedPostureIndex = i;
		}
	}

	return this->pastTimeAndPostures[delayedPostureIndex].posture;
}


void MirrorTherapyController::invertPosture(ManNiiPosture &posture, const ManNiiPosture &delayedPosture)
{
	// left hand motion affects right hand.
	if (this->reverseMode == reverseModes[RIGHTHAND])
	{
		// Get original quaternions.
		double w2, x2, y2, z2, w3, x3, y3, z3;
		delayedPosture.joint[ManNiiPosture::LARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
		delayedPosture.joint[ManNiiPosture::LARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);

		// Set reverse quaternions.
		posture.joint[ManNiiPosture::RARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
		posture.joint[ManNiiPosture::RARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
	}
	// right hand motion affects left hand.
	else if (this->reverseMode == reverseModes[LEFTHAND])
	{
		// Get original quaternions.
		double w2, x2, y2, z2, w3, x3, y3, z3;
		delayedPosture.joint[ManNiiPosture::RARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
		delayedPosture.joint[ManNiiPosture::RARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);

		// Set reverse quaternions.
		posture.joint[ManNiiPosture::LARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
		posture.joint[ManNiiPosture::LARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
	}
}


void MirrorTherapyController::changeAvatar(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyAvatar) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyAvatar);
		this->bodyAvatarName = it->second[0];

		if(std::string(myname())==this->bodyAvatarName){ this->kinectV2DeviceManager.iniPos.z = 0.0; }
		else                                           { this->kinectV2DeviceManager.iniPos.z = 1000.0; }

		this->kinectV2DeviceManager.started = false;

		if(std::string(myname())!=this->bodyAvatarName)
		{
			SigCmn::Vector3 zeroPos; // Dummy position
			zeroPos.x = 0.0;
			zeroPos.y = 0.0;
			zeroPos.z = 0.0;

			SimObj *my = getObj(myname());
			this->kinectV2DeviceManager.setRootPosition(my, zeroPos); // Blow off.
		}
	}
}

void MirrorTherapyController::setReverseModeAndDelayTime(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyReverse) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyReverse);
		this->reverseMode = it->second[0];

		std::cout << "Set reverse mode:" << this->reverseMode << std::endl;
	}

	if (map.find(msgKeyDelay) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyDelay);
		this->targetDelayTime = atof(it->second[0].c_str());

		std::cout << "Set target delay time:" << this->targetDelayTime << std::endl;
	}
}



///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void MirrorTherapyController::readIniFileAndInitialize()
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
	return new MirrorTherapyController;
}

