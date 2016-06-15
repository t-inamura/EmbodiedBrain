//#pragma warning(disable:4251)

#include <sigverse/tools/LinkageGraspGUI/Param.h>
#include <sigverse/tools/LinkageGraspGUI/AvatarController.h>
#include <sigverse/plugin/common/CheckRecvSIGServiceData.h>

#include <iostream>
#include <sstream>
#include <math.h>
#include <tchar.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>

#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>


/*
 * Connect to SIGServer.
 */
void AvatarController::connectSIGServer(const std::string ipAddress, const int portNum)
{
	this->m_srv = new sigverse::SIGService(Param::getSigServiceName());
	
	this->m_srv->connect(ipAddress, portNum);

	//The following commands are for automatically shutdown at QuitSimulation.
	this->m_srv->connectToViewer();
	this->m_srv->setAutoExitProc(true);
}

/*
 * Disconnect from all controllers.
 */
void AvatarController::disconnectFromAllController()
{
//	this->m_srv->disconnectFromAllController();

	this->m_srv->disconnect();
}


/*
 * Start checking received data from SIGService.
 */
void AvatarController::checkRecvSIGServiceData()
{
	CheckRecvSIGServiceData checkRecvSIGServiceData;

	// Execute in another thread.
	boost::thread thCheckRecvData(boost::bind(&CheckRecvSIGServiceData::run, &checkRecvSIGServiceData, this->m_srv));
}


/*
 * Send message to SIGVerse controller.
 */
void AvatarController::sendMessageToController(std::string msg)
{
	try
	{
		// Connected controller number
		int connectedNum = this->m_srv->getConnectedControllerNum();

		if (connectedNum > 0)
		{
			// Send message to all connected controllers
			std::vector<std::string> names = this->m_srv->getAllConnectedEntitiesName();

			for (int i = 0; i < connectedNum; i++)
			{
				this->m_srv->sendMsgToCtr(names[i].c_str(), msg);
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
