/*
 * ==========================================================================================
 * @brief  This object keeps horizontality.
 * @deprecated
 * @file   Linkage_Obon.cpp
 * @date   2013/4/18
 * @author National Institute of Informatics
 * @par    1.0.0
 * ==========================================================================================
 */
#include "Linkage_Grasp.h"

/*!
 * @brief Define lower limit (likes ground) of y value in grasping Obon.
 */
#define LIMIT_Y_REVISE (LIMIT_Y - 1)

/*!
 * @brief Move to the initial position after received order.
 */
class ObonController : public Controller {
    public:
        /*
         * @brief Keep initial angle to keep  horizontality to the ground.
         */
	double onAction(ActionEvent &evt);

        /*!
         * @brief keep initial angle as initial process.
         */
        void onInit(InitEvent &evt);

        /*!
         * @brief Message heard by the robot.
         */
        void onRecvMsg(RecvMsgEvent &evt);

        /*
         * @brief Collision check
         */
        void onCollision(CollisionEvent &evt);

    private:
        /*!
         * @brief Initial angle.
         */
        Rotation br;

};

/*!
 * @brief Obon adjust its orientatnoin to keep horizontality in grasping.
 */
double ObonController::onAction(ActionEvent &evt) 
{
    try {
        /* Keep horizontality to the ground */
        SimObj *myself = getObj(myname());
        if (!myself->dynamics()) 
        { 
            myself->setRotation(br);
        }
        /* Not to be below the prescribed height*/
        Vector3d myself_pos;
        myself->getPosition(myself_pos);
        if (myself_pos.y() < LIMIT_Y_REVISE) 
        {
            myself->setPosition(myself_pos.x(), LIMIT_Y_REVISE, myself_pos.z());
        } 
    } 
    catch (SimObj::Exception &err) 
    {
        LOG_MSG(("Exception: %s", err.msg()));
    }
    return 0.1;
}

/*!
 * @brief Initializing. Keep initial angle.
 */
void ObonController::onInit(InitEvent &evt) 
{
    try {
        SimObj *myself = getObj(myname());
        if (!myself->dynamics()) {
            /* Keep my initial angle */
            myself->getRotation(br);
        }
    } catch (SimObj::Exception &err) {
        LOG_MSG(("Exception: %s", err.msg()));
    }
}

/*!
 * @brief Message robot heard
 */
void ObonController::onRecvMsg(RecvMsgEvent &evt) 
{
}

/*
 * @brief Collision check.
 */
void ObonController::onCollision(CollisionEvent &evt) 
{
}

/*!
 * @brief transmit myself to the sentral server.
 */
extern "C" Controller * createController () 
{
    return new ObonController;
}

