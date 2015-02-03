/*
 * ===========================================================================================================
 * @brief  Controll the camera.
 * @file   CameraController.cpp
 * @date   2014/12/16
 * @author National Institute of Informatics
 * @par    1.0.0
 * ===========================================================================================================
 */

#include "CameraController.h"
#include <fstream>

class CameraController : public Controller
{
private:
	double mRadius;
	int mStep;
	double mTime;
	double mCurrentDeg;
	Vector3d mDefaultPosition;
	Vector3d mDefaultAxis;
	double mDefaultAngle;
	double mFps;
	std::string mCameraMode;

	Vector3d mCurrentAxis;
	double mCurrentAngle; //

public:
	void onInit(InitEvent &evt);
	double onAction(ActionEvent &evt);
	void onRecvMsg(RecvMsgEvent &evt);
};

void CameraController::onInit(InitEvent &evt)
{
	SimObj *myself = getObj(myname());

	mStep = 0;
	mTime = 0.0;
	mCurrentDeg = 0.0;
	mRadius = 100;
	mFps = 0.1;
	mCameraMode = "FrontCamera";
	mCurrentAngle = 0.0;
	mCurrentAxis = Vector3d(0.0, 1.0, 0.0);

	myself->getPosition(mDefaultPosition);
	mDefaultAxis = mCurrentAxis;
	mDefaultAngle = mCurrentAngle;

	// ビデオよう
		//double px, py, pz, ax, ay, az, angle;
		//Vector3d msgPosition;
			//px = 100.0;
			//py = 150.0;
			//pz = 180.0;
			//ax = 0.0;
			//ay = 1.0;
			//az = 0.0;
			//angle = DEG2RAD(180.0);
			//msgPosition = Vector3d(px, py, pz);
			//myself->setPosition(msgPosition);
			//myself->setAxisAndAngle(ax, ay, az, angle);
			//mCurrentAxis = Vector3d(ax, ay, az);
			//mCurrentAngle = angle;

}

double CameraController::onAction(ActionEvent &evt)
{
	SimObj *myself = getObj(myname());

	if (mCameraMode == "revolve") {
		// get current postion.
		Vector3d currentPos;
		myself->getPosition(currentPos);

		//ofsPos << currentPos.x() << "," << currentPos.y() << "," << currentPos.z() << "," << mCurrentAngle << std::endl;

		//
		double theta = DEG2RAD(mCurrentDeg);

		// calc next postion.
		const double rcost = mRadius * cos(theta);
		const double rsint = mRadius * sin(theta);
		Vector3d nextPos;
		nextPos.x(mDefaultPosition.x() + rcost);
		nextPos.z(mDefaultPosition.z() + rsint);
		nextPos.y(currentPos.y());


		const double angle = - atan2(rsint, rcost);
		const double angleOffset = - PI / 2.25;

		/* カメラ画像がガタガタする問題について
		 * (1) カメラの向きを変える
		 * (2) カメラの位置を変える
		 * という処理を行う際に、処理ごとに描画を行っているためカメラ視点の画像が
		 * ガタガタしているように見える。
		 * 対策a: カメラの向きと位置を同時に変更する。
		 * 対策b: (1)直後の再描画をキャンセルして、(2)が終わったら再描画する。
		 * 対策abともに、方法が分からないので保留。
		 */
		myself->setAxisAndAngle(0, 1, 0, angle + angleOffset); // (1)
		myself->setPosition(nextPos); // (2)
		mCurrentAngle = angle + angleOffset;
		mCurrentAxis = Vector3d(0.0, 1.0, 0.0);

		mCurrentDeg = mCurrentDeg + 1;
	}

	// output current postion and direction.
	//Vector3d currentPos;
	//myself->getPosition(currentPos);
	//printf("\rpos, axis, angle: %3.0f, %3.0f, %3.0f, %3.1f, %3.1f, %3.1f, %3.3f",
	//currentPos.x(), currentPos.y(), currentPos.z(), mCurrentAxis.x(), mCurrentAxis.y(), mCurrentAxis.z(), RAD2DEG(mCurrentAngle));
	//fflush(stdout);


	return mFps;
}

void CameraController::onRecvMsg(RecvMsgEvent &evt)
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
			const Vector3d frontPosition = Vector3d(0.0, 105.0, 60.0);
			const double frontAngle = DEG2RAD(-10.0);
			myself->setPosition(frontPosition);
			myself->setAxisAndAngle(1, 0, 0, frontAngle);
//			px = 100.0;
//			py = 150.0;
//			pz = 180.0;
//			ax = 0.0;
//			ay = 1.0;
//			az = 0.0;
//			angle = DEG2RAD(180.0);
//			msgPosition = Vector3d(px, py, pz);
//			myself->setPosition(msgPosition);
//			myself->setAxisAndAngle(ax, ay, az, angle);
//			mCurrentAxis = Vector3d(ax, ay, az);
//			mCurrentAngle = angle;
//			printf("\n");
		}
//		else if (strcmp(msg, "RearCamera") == 0) {
//			px = 100.0;
//			py = 150.0;
//			pz = -90.0;
//			ax = 0.0;
//			ay = 1.0;
//			az = 0.0;
//			angle = DEG2RAD(0.0);
//			msgPosition = Vector3d(px, py, pz);
//			myself->setPosition(msgPosition);
//			myself->setAxisAndAngle(ax, ay, az, angle);
//			mCurrentAxis = Vector3d(ax, ay, az);
//			mCurrentAngle = angle;
//			printf("\n");
//		}
		else if (strcmp(msg, "Default") == 0) {
			myself->setPosition(mDefaultPosition);
			myself->setAxisAndAngle(mDefaultAxis.x(), mDefaultAxis.y(), mDefaultAxis.z(), mDefaultAngle);
		}
		else if (strcmp(msg, "Mode") == 0) {
			std::string mode = strtok(NULL, ",");
			if (mode == "revolve"){
				mCameraMode = "revolve";
			}
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
	return new CameraController;
}
