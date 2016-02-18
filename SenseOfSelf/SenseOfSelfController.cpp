/*
 * SenseOfSelfController.cpp
 *
 *  Created on: 2015/06/08
 *      Author: Yamada@tome
 */
#include "SenseOfSelfController.h"

#include <sys/time.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <math.h>
#include <sigverse/plugin/common/sensor/SensorData.h>

///@brief Initialize this controller.
void SenseOfSelfController::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();

	SimObj *myself = getObj(myname());

	this->kinectV2DeviceManager.initPositionAndRotation(myself);

	if(this->isBodyFixed)
	{
		myself->setJointAngle("LARM_JOINT2",  DEG2RAD(-90));
//		myself->setJointAngle("HEAD_JOINT1",  DEG2RAD(-bodyAngle));
//		myself->setJointAngle("WAIST_JOINT1", DEG2RAD(+bodyAngle));
	}
	if(isHeadFixed)
	{
		if(tmpUsingOculus)
		{
			std::cout << "この構成は想定外なのでやめてください。" << std::endl;
			exit(1);
		}
		if(std::string(myname())==this->middleArmManName)
		{
			myself->setJointAngle("HEAD_JOINT0",  DEG2RAD(-15));
			myself->setJointAngle("HEAD_JOINT1",  DEG2RAD(35));
		}
		if(std::string(myname())==this->longArmManName)
		{
			myself->setJointAngle("HEAD_JOINT0",  DEG2RAD(-5));
			myself->setJointAngle("HEAD_JOINT1",  DEG2RAD(20));
		}
		if(std::string(myname())==this->shortArmManName)
		{
			myself->setJointAngle("HEAD_JOINT0",  DEG2RAD(-20));
			myself->setJointAngle("HEAD_JOINT1",  DEG2RAD(40));
		}
	}

//	SimObj *objPlateS = getObj(plateSName.c_str());
//	objPlateS->getPosition(iniPosPlateS);
//	objPlateS->setPosition(iniPosPlateS.x(),iniPosPlateS.y(),iniPosPlateS.z()+1000); //遠くに飛ばす
//
//	SimObj *objPlateM = getObj(plateMName.c_str());
//	objPlateM->getPosition(iniPosPlateM); //デフォルトなので飛ばさない
//
//	SimObj *objPlateL = getObj(plateLName.c_str());
//	objPlateL->getPosition(iniPosPlateL);
//	objPlateL->setPosition(iniPosPlateL.x(),iniPosPlateL.y(),iniPosPlateL.z()+1000); //遠くに飛ばす

//	// For Mirror therapy initialize.
//	this->reverseMode = reverseModes[RIGHTHAND]; // Set reverse mode.
//
//	// Set delay variables.
//	gettimeofday(&this->initTimeVal, NULL);
//	this->frameNumber = 0;
//	this->targetDelayTime = defaultDelayTime;
//	this->pastTimeAndPostures = std::vector<TimeAndPosture>(timeSeriesBufferSize);

	if(std::string(myname())==headName)
	{
		this->isHead = true;
	}
	//Set default avatar
	this->bodyAvatarName = this->middleArmManName;

//	if(this->isHead)
//	{
//		this->addPosition(plateSName, 0.0, 0.0, 1000.0); //遠くに飛ばす
//	}

	 //遠くに飛ばす
	if(std::string(myname())==this->bodyAvatarName)
	{
		this->kinectV2DeviceManager.iniPos.z = 0.0;
	}
	else
	{
		this->kinectV2DeviceManager.iniPos.z = 1000.0;
	}

	this->guiService = NULL;
}

///@brief Movement of the robot.
double SenseOfSelfController::onAction(ActionEvent &evt)
{
	if(this->isHead)
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

		if(tmpUsingOculus)
		{
			bool oculusDK2Available = checkService(this->oculusDK2DeviceManager.serviceName);

			if (oculusDK2Available && this->oculusDK2DeviceManager.service == NULL)
			{
				this->oculusDK2DeviceManager.service = connectToService(this->oculusDK2DeviceManager.serviceName);
			}
			else if (!oculusDK2Available && this->oculusDK2DeviceManager.service != NULL)
			{
				this->oculusDK2DeviceManager.service = NULL;
			}
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
	}

	return 1.0;
}


