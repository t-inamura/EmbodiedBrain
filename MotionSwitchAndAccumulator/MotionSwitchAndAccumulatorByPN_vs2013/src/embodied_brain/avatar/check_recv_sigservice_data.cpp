/*
 * SIGServiceデータ受信チェッククラス
 */
#include <SIGService/SIGService.h>
#include <embodied_brain/avatar/check_recv_sigservice_data.h>

/*
 * 定期的にSIGServiceデータの受信チェックを行う
 */
void CheckRecvSIGServiceData::run(sigverse::SIGService *m_srv)
{
	while(true)
	{
		m_srv->checkRecvData(1);

		//1秒スリープ
		Sleep(1000);
	}
};
