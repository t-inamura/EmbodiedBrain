///*
// * ==========================================================================================
// * @brief  This object keeps horizontality.
// * @deprecated
// * @file   LinkageObjectController.cpp
// * @date   2015/08/25
// * @author National Institute of Informatics
// * @par    1.0.0
// * ==========================================================================================
// */
//#include "LinkageObjectController.h"
//
///*!
// * @brief Adjust its orientatnoin to keep horizontality in grasping.
// */
//double LinkageObjectController::onAction(ActionEvent &evt)
//{
//	try
//	{
//		/* Keep horizontality to the ground */
//		SimObj *myself = getObj(myname());
//
//		if (!myself->dynamics())
//		{
//			myself->setRotation(this->br);
//		}
//	}
//	catch (SimObj::Exception &err)
//	{
//		LOG_MSG(("Exception: %s", err.msg()));
//	}
//
//	return 0.5;
//}
//
//
///*!
// * @brief Initializing. Keep initial angle.
// */
//void LinkageObjectController::onInit(InitEvent &evt)
//{
//	try
//	{
//		SimObj *myself = getObj(myname());
//
//		if (!myself->dynamics())
//		{
//			/* Keep my initial angle */
//			myself->getRotation(this->br);
//		}
//	}
//	catch (SimObj::Exception &err)
//	{
//		LOG_MSG(("Exception: %s", err.msg()));
//	}
//}
//
//
///*!
// * @brief Message robot heard
// */
//void LinkageObjectController::onRecvMsg(RecvMsgEvent &evt)
//{
//}
//
///*
// * @brief Collision check.
// */
//void LinkageObjectController::onCollision(CollisionEvent &evt)
//{
//}
//
///*!
// * @brief transmit myself to the sentral server.
// */
//extern "C" Controller * createController ()
//{
//	return new LinkageObjectController;
//}
//
