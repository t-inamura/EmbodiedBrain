/*
 * LinkageController.h
 *
 *  Created on: 2015/08/25
 *      Author: Yamada@tome
 */
#ifndef SIGVERSE_LINKAGE_CONTROLLER_H
#define SIGVERSE_LINKAGE_CONTROLLER_H

#include <string>
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

	// Link name in Avatar. It is used when grasping.
	static const std::string rightLink4Hand; // Right hand. For example, RARM_LINK7
	static const std::string leftLink4Hand;  // Left hand.  For example, LARM_LINK7
	static const std::string rightLink4Foot; // Right foot. For example, RLEG_LINK6
	static const std::string leftLink4Foot;  // Left foot.  For example, LLEG_LINK6

	// Entity name in world file.
	static const std::string chairName;   // Chair name

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
	// Reset variables for Hand state in Limb mode.
	void resetVariables4Hand();
	// Reset variables for Foot state in Limb mode.
	void resetVariables4Foot();
	// Reset variables for Grasp mode.
	void resetVariables4GraspMode(std::string graspMode);


	/*
	 * Check service provider in onAction method.
	 * Return a new interval of onAction.
	 */
	double checkServiceForOnAction(const double intervalOfOnAction);
	// Check grasp status for SANDWICH. SANDWICH is one of the grasp mode.
	void checkGraspStatus4Sandwich();
	// Check grasp status for GRASP_RIGHT and GRASP_LEFT. GRASP_RIGHT and GRASP_LEFT are one of the grasp mode.
	void checkGraspStatus4Grasp();
	// Get nearest target(linkage object) info through args.
	double getNearestTargetInfo(SimObj **myself, Vector3d &myPartsPos, SimObj **target, Vector3d &targetPos, std::string &targetName);
	//Checking Hand state. Closed or not.
	bool isHandClosed(std::list<KinectV2SensorData::HandState> handStateHistory);
	// Check trying to grasp for GRASP_RIGHT and GRASP_LEFT. GRASP_RIGHT and GRASP_LEFT are one of the grasp mode.
	bool checkTrying2Grasp4Grasp(const bool isRightHandClosedNew, const bool isLeftHandClosedNew) const;


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


	// Kinect v2 device manager.
	KinectV2DeviceManager  kinectV2DeviceManager;
	// Oculus Rift DK1 device manager.
	OculusDK1DeviceManager oculusDK1DeviceManager;
	// GUI service.
	BaseService *guiService;
	// GUI service name.
	std::string guiServiceName;

	// Limb mode. Using hand or using foot.
	std::string limbMode;
	// Grasp mode. Grasping or Sandwiching.
	std::string graspMode;
	// Reverse mode. Right to Left or Left to Right.
	std::string reverseMode;

	std::vector<std::string> linkObjNameList4Hand; // Target object name list for Hand.
	std::vector<std::string> linkObjNameList4Foot; // Target object name list for Foot.
	std::vector<std::string> linkObjNameList;      // Target object name list.

	// If true, ROOT_JOINT and WAIST_JOINT are Fixed.
	bool isWaistFixed;

	// Correction angle of x-direction slope for human avatar. Unit is degree.
	double correctionAngle;


	Vector3d tableIniPos;  // Initial position of the table.

	std::string rightLink; // Right link name in Avatar. It is used when grasping.
	std::string leftLink;  // Left  Link name in Avatar. It is used when grasping.

	std::list<KinectV2SensorData::HandState> rightHandStateHistory; // History of Right hand state.
	std::list<KinectV2SensorData::HandState> leftHandStateHistory;  // History of Left  hand state.

	bool isLeftHandClosed;  // List of flag. Left hand is closed or not.
	bool isRightHandClosed; // List of flag. Right hand is closed or not.

	// Distance of both links for SANDWICH. SANDWICH is one of the grasp mode.
	double distanceBetweenBothLinks;

	// Distance of the avatar and the grasped object.
	Vector3d distanceOfAvatarAndGraspedObject;

	// Whether grasping or not.
	bool isGrasping;

	// Oculus rift is used or not.
	bool usingOculus;
};


#endif // SIGVERSE_LINKAGE_CONTROLLER_H
