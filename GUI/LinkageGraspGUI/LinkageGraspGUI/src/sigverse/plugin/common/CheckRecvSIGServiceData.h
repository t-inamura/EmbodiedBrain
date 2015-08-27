#ifndef SIGVERSE_CHECK_RECV_SIGSERVICE_DATA_H
#define SIGVERSE_CHECK_RECV_SIGSERVICE_DATA_H

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

#endif // SIGVERSE_CHECK_RECV_SIGSERVICE_DATA_H
