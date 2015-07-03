/*
 * ManNiiAvatarController.h
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */
#ifndef __MAN_NII_AVATAR_CONTROLLER_BY_KINECT_V2_H__
#define __MAN_NII_AVATAR_CONTROLLER_BY_KINECT_V2_H__

#include <sigverse/Controller.h>
#include <sigverse/comm/controller/Controller.h>
#include <sigverse/ControllerEvent.h>

#include "../ControllerCommon/ManNiiAvatarController.h"
#include "../ControllerCommon/ManNiiPosture.h"
#include "../../common/device/KinectV2SensorData.h"


class ManNiiAvatarControllerByKinectV2 : public ManNiiAvatarController
{
private:
	///@brief Parameter file name.
	static const std::string parameterFileName;

	//Parameter file information
	static const std::string paramFileKeyKinectV2SensorDataMode;
	static const std::string paramFileKeyKinectV2ScaleRatio;
	static const std::string paramFileValKinectV2SensorDataModeDefault;
	static const double      paramFileValKinectV2ScaleRatioDefault;

	static const double normalization_range;

	//ManNiiAvatarPosture posture;
	BaseService *kinectV2Service;
	std::string kinectV2ServiceName;
	std::string kinectV2DeviceType;
	std::string kinectV2DeviceUniqueID;

	std::string sensorDataModeStr;
	double      scaleRatio;

	void readIniFile();

	SigCmn::Vector3 startpos;
	bool started = false;

	SigCmn::Vector3 iniPos;
	double yrot;

public:
	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

//	///@brief control avatar by Positions (sensor data mode = Position)
//	void controlAvatarByPositions(const KinectV2SensorData &sensorData);
//
//	///@brief control avatar by Quaternions (sensor data mode = Quaternion)
//	void controlAvatarByQuaternions(const KinectV2SensorData &sensorData);

	///@brief Convert euler angle to avatar posture structure.
	ManNiiPosture convertKinectV2JointOrientations2ManNiiPosture(const KinectV2SensorData::KinectV2JointOrientation* kinectV2Joints);
	ManNiiPosture convertKinectV2JointPosition2ManNiiPosture(const KinectV2SensorData::KinectV2JointPosition* positionArray);

	void setPosition(SimObj *obj, const SigCmn::Vector3 &pos);
	void setJointQuaternion(SimObj *obj, const ManNiiPosture::ManNiiJoint &jq);
	void setJointQuaternionsForKinect(SimObj *obj, const ManNiiPosture &manNiiPosture);
};

#endif //__MAN_NII_AVATAR_CONTROLLER_BY_KINECT_V2_H__