///@brief Message from device.
void SenseOfSelfController::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		if(!this->isHead && std::string(evt.getSender())!=headName){ return; }

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
			if(this->isHead)
			{
				sendMsg(this->middleArmManName, allMsg);
				sendMsg(this->longArmManName,   allMsg);
				sendMsg(this->shortArmManName,  allMsg);
//				sendMsg(this->robotArmManName,  allMsg);
//				sendMsg(this->bodyAvatarName,   allMsg);
			}

			std::string deviceTypeValue = sensorDataMap[MSG_KEY_DEV_TYPE][0];
			std::string deviceUniqueId  = sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0];

			/*
			 * Kinect V2
			 */
			if (deviceTypeValue == this->kinectV2DeviceManager.deviceType && deviceUniqueId == this->kinectV2DeviceManager.deviceUniqueID)
			{
				if(this->isHead && !this->kinectV2DeviceManager.started)
				{
					sendMsg(this->middleArmManName, allMsg);
					sendMsg(this->longArmManName,   allMsg);
					sendMsg(this->shortArmManName,  allMsg);
//					sendMsg(this->robotArmManName,  allMsg);
				}

				// Decode message to sensor data of kinect v2.
				KinectV2SensorData sensorData;
				sensorData.setSensorData(sensorDataMap);

				KinectV2DeviceManager::correctSlope(sensorData.jointPositions, DEG2RAD(this->correctionAngle));


//				this->correctSensorDataBeforeConvert(sensorData);

				// Convert kinect v2 quaternions(orientations) to man-nii posture(sigverse quaternion format).
//				ManNiiPosture posture = KinectV2DeviceManager::convertSensorData2ManNiiPosture(sensorData);
				ManNiiPosture posture;
				this->correctPostureAfterConvert(posture, sensorData);

				// Invalidate body rotation.
				if(this->isBodyFixed)
				{
					posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::WAIST_JOINT1].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);

					posture.joint[ManNiiPosture::LARM_JOINT2].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::LARM_JOINT3].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
//					posture.joint[ManNiiPosture::LARM_JOINT5].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);

					posture.joint[ManNiiPosture::RLEG_JOINT2].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::LLEG_JOINT2].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::RLEG_JOINT4].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::LLEG_JOINT4].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
				}

				if (this->usingOculus)
				{
					posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
//					posture.joint[ManNiiPosture::HEAD_JOINT1].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
				}

//				if(std::string(myname())==mirrorTherapyManName && false)
//				{
//					// Store current posture and time stamp.
//					int currentIndex = this->storeCurrentPosture(posture);
//
//					// Search nearest neighbor of target delay time in stored time stamps.
//					//   last 500 postures is in this->pastPostures.
//					//   find nearest index.
//					ManNiiPosture delayedPosture = this->getDelayedPosture(currentIndex);
//
//					this->invertPosture(posture, delayedPosture);
//				}

				// Set SIGVerse quaternions and positions
				SimObj *obj = getObj(myname());
				// Fixed waist position.
//				this->kinectV2DeviceManager.setRootPosition(obj, sensorData.rootPosition);
				KinectV2DeviceManager::setJointQuaternions2ManNii(obj, posture, sensorData);

//				if(this->isHeadFixed)
//				{
//					obj->setJointAngle("HEAD_JOINT1",  DEG2RAD(headAngle));
//				}
//
//				if(this->isBodyFixed)
//				{
//					obj->setJointAngle("WAIST_JOINT1", DEG2RAD(bodyAngle));
//				}
			}
			/*
			 * Oculus DK2
			 */
			else if (deviceTypeValue == this->oculusDK2DeviceManager.deviceType && deviceUniqueId ==  this->oculusDK2DeviceManager.deviceUniqueID)
			{
				if(tmpUsingOculus)
				{
					if(!this->usingOculus){ this->usingOculus = true; }

					OculusRiftDK2SensorData sensorData;
					sensorData.setSensorData(sensorDataMap);

					ManNiiPosture posture = OculusDK2DeviceManager::convertQuaternion2ManNiiPosture(sensorData.getQuaternion());

//					if(KinectV2DeviceManager::handMode==KinectV2DeviceManager::HandMode::NORMAL)
//					{
//						// Correct the angle of x-direction slope for neck.
//						Quaternion newNeckQuaternion = Quaternion::calcCrossProduct(this->correctBodyQuaternion, posture.joint[ManNiiPosture::HEAD_JOINT1].quaternion);
//						posture.joint[ManNiiPosture::HEAD_JOINT1].quaternion = newNeckQuaternion;
//					}

					SimObj *obj = getObj(myname());
					OculusDK2DeviceManager::setJointQuaternions2ManNii(obj, posture);
				}
			}
		}
		// Not a device.(Message from Change Avatar GUI)
		else
		{
			if(this->isHead)
			{
				sendMsg(this->middleArmManName, allMsg);
				sendMsg(this->longArmManName,   allMsg);
				sendMsg(this->shortArmManName,  allMsg);
//				sendMsg(this->robotArmManName,  allMsg);
			}

			this->changeAvatar(sensorDataMap);

//			if(std::string(myname())==mirrorTherapyManName && false)
//			{
//				this->setReverseModeAndDelayTime(sensorDataMap);
//			}
		}
	}
	catch(...)
	{
		LOG_MSG(("SIGViewer message is invalid."));
	}
}


