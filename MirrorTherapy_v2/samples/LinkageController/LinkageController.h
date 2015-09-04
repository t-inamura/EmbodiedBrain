/*
 * LinkageController.h
 *
 *  Created on: 2015/08/25
 *      Author: Yamada@tome
 */
#ifndef SIGVERSE_LINKAGE_CONTROLLER_H
#define SIGVERSE_LINKAGE_CONTROLLER_H

#include <string>
#include <mutex>
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
#include "LinkageCommon.h"

// Mutex for Hand state.
std::mutex mtxForHandState;

class LinkageController : public ManNiiAvatarController
{
public:
	typedef LinkageCommon               Cmn;
	typedef LinkageCommon::LimbModeType CmnLMT;

	/*
	 * Grasp mode.
	 *   GRASP means that the avatar grasps with one hand.
	 *   SANDWICH means that the avatar sandwiches with both hands.
	 */
	enum GraspModeType
	{
		SANDWICH    = 0,
		GRASP_RIGHT = 1,
		GRASP_LEFT  = 2,
		GraspMode_Count = (GRASP_LEFT + 1)
	};

	/*
	 * Reverse mode.
	 *   RIGHT means that Left hand motion is copied to right hand.
	 */
	enum ReverseModeType
	{
		LEFT        = 0,
		RIGHT       = 1,
		NOT_REVERSE = 2,
		ReverseMode_Count = (NOT_REVERSE + 1)
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
	static const std::string paramFileKeyLinkageGraspGraspMode;
	static const std::string paramFileValLinkageGraspGraspModeDefault;
	static const std::string paramFileKeyLinkageGraspReverseMode;
	static const std::string paramFileValLinkageGraspReverseModeDefault;

	static const std::string paramFileKeyLinkageGraspLinkObjList4Hand;
	static const std::string paramFileValLinkageGraspLinkObjList4HandDefault;
	static const std::string paramFileKeyLinkageGraspLinkObjList4Foot;
	static const std::string paramFileValLinkageGraspLinkObjList4FootDefault;

	static const std::string paramFileKeyLinkageGraspFixedWaist;
	static const bool        paramFileValLinkageGraspFixedWaistDefault;
	static const std::string paramFileKeyLinkageGraspCorrectionAngle;
	static const double      paramFileValLinkageGraspCorrectionAngleDefault;

	// Message key string. This is used in onRecvMsg.
	static const std::string msgKeyLimbMode;
	static const std::string msgKeyGraspMode;
	static const std::string msgKeyReverseMode;
	static const std::string msgKeyFixedWaist;

	// Grasp mode string array list.
	static const std::string graspModes[GraspMode_Count];

	// Reverse mode string array list.
	static const std::string reverseModes[ReverseMode_Count];

	// Entity name in world file.
	static const std::string chairName;            // Chair name

	// Link name in Avatar. It is used when grasping.
	static const std::string rightLink4Hand; // Right hand. For example, RARM_LINK7
	static const std::string leftLink4Hand;  // Left hand.  For example, LARM_LINK7
	static const std::string rightLink4Foot; // Right foot. For example, RLEG_LINK6
	static const std::string leftLink4Foot;  // Left foot.  For example, LLEG_LINK6

	// This controller is fixed position.
	static const bool isPositionFixed;

	// Number of Hand state history. This is for GRASP. GRASP is one of the grasp mode.
	static const int numOfHandStateHistory = 10;
	// Threshold number of judging hand state. This is for GRASP. GRASP is one of the grasp mode.
	static const int thresholdNumOfJudgingHandState = 3;

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

	// Check grasp status for SANDWICH. SANDWICH is one of the grasp mode.
	void checkGraspStatus4Sandwich();
	// Check grasp status for GRASP_RIGHT and GRASP_LEFT. GRASP_RIGHT and GRASP_LEFT are one of the grasp mode.
	void checkGraspStatus4Grasp();
	// Get nearest target info through args.
	double getNearestTargetInfo(SimObj **myself, Vector3d &myPartsPos, SimObj **target, Vector3d &targetPos, std::string &targetName);
	// Check trying to grasp for GRASP_RIGHT and GRASP_LEFT. GRASP_RIGHT and GRASP_LEFT are one of the grasp mode.
	bool checkTrying2Grasp4Grasp(const bool isRightHandClosedNew, const bool isLeftHandClosedNew) const;

	//Checking Hand state. Closed or not.
	bool isHandClosed(std::list<KinectV2SensorData::HandState> handStateHistory);

	// Make invert posture from current posture.
	void makeInvertPosture(ManNiiPosture &posture);

	// Change mode. (Limb mode or Reverse mode)
	void changeMode(const std::map<std::string, std::vector<std::string> > &map);
	// Change Limb mode.
	void changeLimbMode(const std::map<std::string, std::vector<std::string> > &map);
	// Change Grasp mode.
	void changeGraspMode(const std::map<std::string, std::vector<std::string> > &map);
	// Change Reverse mode.
	void changeReverseMode(const std::map<std::string, std::vector<std::string> > &map);
	// Change Fixed Waist option.
	void changeFixedWaist(const std::map<std::string, std::vector<std::string> > &map);

