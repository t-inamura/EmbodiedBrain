/*
 * MirrorTherapyController.cpp
 *
 *  Created on: 2015/03/25
 *      Author: Nozaki
 */
#include <sigverse/controller/MirrorTherapyController/MirrorTherapyController.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sys/time.h>
#include <typeinfo>

const std::string MirrorTherapyController::parameterFileName = "MirrorTherapy.ini";

const std::string MirrorTherapyController::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string MirrorTherapyController::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string MirrorTherapyController::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string MirrorTherapyController::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string MirrorTherapyController::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";

const std::string MirrorTherapyController::paramFileKeyOculusDK1ServiceName   = "OculusDK1.service_name";
const std::string MirrorTherapyController::paramFileKeyOculusDK1Devicetype    = "OculusDK1.device_type";
const std::string MirrorTherapyController::paramFileKeyOculusDK1DeviceUniqueID= "OculusDK1.device_unique_id";

const std::string MirrorTherapyController::paramFileValKinectV2SensorDataModeDefault = "QUATERNION";
const double      MirrorTherapyController::paramFileValKinectV2ScaleRatioDefault     = 10000.0;

const std::string MirrorTherapyController::msgKeyReverse = "REVERSE";
const std::string MirrorTherapyController::msgKeyDelay   = "DELAY";

const double MirrorTherapyController::defaultDelayTime      = 1000.0;
const int    MirrorTherapyController::timeSeriesBufferSize  = 500;

const std::string MirrorTherapyController::reverseModes[ReverseMode_Count] = { "NOREVERSE", "RIGHTHAND", "LEFTHAND" };


///@brief Initialize this controller.
void MirrorTherapyController::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();

	SimObj *myself = getObj(myname());

	this->kinectV2DeviceManager.initPositionAndRotation(myself);


	// For Mirror therapy initialize.
	this->reverseMode = reverseModes[RIGHTHAND]; // Set reverse mode.

	// Set delay variables.
	gettimeofday(&this->initTimeVal, NULL);
	this->frameNumber = 0;
	this->targetDelayTime = defaultDelayTime;
	this->pastTimeAndPostures = std::vector<TimeAndPosture>(timeSeriesBufferSize);
}

///@brief Movement of the robot.
double MirrorTherapyController::onAction(ActionEvent &evt)
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

	return 1.0;
}


///@brief Message from device.
void MirrorTherapyController::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		// Receive message.
		const std::string allMsg = evt.getMsg();

//		std::cout << allMsg << std::endl;

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
				ManNiiPosture manNiiPosture = KinectV2DeviceManager::convertKinectData2ManNiiPosture(sensorData);


				// Store current posture and time stamp.
				int currentIndex = this->storeCurrentPosture(manNiiPosture);

				// Search nearest neighbor of target delay time in stored time stamps.
				//   last 500 postures is in this->pastPostures.
				//   find nearest index.
				ManNiiPosture delayedPosture = this->getDelayedPosture(currentIndex);

				this->invertPosture(manNiiPosture, delayedPosture);


				// Set SIGVerse quaternions and positions
				SimObj *obj = getObj(myname());
				this->kinectV2DeviceManager.setRootPosition(obj, sensorData.rootPosition);
				this->kinectV2DeviceManager.setJointQuaternions2ManNii(obj, manNiiPosture, sensorData);

				this->frameNumber++;
			}
			/*
			 * Oculus DK1
			 */
			else if (deviceTypeValue == this->oculusDK1DeviceManager.deviceType && deviceUniqueId ==  this->oculusDK1DeviceManager.deviceUniqueID)
			{
				OculusRiftDK1SensorData sensorData;
				sensorData.setSensorData(sensorDataMap);

				ManNiiPosture manNiiPosture = this->oculusDK1DeviceManager.convertEulerAngle2ManNiiPosture(sensorData.getEulerAngle());

				SimObj *obj = getObj(myname());
				this->oculusDK1DeviceManager.setJointQuaternions2ManNii(obj, manNiiPosture);
			}
		}
		/*
		 * Not a device.
		 */
		else
		{
			this->setReverseModeAndDelayTime(sensorDataMap);
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


bool MirrorTherapyController::setReverseModeAndDelayTime(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyReverse) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyReverse);
		this->reverseMode = it->second[0];

		std::cout << "Set reverse mode:" << this->reverseMode << std::endl;
		return true;
	}
	else if (map.find(msgKeyDelay) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyDelay);
		this->targetDelayTime = atof(it->second[0].c_str());

		std::cout << "Set target delay time:" << this->targetDelayTime << std::endl;
		return true;
	}
	return false;
}



