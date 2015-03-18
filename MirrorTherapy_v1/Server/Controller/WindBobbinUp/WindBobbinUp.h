/*
 * WindBobbinUp.h
 *
 *  Created on: 2015/01/21
 *      Author: sigverse
 */

#ifndef WINDBOBBINUP_H_
#define WINDBOBBINUP_H_

/*
 * =====================================================================
 * @brief Make behavior like wind a bobbin up.
 * @file WindBobbinUp.h
 * @date 2015/01/16
 * @author National Institute of Informatics.
 * =====================================================================
 */

#include <sigverse/Controller.h>
#include <sigverse/modelerr.h>
#include <sigverse/ControllerEvent.h>
#include <sigverse/SimObj.h>
#include <sigverse/Logger.h>
#include <sigverse/Vector3d.h>
#include <sigverse/ViewImage.h>
#include <math.h>

class Quaternion
{
private:
	double mW;
	double mX;
	double mY;
	double mZ;

public:
	Quaternion();
	Quaternion(const double w, const double x, const double y, const double z);
	void getQuaternion(double& w, double &x, double &y, double &z);
	void setQuaternion(const double w, const double x, const double y, const double z);
	double w() const { return mW; };
	double x() const { return mX; };
	double y() const { return mY; };
	double z() const { return mZ; };
};

typedef std::map<std::string, Quaternion> QMap;

class QMapStamp
{
public:
	double timeStamp;
	QMap qmap;
};

class WindBobbinUpController : public Controller
{
public:

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	///@brief Split the message.
	static void splitString(const std::string &inputString, const std::string &delimiter, std::vector<std::string> &output);

	///@brief Make the Quaternion-map by received message.
	void makeQMapFromReceivedMessage(const std::string &message, std::string &firstOfMessage, QMap &qmap);

	///@brief Store the Quaternion-map to the quaternion record.
	void storeQuaternion(const std::string &firstOfMessage, const QMap &qmap, const double currentTimeStamp);

	///@brief Play back.
	void playBack(const std::string &firstOfMessage, QMap &qmap, const double currentTimeStamp);

	///@brief Generate behavior(whole body quaternion) depending on the modes.
	void generateQuaternionDependOnModes(const std::string &firstOfMessage, const double currentTimeStamp, QMap &qmap);

	void reverseArm(const std::string &firstOfMessage, QMap &qmap);
	///@brief Play back.
	//void playBackForOculus(QMap &qmap, const double currentTimeStamp);

	///@brief Generate time stamp.
	const double generateCurrentTimeStamp();

	///@brief Get quaternion map from
	void setQMapFromJointNameAndQuaternionForKinect(QMap &qmap, const std::vector<std::string> &jointNameAndQuaternionsVector);

	void setQMapFromJointNameAndQuaternionForOculus(QMap &qmap, const std::string &message);

private:

	///@brief Pointer to the Kinect service.
	BaseService *kinectService;

	///@brief Pointer to the Oculus service.
	BaseService *oculusService;

	///@brief Mode of robot (reverse right hand, reverse left hand, etc.).
	std::string reverseMode;

	///@brief Flag of delay.
	bool delayFlag;

	///@brief Delay time[ms] of robot reaction.
	double inputDelayTime;

	///@brief Flag of use Kinect.
	bool usingKinect;

	///@brief Flag of use Oculus.
	bool usingOculus;
	bool chk_srv;

	///@brief Mode of received message.
	//std::string kindOfMessage;

	///@brief Size of record to generate delay motion.
	static const int recordBufferSize = 500;

	///@brief Time stamp on initialize.
	struct timeval initTimeVal;

	///@brief Total frames from the process started.
	unsigned long frameNumber;

	///@brief Record of Quaternion-map and its timestamp.
	std::vector<QMapStamp> quaternionRecordForKinect;

	///@brief Record of Quaternion-map and its timestamp.
	std::vector<QMapStamp> quaternionRecordForOculus;

	///@brief Valuables for Oculus service.
	double m_posx, m_posy, m_posz;
	double m_yrot;
	double m_range;
	int m_maxsize;
	double m_qw, m_qy, m_qx, m_qz;
	double pyaw, ppitch, proll;

//	///@brief Waiting frames for delay mode.
//	int initialWaitFrame;
};

Quaternion::Quaternion()
{
	mW = 0;
	mX = 0;
	mY = 0;
	mZ = 0;
}
Quaternion::Quaternion(const double w, const double x, const double y, const double z)
{
	mW = w;
	mX = x;
	mY = y;
	mZ = z;
}
void Quaternion::getQuaternion(double &w, double &x, double &y, double &z)
{
	w = mW;
	x = mX;
	y = mY;
	z = mZ;
}
void Quaternion::setQuaternion(const double w, const double x, const double y, const double z)
{
	mW = w;
	mX = x;
	mY = y;
	mZ = z;
}





#endif /* WINDBOBBINUP_H_ */
