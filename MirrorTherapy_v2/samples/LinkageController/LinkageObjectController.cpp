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
#include "LinkageObjectController.h"

#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

const std::string LinkageCommon::limbModes[LimbMode_Count] = { "HAND", "FOOT" };
const std::string LinkageCommon::tableName  = "table";

const double LinkageObjectController::gravity = -980.665;


/*
 * Initialize this controller.
 */
void LinkageObjectController::onInit(InitEvent &evt)
{
	try
	{
		this->elapsedTimeFromReleased = 0.0;

		SimObj *myself = getObj(myname());

		if (!myself->dynamics())
		{
			// Save initial rotation for keeping initial rotation.
			myself->getRotation(this->iniRot);
		}

		// Save initial position.
		myself->getPosition(this->iniPos);

		// Shift position of the table.
		SimObj *table = getObj(LinkageCommon::tableName.c_str());
		table->getPosition(this->tableIniPos);

		if(iniPos.y() > this->tableIniPos.y())
		{
			this->limbModeType = LinkageCommon::LimbModeType::HAND;
		}
		else
		{
			this->limbModeType = LinkageCommon::LimbModeType::FOOT;
		}

		this->minPos.x(-10000.0);           // No limit.
		this->minPos.y(this->iniPos.y());   // Same as initial position.
		this->minPos.z(this->iniPos.z()-8); // -8 is fixed value.

		this->maxPos.x(+10000.0);           // No limit.
		this->maxPos.y(+10000.0);           // No limit.
		this->maxPos.z(this->iniPos.z()+8); // +8 is fixed value.
	}
	catch (SimObj::Exception &err)
	{
		LOG_MSG(("Exception: %s", err.msg()));
	}
}


/*
 * Movement of the avatar.
 */
double LinkageObjectController::onAction(ActionEvent &evt)
{
	double interval = 0.1;

	try
	{
		SimObj *myself = getObj(myname());

		if (!myself->dynamics())
		{
			// Keep initial rotation.
			myself->setRotation(this->iniRot);
		}

		// When the avatar is not grasped.
		if(!this->isGrasped)
		{
			Vector3d myPos;
			myself->getPosition(myPos);

			// If the object is in free fall.
			if(myPos.y() > this->iniPos.y())
			{
				/*
				 * Calculate position of object. And set position of object.
				 */
				this->elapsedTimeFromReleased+=interval;

				//Calculate distance of free fall.
				//  distance = 1/2gt^2-1/2g(t-a)^2 = ag(2t-a)/2
				double distanceOfFreeFall = interval*this->gravity*(2.0*this->elapsedTimeFromReleased-interval)/2.0;

				double newYpos = myPos.y()+distanceOfFreeFall;

				Vector3d newTargetPos;
				newTargetPos.x(myPos.x());
				newTargetPos.y(newYpos);
				newTargetPos.z(myPos.z());

				//Reposition after landing.
				if(newTargetPos.y() <= this->iniPos.y())
				{
					if(newTargetPos.y() < this->minPos.y()){ newTargetPos.y(this->minPos.y()); }
					if(newTargetPos.z() < this->minPos.z()){ newTargetPos.z(this->minPos.z()); }
					if(newTargetPos.z() > this->maxPos.z()){ newTargetPos.z(this->maxPos.z()); }
				}

				myself->setPosition(newTargetPos);
			}
		}
		else
		{
			this->elapsedTimeFromReleased = 0.0;
		}
	}
	catch (SimObj::Exception &err)
	{
		LOG_MSG(("Exception: %s", err.msg()));
	}

	return interval;
}


/*
 * Receive message.
 */
void LinkageObjectController::onRecvMsg(RecvMsgEvent &evt)
{
	const std::string msg = evt.getMsg();

	// Split message by ":".
	std::vector<std::string> msgItems;
	boost::split(msgItems, msg, boost::is_any_of(":"));

	std::string msgType = msgItems[0];

	if(msgType=="GRASP")
	{
		this->isGrasped = true;
	}
	else if(msgType=="RELEASE")
	{
		this->isGrasped = false;
	}
	else if(msgType=="RESTART")
	{
		if(msgItems.size()!=2)
		{
			LOG_MSG(("Illegal message: %s", msg.c_str()));
			return;
		}

		std::string limbMode = msgItems[1];

		//Offset for changing limb mode.
		double offset = 0.0;

		if(limbMode!=LinkageCommon::limbModes[this->limbModeType])
		{
			offset= 1000.0; // Long enough.
		}

		SimObj *myself = getObj(myname());

		Vector3d newPos;
		newPos.x(this->iniPos.x());
		newPos.y(this->iniPos.y());
		newPos.z(this->iniPos.z()+offset);

		myself->setPosition(newPos);
	}
	else if(msgType=="SET_POSITION")
	{
		if(msgItems.size()!=4)
		{
			LOG_MSG(("Illegal message: %s", msg.c_str()));
			return;
		}

		double newx = std::atof(msgItems[1].c_str());
		double newy = std::atof(msgItems[2].c_str());
		double newz = std::atof(msgItems[3].c_str());

		SimObj *myself = getObj(myname());

		Vector3d newPos;
		newPos.x(newx);
		newPos.y(newy);
		newPos.z(newz);

		// Reposition after landing.
		if(newPos.y() <= this->minPos.y())
		{
			if(newPos.y() < this->minPos.y()){ newPos.y(this->minPos.y()); }
			if(newPos.z() < this->minPos.z()){ newPos.z(this->minPos.z()); }
			if(newPos.z() > this->maxPos.z()){ newPos.z(this->maxPos.z()); }
		}

		myself->setPosition(newPos);
	}
}

/*
 * Collision detection.
 */
void LinkageObjectController::onCollision(CollisionEvent &evt)
{
}

/*
 * Create and return instance of this controller.
 */
extern "C" Controller * createController ()
{
	return new LinkageObjectController;
}

