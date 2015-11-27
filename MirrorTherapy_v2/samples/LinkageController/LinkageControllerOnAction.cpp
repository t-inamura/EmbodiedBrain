/*
 * LinkageControllerOnAction.cpp
 *
 *  Created on: 2015/09/04
 *      Author: Yamada@tome
 */

#include "LinkageController.h"

#include <ctime>
#include <sys/time.h>

/*
 * Movement of the avatar.
 */
double LinkageController::onAction(ActionEvent &evt)
{
	const double interval = 0.1; // 0.1[s]

	try
	{
//		struct timeval start_timeval, end_timeval;
//		double sec_timeofday;
//
//		gettimeofday( &start_timeval, NULL );

		if(this->graspMode==graspModes[SANDWICH])
		{
			this->checkGraspStatus4Sandwich();
		}
		else if(this->graspMode==graspModes[GRASP_RIGHT] || this->graspMode==graspModes[GRASP_LEFT])
		{
			this->checkGraspStatus4Grasp();
		}

//		gettimeofday( &end_timeval, NULL );
//		sec_timeofday = (end_timeval.tv_sec - start_timeval.tv_sec) + (end_timeval.tv_usec - start_timeval.tv_usec) / 1000.0;
//		std::cout << "onAction time is " << sec_timeofday << std::setfill('0') << 3 << "[ms]" << std::endl;
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
	catch(...)
	{
		LOG_MSG(("Exception occurred in onAction."));
	}

	return checkServiceForOnAction(interval);
}


/*
 * Check service provider in onAction method.
 *
 * Return a new interval of onAction.
 */
double LinkageController::checkServiceForOnAction(const double intervalOfOnAction)
{
	try
	{
		static time_t startTime = 0;
		time_t now;

		// Only the first time.
		if(startTime==0)
		{
			std::time( &startTime );

			if(this->intervalOfCheckingService < intervalOfOnAction)
			{
				LOG_MSG(("A checking service interval is shorter than onAction interval. So repeat onAction using a checking service interval."));
			}
		}

		std::time( &now );

		// Get elapsed time from last checking.
		double elapsedTime = std::difftime( now, startTime );

		// If enough time has elapsed, check service provider.
		if(elapsedTime > this->intervalOfCheckingService)
		{
			// Reset startTime;
			startTime = now;

			// Check Kinect v2 service provider.
			bool kinectV2Available = checkService(this->kinectV2DeviceManager.serviceName);

			if (kinectV2Available && this->kinectV2DeviceManager.service == NULL)
			{
				this->kinectV2DeviceManager.service = connectToService(this->kinectV2DeviceManager.serviceName);
			}
			else if (!kinectV2Available && this->kinectV2DeviceManager.service != NULL)
			{
				this->kinectV2DeviceManager.service = NULL;
			}

			// Check Oculus Rift DK1 service provider.
			bool oculusDK1Available = checkService(this->oculusDK1DeviceManager.serviceName);

			if (oculusDK1Available && this->oculusDK1DeviceManager.service == NULL)
			{
				this->oculusDK1DeviceManager.service = connectToService(this->oculusDK1DeviceManager.serviceName);
			}
			else if (!oculusDK1Available && this->oculusDK1DeviceManager.service != NULL)
			{
				this->oculusDK1DeviceManager.service = NULL;
			}

			// Check GUI service provider.
			bool guiAvailable = checkService(this->guiServiceName);

			if (guiAvailable && this->guiService == NULL)
			{
				this->guiService = connectToService(this->guiServiceName);
			}
			else if (!guiAvailable && this->guiService != NULL)
			{
				this->guiService = NULL;
			}
		}
	}
	catch(...)
	{
		LOG_MSG(("Exception occurred in checkServiceForThread."));
	}

	// Change interval of onAction in interval of checking service.
	if(this->intervalOfCheckingService < intervalOfOnAction)
	{
		return this->intervalOfCheckingService;
	}

	return intervalOfOnAction;
}


/*
 * Check grasp status for SANDWICH. SANDWICH is one of the grasp mode.
 */
void LinkageController::checkGraspStatus4Sandwich()
{
	SimObj *myself, *target;
	Vector3d myPartsPos, targetPos;
	std::string targetName;

	// Get nearest target(linkage object) info.
	// And distance2target is the distance between myPartsPos and targetPos.
	double distance2target = this->getNearestTargetInfo(&myself, myPartsPos, &target, targetPos, targetName);

	// The distance between both links at this time.
	Vector3d distanceBetweenBothLinksNow;
	Vector3d rightPos, leftPos;
	myself->getParts(this->rightLink.c_str())->getPosition(rightPos);
	myself->getParts(this->leftLink .c_str())->getPosition(leftPos);

	distanceBetweenBothLinksNow.x(rightPos.x() - leftPos.x());
	distanceBetweenBothLinksNow.y(rightPos.y() - leftPos.y());
	distanceBetweenBothLinksNow.z(rightPos.z() - leftPos.z());

	double graspRadius = target->getAttr("graspRadius").getDouble();

	// When the avatar is grasping object.
	if (this->isGrasping)
	{
		// Release.
		if (distance2target > graspRadius+10 || distanceBetweenBothLinksNow.length() > 2.0*graspRadius+10) // 10 is the play.
		{
			this->isGrasping = false;
			// Send message to release target object.
			this->sendMsg(targetName, "RELEASE");

			LOG_MSG(("Release %s.", targetName.c_str()));
		}
		// Doesn't release.
		else
		{
			Vector3d newTargetPos;
			newTargetPos.x(myPartsPos.x() + distanceOfAvatarAndGraspedObject.x());
			newTargetPos.y(myPartsPos.y() + distanceOfAvatarAndGraspedObject.y());
			newTargetPos.z(myPartsPos.z() + distanceOfAvatarAndGraspedObject.z());

//			if(this->isWaistFixed){ newTargetPos.x(0.0); }

			// Send message to move target object.
			std::stringstream msgSS;
			msgSS << "SET_POSITION:" << newTargetPos.x() << ":" << newTargetPos.y() << ":" << newTargetPos.z();
			this->sendMsg(targetName, msgSS.str());

//			std::cout << "setpos:" << myPartsPos.x() << ", dis:"<< diffAvatarAndGraspedObject.x() << std::endl;
		}
	}
	// When the avatar is not grasping object.
	else
	{
		if(distance2target < graspRadius+3 && distanceBetweenBothLinksNow.length() < 2.0*graspRadius && this->distanceBetweenBothLinks > 2.0*graspRadius) // 3 is the play.
		{
			// Send message to change state of grasp.
			this->sendMsg(targetName, "GRASP");

			// Calculate distance between avatar and grasped object.
			this->distanceOfAvatarAndGraspedObject.x(targetPos.x()-myPartsPos.x());
			this->distanceOfAvatarAndGraspedObject.y(targetPos.y()-myPartsPos.y());
			this->distanceOfAvatarAndGraspedObject.z(targetPos.z()-myPartsPos.z());

			this->isGrasping = true;

			LOG_MSG(("Grasped the %s.", targetName.c_str()));
		}
	}

	this->distanceBetweenBothLinks = distanceBetweenBothLinksNow.length();
}


/*
 * Check grasp status for GRASP_RIGHT and GRASP_LEFT. GRASP_RIGHT and GRASP_LEFT are one of the grasp mode.
 */
void LinkageController::checkGraspStatus4Grasp()
{
	SimObj *myself, *target;
	Vector3d myPartsPos, targetPos;
	std::string targetName;

	// Get nearest target(linkage object) info.
	// And distance2target is the distance between myPartsPos and targetPos.
	double distance2target = this->getNearestTargetInfo(&myself, myPartsPos, &target, targetPos, targetName);

	// Get state of hand. Closed or not.
	bool isLeftHandClosedNew  = this->isHandClosed(this->leftHandStateHistory);
	bool isRightHandClosedNew = this->isHandClosed(this->rightHandStateHistory);

	double graspRadius = target->getAttr("graspRadius").getDouble();

	// When the avatar is grasping object.
	if (this->isGrasping)
	{
		// Release
		if (distance2target  > graspRadius+10 || // 10 is the play.
		   (this->graspMode==graspModes[GRASP_RIGHT] && !isRightHandClosedNew) ||
		   (this->graspMode==graspModes[GRASP_LEFT] && !isLeftHandClosedNew))
		{
			this->isGrasping = false;
			// Send message to release target object.
			this->sendMsg(targetName, "RELEASE");

			LOG_MSG(("Release %s.", targetName.c_str()));
		}
		// Doesn't release.
		else
		{
			Vector3d newTargetPos;
			newTargetPos.x(myPartsPos.x() + distanceOfAvatarAndGraspedObject.x());
			newTargetPos.y(myPartsPos.y() + distanceOfAvatarAndGraspedObject.y());
			newTargetPos.z(myPartsPos.z() + distanceOfAvatarAndGraspedObject.z());

//			if(this->isWaistFixed){ newTargetPos.x(0.0); }

			// Send message to move target object.
			std::stringstream msgSS;
			msgSS << "SET_POSITION:" << newTargetPos.x() << ":" << newTargetPos.y() << ":" << newTargetPos.z();
			this->sendMsg(targetName, msgSS.str());

//			std::cout << "setpos:" << myPartsPos.x() << ", dis:"<< diffAvatarAndGraspedObject.x() << std::endl;
		}
	}
	// When the avatar is not grasping object.
	else
	{
//		std::cout << "diff.length():" << diff.length() << ", checkTrying:" << this->checkTrying2Grasp4Grasp(isRightHandClosedNew, isLeftHandClosedNew) << std::endl;

		if(distance2target < graspRadius && this->checkTrying2Grasp4Grasp(isRightHandClosedNew, isLeftHandClosedNew))
		{
			// Send message to change state of grasp.
			this->sendMsg(targetName, "GRASP");

			// Calculate distance between avatar and grasped object.
			this->distanceOfAvatarAndGraspedObject.x(targetPos.x()-myPartsPos.x());
			this->distanceOfAvatarAndGraspedObject.y(targetPos.y()-myPartsPos.y());
			this->distanceOfAvatarAndGraspedObject.z(targetPos.z()-myPartsPos.z());

			this->isGrasping = true;

			LOG_MSG(("Grasped the %s.", targetName.c_str()));
		}
	}

	this->isLeftHandClosed  = isLeftHandClosedNew;
	this->isRightHandClosed = isRightHandClosedNew;
}

/*
 * Get nearest target(linkage object) info through args.
 */
double LinkageController::getNearestTargetInfo(SimObj **myself, Vector3d &myPartsPos, SimObj **target, Vector3d &targetPos, std::string &targetName)
{
	*myself = getObj(myname());

	if(this->graspMode==graspModes[SANDWICH])
	{
		Vector3d rightPos, leftPos, distanceBetweenBothNow;
		(*myself)->getParts(this->rightLink.c_str())->getPosition(rightPos);
		(*myself)->getParts(this->leftLink .c_str())->getPosition(leftPos);

		// MyPartsPos is Midpoint of both links.
		myPartsPos.x((rightPos.x()+leftPos.x())/2.0);
		myPartsPos.y((rightPos.y()+leftPos.y())/2.0);
		myPartsPos.z((rightPos.z()+leftPos.z())/2.0);
	}
	else if(this->graspMode==graspModes[GRASP_RIGHT])
	{
		CParts *myParts  = (*myself)->getParts(this->rightLink.c_str());
		myParts->getPosition(myPartsPos);
	}
	else if(this->graspMode==graspModes[GRASP_LEFT])
	{
		CParts *myParts  = (*myself)->getParts(this->leftLink.c_str());
		myParts->getPosition(myPartsPos);
	}

	std::vector<std::string>::iterator it;

	double distance = 1000.0; // Long enough.

	for (it = this->linkObjNameList.begin(); it != this->linkObjNameList.end(); it++)
	{
		SimObj *targetTmp = getObj((*it).c_str());

		if (!targetTmp){ continue; }

		Vector3d targetPosTmp;
		targetTmp->getPosition(targetPosTmp);

		// Calculate distance between avatar and grasped object.
		Vector3d diff;
		diff.x(targetPosTmp.x()-myPartsPos.x());
		diff.y(targetPosTmp.y()-myPartsPos.y());
		diff.z(targetPosTmp.z()-myPartsPos.z());

		// Search the nearest target.
		if(diff.length() < distance)
		{
			*target    = targetTmp;
			targetPos  = targetPosTmp;
			targetName = (*it);
			distance   = diff.length();
		}
	}

	return distance;
}


/*
 * Checking Hand state. Closed or not.
 * Check the state of the past few points, and then return true if there are closed more than a certain number.
 */
bool LinkageController::isHandClosed(std::list<KinectV2SensorData::HandState> handStateHistory)
{
	if(handStateHistory.size()<numOfHandStateHistory){ return false; }

	std::list<KinectV2SensorData::HandState>::iterator it;

	int cntOfClosed = 0;

	for (it = handStateHistory.begin(); it != handStateHistory.end(); it++)
	{
		if(*it==KinectV2SensorData::HandState::HandState_Closed)
		{
			cntOfClosed++;
		}
	}

	return cntOfClosed >=thresholdNumOfJudgingHandState;
}


/*
 * Check whether trying to grasp for GRASP_RIGHT and GRASP_LEFT. GRASP_RIGHT and GRASP_LEFT are one of the grasp mode.
 */
bool LinkageController::checkTrying2Grasp4Grasp(const bool isRightHandClosedNew, const bool isLeftHandClosedNew) const
{
//	std::cout << "revMode:" << this->reverseMode << ", graspMode:" << this->graspMode << ", rightOld:" << this->isRightHandClosed << ", rightNew:" << isRightHandClosedNew << std::endl;

	if(this->reverseMode==reverseModes[NOT_REVERSE])
	{
		if(this->graspMode==graspModes[GRASP_RIGHT] && !this->isRightHandClosed && isRightHandClosedNew){ return true; }
		if(this->graspMode==graspModes[GRASP_LEFT]  && !this->isLeftHandClosed  && isLeftHandClosedNew) { return true; }
	}
	else if(this->reverseMode==reverseModes[RIGHT])
	{
		if(!this->isLeftHandClosed && isLeftHandClosedNew){ return true; }
	}
	else if(this->reverseMode==reverseModes[LEFT])
	{
		if(!this->isRightHandClosed && isRightHandClosedNew){ return true; }
	}

	return false;
}
