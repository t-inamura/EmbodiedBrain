/*
 * MirrorTherapyController.cpp
 *
 *  Created on: 2015/03/25
 *      Author: Nozaki
 */

#include "../../Common/SensorData.h"
#include "MirrorTherapyController.h"
#include <sys/time.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>

enum ReverseModeType
{
	NOREVERSE = 0,
	RIGHTHAND = 1,
	LEFTHAND  = 2,
	ReverseMode_Count = (LEFTHAND + 1)
};

static const std::string reverseModes[ReverseMode_Count] =
{
	"NOREVERSE",
	"RIGHTHAND",
	"LEFTHAND"
};

///@brief Initialize this controller.
void MirrorTherapyController::onInit(InitEvent &evt)
{
	this->parameterFileName = PARAM_FILE_NAME_MIRROR_THERAPY_INI;
	readIniFile();

	this->oculusDK1Service = NULL;
	this->kinectV2Service  = NULL;

	this->defaultHeadJoint0Quaternion[0] = 1.0;
	this->defaultHeadJoint0Quaternion[1] = 0.0;
	this->defaultHeadJoint0Quaternion[2] = 0.0;
	this->defaultHeadJoint0Quaternion[3] = 0.0;

	this->prevYaw = this->prevPitch = this->prevRoll = 0.0;

	SimObj *myself = getObj(myname());

	// For Mirror therapy initialize.
	this->reverseMode = reverseModes[RIGHTHAND]; // Set reverse mode.

	// Set delay variables.
	this->targetDelayTime = DEFAULT_DELAY_TIME;
	gettimeofday(&this->initTimeVal, NULL);
	this->frameNumber = 0;
	this->pastPostures = std::vector<TimeAndPostureType>(TIME_SERIES_BUFFER_SIZE);
}

///@brief Movement of the robot.
double MirrorTherapyController::onAction(ActionEvent &evt)
{
	bool oculusDK1Available = checkService(this->oculusDK1ServiceName);

	if (oculusDK1Available && this->oculusDK1Service == NULL)
	{
		this->oculusDK1Service = connectToService(this->oculusDK1ServiceName);
	}
	else if (!oculusDK1Available && this->oculusDK1Service != NULL)
	{
		this->oculusDK1Service = NULL;
	}

	bool kinectV2Available = checkService(this->kinectV2ServiceName);

	if (kinectV2Available && this->kinectV2Service == NULL)
	{
		this->kinectV2Service = connectToService(this->kinectV2ServiceName);
	}
	else if (!kinectV2Available && this->kinectV2Service != NULL)
	{
		this->kinectV2Service = NULL;
	}

	return 1.0;
}


///@brief Message heard by the robot.
void MirrorTherapyController::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		// Receive message.
		const std::string allMsg = evt.getMsg();

