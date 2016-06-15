#ifndef SIGVERSE_MIRROR_THERAPY_GUI_AVATAR_CONTROLLER_H
#define SIGVERSE_MIRROR_THERAPY_GUI_AVATAR_CONTROLLER_H

#include <list>
#include <map>

#include <SIGService/SIGService.h>


/*
 * アバター操作クラス
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

#endif // SIGVERSE_MIRROR_THERAPY_GUI_AVATAR_CONTROLLER_H




