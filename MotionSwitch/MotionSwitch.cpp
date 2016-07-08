/*
 * MotionSwitch
 *
 *  Created on: 2016/07/08
 *      Author: Yamada
 */

#include "MotionSwitch.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/SensorData.h>
#include <cmath>

///@brief Initialize this controller.
void MotionSwitch::onInit(InitEvent &evt)
{
	this->readIniFileAndInitialize();

	SimObj *myself = getObj(myname());

	this->perceptionNeuronDeviceManager.getInitialPositionAndRotation(myself);
}


double MotionSwitch::onAction(ActionEvent &evt)
{
	static int roopCnt = 0;

	dReal waistQuaternionArray[4];

	// Get the absolute quaternion of waist link.
	SimObj *obj = getObj(myname());
	obj->getPartsQuaternion(waistQuaternionArray[0], waistQuaternionArray[1], waistQuaternionArray[2], waistQuaternionArray[3], "W_L1");
	this->waistLinkQuaternion.setQuaternion(waistQuaternionArray[0], waistQuaternionArray[1], waistQuaternionArray[2], waistQuaternionArray[3]);

	if(roopCnt % 100 == 0)
	{
		bool perceptionNeuronAvailable = checkService(this->perceptionNeuronDeviceManager.serviceName);

		if (perceptionNeuronAvailable && this->perceptionNeuronDeviceManager.service == NULL)
		{
			this->perceptionNeuronDeviceManager.service = connectToService(this->perceptionNeuronDeviceManager.serviceName);
		}
		else if (!perceptionNeuronAvailable && this->perceptionNeuronDeviceManager.service != NULL)
		{
			this->perceptionNeuronDeviceManager.service = NULL;
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
	}
	roopCnt++;

	return 0.01;
}

///@brief Receive Message.
void MotionSwitch::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		std::string allMsg = evt.getMsg();

//		std::cout << "msg:" << allMsg << std::endl;

		// Decode message to sensor data of Perception Neuron.
		std::map<std::string, std::vector<std::string> > sensorDataMap = PerceptionNeuronSensorData::decodeSensorData(allMsg);

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
			if (deviceTypeValue == this->perceptionNeuronDeviceManager.deviceType && deviceUniqueId == this->perceptionNeuronDeviceManager.deviceUniqueID)
			{
				// Make sensor data.
				PerceptionNeuronSensorData sensorData;

				sensorData.setSensorData(sensorDataMap);

				ManBvhPosture posture = this->perceptionNeuronDeviceManager.convertSensorData2ManBvhPosture(sensorData);

				if (this->usingOculus)
				{
					posture.joint[ManBvhPosture::NECK_JOINT].isValid = false;
				}

				// Set the posture to avatar.
				SimObj *obj = getObj(myname());
				this->perceptionNeuronDeviceManager.setPosture2ManBvhYXZ(obj, posture);
			}
			/*
			 * Oculus Rift DK2
			 */
			else if (deviceTypeValue == this->oculusDK2DeviceManager.deviceType && deviceUniqueId ==  this->oculusDK2DeviceManager.deviceUniqueID)
			{
				/*
				 * Move the head to the main body.
				 */
				OculusRiftDK2SensorData sensorData;
				sensorData.setSensorData(sensorDataMap);

				ManNiiPosture posture = OculusDK2DeviceManager::convertQuaternion2ManNiiPosture(sensorData.getQuaternion());

				Quaternion headQuaternion = posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion;

				// Change the absolute quaternion into the relative quaternion.
				headQuaternion = Quaternion::multiplyNI(this->waistLinkQuaternion, headQuaternion);

				SimObj *obj = getObj(myname());

				if(!this->usingOculus)
				{
					obj->setJointAngle("NECK_JOINT_X", 0.0);
					obj->setJointAngle("NECK_JOINT_Y", 0.0);
					obj->setJointAngle("NECK_JOINT_Z", 0.0);

					this->usingOculus = true;
				}

				this->oculusDK2DeviceManager.setJointQuaternion(obj, "NECK_JOINT_Y", headQuaternion);
			}
		}
	}
	catch(SimObj::Exception &err)
	{
		LOG_MSG(("Exception: %s", err.msg()));
	}
	catch(std::string &err)
	{
		LOG_MSG(("Exception: %s", err.c_str()));
	}
	catch(...)
	{
		std::cout << "some error occurred." << std::endl;
	}
}


///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void MotionSwitch::readIniFileAndInitialize()
{
	try
	{
		std::ifstream ifs(parameterFileName.c_str());

		std::string perceptionNeuronServiceName;
		std::string perceptionNeuronDeviceType;
		std::string perceptionNeuronDeviceUniqueID;

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

		perceptionNeuronServiceName    = pt.get<std::string>(paramFileKeyPerceptionNeuronServiceName);
		perceptionNeuronDeviceType     = pt.get<std::string>(paramFileKeyPerceptionNeuronDevicetype);
		perceptionNeuronDeviceUniqueID = pt.get<std::string>(paramFileKeyPerceptionNeuronDeviceUniqueID);

		oculusDK2ServiceName    = pt.get<std::string>(paramFileKeyOculusDK2ServiceName);
		oculusDK2DeviceType     = pt.get<std::string>(paramFileKeyOculusDK2Devicetype);
		oculusDK2DeviceUniqueID = pt.get<std::string>(paramFileKeyOculusDK2DeviceUniqueID);

		std::cout << paramFileKeyPerceptionNeuronServiceName    << ":" << perceptionNeuronServiceName    << std::endl;
		std::cout << paramFileKeyPerceptionNeuronDevicetype     << ":" << perceptionNeuronDeviceType     << std::endl;
		std::cout << paramFileKeyPerceptionNeuronDeviceUniqueID << ":" << perceptionNeuronDeviceUniqueID << std::endl;

		std::cout << paramFileKeyOculusDK2ServiceName    << ":" << oculusDK2ServiceName    << std::endl;
		std::cout << paramFileKeyOculusDK2Devicetype     << ":" << oculusDK2DeviceType     << std::endl;
		std::cout << paramFileKeyOculusDK2DeviceUniqueID << ":" << oculusDK2DeviceUniqueID << std::endl;

		this->perceptionNeuronDeviceManager = PerceptionNeuronDeviceManager(perceptionNeuronServiceName, perceptionNeuronDeviceType, perceptionNeuronDeviceUniqueID);
		this->oculusDK2DeviceManager        = OculusDK2DeviceManager(oculusDK2ServiceName, oculusDK2DeviceType, oculusDK2DeviceUniqueID);
	}
	catch (boost::exception &ex)
	{
		std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
	}
}



extern "C" Controller * createController()
{
	return new MotionSwitch;
}