//		std::cout << allMsg << std::endl;

		// Get device type from message.
		SensorData sensorData;
		std::map<std::string, std::vector<std::string> > sensorDataMap = sensorData.decodeSensorData(allMsg);

		// Device
		if (sensorDataMap.find(MSG_KEY_DEV_TYPE) != sensorDataMap.end())
		{
			std::string deviceTypeValue = sensorDataMap[MSG_KEY_DEV_TYPE][0];
			std::string deviceUniqueId  = sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0];

			//Kinect V2
			if (deviceTypeValue == this->kinectV2DeviceType && deviceUniqueId == this->kinectV2DeviceUniqueID)
			{
				// Decode message to sensor data of kinect v2.
				KinectV2SensorData kinectV2SensorData;
				kinectV2SensorData.setSensorData(sensorDataMap);

				// Get quaternions(orientations) of kinect v2 format, from sensor data of kinect v2.
				KinectV2SensorData::KinectV2JointOrientation tmpKinectV2JointOrientations[KinectV2SensorData::KinectV2JointType_Count];
				kinectV2SensorData.getKinectV2JointOrientation(tmpKinectV2JointOrientations);

				// Convert kinect v2 quaternions(orientations) to man-nii posture(sigverse quaternion format).
				ManNiiPosture manNiiPosture = ManNiiPosture();
				this->convertKinectV2JointOrientations2ManNiiPosture(tmpKinectV2JointOrientations, manNiiPosture);

				// Store current posture and time stamp.
				TimeAndPostureType current;
				current.timeStamp = this->generateCurrentTimeStamp();

				for (int i = 0; i < ManNiiPosture::ManNiiJointType_Count; i++)
				{
					current.posture.joint[i] = manNiiPosture.joint[i];
				}

				const unsigned long index = this->frameNumber % TIME_SERIES_BUFFER_SIZE;
				this->pastPostures[index] = current;

				// Search nearest neighbor of target delay time in stored time stamps.
				//   last 500 postures is in this->pastPostures.
				//   find nearest index.
				int nearestIndexTmp = index;

				if (this->targetDelayTime > 0.0)  // If targetDelayTime < 0, don't do anything.
				{
					if(current.timeStamp > this->targetDelayTime)  // If elapsed time < targetDelayTime, don't do anything.
					{
						const double idealPastTime = current.timeStamp - (this->targetDelayTime);

						double nearestTimeDistance = DBL_MAX;
						nearestIndexTmp = 0;

						for (int i = TIME_SERIES_BUFFER_SIZE - 1; i >= 0; i--)
						{
							double tmpTimeDistance = abs(idealPastTime - this->pastPostures[i].timeStamp);

							if (tmpTimeDistance < nearestTimeDistance)
							{
								nearestTimeDistance = tmpTimeDistance;
								nearestIndexTmp = i;
							}
						}
					}
				}

				const int nearestIndex = nearestIndexTmp;

				// left hand motion affects right hand.
				if (this->reverseMode == reverseModes[RIGHTHAND])
				{
					// Get original quaternions.
					double w2, x2, y2, z2, w3, x3, y3, z3, w5, x5, y5, z5; // These are temporary variables.
					this->pastPostures[nearestIndex].posture.joint[ManNiiPosture::LARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
					this->pastPostures[nearestIndex].posture.joint[ManNiiPosture::LARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);

					// Set reverse quaternions.
					manNiiPosture.joint[ManNiiPosture::RARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
					manNiiPosture.joint[ManNiiPosture::RARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
					//tmpManNiiPosture.joints[RARM_JOINT5].quaternion.setQuaternions(w5, -x5, -y5, -z5);
				}
				// right hand motion affects left hand.
				else if (this->reverseMode == reverseModes[LEFTHAND])
				{
					// Get original quaternions.
					double w2, x2, y2, z2, w3, x3, y3, z3, w5, x5, y5, z5;
					this->pastPostures[nearestIndex].posture.joint[ManNiiPosture::RARM_JOINT2].quaternion.getQuaternion(w2, x2, y2, z2);
					this->pastPostures[nearestIndex].posture.joint[ManNiiPosture::RARM_JOINT3].quaternion.getQuaternion(w3, x3, y3, z3);

					// Set reverse quaternions.
					manNiiPosture.joint[ManNiiPosture::LARM_JOINT2].quaternion.setQuaternion(w2, x2, -y2, -z2);
					manNiiPosture.joint[ManNiiPosture::LARM_JOINT3].quaternion.setQuaternion(w3, x3, -y3, -z3);
					//tmpManNiiPosture.joints[RARM_JOINT5].quaternion.setQuaternions(w5, -x5, -y5, -z5);
				}

				// Set sigverse quaternions
				SimObj *obj = getObj(myname());
				this->setJointQuaternionsForKinect(obj, manNiiPosture);

				this->frameNumber++;
			}
			// Oculus DK1
			else if (deviceTypeValue == this->oculusDK1DeviceType && deviceUniqueId ==  this->oculusDK1DeviceUniqueID)
			{
				OculusRiftDK1SensorData oculusRiftDK1SensorData;
				oculusRiftDK1SensorData.setSensorData(sensorDataMap);

				SensorData::EulerAngleType eulerAngle;
				eulerAngle.yaw   = oculusRiftDK1SensorData.getYaw();
				eulerAngle.pitch = oculusRiftDK1SensorData.getPitch();
				eulerAngle.roll  = oculusRiftDK1SensorData.getRoll();

				ManNiiPosture manNiiPosture = ManNiiPosture();
				this->convertEulerAngle2ManNiiPosture(eulerAngle, manNiiPosture);

				SimObj *obj = getObj(myname());
				this->setJointQuaternionsForOculus(obj, manNiiPosture);
			}
		}
		// Not a device.
		else
		{
			this->setReverseModeAndDelayTime(sensorDataMap);
		}
	}
	catch(SimObj::NoAttributeException &err)
	{
		LOG_MSG(("NoAttributeException: %s", err.msg()));
	}
	catch(SimObj::AttributeReadOnlyException &err)
	{
		LOG_MSG(("AttributeReadOnlyException: %s", err.msg()));
	}
	catch(SimObj::Exception &err)
	{
		LOG_MSG(("Exception: %s", err.msg()));
	}
}

