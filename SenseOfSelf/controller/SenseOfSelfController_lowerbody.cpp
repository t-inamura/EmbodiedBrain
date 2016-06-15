/*
 * SenseOfSelfController.cpp
 *
 *  Created on: 2015/12/21
 *      Author: Yamada@tome
 */
#include "SenseOfSelfController_lowerbody.h"

///@brief Initialize this controller.
void SenseOfSelfController_lowerbody::onInit(InitEvent &evt)
{
	SimObj *myself = getObj(myname());

	myself->setJointAngle("HEAD_JOINT1",  DEG2RAD(-25));
	myself->setJointAngle("WAIST_JOINT1", DEG2RAD(+25));

	myself->setJointAngle("LLEG_JOINT2", DEG2RAD(-90));
	myself->setJointAngle("LLEG_JOINT4", DEG2RAD(+90));
	myself->setJointAngle("RLEG_JOINT2", DEG2RAD(-90));
	myself->setJointAngle("RLEG_JOINT4", DEG2RAD(+90));
}

///@brief Movement of the robot.
double SenseOfSelfController_lowerbody::onAction(ActionEvent &evt)
{
	return 1.0;
}

///@brief Message from device.
void SenseOfSelfController_lowerbody::onRecvMsg(RecvMsgEvent &evt)
{
}

extern "C" Controller * createController()
{
	return new SenseOfSelfController_lowerbody;
}

