/*
 * SIGServiceデータ受信チェッククラス
 */
#include <SIGService/SIGService.h>
#include <sigverse/plugin/common/CheckRecvSIGServiceData.h>

/*
 * 定期的にSIGServiceデータの受信チェックを行う
 */
void CheckRecvSIGServiceData::run(sigverse::SIGService *m_srv)
{
	while(true)
	{
		m_srv->checkRecvData(1);

		//sleep 1 second
		Sleep(1000);
	}
};
