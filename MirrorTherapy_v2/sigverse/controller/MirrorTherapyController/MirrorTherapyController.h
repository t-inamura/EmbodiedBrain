/*
 * MirrorTherapyController.h
 *
 *  Created on: 2015/03/25
 *      Author: Nozaki
 */
#ifndef __MIRROR_THERAPY_CONTROLLER_H__
#define __MIRROR_THERAPY_CONTROLLER_H__

#include <sigverse/Controller.h>
#include <sigverse/ControllerEvent.h>

#include <sigverse/common/device/KinectV2SensorData.h>
#include <sigverse/common/device/OculusRiftDK1SensorData.h>
#include <sigverse/controller/common/ManNiiPosture.h>
#include <sigverse/controller/common/ManNiiAvatarController.h>
#include <sigverse/controller/common/device/KinectV2DeviceManager.h>
#include <sigverse/controller/common/device/OculusDK1DeviceManager.h>


class MirrorTherapyController : public ManNiiAvatarController
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

	//static param
	static const std::string msgKeyReverse;
	static const std::string msgKeyDelay;

	static const double defaultDelayTime;
	static const int    timeSeriesBufferSize;

	enum ReverseModeType
	{
		NOREVERSE = 0,
		RIGHTHAND = 1,
		LEFTHAND  = 2,
		ReverseMode_Count = (LEFTHAND + 1)
	};

	static const std::string reverseModes[ReverseMode_Count];

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

	///@brief Modify the mode for MirrorTherapy.
	bool setReverseModeAndDelayTime(const std::map<std::string, std::vector<std::string> > &map);


	KinectV2DeviceManager  kinectV2DeviceManager;
	OculusDK1DeviceManager oculusDK1DeviceManager;

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
};

#endif //__MIRROR_THERAPY_CONTROLLER_H__