void SenseOfSelfController::correctSensorDataBeforeConvert(KinectV2SensorData &sensorData)
{
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::SpineBase].position.x = 0.0;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::SpineBase].position.z = 0.0;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::Neck].position.x = 0.0;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::Neck].position.z = 0.0;

	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristLeft].position += sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HandLeft].position;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristLeft].position /= 2.0;

	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristRight].position += sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HandRight].position;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristRight].position /= 2.0;

	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HipLeft].position.y = 0.0;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HipLeft].position.z = 0.0;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HipRight].position.y = 0.0;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HipRight].position.z = 0.0;
}

void SenseOfSelfController::correctPostureAfterConvert(ManNiiPosture &posture, KinectV2SensorData &sensorData)
{
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristLeft].position += sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HandLeft].position;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristLeft].position /= 2.0;

	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristRight].position += sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::HandRight].position;
	sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristRight].position /= 2.0;


	SigCmn::Vector3 rshoul;
	rshoul.x = 0.0;
	rshoul.y = 0.0;
	rshoul.z = 0.0;

	SigCmn::Vector3 relb;
	double sqrt6 = sqrt(6.0);
	relb.x = +1.0 / sqrt6;
	relb.y = -2.0 / sqrt6;
	relb.z = -1.0 / sqrt6;

	SigCmn::Vector3 krsh_vec;
	SigCmn::diffVec(krsh_vec, rshoul, relb);

	SigCmn::Vector3 srsh_vec = ManNiiPosture::getSigVec(ManNiiPosture::RSHOULDER);
	posture.joint[ManNiiPosture::RARM_JOINT2].quaternion = Quaternion::calcQuaternionFromVector(srsh_vec, krsh_vec);

	Quaternion q_rarm_link2_inv = Quaternion::calcInverse(posture.joint[ManNiiPosture::RARM_JOINT2].quaternion);
	Quaternion q_rarm_link4 = SenseOfSelfController::calcHandQuaternion(sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::WristRight].position, sensorData.jointPositions[KinectV2SensorData::KinectV2JointType::ElbowRight].position, true);
	posture.joint[ManNiiPosture::RARM_JOINT3].quaternion = Quaternion::calcCrossProduct(q_rarm_link2_inv, q_rarm_link4);
}


//const double SenseOfSelfController::generateCurrentTimeStamp()
//{
//	// Get current timestamp.
//	struct timeval currentTimeVal;
//	gettimeofday(&currentTimeVal, NULL);
//	const double currentTimeStamp = (currentTimeVal.tv_sec - this->initTimeVal.tv_sec) * 1000.0 + (currentTimeVal.tv_usec - this->initTimeVal.tv_usec) / 1000.0;
//	return currentTimeStamp;
//}
//
//
//int SenseOfSelfController::storeCurrentPosture(const ManNiiPosture &posture)
//{
//	TimeAndPosture current;
//	current.timeStamp = this->generateCurrentTimeStamp();
//
//	for (int i = 0; i < ManNiiPosture::ManNiiJointType_Count; i++)
//	{
//		current.posture.joint[i] = posture.joint[i];
//	}
//
//	int index = this->frameNumber % timeSeriesBufferSize;
//	this->pastTimeAndPostures[index] = current;
//
//	this->frameNumber++;
//
//	return index;
//}
//
//
//ManNiiPosture SenseOfSelfController::getDelayedPosture(const int currentIndex)
//{
//	// If targetDelayTime <= 0, don't do anything.
//	if (this->targetDelayTime <= 0.0){ return this->pastTimeAndPostures[currentIndex].posture; }
//
//	// If elapsed time < targetDelayTime, don't do anything.
//	if(this->pastTimeAndPostures[currentIndex].timeStamp < this->targetDelayTime){ return this->pastTimeAndPostures[currentIndex].posture; }
//
//
//	const double idealPastTime = this->pastTimeAndPostures[currentIndex].timeStamp - this->targetDelayTime;
//
//	int delayedPostureIndex = 0;
//	double nearestTimeDistance = DBL_MAX;
//
//	// Search nearest index to the delay time.
//	for (int i = timeSeriesBufferSize-1; i >= 0; i--)
//	{
//		double tmpTimeDistance = abs(idealPastTime - this->pastTimeAndPostures[i].timeStamp);
//
//		if (tmpTimeDistance < nearestTimeDistance)
//		{
//			nearestTimeDistance = tmpTimeDistance;
//			delayedPostureIndex = i;
//		}
//	}
//
//	return this->pastTimeAndPostures[delayedPostureIndex].posture;
//}
//
//
//void SenseOfSelfController::invertPosture(ManNiiPosture &posture, const ManNiiPosture &delayedPosture)
//{
//	// left hand motion affects right hand.
//	if (this->reverseMode == reverseModes[RIGHTHAND])
//	{
//		// Get original quaternions.
//		double w2, x2, y2, z2, w3, x3, y3, z3;
//		delayedPosture.joint[ManNiiPosture::LARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
//		delayedPosture.joint[ManNiiPosture::LARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);
//
//		// Set reverse quaternions.
//		posture.joint[ManNiiPosture::RARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
//		posture.joint[ManNiiPosture::RARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
//	}
//	// right hand motion affects left hand.
//	else if (this->reverseMode == reverseModes[LEFTHAND])
//	{
//		// Get original quaternions.
//		double w2, x2, y2, z2, w3, x3, y3, z3;
//		delayedPosture.joint[ManNiiPosture::RARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
//		delayedPosture.joint[ManNiiPosture::RARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);
//
//		// Set reverse quaternions.
//		posture.joint[ManNiiPosture::LARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
//		posture.joint[ManNiiPosture::LARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
//	}
//}


