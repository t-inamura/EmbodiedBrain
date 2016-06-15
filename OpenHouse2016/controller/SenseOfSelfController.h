/*
 * SenseOfSelfController.h
 *
 *  Created on: 2016/04/26
 *      Author: Yamada
 */
#ifndef SIGVERSE_SENSE_OF_SELF_CONTROLLER_H
#define SIGVERSE_SENSE_OF_SELF_CONTROLLER_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>
#include <sigverse/commonlib/comm/controller/Controller.h>

#include <sigverse/plugin/controller/common/AgentController.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/controller/common/sensor/KinectV2DeviceManager.h>
#include <sigverse/plugin/controller/common/sensor/OculusDK2DeviceManager.h>
#include <sigverse/plugin/common/sensor/KinectV2SensorData.h>
#include <sigverse/plugin/common/sensor/OculusRiftDK2SensorData.h>

#define DEG2RAD(DEG) ( (M_PI) * (DEG) / 180.0 )

class SenseOfSelfController : public ManNiiAvatarController
{
public:
	//Parameter file key name.
	static const std::string paramFileKeyKinectV2ServiceName;
	static const std::string paramFileKeyKinectV2Devicetype;
	static const std::string paramFileKeyKinectV2DeviceUniqueID;

	static const std::string paramFileKeyKinectV2SensorDataMode;
	static const std::string paramFileKeyKinectV2ScaleRatio;
	static const std::string paramFileKeyKinectV2CorrectionAngle;
//	static const std::string paramFileKeyKinectV2HandMode;

	static const std::string paramFileKeyOculusDK2ServiceName;
	static const std::string paramFileKeyOculusDK2Devicetype;
	static const std::string paramFileKeyOculusDK2DeviceUniqueID;

	static const std::string paramFileKeyChangeAvatarGUIServiceName;

	//static param
	static const std::string msgKeyAvatar;
	static const std::string msgKeyDesk;
	static const std::string msgKeyPlate;

	static const std::string middleArmManName;
	static const std::string longArmManName;
	static const std::string shortArmManName;

	static const std::string plate60Name;
	static const std::string plate90Name;

	static const std::string deskName;

	///@brief Structure of posture record (with time stamp).
	struct TimeAndPosture
	{
		long timeStamp;
		ManNiiPosture posture;
	};

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	void readIniFileAndInitialize();

	void correctSensorDataBeforeConvert(KinectV2SensorData &sensorData);
	void correctPostureAfterConvert(ManNiiPosture &posture, KinectV2SensorData &sensorData);

	void changeAvatar(const std::map<std::string, std::vector<std::string> > &map);

	void addPosition(const std::string name, const double dx, const double dy, const double dz);

	static const Quaternion calcHandQuaternion(const SigCmn::Vector3 &wristPos, const SigCmn::Vector3 &elbowPos, const bool directionIsPositive);
	static const Quaternion calcHandQuaternion2(const SigCmn::Vector3 &wristPos, const SigCmn::Vector3 &elbowPos, const bool directionIsPositive);


	KinectV2DeviceManager  kinectV2DeviceManager;
	OculusDK2DeviceManager oculusDK2DeviceManager;

//	bool isHead = false;
	bool usingOculus = false;

	//tmp parameter
	bool isBodyFixed = true;
	double bodyAngle = 0;
	bool isHeadFixed = false; //headFixedの時はOculusは使用しない前提
	double headAngle = 40;
	bool tmpUsingOculus = true;

	std::string bodyAvatarName;

	BaseService *guiService;
	std::string guiServiceName;

	double correctionAngle;

	Quaternion rootJointQuaternion, waistJointQuaternion;
};


const std::string AgentController::parameterFileName = "SenseOfSelf.ini";

const std::string SenseOfSelfController::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string SenseOfSelfController::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string SenseOfSelfController::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string SenseOfSelfController::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string SenseOfSelfController::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";
const std::string SenseOfSelfController::paramFileKeyKinectV2CorrectionAngle= "KinectV2.correction_angle";


const std::string SenseOfSelfController::paramFileKeyOculusDK2ServiceName   = "OculusDK2.service_name";
const std::string SenseOfSelfController::paramFileKeyOculusDK2Devicetype    = "OculusDK2.device_type";
const std::string SenseOfSelfController::paramFileKeyOculusDK2DeviceUniqueID= "OculusDK2.device_unique_id";

const std::string SenseOfSelfController::paramFileKeyChangeAvatarGUIServiceName = "ChangeAvatarGUI.service_name";

const std::string SenseOfSelfController::msgKeyAvatar  = "AVATAR";
const std::string SenseOfSelfController::msgKeyDesk  = "DESK";
const std::string SenseOfSelfController::msgKeyPlate  = "PLATE";


const std::string SenseOfSelfController::middleArmManName = "sos_arm_middle_man";
const std::string SenseOfSelfController::longArmManName   = "sos_arm_long_man";
const std::string SenseOfSelfController::shortArmManName  = "sos_arm_short_man";

const std::string SenseOfSelfController::plate60Name = "sos_plate60";
const std::string SenseOfSelfController::plate90Name = "sos_plate90";

const std::string SenseOfSelfController::deskName = "sos_table";

#endif // SIGVERSE_SENSE_OF_SELF_CONTROLLER_H
