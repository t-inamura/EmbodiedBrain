#ifndef EMBODIED_BRAIN_CHECK_RECV_SIGSERVICE_DATA_H
#define EMBODIED_BRAIN_CHECK_RECV_SIGSERVICE_DATA_H

#include <SIGService/SIGService.h>

/*
 * SIGServiceデータ受信チェッククラス
 */
class CheckRecvSIGServiceData
{
private:

public:
	void run(sigverse::SIGService *m_srv);
};

#endif // EMBODIED_BRAIN_CHECK_RECV_SIGSERVICE_DATA_H