void SenseOfSelfController::changeAvatar(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyAvatar) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyAvatar);

		if(this->bodyAvatarName == it->second[0]){ return; }

		this->bodyAvatarName = it->second[0];

		if(this->isHead)
		{
			std::cout << "Change Avatar:" << this->bodyAvatarName << std::endl;

			SimObj *objPlate60 = getObj(plate60Name.c_str());
			SimObj *objPlate90 = getObj(plate90Name.c_str());

			Vector3d posPlate60, posPlate90;

			objPlate60->getPosition(posPlate60);
			objPlate90->getPosition(posPlate90);

			double handLen = 28.0;
			double rate;

			if(it->second[0]==middleArmManName)
			{
				rate = 1.0;
			}
			else if(it->second[0]==longArmManName)
			{
				rate = 1.55;
			}
			else if(it->second[0]==shortArmManName)
			{
				rate = 0.75;
			}

			posPlate60.x(rate * handLen * sin(DEG2RAD(30))-18.0);
			posPlate60.z(rate * handLen * cos(DEG2RAD(30))+0.0);
			posPlate90.z(rate * handLen                   +0.0);

			objPlate60->setPosition(posPlate60);
			objPlate90->setPosition(posPlate90);

			std::cout << "plate60.x=" << posPlate60.x() << ", plate60.z=" << posPlate60.z() << std::endl;
			std::cout << "plate90.z=" << posPlate90.z() << std::endl;
		}
		else
		{
			if(std::string(myname())==this->bodyAvatarName)
			{
				this->kinectV2DeviceManager.iniPos.z = 0.0;
			}
			else
			{
				this->kinectV2DeviceManager.iniPos.z = 1000.0;
			}

			SigCmn::Vector3 zeroPos;
			zeroPos.x = 0.0;
			zeroPos.y = 0.0;
			zeroPos.z = 0.0;

			SimObj *my = getObj(myname());
//			this->kinectV2DeviceManager.setRootPosition(my, this->kinectV2DeviceManager.rootPos);
			this->kinectV2DeviceManager.setRootPosition(my, zeroPos);
		}
	}
	if (map.find(msgKeyDesk) != map.end())
	{
		if(this->isHead)
		{
			std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyDesk);
			double deskDY = std::atof(it->second[0].c_str());

			this->addPosition(deskName, 0.0, deskDY, 0.0);

			this->addPosition(plate60Name, 0.0, deskDY, 0.0);
			this->addPosition(plate90Name, 0.0, deskDY, 0.0);
		}
	}
	if (map.find(msgKeyPlate) != map.end())
	{
		if(this->isHead)
		{
			std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyPlate);
			double plateDX = std::atof(it->second[0].c_str());
			double plateDZ = std::atof(it->second[1].c_str());

			this->addPosition(plate60Name, plateDX, 0.0, plateDZ);
			this->addPosition(plate90Name, plateDX, 0.0, plateDZ);
		}
	}
}


