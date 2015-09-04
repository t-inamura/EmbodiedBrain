/*
 * LinkageControllerOnRecvMsg.cpp
 *
 *  Created on: 2015/09/04
 *      Author: Yamada@tome
 */

#include "LinkageController.h"

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

				// Save Hand state.
				this->rightHandStateHistory.push_front(sensorData.getRightHandState());
				this->leftHandStateHistory .push_front(sensorData.getLeftHandState());
				while(this->rightHandStateHistory.size()>numOfHandStateHistory){ this->rightHandStateHistory.pop_back(); }
				while(this->leftHandStateHistory .size()>numOfHandStateHistory){ this->leftHandStateHistory .pop_back(); }

				// Convert kinect v2 quaternions(orientations) to man-nii posture(sigverse quaternion format).
				ManNiiPosture posture = KinectV2DeviceManager::convertSensorData2ManNiiPosture(sensorData);

				// When using oculus, set invalid head quaternion. (When all values are zero, it means invalid.)
				if (this->usingOculus)
				{
					posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::HEAD_JOINT1].quaternion.setQuaternion(0.0, 0.0, 0.0, 0.0);
				}

				double radx = this->correctionAngle * M_PI / 180.0;
				Quaternion correctQuaternion(std::cos(radx/2.0),std::sin(radx/2.0),0.0,0.0);
				Quaternion newQuaternion = Quaternion::calcCrossProduct(posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion, correctQuaternion);
				posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion = newQuaternion;

				// Invalidate body rotation.
				if(this->isWaistFixed)
				{
					posture.joint[ManNiiPosture::ROOT_JOINT0].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
					posture.joint[ManNiiPosture::WAIST_JOINT1].quaternion.setQuaternion(1.0, 0.0, 0.0, 0.0);
				}

				// Invert posture.
				if(this->reverseMode != reverseModes[NOT_REVERSE])
				{
					this->makeInvertPosture(posture);
				}

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
		LOG_MSG(("Message is invalid."));
	}
}


/*
 * Make invert posture from current posture.
 */
void LinkageController::makeInvertPosture(ManNiiPosture &posture)
{
	// Left hand motion affects right hand.
	if (this->limbMode ==Cmn::limbModes[CmnLMT::HAND] && this->reverseMode == reverseModes[RIGHT])
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
	else if (this->limbMode ==Cmn::limbModes[CmnLMT::HAND] && this->reverseMode == reverseModes[LEFT])
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
	else if (this->limbMode ==Cmn::limbModes[CmnLMT::FOOT] && this->reverseMode == reverseModes[RIGHT])
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
	else if (this->limbMode ==Cmn::limbModes[CmnLMT::FOOT] && this->reverseMode == reverseModes[LEFT])
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

	// Change Grasp mode.
	this->changeGraspMode(map);

	// Change Reverse mode.
	this->changeReverseMode(map);

	// Change Fixed Waist option.
	this->changeFixedWaist(map);
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
			std::cout << "Limb mode is already " << this->limbMode << "." << std::endl;
			return;
		}

		if(it->second[0]==Cmn::limbModes[CmnLMT::HAND])
		{
			this->resetVariables4Hand();
		}
		else if(it->second[0]==Cmn::limbModes[CmnLMT::FOOT])
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
 * Change grasp mode.
 */
void LinkageController::changeGraspMode(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyGraspMode) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyGraspMode);

		if(this->graspMode == it->second[0])
		{
			std::cout << "Grasp mode is already " << this->graspMode << "." << std::endl;
			return;
		}

		if(it->second[0]==this->graspModes[GRASP_RIGHT] || it->second[0]==this->graspModes[GRASP_LEFT] || it->second[0]==this->graspModes[SANDWICH])
		{
			this->resetVariables4GraspMode(it->second[0]);
		}
		else
		{
			std::cout << "It is invalid value. GRASP_MODE=" << it->second[0] << std::endl;
			return;
		}

		std::cout << "Set grasp mode:" << this->graspMode << std::endl;
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
			std::cout << "Reverse mode is already " << this->reverseMode << "." << std::endl;
			return;
		}

		if(it->second[0]==reverseModes[LEFT] || it->second[0]==reverseModes[RIGHT] || it->second[0]==reverseModes[NOT_REVERSE])
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
 * Change Fixed Waist option.
 */
void LinkageController::changeFixedWaist(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(msgKeyFixedWaist) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(msgKeyFixedWaist);

		if(this->isWaistFixed == boost::iequals(it->second[0], "true"))
		{
			std::cout << "Fixed Waist option is already " << it->second[0] << "." << std::endl;
			return;
		}

		this->isWaistFixed = boost::iequals(it->second[0], "true");

		std::cout << "Set Fixed Waist option:" << this->isWaistFixed << std::endl;
	}
}


