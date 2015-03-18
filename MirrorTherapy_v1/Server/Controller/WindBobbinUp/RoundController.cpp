/*
 * ===========================================================================================================
 * @brief  Controll the round mark.
 * @file   RoundMarker.cpp
 * @date   2015/01/07
 * @author National Institute of Informatics
 * @par    1.0.0
 * ===========================================================================================================
 */

#include <sigverse/Controller.h>
#include <sigverse/modelerr.h>
#include <sigverse/ControllerEvent.h>
#include <sigverse/SimObj.h>
#include <sigverse/Logger.h>
#include <sigverse/Vector3d.h>
#include <sigverse/ViewImage.h>
#include <math.h>
#include <fstream>

#include <sys/time.h>

/*!
 * @brief Define circular constant.
 */
#define PI 3.141592

/*!
 * @brief Translate degree to radian.
 */
#define DEG2RAD(DEG) ( (PI) * (DEG) / 180.0 )

static double RAD2DEG(const double RAD) {
	return RAD * 180.0 / PI;
}

static int RAD2DEG(const int RAD) {
	return (int)(RAD2DEG((double)RAD) + 0.5);
}


class RoundController : public Controller
{
private:
	double mRadius;
	int mStep;
	double mTime;
	double mCurrentDeg;
	Vector3d mRoundCenter;
	double mFps;
	std::string mCameraMode;
	
	double mDeltaTheta;
	
	Vector3d mCurrentAxis;
	double mCurrentAngle; //
	struct timeval mInitTime;
	struct timeval mActionTime;
	double mVelocity;
	
public:
	void onInit(InitEvent &evt);
	double onAction(ActionEvent &evt);
	void onRecvMsg(RecvMsgEvent &evt);
};

void RoundController::onInit(InitEvent &evt)
{
	SimObj *myself = getObj(myname());
	
	mStep = 0;
	mTime = 0.0;
	mCurrentDeg = 0.0;
	mRadius = 30;
	mFps = 30.0;
	mCameraMode = "revolve"; 
	mCurrentAngle = 0.0;
	mCurrentAxis = Vector3d(0.0, 1.0, 0.0);
	mRoundCenter = Vector3d(0.0, 120.0, 140.0);
	mDeltaTheta = 180.0 / mFps;
	
	gettimeofday(&mInitTime, NULL);
	gettimeofday(&mActionTime, NULL);
	
	mVelocity = 180.0 / 1000.0; // velocity of the rounding mark [deg/ms].
}

double RoundController::onAction(ActionEvent &evt)
{
	SimObj *myself = getObj(myname());
	
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	const double actionInterval = (currentTime.tv_sec - mActionTime.tv_sec) * 1000.0 + (currentTime.tv_usec - mActionTime.tv_usec) / 1000.0;
	const double throughTimeFromInit = (currentTime.tv_sec - mInitTime.tv_sec) * 1000.0 + (currentTime.tv_usec - mInitTime.tv_usec) / 1000.0;
	const double theta = mVelocity * throughTimeFromInit;
	
	if (mCameraMode == "revolve") {
		// get current postion.
		Vector3d currentPos;
		myself->getPosition(currentPos);
		
		//ofsPos << currentPos.x() << "," << currentPos.y() << "," << currentPos.z() << "," << mCurrentAngle << std::endl;
		
		// 
		double theta_rad = DEG2RAD(theta);
		
		
		// calc next postion.
		const double rcost = mRadius * cos(theta_rad);
		const double rsint = mRadius * sin(theta_rad);
		Vector3d nextPos;
		nextPos.x(mRoundCenter.x() + rcost);
		nextPos.y(mRoundCenter.y() + rsint);
		nextPos.z(mRoundCenter.z());
		
		myself->setPosition(nextPos);
			
		//mCurrentDeg = mCurrentDeg + mDeltaTheta;		
	}
	
	// output current postion and direction.
	Vector3d currentPos;
	myself->getPosition(currentPos);
	
	//printf("\rpos(3)[cm], angle(1)[deg], interval[s]: %3.0f, %3.0f, %3.0f, %3.3f, %3.3f", 
	//currentPos.x(), currentPos.y(), currentPos.z(), theta, actionInterval);
	//fflush(stdout);
	
	mActionTime = currentTime;
	return 1.0 / mFps;
}

void RoundController::onRecvMsg(RecvMsgEvent &evt)
{
	try {
		
		mCameraMode = "Stop";
		
		SimObj *myself = getObj(myname());
		std::string sender = evt.getSender();
		std::string message = evt.getMsg();
		char *msg = strtok((char*)message.c_str(), " ");
		double px, py, pz, ax, ay, az, angle;
		Vector3d msgPosition;

		if (strcmp(msg, "Position") == 0) {
			px = atof(strtok(NULL, ","));
			py = atof(strtok(NULL, ","));
			pz = atof(strtok(NULL, ","));
			msgPosition = Vector3d(px, py, pz);
			myself->setPosition(msgPosition);
			printf("\n");
		}
		else if (strcmp(msg, "AxisAndAngle") == 0) {
			ax = atof(strtok(NULL, ","));
			ay = atof(strtok(NULL, ","));
			az = atof(strtok(NULL, ","));
			angle = DEG2RAD(atof(strtok(NULL, ",")));
			myself->setAxisAndAngle(ax, ay, az, angle);
			mCurrentAxis = Vector3d(ax, ay, az);
			mCurrentAngle = angle;
			printf("\n");
		}
		else if (strcmp(msg, "FrontCamera") == 0) {
			px = 100.0;
			py = 150.0;
			pz = 180.0;
			ax = 0.0;
			ay = 1.0;
			az = 0.0;
			angle = DEG2RAD(180.0);
			msgPosition = Vector3d(px, py, pz);
			myself->setPosition(msgPosition);
			myself->setAxisAndAngle(ax, ay, az, angle);
			mCurrentAxis = Vector3d(ax, ay, az);
			mCurrentAngle = angle;
			printf("\n");
		}
		else if (strcmp(msg, "RearCamera") == 0) {
			px = 100.0;
			py = 150.0;
			pz = -90.0;
			ax = 0.0;
			ay = 1.0;
			az = 0.0;
			angle = DEG2RAD(0.0);
			msgPosition = Vector3d(px, py, pz);
			myself->setPosition(msgPosition);
			myself->setAxisAndAngle(ax, ay, az, angle);
			mCurrentAxis = Vector3d(ax, ay, az);
			mCurrentAngle = angle;
			printf("\n");
		}
		else if (strcmp(msg, "Mode") == 0) {
			std::string mode = strtok(NULL, ",");
			if (mode == "revolve"){
				mCameraMode = "revolve";
			}
		}
		else if (strcmp(msg, "Velocity") == 0) {
			mVelocity = atof(strtok(NULL, ","));
		}
		else {
            /* Display message received. */
            LOG_MSG(("[%s][%d]%s received message of %s by %s", __FUNCTION__, __LINE__, myname(), message.c_str(), evt.getSender()));

        }
		
    } catch(SimObj::NoAttributeException &err) {
        LOG_MSG(("NoAttributeException: %s", err.msg()));

    } catch(SimObj::AttributeReadOnlyException &err) {
        LOG_MSG(("AttributeReadOnlyException: %s", err.msg()));

    } catch(SimObj::Exception &err) {
        LOG_MSG(("Exception: %s", err.msg()));
    }
}

//自身のインスタンスをSIGVerseに返します  
extern "C" Controller * createController() { 
	return new RoundController;  
}  