void SenseOfSelfController::addPosition(const std::string name, const double dx, const double dy, const double dz)
{
	SimObj *objPlate = getObj(name.c_str());

	Vector3d posPlate;
	objPlate->getPosition(posPlate);
	objPlate->setPosition(posPlate.x()+dx, posPlate.y()+dy, posPlate.z()+dz);
}


//void SenseOfSelfController::setReverseModeAndDelayTime(const std::map<std::string, std::vector<std::string> > &map)
//{
//	if (map.find(msgKeyReverse) != map.end())
//	{
//		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyReverse);
//		this->reverseMode = it->second[0];
//
//		std::cout << "Set reverse mode:" << this->reverseMode << std::endl;
//	}
//
//	if (map.find(msgKeyDelay) != map.end())
//	{
//		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyDelay);
//		this->targetDelayTime = atof(it->second[0].c_str());
//
//		std::cout << "Set target delay time:" << this->targetDelayTime << std::endl;
//	}
//}


/*
 */
const Quaternion SenseOfSelfController::calcHandQuaternion2(const SigCmn::Vector3 &wristPos, const SigCmn::Vector3 &elbowPos, const bool directionIsPositive)
{
	//tilt 3 degrees.
	double correctionAngle=30;

	if(directionIsPositive)
	{
		correctionAngle *= +1.0;
	}
	else
	{
		correctionAngle *= -1.0;
	}

	double radZ = correctionAngle * M_PI / 180.0;

	Quaternion q_3 = Quaternion(std::cos(radZ/2.0),0,0,std::sin(radZ/2.0));

	//xz
	Quaternion q_xy = calcHandQuaternion(wristPos, elbowPos, directionIsPositive);

	Quaternion retVal;

	retVal = Quaternion::calcCrossProduct(q_xy,q_3);
	retVal = Quaternion::calcCrossProduct(retVal,Quaternion::calcInverse(q_xy));

	return retVal;
}


/*
 */
const Quaternion SenseOfSelfController::calcHandQuaternion(const SigCmn::Vector3 &wristPos, const SigCmn::Vector3 &elbowPos, const bool directionIsPositive)
{
	double dx = wristPos.x - elbowPos.x;
	double dz = wristPos.z - elbowPos.z;

	double norm = std::sqrt(dx*dx + dz*dz);

	if(norm==0){ return Quaternion(1.0, 0.0, 0.0, 0.0); }

	double dxDivNorm = dx / norm;
	double rad;

	if(directionIsPositive)
	{
		rad = +std::acos(+dxDivNorm);
	}
	else
	{
		rad = -std::acos(-dxDivNorm);
	}

	Quaternion retVal;

	double sinVal = std::sin(rad/2);

	retVal.w = std::cos(rad/2);
	retVal.x = 0.0 * sinVal;
	retVal.y = 1.0 * sinVal;
	retVal.z = 0.0 * sinVal;

	return retVal;
}



///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void SenseOfSelfController::readIniFileAndInitialize()
{
	std::ifstream ifs(parameterFileName.c_str());

	std::string kinectV2ServiceName;
	std::string kinectV2DeviceType;
	std::string kinectV2DeviceUniqueID;
	double      scaleRatio;
	std::string sensorDataModeStr;
//	std::string kinectV2HandModeStr;

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
	try
	{
		std::cout << "Read " << parameterFileName << std::endl;
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(parameterFileName, pt);

		kinectV2ServiceName    = pt.get<std::string>(paramFileKeyKinectV2ServiceName);
		kinectV2DeviceType     = pt.get<std::string>(paramFileKeyKinectV2Devicetype);
		kinectV2DeviceUniqueID = pt.get<std::string>(paramFileKeyKinectV2DeviceUniqueID);

		sensorDataModeStr     = pt.get<std::string>(paramFileKeyKinectV2SensorDataMode);
		scaleRatio            = pt.get<double>     (paramFileKeyKinectV2ScaleRatio);
		this->correctionAngle = pt.get<double>     (paramFileKeyKinectV2CorrectionAngle);
//		kinectV2HandModeStr   = pt.get<std::string>(paramFileKeyKinectV2HandMode);

		oculusDK2ServiceName    = pt.get<std::string>(paramFileKeyOculusDK2ServiceName);
		oculusDK2DeviceType     = pt.get<std::string>(paramFileKeyOculusDK2Devicetype);
		oculusDK2DeviceUniqueID = pt.get<std::string>(paramFileKeyOculusDK2DeviceUniqueID);

		this->guiServiceName = pt.get<std::string>(paramFileKeyChangeAvatarGUIServiceName);
	}
	catch (boost::exception &ex)
	{
		std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
	}

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



extern "C" Controller * createController()
{
	return new SenseOfSelfController;
}

