/*
 * SenseOfSelfController.h
 *
 *  Created on: 2015/06/08
 *      Author: Yamada@tome
 */
#ifndef SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_H
#define SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>
#include <sigverse/commonlib/comm/controller/Controller.h>

#include <sigverse/plugin/controller/common/AvatarController.h>
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
//	///@brief Structure of posture record (with time stamp).
//	struct TimeAndPostureType
//	{
//		long timeStamp;
//
//		Quaternion qLARM_JOINT2;
//		Quaternion qLARM_JOINT3;
//		Quaternion qRARM_JOINT2;
//		Quaternion qRARM_JOINT3;
////
////		ManNiiPosture posture;
//	};
//
//	///@brief Posture record.
//	std::vector<TimeAndPostureType> pastPostures;

	///@brief Parameter file name.
	static const std::string parameterFileName;

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
//	static const std::string msgKeyReverse;
//	static const std::string msgKeyDelay;
	static const std::string msgKeyDesk;
	static const std::string msgKeyPlate;

//	static const double defaultDelayTime;
//	static const int    timeSeriesBufferSize;
//
//	enum ReverseModeType
//	{
//		RIGHTHAND = 0,
//		LEFTHAND  = 1,
//		NOREVERSE = 2,
//		ReverseMode_Count = (NOREVERSE + 1)
//	};
//
//	static const std::string reverseModes[ReverseMode_Count];

	static const std::string headName;
	static const std::string middleArmManName;
	static const std::string longArmManName;
	static const std::string shortArmManName;
//	static const std::string robotArmManName;

	static const std::string plate60Name;
	static const std::string plate90Name;
//	static const std::string plateLName;

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

//	///@brief Generate time stamp for posture record.
//	const double generateCurrentTimeStamp();
//
//	int storeCurrentPosture(const ManNiiPosture &posture);
//
//	ManNiiPosture getDelayedPosture(const int currentIndex);
//
//	void invertPosture(ManNiiPosture &posture, const ManNiiPosture &delayedPosture);

	void changeAvatar(const std::map<std::string, std::vector<std::string> > &map);

	void addPosition(const std::string name, const double dx, const double dy, const double dz);

//	///@brief Modify the mode for MirrorTherapy.
//	void setReverseModeAndDelayTime(const std::map<std::string, std::vector<std::string> > &map);

	static const Quaternion calcHandQuaternion(const SigCmn::Vector3 &wristPos, const SigCmn::Vector3 &elbowPos, const bool directionIsPositive);
	static const Quaternion calcHandQuaternion2(const SigCmn::Vector3 &wristPos, const SigCmn::Vector3 &elbowPos, const bool directionIsPositive);


	KinectV2DeviceManager  kinectV2DeviceManager;
	OculusDK2DeviceManager oculusDK2DeviceManager;

//	///@brief For Mirror therapy variables.
//	std::string reverseMode;
//
//	///@brief Time stamp on initialize.
//	timeval initTimeVal;
//
//	///@brief Frame numbers during receive message from kinect v2 plugin.
//	unsigned long frameNumber;
//
//	///@brief Target delay time.
//	double targetDelayTime;
//
//	///@brief Posture record.
//	std::vector<TimeAndPosture> pastTimeAndPostures;


	bool isHead = false;
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

//	Quaternion correctBodyQuaternion;
//	Quaternion correctForeQuaternion;
//	Vector3d iniPosPlateS, iniPosPlateM, iniPosPlateL;
};


const std::string SenseOfSelfController::parameterFileName = "SenseOfSelf.ini";

const std::string SenseOfSelfController::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string SenseOfSelfController::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string SenseOfSelfController::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string SenseOfSelfController::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string SenseOfSelfController::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";
const std::string SenseOfSelfController::paramFileKeyKinectV2CorrectionAngle= "KinectV2.correction_angle";
//const std::string SenseOfSelfController::paramFileKeyKinectV2HandMode       = "KinectV2.hand_mode";


const std::string SenseOfSelfController::paramFileKeyOculusDK2ServiceName   = "OculusDK2.service_name";
const std::string SenseOfSelfController::paramFileKeyOculusDK2Devicetype    = "OculusDK2.device_type";
const std::string SenseOfSelfController::paramFileKeyOculusDK2DeviceUniqueID= "OculusDK2.device_unique_id";

const std::string SenseOfSelfController::paramFileKeyChangeAvatarGUIServiceName = "ChangeAvatarGUI.service_name";

const std::string SenseOfSelfController::msgKeyAvatar  = "AVATAR";
//const std::string SenseOfSelfController::msgKeyReverse = "REVERSE";
//const std::string SenseOfSelfController::msgKeyDelay   = "DELAY";
const std::string SenseOfSelfController::msgKeyDesk  = "DESK";
const std::string SenseOfSelfController::msgKeyPlate  = "PLATE";

//const double SenseOfSelfController::defaultDelayTime      = 1000.0;
//const int    SenseOfSelfController::timeSeriesBufferSize  = 500;
//
//const std::string SenseOfSelfController::reverseModes[ReverseMode_Count] = { "RIGHTHAND", "LEFTHAND", "NOREVERSE" };

const std::string SenseOfSelfController::headName         = "man_nii_head";
const std::string SenseOfSelfController::middleArmManName = "arm_middle_man";
const std::string SenseOfSelfController::longArmManName   = "arm_long_man";
const std::string SenseOfSelfController::shortArmManName  = "arm_short_man";
//const std::string SenseOfSelfController::robotArmManName      = "right_arm_robot_man";

const std::string SenseOfSelfController::plate60Name = "plate60";
const std::string SenseOfSelfController::plate90Name = "plate90";
//const std::string SenseOfSelfController::plateLName = "plateL";

const std::string SenseOfSelfController::deskName = "table";

#endif // SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_H
