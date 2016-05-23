/*
 * LinkageController.cpp
 *
 *  Created on: 2016/04/26
 *      Author: Yamada
 */

#include "LinkageController.h"

const std::string LinkageCommon::limbModes[LimbMode_Count] = { "HAND", "FOOT" };
const std::string LinkageCommon::tableName           = "linkage_table";

const std::string AgentController::parameterFileName = "LinkageController.ini";

const std::string LinkageController::paramFileKeyKinectV2ServiceName    = "KinectV2.service_name";
const std::string LinkageController::paramFileKeyKinectV2Devicetype     = "KinectV2.device_type";
const std::string LinkageController::paramFileKeyKinectV2DeviceUniqueID = "KinectV2.device_unique_id";

const std::string LinkageController::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string LinkageController::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";

const std::string LinkageController::paramFileKeyOculusDK2ServiceName   = "OculusDK2.service_name";
const std::string LinkageController::paramFileKeyOculusDK2Devicetype    = "OculusDK2.device_type";
const std::string LinkageController::paramFileKeyOculusDK2DeviceUniqueID= "OculusDK2.device_unique_id";

const std::string LinkageController::paramFileKeyLinkageGraspGUIServiceName        = "LinkageGraspGUI.service_name";

const std::string LinkageController::paramFileKeyLinkageGraspLimbMode           = "LinkageGrasp.limb_mode";
const std::string LinkageController::paramFileKeyLinkageGraspGraspMode          = "LinkageGrasp.grasp_mode";
const std::string LinkageController::paramFileKeyLinkageGraspReverseMode        = "LinkageGrasp.reverse_mode";

const std::string LinkageController::paramFileKeyLinkageGraspLinkObjList4Hand        = "LinkageGrasp.link_obj_4_hand";
const std::string LinkageController::paramFileKeyLinkageGraspLinkObjList4Foot        = "LinkageGrasp.link_obj_4_foot";

const std::string LinkageController::paramFileKeyLinkageGraspFixedWaist             = "LinkageGrasp.fixed_waist";
const std::string LinkageController::paramFileKeyLinkageGraspCorrectionAngle        = "LinkageGrasp.correction_angle";

const std::string LinkageController::msgKeyLimbMode    = "LIMB_MODE";
const std::string LinkageController::msgKeyGraspMode   = "GRASP_MODE";
const std::string LinkageController::msgKeyReverseMode = "REVERSE_MODE";
const std::string LinkageController::msgKeyFixedWaist  = "FIXED_WAIST";

const std::string LinkageController::graspModes[GraspMode_Count]     = { "SANDWICH", "GRASP_RIGHT", "GRASP_LEFT" };
const std::string LinkageController::reverseModes[ReverseMode_Count] = { "LEFT", "RIGHT", "NOT_REVERSE" };

const std::string LinkageController::rightLink4Hand = "RARM_LINK7";
const std::string LinkageController::leftLink4Hand  = "LARM_LINK7";
const std::string LinkageController::rightLink4Foot = "RLEG_LINK6";
const std::string LinkageController::leftLink4Foot  = "LLEG_LINK6";

const std::string LinkageController::chairName           = "linkage_chair";

const bool        LinkageController::isPositionFixed = true;



/*
 * Collision detection.
 */
void LinkageController::onCollision(CollisionEvent &evt)
{
}


/*
 * Create and return instance of this controller.
 */
extern "C" Controller * createController()
{
	return new LinkageController;
}

