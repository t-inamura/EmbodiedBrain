/*
 * ===========================================================================================================
 * @brief  Apply joint angle information form Kinect V2 to human avator.
 * @file   Linkage_Grasp.cpp
 * @date   2014/11/21
 * @author National Institute of Informatics
 * @par    1.0.0
 * ===========================================================================================================
 */
#include "Linkage_Grasp.h"
#include "MirrorTherapy.h"//20141126 tome-ikeda

#define NONE 0
#define REVERSE_RIGHT_HAND 1
#define REVERSE_LEFT_HAND 2
#define REVERSE_RIGHT_FOOT 3
#define REVERSE_LEFT_FOOT 4


/*!
 * @brief This robot imitates the movement of human left arm with its right arm. 
 When the right hand touches the Obon on the desk, it grasps the Obon.
 */
class LinkageController : public Controller {
    public:
        /*!
         * @brief Movement of the robot
         */
        double onAction(ActionEvent &evt);

        /*! 
         * @brief Message heard by the robot.
         */
        void onRecvMsg(RecvMsgEvent &evt);

        /*!
         * @brief Initializing
         */
        void onInit(InitEvent &evt); 

#ifndef _NO_GRASP
        /*
         * @brief Collision detection.
         */
        void onCollision(CollisionEvent &evt);
#endif

    private:
        /*!
         * @brief Hand opposite to grasping hand.
         */
        const char *GRASP_PARTS;

        /*
         * @brief Get right and left arm parts for invert.
         */
        const char *GRASP_ARM_LINK[2];

        /*
         * @brief Get right and left leg parts for invert.
         */
        const char *GRASP_FOOT_LINK[2];

        int reverse_hand_mode;
        int reverse_foot_mode;


        /*!
         * @brief Put initial position in the xyz coordinate system.
         */
        double pos[3];

        /*!
         * @brief Available range.
         */
        double range;

        /*!
         * @brief Maximum number of joints processed at onece.
         */
        int maxsize;  

        /*!
         * @brief Whether grasping or not.
         */
        bool grasp;

        /*!
         * @brief Initial position of object grasped.
         */
        double gb[3];

        /*!
         * @brief Initial angle of object grasped.
         */
         Rotation br;  

        /*!
         * @brief Pointer to the Kinect service.
         */
        BaseService *service; 

};

/*!
 * @brief Initializing and adjustment of arm position.
 */
void LinkageController::onInit(InitEvent &evt) {

    /* Set default position of myself */
    SimObj *myself = getObj(myname());  
    pos[X] = myself->x();  
    pos[Y] = myself->y();  
    pos[Z] = myself->z();  
    range  = 0.1;

    /* Initialize the service area*/
    service = NULL;
    LOG_MSG(("service is initialized"));

    /* Adjustment of knee angles to sit on the chair */
    /* Root of both legs */
    myself->setJointAngle("RLEG_JOINT2", DEG2RAD(-90)); 
    myself->setJointAngle("LLEG_JOINT2", DEG2RAD(-90)); 
    /* Both knee */
    myself->setJointAngle("RLEG_JOINT4", DEG2RAD( 90)); 
    myself->setJointAngle("LLEG_JOINT4", DEG2RAD( 90)); 

    /* Maximum number processed at once is 15  */  
    //maxsize = 15;  
    maxsize = 100;//20141128tome-ikeda  

 
    /* At the first, the robot does not grasp anything. */
    /* Do not set true. it results bug caused by developing mode settings */
    grasp = false;

//     /* Define right or left of grasping parts*/
//     /* Use hands */
// #ifndef _REVERSE_GRASP
//     /* Left arm based */
//     GRASP_ARM_LINK[LEFT ] = "LARM";
//     GRASP_ARM_LINK[RIGHT] = "RARM";
// #else
//     /* Right arm based */
//     GRASP_ARM_LINK[RIGHT] = "LARM";
//     GRASP_ARM_LINK[LEFT ] = "RARM";
// #endif 
//     /* Use legs*/
// #ifndef _REVERSE_GRASP
//     /* Left arm based */
//     GRASP_FOOT_LINK[LEFT ] = "LLEG";
//     GRASP_FOOT_LINK[RIGHT] = "RLEG";
// #else
//     /* Right arm based */
//     GRASP_FOOT_LINK[RIGHT] = "LLEG";
//     GRASP_FOOT_LINK[LEFT ] = "RLEG";
// #endif

//     /* Define parts grasping*/
// #ifndef _USE_FOOT
//     /* Use hands */
// #ifndef _REVERSE_GRASP
//     /* Left leg based */
//     GRASP_PARTS = "LARM_LINK7";
// #else
//     /* Right leg based  */
//     GRASP_PARTS = "RARM_LINK7";
// #endif
// #else
//     /* Use legs */
// #ifndef _REVERSE_GRASP
//     /*  Left leg based */
//     GRASP_PARTS = "LLEG_LINK6";
// #else
//     /* Right leg based */
//     GRASP_PARTS = "RLEG_LINK6";
// #endif
// #endif

    /* Set position of objects grasped*/
    SimObj *target = getObj(GRASP_OBJECT);
    Vector3d target_pos;
    target->getPosition(target_pos);
    gb[X] = target_pos.x();
    gb[Y] = target_pos.y();
    gb[Z] = target_pos.z();

    /* Get angle of a object grasped*/
    target->getRotation(br);                      

}

