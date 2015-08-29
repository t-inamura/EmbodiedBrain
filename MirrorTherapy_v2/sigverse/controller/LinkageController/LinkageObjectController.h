/*
 * ==========================================================================================
 * @brief  Controller for Linkage object.
 * @deprecated
 * @file   LinkageObjectController.cpp
 * @date   2015/08/28
 * @author National Institute of Informatics
 * @par    1.0.0
 * ==========================================================================================
 */

#ifndef SIGVERSE_LINKAGE_OBJECT_CONTROLLER_H
#define SIGVERSE_LINKAGE_OBJECT_CONTROLLER_H

#include <sigverse/controller/LinkageController/LinkageCommon.h>

class LinkageObjectController : public Controller
{
public:
	/*
	 * Movement of the avatar.
	 */
	double onAction(ActionEvent &evt);

	/*
	 * Initialize this controller.
	 */
	void onInit(InitEvent &evt);

	/*
	 * Receive message.
	 */
	void onRecvMsg(RecvMsgEvent &evt);

	/*
	 * Collision detection.
	 */
	void onCollision(CollisionEvent &evt);

private:

	// Gravity value. (Unit of length is [cm].)
	static const double gravity;

	// Limb mode type.
	LinkageCommon::LimbModeType limbModeType;

	// Initial rotation.
	Rotation iniRot;

	// Initial position.
	Vector3d iniPos;

	Vector3d minPos; // Minimum position.
	Vector3d maxPos; // Maximum position.

	// Elapsed time from the object is released.
	double elapsedTimeFromReleased;
};

const double LinkageObjectController::gravity = -980.665;

#endif SIGVERSE_LINKAGE_OBJECT_CONTROLLER_H