///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void MirrorTherapyController::readIniFileAndInitialize()
{
	std::ifstream ifs(MirrorTherapyController::parameterFileName.c_str());

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
		std::cout << "Not exist : " << MirrorTherapyController::parameterFileName << std::endl;
		std::cout << "Use default parameter." << std::endl;

		kinectV2ServiceName    = SERVICE_NAME_KINECT_V2;
		kinectV2DeviceType     = DEV_TYPE_KINECT_V2;
		kinectV2DeviceUniqueID = DEV_UNIQUE_ID_0;

		sensorDataModeStr = MirrorTherapyController::paramFileValKinectV2SensorDataModeDefault;
		scaleRatio        = MirrorTherapyController::paramFileValKinectV2ScaleRatioDefault;

		oculusDK1ServiceName    = SERVICE_NAME_OCULUS_DK1;
		oculusDK1DeviceType     = DEV_TYPE_OCULUS_DK1;
		oculusDK1DeviceUniqueID = DEV_UNIQUE_ID_0;
	}
	// Parameter file is exists.
	else
	{
		try
		{
			std::cout << "Read " << MirrorTherapyController::parameterFileName << std::endl;
			boost::property_tree::ptree pt;
			boost::property_tree::read_ini(MirrorTherapyController::parameterFileName, pt);

			kinectV2ServiceName    = pt.get<std::string>(paramFileKeyKinectV2ServiceName);
			kinectV2DeviceType     = pt.get<std::string>(paramFileKeyKinectV2Devicetype);
			kinectV2DeviceUniqueID = pt.get<std::string>(paramFileKeyKinectV2DeviceUniqueID);

			sensorDataModeStr = pt.get<std::string>(MirrorTherapyController::paramFileKeyKinectV2SensorDataMode);
			scaleRatio        = pt.get<double>     (MirrorTherapyController::paramFileKeyKinectV2ScaleRatio);

			oculusDK1ServiceName    = pt.get<std::string>(paramFileKeyOculusDK1ServiceName);
			oculusDK1DeviceType     = pt.get<std::string>(paramFileKeyOculusDK1Devicetype);
			oculusDK1DeviceUniqueID = pt.get<std::string>(paramFileKeyOculusDK1DeviceUniqueID);
		}
		catch (boost::exception &ex)
		{
			std::cout << MirrorTherapyController::parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		}
	}

	std::cout << paramFileKeyKinectV2ServiceName    << ":" << kinectV2ServiceName    << std::endl;
	std::cout << paramFileKeyKinectV2Devicetype     << ":" << kinectV2DeviceType     << std::endl;
	std::cout << paramFileKeyKinectV2DeviceUniqueID << ":" << kinectV2DeviceUniqueID << std::endl;

	std::cout << MirrorTherapyController::paramFileKeyKinectV2SensorDataMode << ":" << sensorDataModeStr << std::endl;
	std::cout << MirrorTherapyController::paramFileKeyKinectV2ScaleRatio     << ":" << scaleRatio << std::endl;

	std::cout << paramFileKeyOculusDK1ServiceName    << ":" << oculusDK1ServiceName    << std::endl;
	std::cout << paramFileKeyOculusDK1Devicetype     << ":" << oculusDK1DeviceType     << std::endl;
	std::cout << paramFileKeyOculusDK1DeviceUniqueID << ":" << oculusDK1DeviceUniqueID << std::endl;


	this->kinectV2DeviceManager = KinectV2DeviceManager(kinectV2ServiceName, kinectV2DeviceType, kinectV2DeviceUniqueID, scaleRatio);

	// Set sensor data mode.
	KinectV2SensorData::setSensorDataMode(sensorDataModeStr);

	this->oculusDK1DeviceManager = OculusDK1DeviceManager(oculusDK1ServiceName, oculusDK1DeviceType, oculusDK1DeviceUniqueID);
}


extern "C" Controller * createController()
{
	return new MirrorTherapyController;
}


