/*
 * HeadController.h
 *
 *  Created on: 2016/04/06
 *      Author: Yamada
 */
#ifndef SIGVERSE_HEAD_CONTROLLER_H
#define SIGVERSE_HEAD_CONTROLLER_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>
#include <sigverse/commonlib/comm/controller/Controller.h>

#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/controller/common/sensor/KinectV2DeviceManager.h>
#include <sigverse/plugin/controller/common/sensor/OculusDK2DeviceManager.h>
#include <sigverse/plugin/common/sensor/KinectV2SensorData.h>
#include <sigverse/plugin/common/sensor/OculusRiftDK2SensorData.h>

class HeadController : public ManNiiAvatarController
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
	static const std::string msgKeyRoom;
	static const std::string msgKeyAvatar;

	static const std::string msgValueRoomMirrorTherapy;
	static const std::string msgValueRoomLinkage;
	static const std::string msgValueRoomSenseOfSelf;

	static const std::string audienceName;
	static const std::string mirrorTherapyManName;
	static const std::string mirrorArmLongManName;
	static const std::string mirrorArmShortManName;
	static const std::string mirrorRightArmRobotManName;
	static const std::string linkageManName;
	static const std::string sosArmMiddleManName;
	static const std::string sosArmLongManName;
	static const std::string sosArmShortManName;

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	void readIniFileAndInitialize();


	void changeAvatar(const std::map<std::string, std::vector<std::string> > &map);

	KinectV2DeviceManager  kinectV2DeviceManager;
	OculusDK2DeviceManager oculusDK2DeviceManager;

	std::string bodyAvatarName;

	BaseService *guiService;
	std::string guiServiceName;

	double correctionAngle;
};


const std::string AgentController::parameterFileName = "Head.ini";

const std::string HeadController::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string HeadController::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string HeadController::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string HeadController::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string HeadController::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";
const std::string HeadController::paramFileKeyKinectV2CorrectionAngle= "KinectV2.correction_angle";


const std::string HeadController::paramFileKeyOculusDK2ServiceName   = "OculusDK2.service_name";
const std::string HeadController::paramFileKeyOculusDK2Devicetype    = "OculusDK2.device_type";
const std::string HeadController::paramFileKeyOculusDK2DeviceUniqueID= "OculusDK2.device_unique_id";

const std::string HeadController::paramFileKeyChangeAvatarGUIServiceName = "ChangeAvatarGUI.service_name";

const std::string HeadController::msgKeyRoom    = "ROOM";
const std::string HeadController::msgKeyAvatar  = "AVATAR";

const std::string HeadController::msgValueRoomMirrorTherapy  = "MirrorTherapy";
const std::string HeadController::msgValueRoomLinkage        = "Linkage";
const std::string HeadController::msgValueRoomSenseOfSelf    = "SenseOfSelf";

const std::string HeadController::audienceName               = "Audience";
const std::string HeadController::mirrorTherapyManName       = "mirror_therapy_man";
const std::string HeadController::mirrorArmLongManName       = "mirror_arm_long_man";
const std::string HeadController::mirrorArmShortManName      = "mirror_arm_short_man";
const std::string HeadController::mirrorRightArmRobotManName = "mirror_right_arm_robot_man";
const std::string HeadController::linkageManName             = "linkage_man";
const std::string HeadController::sosArmMiddleManName        = "sos_arm_middle_man";
const std::string HeadController::sosArmLongManName          = "sos_arm_long_man";
const std::string HeadController::sosArmShortManName         = "sos_arm_short_man";

#endif // SIGVERSE_HEAD_CONTROLLER_H
