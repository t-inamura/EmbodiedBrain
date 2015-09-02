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
#include <sigverse/controller/common/ManNiiAvatarController.h>
#include <sigverse/controller/common/device/KinectV2DeviceManager.h>
#include <sigverse/controller/common/device/OculusDK1DeviceManager.h>
#include "LinkageController/LinkageCommon.h"

class LinkageController : public ManNiiAvatarController
{
public:
	typedef LinkageCommon               Cmn;
	typedef LinkageCommon::LimbModeType CmnLMT;

	/*
	 * Reverse mode.
	 *   RIGHT means that Left hand motion is copied to right hand.
	 */
	enum ReverseModeType
	{
		RIGHT     = 0,
		LEFT      = 1,
		NOREVERSE = 2,
		ReverseMode_Count = (NOREVERSE + 1)
	};

	// Interval of checking service provider.
	static const double intervalOfCheckingService;

	// Parameter file name.
	static const std::string parameterFileName;

	// Parameter file key name and default value.
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

	static const std::string paramFileKeyLinkageGraspGUIServiceName;
	static const std::string paramFileValLinkageGraspGUIServiceNameDefault;

	static const std::string paramFileKeyLinkageGraspLimbMode;
	static const std::string paramFileValLinkageGraspLimbModeDefault;
	static const std::string paramFileKeyLinkageGraspReverseMode;
	static const std::string paramFileValLinkageGraspReverseModeDefault;
	static const std::string paramFileKeyLinkageGraspGraspWithBoth;
	static const bool        paramFileValLinkageGraspGraspWithBothDefault;

	static const std::string paramFileKeyLinkageGraspIsWaistFixed;
	static const bool        paramFileValLinkageGraspIsWaistFixedDefault;

	// Message key string. This is used in onRecvMsg.
	static const std::string msgKeyLimbMode;
	static const std::string msgKeyReverseMode;

	// Reverse mode string array list.
	static const std::string reverseModes[ReverseMode_Count];

	// Entity name in world file.
	static const std::string chairName;            // Chair name
	static const std::string linkageObjName4Hand;  // Linkage object name for Hand mode.
	static const std::string linkageObjName4Foot;  // Linkage object name for Foot mode.

	// Link name in Avatar. It is used when grasping.
	static const std::string rightLink4Hand; // Right hand. For example, RARM_LINK7
	static const std::string leftLink4Hand;  // Left hand.  For example, LARM_LINK7
	static const std::string rightLink4Foot; // Right foot. For example, RLEG_LINK6
	static const std::string leftLink4Foot;  // Left foot.  For example, LLEG_LINK6

	// This controller is fixed position.
	static const bool isPositionFixed;

	// Initialize this controller.
	void onInit(InitEvent &evt);

	// Movement of the avatar.
	double onAction(ActionEvent &evt);

	// Receive message.
	void onRecvMsg(RecvMsgEvent &evt);

	// Collision detection.
	void onCollision(CollisionEvent &evt);

	// Read parameter file and initialize.
	void readParamFileAndInitialize();

	/*
	 * Check service provider in onAction method.
	 * Return a new interval of onAction.
	 */
	double checkServiceForOnAction(const double intervalOfOnAction);

	// Make invert posture from current posture.
	void makeInvertPosture(ManNiiPosture &posture);

	// Change mode. (Limb mode or Reverse mode)
	void changeMode(const std::map<std::string, std::vector<std::string> > &map);
	// Change Limb mode.
	void changeLimbMode(const std::map<std::string, std::vector<std::string> > &map);
	// Change Reverse mode.
	void changeReverseMode(const std::map<std::string, std::vector<std::string> > &map);

	// Reset variables for Hand mode.
	void resetVariables4Hand();
	// Reset variables for Foot mode.
	void resetVariables4Foot();


	KinectV2DeviceManager  kinectV2DeviceManager;  // Kinect v2 device manager.
	OculusDK1DeviceManager oculusDK1DeviceManager; // Oculus Rift DK1 device manager.
	BaseService *guiService;    // GUI service.
	std::string guiServiceName; // GUI service name.