//std::string MirrorTherapyController::getDeviceIDFromMessage(const std::string &message)
//{
//	std::map<std::string, std::vector<std::string> > map = SensorData::convertMessage2Map(message);
//
//	if (map.find(MSG_KEY_DEV_TYPE) == map.end())
//	{
//		return "OTHER_MESSAGE";
//	}
//	else
//	{
//		const std::string devIDValue = map[MSG_KEY_DEV_TYPE][0];
//		return devIDValue;
//	}
//}

const double MirrorTherapyController::generateCurrentTimeStamp()
{
	// Get current timestamp.
	struct timeval currentTimeVal;
	gettimeofday(&currentTimeVal, NULL);
	const double currentTimeStamp = (currentTimeVal.tv_sec - this->initTimeVal.tv_sec) * 1000.0 + (currentTimeVal.tv_usec - this->initTimeVal.tv_usec) / 1000.0;
	return currentTimeStamp;
}

void MirrorTherapyController::setJointQuaternion(SimObj *obj, ManNiiPosture::ManNiiJoint &jq)
{
	obj->setJointQuaternion(ManNiiPosture::manNiiJointTypeStr(jq.jointType).c_str(), jq.quaternion.w, jq.quaternion.x, jq.quaternion.y, jq.quaternion.z);
}

bool MirrorTherapyController::setReverseModeAndDelayTime(const std::map<std::string, std::vector<std::string> > &map)
{
	if (map.find(MSG_KEY_REVERSE) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(MSG_KEY_REVERSE);
		this->reverseMode = it->second[0];

		std::cout << "Set reverse mode:" << this->reverseMode << std::endl;
		return true;
	}
	else if (map.find(MSG_KEY_DELAY) != map.end())
	{
		std::map<std::string, std::vector<std::string> >::const_iterator it = map.find(MSG_KEY_DELAY);
		this->targetDelayTime = atof(it->second[0].c_str());

		std::cout << "Set target delay time:" << this->targetDelayTime << std::endl;
		return true;
	}
	return false;
}


void MirrorTherapyController::setJointQuaternionsForKinect(SimObj *obj, ManNiiPosture &manNiiPosture)
{
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::WAIST_JOINT1]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::RARM_JOINT2]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::LARM_JOINT2]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::RLEG_JOINT2]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::LLEG_JOINT2]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::RLEG_JOINT4]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::LLEG_JOINT4]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::RARM_JOINT3]);
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::LARM_JOINT3]);

}

void MirrorTherapyController::setJointQuaternionsForOculus(SimObj *obj, ManNiiPosture &manNiiPosture)
{
	this->setJointQuaternion(obj, manNiiPosture.joint[ManNiiPosture::HEAD_JOINT0]);
}

