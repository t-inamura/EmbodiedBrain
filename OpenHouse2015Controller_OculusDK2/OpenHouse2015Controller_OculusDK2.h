/*
 * OpenHouse2015Controller_OculusDK2.h
 *
 *  Created on: 2016/04/06
 *      Author: Yamada
 */
#ifndef SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_H
#define SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>
#include <sigverse/commonlib/comm/controller/Controller.h>

#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/controller/common/sensor/KinectV2DeviceManager.h>
#include <sigverse/plugin/controller/common/sensor/OculusDK2DeviceManager.h>
#include <sigverse/plugin/common/sensor/KinectV2SensorData.h>
#include <sigverse/plugin/common/sensor/OculusRiftDK2SensorData.h>

class OpenHouse2015Controller_OculusDK2 : public ManNiiAvatarController
{
public:
	//Parameter file key name.
	static const std::string paramFileKeyKinectV2ServiceName;
	static const std::string paramFileKeyKinectV2Devicetype;
	static const std::string paramFileKeyKinectV2DeviceUniqueID;

	static const std::string paramFileKeyKinectV2SensorDataMode;
	static const std::string paramFileKeyKinectV2ScaleRatio;
	static const std::string paramFileKeyKinectV2CorrectionAngle;

	static const std::string paramFileKeyOculusDK2ServiceName;
	static const std::string paramFileKeyOculusDK2Devicetype;
	static const std::string paramFileKeyOculusDK2DeviceUniqueID;

	static const std::string paramFileKeyChangeAvatarGUIServiceName;

	//static param
	static const std::string msgKeyAvatar;
	static const std::string msgKeyReverse;
	static const std::string msgKeyDelay;

	static const double defaultDelayTime;
	static const int    timeSeriesBufferSize;

	enum ReverseModeType
	{
		RIGHTHAND = 0,
		LEFTHAND  = 1,
		NOREVERSE = 2,
		ReverseMode_Count = (NOREVERSE + 1)
	};

	static const std::string reverseModes[ReverseMode_Count];

	static const std::string headName;
	static const std::string middleArmManName;
	static const std::string longArmManName;
	static const std::string shortArmManName;
	static const std::string robotArmManName;

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


	///@brief Generate time stamp for posture record.
	const double generateCurrentTimeStamp();

	int storeCurrentPosture(const ManNiiPosture &posture);

	ManNiiPosture getDelayedPosture(const int currentIndex);

	void invertPosture(ManNiiPosture &posture, const ManNiiPosture &delayedPosture);

	void changeAvatar(const std::map<std::string, std::vector<std::string> > &map);

	///@brief Modify the mode for MirrorTherapy.
	void setReverseModeAndDelayTime(const std::map<std::string, std::vector<std::string> > &map);


	KinectV2DeviceManager  kinectV2DeviceManager;
	OculusDK2DeviceManager oculusDK2DeviceManager;

	///@brief For Mirror therapy variables.
	std::string reverseMode;

	///@brief Time stamp on initialize.
	timeval initTimeVal;

	///@brief Frame numbers during receive message from kinect v2 plugin.
	unsigned long frameNumber;

	///@brief Target delay time.
	double targetDelayTime;

	///@brief Posture record.
	std::vector<TimeAndPosture> pastTimeAndPostures;


	bool isHead = false;
	bool usingOculus = false;
	Quaternion rootJointQuaternion, waistJointQuaternion;

	std::string bodyAvatarName;

	BaseService *guiService;
	std::string guiServiceName;

	double correctionAngle;
};


const std::string AgentController::parameterFileName = "OpenHouse2015_OculusDK2.ini";

const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";
const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyKinectV2CorrectionAngle= "KinectV2.correction_angle";


const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyOculusDK2ServiceName   = "OculusDK2.service_name";
const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyOculusDK2Devicetype    = "OculusDK2.device_type";
const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyOculusDK2DeviceUniqueID= "OculusDK2.device_unique_id";

const std::string OpenHouse2015Controller_OculusDK2::paramFileKeyChangeAvatarGUIServiceName = "ChangeAvatarGUI.service_name";

const std::string OpenHouse2015Controller_OculusDK2::msgKeyAvatar  = "AVATAR";
const std::string OpenHouse2015Controller_OculusDK2::msgKeyReverse = "REVERSE";
const std::string OpenHouse2015Controller_OculusDK2::msgKeyDelay   = "DELAY";

const double OpenHouse2015Controller_OculusDK2::defaultDelayTime      = 1000.0;
const int    OpenHouse2015Controller_OculusDK2::timeSeriesBufferSize  = 500;

const std::string OpenHouse2015Controller_OculusDK2::reverseModes[ReverseMode_Count] = { "RIGHTHAND", "LEFTHAND", "NOREVERSE" };

const std::string OpenHouse2015Controller_OculusDK2::headName             = "man_nii_head";
const std::string OpenHouse2015Controller_OculusDK2::middleArmManName     = "mirror_therapy_man";
const std::string OpenHouse2015Controller_OculusDK2::longArmManName       = "right_arm_long_man";
const std::string OpenHouse2015Controller_OculusDK2::shortArmManName      = "right_arm_short_man";
const std::string OpenHouse2015Controller_OculusDK2::robotArmManName      = "right_arm_robot_man";

#endif // SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_H