	// Limb mode. Using hand or using foot.
	std::string limbMode;
	// Reverse mode. Right to Left or Left to Right.
	std::string reverseMode;

	Vector3d tableIniPos;           // Initial position of the table.

	std::string rightLink; // Right link name in Avatar. It is used when grasping.
	std::string leftLink;  // Left Link name in Avatar. It is used when grasping.

	// Whether grasping or not.
	bool isGrasping;

	// Target object of grasping.
	std::string linkageObjName;

	// The part name of avatar that is grasping object.
	std::string myGraspingPartName;

	// Whether using Oculus Rift or not.
	bool usingOculus;

	// Distance of the avatar and the grasped object.
	Vector3d diffAvatarAndGraspedObject;

	// If true, ROOT_JOINT and WAIST_JOINT are Fixed.
	bool isWaistFixed;

	// If false, Avatar grasps with one hand. If true, Avatar grasps with both hands.
	bool graspWithBoth;

	double distanceBetweenBoth;
};


const double LinkageController::intervalOfCheckingService = 3.0;

const std::string LinkageController::parameterFileName = "LinkageController.ini";

const std::string LinkageController::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string LinkageController::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string LinkageController::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string LinkageController::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string LinkageController::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";

const std::string LinkageController::paramFileKeyOculusDK1ServiceName   = "OculusDK1.service_name";
const std::string LinkageController::paramFileKeyOculusDK1Devicetype    = "OculusDK1.device_type";
const std::string LinkageController::paramFileKeyOculusDK1DeviceUniqueID= "OculusDK1.device_unique_id";

const std::string LinkageController::paramFileValKinectV2SensorDataModeDefault = "POSITION";
const double      LinkageController::paramFileValKinectV2ScaleRatioDefault     = 10000.0;

const std::string LinkageController::paramFileKeyLinkageGraspGUIServiceName        = "LinkageGraspGUI.service_name";
const std::string LinkageController::paramFileValLinkageGraspGUIServiceNameDefault = "SVC_LINKAGE_GRASP_GUI";

const std::string LinkageController::paramFileKeyLinkageGraspLimbMode           = "LinkageGrasp.limb_mode";
const std::string LinkageController::paramFileValLinkageGraspLimbModeDefault    = "HAND";
const std::string LinkageController::paramFileKeyLinkageGraspReverseMode        = "LinkageGrasp.reverse_mode";
const std::string LinkageController::paramFileValLinkageGraspReverseModeDefault = "RIGHT";

const std::string LinkageController::paramFileKeyLinkageGraspGraspWithBoth        = "LinkageGrasp.grasp_with_both";
const bool        LinkageController::paramFileValLinkageGraspGraspWithBothDefault = false;
const std::string LinkageController::paramFileKeyLinkageGraspIsWaistFixed         = "LinkageGrasp.is_waist_fixed";
const bool        LinkageController::paramFileValLinkageGraspIsWaistFixedDefault  = false;

const std::string LinkageController::msgKeyLimbMode    = "LIMB_MODE";
const std::string LinkageController::msgKeyReverseMode = "REVERSE_MODE";

const std::string LinkageController::reverseModes[ReverseMode_Count] = { "RIGHT", "LEFT", "NOREVERSE" };

const std::string LinkageController::rightLink4Hand = "RARM_LINK7";
const std::string LinkageController::leftLink4Hand  = "LARM_LINK7";
const std::string LinkageController::rightLink4Foot = "RLEG_LINK6";
const std::string LinkageController::leftLink4Foot  = "LLEG_LINK6";

const std::string LinkageController::chairName           = "chair";
const std::string LinkageController::linkageObjName4Hand = "linkageObj4Hand";
const std::string LinkageController::linkageObjName4Foot = "linkageObj4Foot";

const bool        LinkageController::isPositionFixed = true;

#endif // SIGVERSE_LINKAGE_CONTROLLER_H
