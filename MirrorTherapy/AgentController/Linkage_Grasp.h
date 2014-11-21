/*
 * ===========================================================================================================
 * @brief  Apply joint angle information from Kinect service to humanoid agent.
 * @file   Linkage_Grasp.cpp
 * @date   2013/4/18
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

/*!
 * @brief Define circular constant.
 */
#define PI 3.141592

/*!
 * @brief Translate degree to radian.
 */
#define DEG2RAD(DEG) ( (PI) * (DEG) / 180.0 )

/*!
 * @brief Defien Kinect service name.
 */
#define SIGSERVICE "SIGKINECT_Linkage"

/*!
 * @brief Define object name grasped
 */
#ifndef _USE_FOOT
#define GRASP_OBJECT "Tray"
#else
#define GRASP_OBJECT "Ball"
#endif

/*!
 * @brief Define port number in Kinect service.
 * @brief Port forwarding is better.
 */
#ifdef _STATIC_PORT
#define SIGPORT 9040
#endif

/*!
 * @brief Define data type of collided object as short name. 
 */
typedef CollisionEvent::WithC C;

/*!
 * @brief  Define the order of grasping parts. this data mekes easy to invert left and right. 
 */
enum TARGET_LINK {
    LEFT,
    RIGHT
};

/*!
 * @brief Define the order of coordinate axes.
 */
enum TARGET_AXIS {
    X,
    Y,
    Z
};

/*!
 * @brief Define lower limit of y value in grasping object.
 */
#ifndef _USE_FOOT
#define LIMIT_Y 65.0
#else
#define LIMIT_Y 10.0
#endif