	// Reset variables for Hand state in Limb mode.
	void resetVariables4Hand();
	// Reset variables for Foot state in Limb mode.
	void resetVariables4Foot();
	// Reset variables for Grasp mode.
	void resetVariables4GraspMode(std::string graspMode);

	KinectV2DeviceManager  kinectV2DeviceManager;  // Kinect v2 device manager.
	OculusDK1DeviceManager oculusDK1DeviceManager; // Oculus Rift DK1 device manager.
	BaseService *guiService;    // GUI service.
	std::string guiServiceName; // GUI service name.


	// Limb mode. Using hand or using foot.
	std::string limbMode;
	// Grasp mode. Grasping or Sandwiching.
	std::string graspMode;
	// Reverse mode. Right to Left or Left to Right.
	std::string reverseMode;

	Vector3d tableIniPos;           // Initial position of the table.

	std::string rightLink; // Right link name in Avatar. It is used when grasping.
	std::string leftLink;  // Left Link name in Avatar. It is used when grasping.

	// Whether grasping or not.
	bool isGrasping;

	// Target object name list for Hand.
	std::vector<std::string> linkObjNameList4Hand;
	// Target object name list for Foot.
	std::vector<std::string> linkObjNameList4Foot;
	// Target object name list.
	std::vector<std::string> linkObjNameList;

	// The part name of avatar that is grasping object.
	std::string myGraspingPartName;

	// Whether using Oculus Rift or not.
	bool usingOculus;

	// Distance of the avatar and the grasped object.
	Vector3d distanceOfAvatarAndGraspedObject;

	// If true, ROOT_JOINT and WAIST_JOINT are Fixed.
	bool isWaistFixed;

	// Correction angle of x-direction slope for human avatar. Unit is degree.
	double correctionAngle;

	// Distance of both hands for SANDWICH. SANDWICH is one of the grasp mode.
	double distanceBetweenBoth;

	// History of Left hand state.
	std::list<KinectV2SensorData::HandState> leftHandStateHistory;
	// History of Left hand state.
	std::list<KinectV2SensorData::HandState> rightHandStateHistory;

	// List of flag. Left hand is closed or not.
	bool isLeftHandClosed;
	// List of flag. Right hand is closed or not.
	bool isRightHandClosed;
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
const std::string LinkageController::paramFileKeyLinkageGraspGraspMode          = "LinkageGrasp.grasp_mode";
const std::string LinkageController::paramFileValLinkageGraspGraspModeDefault   = "SANDWICH";
const std::string LinkageController::paramFileKeyLinkageGraspReverseMode        = "LinkageGrasp.reverse_mode";
const std::string LinkageController::paramFileValLinkageGraspReverseModeDefault = "RIGHT";

const std::string LinkageController::paramFileKeyLinkageGraspLinkObjList4Hand        = "LinkageGrasp.link_obj_4_hand";
const std::string LinkageController::paramFileValLinkageGraspLinkObjList4HandDefault = "linkObj4Hand";
const std::string LinkageController::paramFileKeyLinkageGraspLinkObjList4Foot        = "LinkageGrasp.link_obj_4_foot";
const std::string LinkageController::paramFileValLinkageGraspLinkObjList4FootDefault = "linkObj4Foot";

const std::string LinkageController::paramFileKeyLinkageGraspFixedWaist             = "LinkageGrasp.fixed_waist";
const bool        LinkageController::paramFileValLinkageGraspFixedWaistDefault      = false;
const std::string LinkageController::paramFileKeyLinkageGraspCorrectionAngle        = "LinkageGrasp.correction_angle";
const double      LinkageController::paramFileValLinkageGraspCorrectionAngleDefault = 0.0;

const std::string LinkageController::msgKeyLimbMode    = "LIMB_MODE";
const std::string LinkageController::msgKeyGraspMode   = "GRASP_MODE";
const std::string LinkageController::msgKeyReverseMode = "REVERSE_MODE";
const std::string LinkageController::msgKeyFixedWaist  = "FIXED_WAIST";

const std::string LinkageController::graspModes[GraspMode_Count]     = { "SANDWICH", "GRASP_RIGHT", "GRASP_LEFT" };
const std::string LinkageController::reverseModes[ReverseMode_Count] = { "LEFT", "RIGHT", "NOT_REVERSE" };

const std::string LinkageController::rightLink4Hand = "RARM_LINK7";
const std::string LinkageController::leftLink4Hand  = "LARM_LINK7";
const std::string LinkageController::rightLink4Foot = "RLEG_LINK6";
const std::string LinkageController::leftLink4Foot  = "LLEG_LINK6";

const std::string LinkageController::chairName           = "chair";

const bool        LinkageController::isPositionFixed = true;

#endif // SIGVERSE_LINKAGE_CONTROLLER_H
