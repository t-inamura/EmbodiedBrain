/*
 * LinkageController.h
 *
 *  Created on: 2015/08/25
 *      Author: Yamada@tome
 */
#ifndef SIGVERSE_LINKAGE_CONTROLLER_H
#define SIGVERSE_LINKAGE_CONTROLLER_H

#include <sigverse/Controller.h>
#include <sigverse/ControllerEvent.h>
#include <sigverse/comm/controller/Controller.h>

#include <sigverse/common/device/KinectV2SensorData.h>
#include <sigverse/common/device/OculusRiftDK1SensorData.h>
#include <sigverse/controller/common/AvatarController.h>
#include <sigverse/controller/common/ManNiiPosture.h>
#include <sigverse/controller/MirrorTherapyController/MirrorTherapyController.h>

#include <thread>

class LinkageController : public ManNiiAvatarController
{
public:
	///@brief Parameter file name.
	static const std::string parameterFileName;

	//Parameter file key name.
	static const std::string paramFileKeyKinectV2ServiceName;
	static const std::string paramFileKeyKinectV2Devicetype;
	static const std::string paramFileKeyKinectV2DeviceUniqueID;

	static const std::string paramFileKeyKinectV2SensorDataMode;
	static const std::string paramFileKeyKinectV2ScaleRatio;

	static const std::string paramFileKeyOculusDK1ServiceName;
	static const std::string paramFileKeyOculusDK1Devicetype;
	static const std::string paramFileKeyOculusDK1DeviceUniqueID;

	static const std::string paramFileValKinectV2SensorDataModeDefault;
	static const double      paramFileValKinectV2ScaleRatioDefault;

	static const std::string paramFileKeyChangeModeGUIServiceName;
	static const std::string paramFileValChangeModeGUIServiceNameDefault;

	//static param
	static const std::string msgKeyLimbMode;
	static const std::string msgKeyReverse;

	enum LimbModeType
	{
		HAND = 0,
		FOOT = 1,
		LimbMode_Count = (FOOT + 1)
	};

	enum ReverseModeType
	{
		RIGHT     = 0,
		LEFT      = 1,
		NOREVERSE = 2,
		ReverseMode_Count = (NOREVERSE + 1)
	};

	static const std::string limbModes[LimbMode_Count];
	static const std::string reverseModes[ReverseMode_Count];

	static const std::string chairName;
	static const std::string tableName;
	static const std::string linkageObjName4Hand;
	static const std::string linkageObjName4Foot;

	static const double linkageObjWidth4Hand;
	static const double linkageObjWidth4Foot;

	static const double tableHeight;
	static const double floorHeight;

	static const double shiftDistanceForChangingObj;

//	~LinkageController();

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Collision detection.
	void onCollision(CollisionEvent &evt);

	void readIniFileAndInitialize();

	void checkServiceForThread();

	void makeInvertPosture(ManNiiPosture &posture);

	void changeMode(const std::map<std::string, std::vector<std::string> > &map);


	KinectV2DeviceManager  kinectV2DeviceManager;
	OculusDK1DeviceManager oculusDK1DeviceManager;

	Vector3d tableIniPos;
	Vector3d linkageObjIniPos4Hand;
	Vector3d linkageObjIniPos4Foot;

	///@brief Whether grasping or not.
	bool isGrasping;

	std::string linkageObjName;
	double      linkageObjWidth;
	Vector3d    linkageObjIniPos;
	double      puttingHeight;

	///@brief For Mirror therapy variables.
	std::string limbMode;
	std::string reverseMode;

	std::string myGraspingPart;

	bool usingOculus;

	BaseService *guiService;
	std::string guiServiceName;

	///@brief thread for check service.
	std::thread thCheckService;

	double elapsedTimeSinceRelease;
};

#endif // SIGVERSE_LINKAGE_CONTROLLER_H
