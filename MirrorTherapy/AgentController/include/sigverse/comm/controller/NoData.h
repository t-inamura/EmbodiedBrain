/* $Id: NoData.h,v 1.1.1.1 2011-03-25 02:18:50 okamoto Exp $ */ 
#ifndef CommController_NoData_h
#define CommController_NoData_h

/**
 * @brief データのないイベントクラス
 *
 * イベントの通知のみを行い、データを持たないコントローライベント。
 */
class NoDataEvent
{
public:
	bool	set(int packetNum, int seq, char *data, int n) { return true;} 
};

/**
 * @brief コントローラonInit イベントクラス
 *
 * @see CommData::InvokeOnInit
 */
class InitEvent : public NoDataEvent {};

#endif // CommController_NoData_h
 
