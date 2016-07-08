#ifndef EMBODIED_BRAIN_MSW_RECORDING_INFO_DAO_H
#define EMBODIED_BRAIN_MSW_RECORDING_INFO_DAO_H

#pragma warning(disable:4251)

#include <string>
#include <time.h>
#include <list>
#include <cppconn/prepared_statement.h>

/*
 * 動作切替実験・収録情報関連テーブル操作クラス
 */
class MswRecordingInfoDAO
{
public:
	struct DataSet
	{
		int          afterSwitchingRecId;
		int          beforeSwitchingRecId;
		int          fakeRecId;
		std::string  memo;
	};

public:
	static bool duplicationCheck(const int recId);
	static void insertDatabase(const MswRecordingInfoDAO::DataSet &motionInfo);
	static void insertDatabaseExec(const MswRecordingInfoDAO::DataSet &motionInfo);
	static void insert(sql::Connection *con, const MswRecordingInfoDAO::DataSet &motionInfo);
};

#endif //EMBODIED_BRAIN_MSW_RECORDING_INFO_DAO_H
