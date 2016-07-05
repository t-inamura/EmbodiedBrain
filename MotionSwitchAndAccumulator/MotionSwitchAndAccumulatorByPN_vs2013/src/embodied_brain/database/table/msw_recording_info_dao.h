#ifndef EMBODIED_BRAIN_IMITATION_DAO_H
#define EMBODIED_BRAIN_IMITATION_DAO_H

#pragma warning(disable:4251)

#include <string>
#include <time.h>
#include <list>
#include <cppconn/prepared_statement.h>

/*
 * 真似情報関連テーブル操作クラス
 */
class MswRecordingInfoDAO
{
public:
	struct DataSet
	{
		int          groupId;
		int          beforeSwitchingRecId;
		int          afterSwitchingRecId;
		int          afterSwitchingFakeRecId;
		std::string  memo;
	};

public:
	static bool duplicationCheck(const int groupId);
	static void insertDatabase(const MswRecordingInfoDAO::DataSet &motionInfo);
	static void insertDatabaseExec(const MswRecordingInfoDAO::DataSet &motionInfo);
	static void insert(sql::Connection *con, const MswRecordingInfoDAO::DataSet &motionInfo);
};

#endif //EMBODIED_BRAIN_IMITATION_DAO_H
