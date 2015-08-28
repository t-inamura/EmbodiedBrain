#ifndef SIGVERSE_LINKAGE_GRASP_GUI_AVATAR_CONTROLLER_H
#define SIGVERSE_LINKAGE_GRASP_GUI_AVATAR_CONTROLLER_H

#include <list>
#include <map>

#include <SIGService/SIGService.h>


/*
 * Controlling Avatar of SIGVerse.
 */
class AvatarController
{
private:

	//SIGService
	sigverse::SIGService *m_srv;

public:

	void connectSIGServer(const std::string ipAddress, const int portNum);
	void disconnectFromAllController();
	void checkRecvSIGServiceData();

	void sendMessageToController(std::string msg);
};

#endif // SIGVERSE_LINKAGE_GRASP_GUI_AVATAR_CONTROLLER_H