void MirrorTherapyController::convertEulerAngle2ManNiiPosture(const SensorData::EulerAngleType &eulerAngle, ManNiiPosture &manNiiAvatarPosture)
{
	dQuaternion qyaw;
	dQuaternion qpitch;
	dQuaternion qroll;

	qyaw[0] = cos(eulerAngle.yaw/2.0);
	qyaw[1] = 0.0;
	qyaw[2] = sin(eulerAngle.yaw/2.0);
	qyaw[3] = 0.0;

	qpitch[0] = cos(-eulerAngle.pitch/2.0);
	qpitch[1] = sin(-eulerAngle.pitch/2.0);
	qpitch[2] = 0.0;
	qpitch[3] = 0.0;

	qroll[0] = cos(-eulerAngle.roll/2.0);
	qroll[1] = 0.0;
	qroll[2] = 0.0;
	qroll[3] = sin(-eulerAngle.roll/2.0);
	dQuaternion tmpQ1;
	dQuaternion tmpQ2;

	dQMultiply0(tmpQ1, qyaw, qpitch);

	dQMultiply0(tmpQ2, tmpQ1, qroll);


	dQuaternion tmpQ3;

	//dQuaternion defaultQ = this->defaultHeadJointQuaternion;
	dQMultiply1(tmpQ3, this->defaultHeadJoint0Quaternion, tmpQ2);

	Quaternion tmpQ4(tmpQ3[0], tmpQ3[1], tmpQ3[2], tmpQ3[3]);

	manNiiAvatarPosture.joint[ManNiiPosture::HEAD_JOINT0].jointType  = ManNiiPosture::HEAD_JOINT0;
	manNiiAvatarPosture.joint[ManNiiPosture::HEAD_JOINT0].quaternion = tmpQ4;
}