/*!
 * @brief Robot movement, for debug. 
 */
double LinkageController::onAction(ActionEvent &evt) {
    if (grasp == true) {
        try {
            /* Keep grasping object*/
            SimObj *myself  = getObj(myname());
            SimObj *grasped = getObj(GRASP_OBJECT);

            if (myself && grasped) {
                if (!myself->dynamics() && !grasped->dynamics()) {
                    /* Keep adjusting the position of object while grasping*/
                    CParts *lparts  = myself->getParts(GRASP_PARTS);

                    /* Get name and position of parts grasped */
                    Vector3d lparts_pos, rparts_pos, lelb_pos, relb_pos;
                    Vector3d grasped_pos;
                    lparts ->getPosition(lparts_pos);
                    grasped->getPosition(grasped_pos);
#ifdef _VERBOSE
                    LOG_MSG(("%s:,x:%f:,y:%f,z:%f", GRASP_OBJECT, grasped_pos.x(), grasped_pos.y(), grasped_pos.z()));
#endif
                    if (grasped_pos.y() < LIMIT_Y) {
                        /* Release object when it is below the predefined position*/
#ifdef _REAL_GRASP
                        lparts->releaseObj();
#endif
                        grasp = false;

                        /* Notice of release object to service provider. */
                        if (service != NULL) {
                            service->sendMsgToSrv("RELEASE");
                            LOG_MSG(("%s send release message to %s service", myname(), SIGSERVICE));
                        }

                        /* Move released object to initial position*/
                        SimObj *target = getObj(GRASP_OBJECT);
                        Vector3d target_pos;
                        target->getPosition(target_pos);

                        /* Set position as its height when it is below the prescribed height(e.g. the case arm pulled down fast) */
                        target->setPosition(target_pos.x(), LIMIT_Y, target_pos.z());                        

#ifdef _OLD_VERSION
                        /* Undo joint angle of the object released*/
                        /* x value */
                        double tx = 2 * asin(br.qx());  
                        double dx = tx + DEG2RAD(-180);  
                        if (dx <= -PI) dx = -1 * dx - PI;
                        target->setAxisAndAngle(1.0, 0.0, 0.0, dx);  

                        /* y value */
                        double ty = 2 * asin(br.qy());  
                        double dy = ty + DEG2RAD(-180);  
                        if (dy <= -PI) dy = -1 * dy - PI;
                        target->setAxisAndAngle(0.0, 1.0, 0.0, dy);  
 
                        /* z value */
                        double tz = 2 * asin(br.qz());  
                        double dz = tz + DEG2RAD(-180);  
                        if (dz <= -PI) dz = -1 * dz - PI;
                        target->setAxisAndAngle(0.0, 0.0, 1.0, dz);  
#endif

                        LOG_MSG(("%s release %s", myname(), GRASP_OBJECT));
                    
                    } else {
                        /* Move object while grasping*/
                        if (lparts_pos.x() != grasped_pos.x() || 
                            lparts_pos.y() != grasped_pos.y() ||
                            lparts_pos.z() != grasped_pos.z()) {
                            /* Adjust poision as it is grasped. */
                            /* Move object about only y axis direction (-5: adustment of hand size) */
                            grasped->setPosition(grasped_pos.x(), lparts_pos.y() - 2, grasped_pos.z()); 
                        }
                    }
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

    /* Called every 0.1 second to execute grasp process*/
    return 0.1;
}



/*!
 * @brief Invert and apply the joint angle information of robot left arm to robot right arm.
 */
void LinkageController::onRecvMsg(RecvMsgEvent &evt) { 
    printf("onRecvMsg\n");

    try {
        /* Output received message as log file.*/
        //std::string message = evt.getMsg();    

        /* Get myself. */  

        /* Get message and message source */
        SimObj *myself = getObj(myname());  
        std::string sender = evt.getSender();    
        std::string message = evt.getMsg(); 
        printf("[%s] %s\n", sender.c_str(), message.c_str());
        LOG_MSG(("[%s] %s", sender.c_str(), message.c_str())); 


        reverse_hand_mode = NONE;
        GRASP_ARM_LINK[LEFT ] = "LARM";
        GRASP_ARM_LINK[RIGHT] = "RARM";
        if(strcmp(message.c_str(), "REVERSE_RIGHT_HAND") == 0)
        {
            /* Left arm based */
            reverse_hand_mode = REVERSE_RIGHT_HAND;
            GRASP_ARM_LINK[LEFT ] = "LARM";
            GRASP_ARM_LINK[RIGHT] = "RARM";

        }
        else if(strcmp(message.c_str(), "REVERSE_LEFT_HAND") == 0)
        {
            /* Right arm based */
            reverse_hand_mode = REVERSE_LEFT_HAND;
            GRASP_ARM_LINK[RIGHT] = "LARM";
            GRASP_ARM_LINK[LEFT ] = "RARM";
        }
        printf("reverse_hand_mode=%d\n",reverse_hand_mode);

        reverse_foot_mode = NONE;
        GRASP_FOOT_LINK[LEFT ] = "LLEG";
        GRASP_FOOT_LINK[RIGHT] = "RLEG";
        if(strcmp(message.c_str(), "REVERSE_RIGHT_FOOT") == 0)
        {
            /* Left leg based */
            reverse_foot_mode = REVERSE_RIGHT_FOOT;
            GRASP_FOOT_LINK[LEFT ] = "LLEG";
            GRASP_FOOT_LINK[RIGHT] = "RLEG";
        }
        else if(strcmp(message.c_str(), "REVERSE_LEFT_FOOT") == 0)
        {
            /* Right leg based */
            reverse_foot_mode = REVERSE_LEFT_FOOT;
            GRASP_FOOT_LINK[RIGHT] = "LLEG";
            GRASP_FOOT_LINK[LEFT ] = "RLEG";
        }
        printf("reverse_foot_mode=%d\n",reverse_foot_mode);

        printf("message=\n%s\n",message.c_str());
        char *msg = strtok((char*)message.c_str(), " ");
        //printf("msg=\n%s\n",msg);
        if (strcmp(msg, "KINECT_DATA") == 0) {  
        	//printf("msg=\n%s\n",msg);
            int i = 0;  
            while(i < maxsize + 1) {  
                i++;  
                //char *type = strtok(msg, ":");
                char *type = strtok(NULL, ":");  
                //printf("type=\n%s\n",type);
                /* When body position notified, */ 
                /* as the joint angle use the body position and notice as initial coordinate,*/
                /* apply below values to the agent*/ 
                if(strcmp(type,"POSITION") == 0) {  
                    double x = atof(strtok(NULL, ","));  
                    double y = atof(strtok(NULL, ","));  
                    double z = atof(strtok(NULL, " "));
                    myself->setPosition(pos[X] + x, pos[Y] + y, pos[Z] + z);  
                    continue;  
  
                /* When body orientation notified, */ 
                /* as the joint angle use the body position and notice as initial coordinate,*/
                /* apply below values to the agent*/ 
                } else if(strcmp(type, "WAIST") == 0) {  
                    double w = atof(strtok(NULL, ","));  
                    double x = atof(strtok(NULL, ","));  
                    double y = atof(strtok(NULL, ","));  
                    double z = atof(strtok(NULL, " "));  
                    myself->setAxisAndAngle(x, y, z, acos(w) * 2);  
                    continue;  
  
                } else if(strcmp(type,"END") == 0) {
                    break;  
  
                /* Notice of joint angle comes*/  
                } else { 
                    /* Get quaternion of each joint angle*/
                    double w   = atof(strtok(NULL, ","));  
                    double x   = atof(strtok(NULL, ","));  
                    double y   = atof(strtok(NULL, ","));  
                    double z   = atof(strtok(NULL, " "));

                    /* normalizing to 1.0, and check if it exceeds or not. */
                    double a   = acos(w) * 2;  
                    double tmp = sin(a / 2);  
                    double vx  = x / tmp;  
                    double vy  = y / tmp;  
                    double vz  = z / tmp;  
                    double len = sqrt(vx * vx + vy * vy + vz * vz);  
                    if (len < (1 - range) || (1 + range) < len) {
                        continue;
                    }  
            
                    /* Unnecessary information does not processed after grasping. */
                    if (strstr(type, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT2")).c_str() ) != NULL 
                     || strstr(type, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT3")).c_str() ) != NULL) {
                        if ((grasp == false && (strcmp(type, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT2-1")).c_str()) != 0 
                                            &&  strcmp(type, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT3-1")).c_str()) != 0)) 
                          || grasp == true  && (strcmp(type, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT2-2")).c_str()) != 0 
                                            &&  strcmp(type, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT3-2")).c_str()) != 0)) {
                            /* In grasping, use 1 joint angle, or use 2 joint angles.*/
                             /* In case 1, 1 no baai ha sayuu no ashi ga taishousei wo motsu  ugoki wo shi?*/
                            /* In case 2, 2 no baai ha hidari to migi ha onaji (soushinaito ryoutei de tsukamemasen)  ugoki wo shimasu?*/
                            continue;    
                        }
                    }
                    /*  Unnecessary information does not processed after grasping. */
                    if (strstr(type, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT2")).c_str() ) != NULL 
                     || strstr(type, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT4")).c_str() ) != NULL) {
                        if ((grasp == false && (strcmp(type, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT2-1")).c_str()) != 0 
                                            &&  strcmp(type, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT4-1")).c_str()) != 0)) 
                          || grasp == true  && (strcmp(type, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT2-2")).c_str()) != 0 
                                            &&  strcmp(type, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT4-2")).c_str()) != 0)) {
                            /* In grasping, use 1 joint angle, or use 2 joint angles. */
                            /* In case 1, 1 no baai ha sayuu no ashi ga taishousei wo motsu  ugoki wo shi?*/
                            /* In case 2, 2 no baai ha hidari to migi ha onaji (soushinaito ryouashi de tsukamemasen) ugoki wo shimasu?*/
                            continue;    
                        }
                    }

                    /* Apply rotation quaternion from Kinect to its joints*/
                    /* Change objects in below cases*/
                    char region[256];
                    if (strstr(type,   (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT2")).c_str()) != NULL) {
                        /* Use below as right shoulder information*/
                        strcpy(region, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT2")).c_str());

                    } else if (strstr(type,   (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT2")).c_str()) != NULL) {
                        /* Use below as right shoulder information */
                        strcpy(region, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT2")).c_str());

                    } else if (strstr(type, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT3")).c_str()) != NULL) {
                        /* Use below as right elbow information */
                        strcpy(region, (std::string(GRASP_ARM_LINK[RIGHT]) + std::string("_JOINT3")).c_str());

                    } else if (strstr(type, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT4")).c_str()) != NULL) {
                        /*  Use below as right elbow information */
                        strcpy(region, (std::string(GRASP_FOOT_LINK[RIGHT]) + std::string("_JOINT4")).c_str());

                    } else {
                        /*  Use below as other */
                        strcpy(region, type);
                    }
#ifdef _VERBOSE
                    LOG_MSG(("grasp:%s,type:%s,w:%f,x:%f,y:%f,z:%f", (grasp == true ? "true" : "false"), region, w, x, y, z));
#endif
                    /* Set rotation quaternion calculated in Kinect*/
                    //printf("----------reverse_hand_mode=%d\n",reverse_hand_mode);
                    //printf("----------reverse_foot_mode=%d\n",reverse_foot_mode);
                    //printf("----------region=\n%s,\tw=%f,\tx=%f,\ty=%f,\tz=%f\n",region,w,x,y,z);

                    if(reverse_hand_mode == REVERSE_RIGHT_HAND)
                    {
                        if(strstr(region,"RARM")!=NULL){ 
                            printf("Conjugate:\t%s\n",type);
                            getConjugateQuaternion(&w,&x,&y,&z);
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        }
                        else if(strstr(region,"LARM")!=NULL)
                        {
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        }    
                    }
                    if(reverse_hand_mode  == REVERSE_LEFT_HAND)
                    {
                        if(strstr(region,"LARM")!=NULL){ 
                            printf("Conjugate:\t%s\n",type);
                            getConjugateQuaternion(&w,&x,&y,&z);
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        } 
                        else if(strstr(region,"RARM")!=NULL)
                        {
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        }             
                    }
                    if(reverse_foot_mode  == REVERSE_RIGHT_FOOT)
                    {
                        if(strstr(region,"RLEG")!=NULL){ 
                            printf("Conjugate:\t%s\n",type);
                            getConjugateQuaternion(&w,&x,&y,&z);
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        }else if(strstr(region,"LLEG")!=NULL)
                        {
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        }           
                    }
                    if(reverse_foot_mode  == REVERSE_LEFT_FOOT)
                    {
                        if(strstr(region,"LLEG")!=NULL){ 
                            printf("Conjugate:\t%s\n",type);
                            getConjugateQuaternion(&w,&x,&y,&z);
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        }else if(strstr(region,"RLEG")!=NULL)
                        {
                            printf("----------region=\n%s, w=%f, x=%f, y=%f, z=%f\n",region,w,x,y,z);
                            myself->setJointQuaternion(region, w, x, y, z); 
                        }

                    }


                    continue; 
                }  
            } 
        } else if (strcmp(msg, "HMD_DATA") == 0) {
            /* Tlanslate YPR from HMD to its quaternion*/ 
            /* and Apply joint angle of avator neck. */
            double y = atof(strtok(NULL, ","));  
            double p = atof(strtok(NULL, ","));  
            double r = atof(strtok(NULL, ","));
    
            /* translate YPR to its quaternion*/
            dQuaternion qy, qp, qr;
            qy[0] = cos(-y / 2.0);
            qy[1] = 0.0;
            qy[2] = sin(-y / 2.0);
            qy[3] = 0.0;

            qp[0] = cos(-p / 2.0);
            qp[1] = sin(-p / 2.0);
            qp[2] = 0.0;
            qp[3] = 0.0;

            qr[0] = cos(-r / 2.0);
            qr[1] = 0.0;
            qr[2] = 0.0;
            qr[3] = sin(-r / 2.0);

            /* Calculate rotation quaternion from YPR, using cross product function of ode*/
            dQuaternion t1, t2;
            dQMultiply0(t1, qy, qp);
            dQMultiply0(t2, t1, qr);

            /* Body orientation fixed. */
            dQuaternion qbody;
            qbody[0] = 1.0;
            qbody[1] = 0.0;
            qbody[2] = 0.0;
            qbody[3] = 0.0;
 
            /* Calculate joint angle f neck from the difference between body orientation and neck orientation. */
            dQuaternion t3;
            dQMultiply1(t3, qbody, t2);

            /* Apply calculated angle to neck.*/
            //if(strstr(msg, "RARM")!=NULL && false){ }
            //getConjugateQuaternion(&t3[0], &t3[1], &t3[2], &t3[3]);
            myself->setJointQuaternion("HEAD_JOINT0", t3[0], t3[1], t3[2], t3[3]);
 
        } else if (strcmp(msg, "INIT") == 0) {
            /* Ajust the position of legs when initializing avator body.*/
            /* Adjust the joint angle of knees to sit on the chair*/
            /* Root of both legs.*/
            myself->setJointAngle("RLEG_JOINT2", DEG2RAD(-90));
            myself->setJointAngle("LLEG_JOINT2", DEG2RAD(-90));
            /* Both knees */
            myself->setJointAngle("RLEG_JOINT4", DEG2RAD( 90));
            myself->setJointAngle("LLEG_JOINT4", DEG2RAD( 90));
 
        } else {
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

#ifndef _NO_GRASP
/*!
 * @brief Grasp Obon when left hand touches it
 */
void LinkageController::onCollision(CollisionEvent &evt) {
    try {
        if (grasp == false) {
            /* Left hand doesn't grasp Obon when it is below the prescribed height*/
            SimObj *myself = getObj(myname());
            CParts *parts = myself->getParts(GRASP_PARTS);
            Vector3d parts_pos;
            parts->getPosition(parts_pos);
            //printf("LIMIT_Y=\n%f\n",LIMIT_Y);
            if (parts_pos.y() >= LIMIT_Y) {    
#ifdef _VERBOSE
                LOG_MSG(("grasp:%s", grasp == true ? "true" : "false"));
#endif
                /* Get the name grasping object */
                const std::vector<std::string> & with = evt.getWith();
    
                /* Get one's parts colliding. */
                const std::vector<std::string> & mparts = evt.getMyParts();

                for (int i = 0; i < with.size(); i++) {
                    /* Grasp object when collision of left hand or arm */
#ifdef _VERBOSE
                    LOG_MSG(("target:%s,parts:%s,define:%s", with[i].c_str(), mparts[i].c_str(), GRASP_PARTS));
#endif
                    /* In consideration of the hand opposite to based hand collides, */
                    /* regard both hands' collision as valid without check whether L or R.*/
                    /*Below is the case the hand(LINK7) or the leg(LINK6) touches*/
                    if (strstr(mparts[i].c_str(), "_LINK7") != NULL || 
                        strstr(mparts[i].c_str(), "_LINK6") != NULL) {
                        /* Grasp object*/
#ifdef _REAL_GRASP
                        parts->graspObj(GRASP_OBJECT);   
#endif
                         /* Keep grasping*/
                        LOG_MSG(("%s grasp %s", GRASP_PARTS, GRASP_OBJECT));
                        grasp = true;

                        /* Notify grasping to service proider.*/
                        if (service == NULL) {
                            if (checkService(SIGSERVICE) == true) {
                                /* Connect to service provider. */
#ifdef _STATIC_PORT
                                service = connectToService(SIGSERVICE, SIGPORT);
#else
                                service = connectToService(SIGSERVICE);
#endif
                                LOG_MSG(("%s connected to %s service", myname(), SIGSERVICE));
                            } else {
                                LOG_MSG(("%s not found", SIGSERVICE));
                            }
                        }
                        if (service != NULL) {
                            /* Notify grasping to the service provider.*/
                            service->sendMsgToSrv("GRASP");
                            LOG_MSG(("%s send grasp message to %s service", myname(), SIGSERVICE));
                        }
        
                        /* Number of object grasped must be up to 1.*/
                        break;
                    } 
                }
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
#endif

/*!
 * @brief Tlansmit itself to sentral server.
 */		
extern "C"  Controller * createController() {
    return new LinkageController;
}

