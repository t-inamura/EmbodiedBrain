/*
 * SenseOfSelfController.h
 *
 *  Created on: 2015/12/21
 *      Author: Yamada@tome
 */
#ifndef SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_LOWER_BODY_H
#define SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_LOWER_BODY_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>
#include <sigverse/commonlib/comm/controller/Controller.h>

#include <sigverse/plugin/controller/common/AvatarController.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>


#define DEG2RAD(DEG) ( (M_PI) * (DEG) / 180.0 )

class SenseOfSelfController_lowerbody : public ManNiiAvatarController
{
public:
	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);
};

#endif // SIGVERSE_OPEN_HOUSE_2015_CONTROLLER_LOWER_BODY_H