void MirrorTherapyController::convertKinectV2JointOrientations2ManNiiPosture(KinectV2SensorData::KinectV2JointOrientation *kinectV2Joints, ManNiiPosture &manNiiPosture)
{
	// TODO: Calculate for Neck, both wrists and both ankles.
	const double coef = 1.0 / sqrt(2.0);

	Quaternion q0 = Quaternion(kinectV2Joints[0].orientation);

	Quaternion q1 = Quaternion(kinectV2Joints[1].orientation);
	Quaternion q1_h = Quaternion(0.0, 0.0, 1.0, 0.0);
	Quaternion q1_rot = Quaternion::calcCrossProduct(q1_h, q1);
	Quaternion q2 = Quaternion(kinectV2Joints[20].orientation);
	Quaternion q2_h = Quaternion(0.0, 0.0, -1.0, 0.0);
	Quaternion q2_rot = Quaternion::calcCrossProduct(q2, q2_h);

	Quaternion q3 = Quaternion(kinectV2Joints[8].orientation);
	Quaternion q4 = Quaternion(kinectV2Joints[9].orientation);
	Quaternion q7 = Quaternion(kinectV2Joints[5].orientation);
	Quaternion q9 = Quaternion(kinectV2Joints[10].orientation);
	Quaternion q13 = Quaternion(kinectV2Joints[17].orientation);
	Quaternion q15 = Quaternion(kinectV2Joints[13].orientation);
	Quaternion q17 = Quaternion(kinectV2Joints[18].orientation);
	Quaternion q19 = Quaternion(kinectV2Joints[14].orientation);
	Quaternion q20 = Quaternion(kinectV2Joints[20].orientation);
	Quaternion q20_rot = Quaternion::calcCrossProduct(q1_h, q20);
	Quaternion q21 = Quaternion(kinectV2Joints[6].orientation);
	Quaternion q25 = Quaternion(kinectV2Joints[11].orientation);
	Quaternion q26 = Quaternion(kinectV2Joints[7].orientation);
	Quaternion q1_con = Quaternion(q20_rot.w, -q20_rot.x, -q20_rot.y, -q20_rot.z);
	Quaternion q30_con = Quaternion(q1_rot.w, -q1_rot.x, -q1_rot.y, -q1_rot.z);
	Quaternion q30 = Quaternion(kinectV2Joints[2].orientation);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////// Neck quaternion //////////////////////
	q30 = Quaternion::calcCrossProduct(q1_h, q30);
	q30 = Quaternion::calcCrossProduct(q30_con, q30);

	////////////////////////////////////////////////////////////////////


	///////////////////////////  Right Arm quaternion //////////////////////
	Quaternion q5;
	Quaternion q5_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q5_z = Quaternion(coef, 0.0, 0.0, -coef);
	Quaternion q5_y = Quaternion(coef, 0.0, -coef, 0.0);
	Quaternion q5_rotx = Quaternion::calcCrossProduct(q5_x, q4);
	q5.w = q5_rotx.w;
	q5.x = q5_rotx.y;
	q5.y = q5_rotx.z;
	q5.z = q5_rotx.x;
	q5 = Quaternion::calcCrossProduct(q2_h, q5);

	q5 = Quaternion::calcCrossProduct(q5_y, q5);
	//q5.w = q5.w;
	q5.x = -q5.x;
	q5.y = -q5.y;
	//q5.z = q5.z;
	q5 = Quaternion::calcCrossProduct(q1_con, q5);
	//////////////////////////////////////////////////////////////////////

	////////////// Left Arm quaternion ///////////////////////////////////
	Quaternion q6;
	Quaternion q6_x = Quaternion(coef, coef, 0.0, 0.0);
	Quaternion q6_z = Quaternion(coef, 0.0, 0.0, coef);
	Quaternion q6_y = Quaternion(coef, 0.0, coef, 0.0);
	Quaternion q6_rotx = Quaternion::calcCrossProduct(q6_x, q7);
	q6.w = q6_rotx.w;
	q6.x = q6_rotx.y;
	q6.y = q6_rotx.z;
	q6.z = q6_rotx.x;
	q6 = Quaternion::calcCrossProduct(q2_h, q6);
	q6 = Quaternion::calcCrossProduct(q6_y, q6);
	//q6.w = q6.w;
	//q6.x = q6.x;
	//q6.y = q6.y;
	//q6.z = q6.z;
	q6 = Quaternion::calcCrossProduct(q1_con, q6);

	//////////////////////////////////////////////////////////////////////////

	///////////////////////////  Right hipe quaternion //////////////////////
	Quaternion q12;
	Quaternion q12_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q12_z = Quaternion(coef, 0.0, 0.0, -coef);
	Quaternion q12_y = Quaternion(coef, 0.0, coef, 0.0);
	Quaternion q12_rotx = Quaternion::calcCrossProduct(q12_x, q13);
	Quaternion q3_h = Quaternion(0.0, 0.0, 1.0, 0.0);
	Quaternion q12_h = Quaternion(0.0, 0.0, 0.0, -1.0);
	q12 = Quaternion::calcCrossProduct(q12_h, q13);
	Quaternion q12_b = Quaternion::calcCrossProduct(q12_y, q12);
	q12.w = q12_b.w;
	q12.x = -q12_b.z;
	q12.y = -q12_b.y;
	q12.z = -q12_b.x;
	//////////////////////////////////////////////////////////////////////

	///////////////////////////  Right leg quaternion //////////////////////
	Quaternion q16;
	Quaternion q16_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q16_z = Quaternion(coef, 0.0, 0.0, -coef);
	Quaternion q16_y = Quaternion(coef, 0.0, coef, 0.0);
	Quaternion q16_rotx = Quaternion::calcCrossProduct(q16_x, q17);
	Quaternion q16_h = Quaternion(0.0, 0.0, 0.0, -1.0);
	q16 = Quaternion::calcCrossProduct(q16_h, q17);
	Quaternion q16_b = Quaternion::calcCrossProduct(q16_y, q16);
	q16.w = q16_b.w;
	q16.x = -q16_b.z;
	q16.y = -q16_b.y;
	q16.z = -q16_b.x;

	Quaternion  q12_con;
	q12_con.w = q12.w;
	q12_con.x = -q12.x;
	q12_con.y = -q12.y;
	q12_con.z = -q12.z;
	q16 = Quaternion::calcCrossProduct(q12_con, q16);
	//////////////////////////////////////////////////////////////////////


	///////////////////////////  left hipe quaternion //////////////////////
	Quaternion q14;
	Quaternion q14_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q14_z = Quaternion(coef, 0.0, 0.0, -coef);
	Quaternion q14_y = Quaternion(coef, 0.0, -coef, 0.0);
	Quaternion q14_rotx = Quaternion::calcCrossProduct(q14_x, q15);

	Quaternion q14_h = Quaternion(0.0, 0.0, 0.0, 1.0);
	q14 = Quaternion::calcCrossProduct(q14_h, q15);
	Quaternion q14_b = Quaternion::calcCrossProduct(q14_y, q14);
	q14.w = q14_b.w;
	q14.x = q14_b.z;
	q14.y = -q14_b.y;
	q14.z = q14_b.x;
	//q14 = Quaternion::calcCrossProduct(q2_h, q14);
	//////////////////////////////////////////////////////////////////////


	///////////////////////////  left leg quaternion //////////////////////
	Quaternion q18;
	Quaternion q18_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q18_z = Quaternion(coef, 0.0, 0.0, -coef);
	Quaternion q18_y = Quaternion(coef, 0.0, -coef, 0.0);
	Quaternion q18_rotx = Quaternion::calcCrossProduct(q18_x, q19);
	Quaternion q18_h = Quaternion(0.0, 0.0, 0.0, 1.0);
	q18 = Quaternion::calcCrossProduct(q18_h, q19);
	Quaternion q18_b = Quaternion::calcCrossProduct(q18_y, q18);
	q18.w = q18_b.w;
	q18.x = q18_b.z;
	q18.y = -q18_b.y;
	q18.z = q18_b.x;

	Quaternion  q14_con;
	q14_con.w = q14.w;
	q14_con.x = -q14.x;
	q14_con.y = -q14.y;
	q14_con.z = -q14.z;
	q18 = Quaternion::calcCrossProduct(q14_con, q18);
	//////////////////////////////////////////////////////////////////////


	////////////// Right for Arm  quaternion /////////////////////////////
	Quaternion q8;
	Quaternion q8_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q8_z = Quaternion(coef, 0.0, 0.0, coef);
	Quaternion q8_y = Quaternion(coef, 0.0, coef, 0.0);
	Quaternion q8_rotx = Quaternion::calcCrossProduct(q8_x, q9);

	q8.w = q8_rotx.w;
	q8.x = q8_rotx.y;
	q8.y = q8_rotx.z;
	q8.z = q8_rotx.x;
	q8 = Quaternion::calcCrossProduct(q8_y, q8);
	Quaternion q8_xx = Quaternion(0.0, 1.0, 0.0, 0.0);
	Quaternion q8_zz = Quaternion(0.0, 0.0, 0.0, 1.0);
	Quaternion  q8_va = Quaternion(q8.w, q8.x, q8.y, q8.z);

	q8 = Quaternion::calcCrossProduct(q2_h, q8);
	q8.w = q8_va.w;
	q8.x = -q8_va.x;
	q8.y = -q8_va.y;
	q8.z = q8_va.z;

	Quaternion q5_con;
	q5_con.w = q5.w;
	q5_con.x = -q5.x;
	q5_con.y = -q5.y;
	q5_con.z = -q5.z;
	q8 = Quaternion::calcCrossProduct(q5_con, q8);
	////////////////////////////////////////////////////////////////////////

	////////////// Left for Arm  quaternion ////////////////////////////////
	Quaternion q22;
	Quaternion q22_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q22_z = Quaternion(coef, 0.0, 0.0, coef);
	Quaternion q22_y = Quaternion(coef, 0.0, -coef, 0.0);
	Quaternion q22_rotx = Quaternion::calcCrossProduct(q22_x, q21);

	q22.w = q22_rotx.w;
	q22.x = q22_rotx.y;
	q22.y = q22_rotx.z;
	q22.z = q22_rotx.x;
	q22 = Quaternion::calcCrossProduct(q22_y, q22);

	Quaternion q22_xx = Quaternion(0.0, 1.0, 0.0, 0.0);
	Quaternion q22_zz = Quaternion(0.0, 0.0, 0.0, 1.0);
	Quaternion  q22_va = Quaternion(q22.w, q22.x, q22.y, q22.z);

	q22 = Quaternion::calcCrossProduct(q2_h, q8);
	q22.w = q22_va.w;
	q22.x = q22_va.x;
	q22.y = -q22_va.y;
	q22.z = -q22_va.z;

	Quaternion q6_con;
	q6_con.w = q6.w;
	q6_con.x = -q6.x;
	q6_con.y = -q6.y;
	q6_con.z = -q6.z;
	q22 = Quaternion::calcCrossProduct(q6_con, q22);

	//////////////////////////////////////////////////////////////////////////



	////////////// Right hand  quaternion ////////////////////////////////////
	Quaternion q27;
	Quaternion q27_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q27_z = Quaternion(coef, 0.0, 0.0, coef);
	Quaternion q27_y = Quaternion(coef, 0.0, -coef, 0.0);
	Quaternion q27_rotx = Quaternion::calcCrossProduct(q27_x, q25);
	q27.w = q27_rotx.w;
	q27.x = q27_rotx.y;
	q27.y = q27_rotx.z;
	q27.z = q27_rotx.x;
	Quaternion q27_xx = Quaternion(0.0, 1.0, 0.0, 0.0);
	Quaternion q27_zz = Quaternion(0.0, 0.0, 0.0, 1.0);
	Quaternion  q27_va = Quaternion(q27.w, q27.x, q27.y, q27.z);
	q27 = Quaternion::calcCrossProduct(q2_h, q27);
	q27.w = q27_va.w;
	q27.x = q27_va.x;
	q27.y = q27_va.y;
	q27.z = q27_va.z;
	Quaternion q8_con;
	q8_con.w = q8.w;
	q8_con.x = -q8.x;
	q8_con.y = -q8.y;
	q8_con.z = -q8.z;
	q27 = Quaternion::calcCrossProduct(q8_con, q27);
	//////////////////////////////////////////////////////////////////////////


	////////////// Left Hand  quaternion ///////////////////////////////////
	Quaternion q28;
	Quaternion q28_x = Quaternion(coef, -coef, 0.0, 0.0);
	Quaternion q28_z = Quaternion(coef, 0.0, 0.0, coef);
	Quaternion q28_y = Quaternion(coef, 0.0, -coef, 0.0);
	Quaternion q28_rotx = Quaternion::calcCrossProduct(q28_x, q26);

	q28.w = q28_rotx.w;
	q28.x = q28_rotx.y;
	q28.y = q28_rotx.z;
	q28.z = q28_rotx.x;
	q28 = Quaternion::calcCrossProduct(q28_y, q28);
	Quaternion q28_xx = Quaternion(0.0, 1.0, 0.0, 0.0);
	Quaternion q28_zz = Quaternion(0.0, 0.0, 0.0, 1.0);
	Quaternion  q28_va = Quaternion(q28.w, q28.x, q28.y, q28.z);
	q28 = Quaternion::calcCrossProduct(q2_h, q28);
	q28.w = q28_va.w;
	q28.x = q28_va.x;
	q28.y = -q28_va.y;
	q28.z = -q28_va.z;
	Quaternion q22_con;
	q22_con.w = q22.w;
	q22_con.x = -q22.x;
	q22_con.y = -q22.y;
	q22_con.z = -q22.z;
	q28 = Quaternion::calcCrossProduct(q22_con, q28);

	//ManNiiAvatarPosture posture;
	manNiiPosture.joint[ManNiiPosture::WAIST_JOINT1].quaternion = q1_rot;
	//manNiiPosture.joint[HEAD_JOINT0].quaternion = q30;
	manNiiPosture.joint[ManNiiPosture::RARM_JOINT2].quaternion = q5;
	manNiiPosture.joint[ManNiiPosture::LARM_JOINT2].quaternion = q6;
	manNiiPosture.joint[ManNiiPosture::RLEG_JOINT2].quaternion = q12;
	manNiiPosture.joint[ManNiiPosture::LLEG_JOINT2].quaternion = q14;
	manNiiPosture.joint[ManNiiPosture::RLEG_JOINT4].quaternion = q16;
	manNiiPosture.joint[ManNiiPosture::LLEG_JOINT4].quaternion = q18;
	manNiiPosture.joint[ManNiiPosture::RARM_JOINT3].quaternion = q8;
	manNiiPosture.joint[ManNiiPosture::LARM_JOINT3].quaternion = q22;
//	this->posture.joint[ManNiiPosture::RARM_JOINT5].quaternion = q27;
//	this->posture.joint[ManNiiPosture::LARM_JOINT5].quaternion = q28;

}


///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void MirrorTherapyController::readIniFile()
{
	std::ifstream ifs(this->parameterFileName.c_str());

	// Parameter file is "not" exists.
	if (ifs.fail())
	{
		std::cout << "Not exist : " << this->parameterFileName << std::endl;
		std::cout << "Use default parameter." << std::endl;

		this->kinectV2ServiceName    = SERVICE_NAME_KINECT_V2;
		this->kinectV2DeviceType     = DEV_TYPE_KINECT_V2;
		this->kinectV2DeviceUniqueID = DEV_UNIQUE_ID_0;

		this->oculusDK1ServiceName    = SERVICE_NAME_OCULUS;
		this->oculusDK1DeviceType     = DEV_TYPE_OCULUS_DK1;
		this->oculusDK1DeviceUniqueID = DEV_UNIQUE_ID_0;
	}
	// Parameter file is exists.
	else
	{
		try
		{
			std::cout << "Read " << this->parameterFileName << std::endl;
			boost::property_tree::ptree pt;
			boost::property_tree::read_ini(this->parameterFileName, pt);

			this->kinectV2ServiceName    = pt.get<std::string>(PARAMETER_FILE_KEY_KINECTV2_SERVICE_NAME);
			this->kinectV2DeviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_KINECTV2_DEVICE_TYPE);
			this->kinectV2DeviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_KINECTV2_DEVICE_UNIQUE_ID);

			this->oculusDK1ServiceName    = pt.get<std::string>(PARAMETER_FILE_KEY_OCULUS_SERVICE_NAME);
			this->oculusDK1DeviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_OCULUS_DEVICE_TYPE);
			this->oculusDK1DeviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_OCULUS_DEVICE_UNIQUE_ID);
		}
		catch (boost::exception &ex)
		{
			std::cout << this->parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		}
	}

	std::cout << PARAMETER_FILE_KEY_KINECTV2_SERVICE_NAME     << ":" << this->kinectV2ServiceName    << std::endl;
	std::cout << PARAMETER_FILE_KEY_KINECTV2_DEVICE_TYPE      << ":" << this->kinectV2DeviceType     << std::endl;
	std::cout << PARAMETER_FILE_KEY_KINECTV2_DEVICE_UNIQUE_ID << ":" << this->kinectV2DeviceUniqueID << std::endl;

	std::cout << PARAMETER_FILE_KEY_OCULUS_SERVICE_NAME     << ":" << this->oculusDK1ServiceName    << std::endl;
	std::cout << PARAMETER_FILE_KEY_OCULUS_DEVICE_TYPE      << ":" << this->oculusDK1DeviceType     << std::endl;
	std::cout << PARAMETER_FILE_KEY_OCULUS_DEVICE_UNIQUE_ID << ":" << this->oculusDK1DeviceUniqueID << std::endl;
}
