#ifndef EMBODIED_BRAIN_IMITATION_DAO_H
#define EMBODIED_BRAIN_IMITATION_DAO_H

#pragma warning(disable:4251)

#include <string>
#include <time.h>
#include <list>
#include <cppconn/prepared_statement.h>

/*
 * MySQL操作クラス
 */
class PmsImitationDAO
{
public:
	struct DataSet
	{
		int          groupId;
		int          recType;
		int          recId;
		int          originalRecId;
		float        conditionPulsePower;
		float        conditionPulseFrequency;
		int          conditionPulseDuration;
		int          conditionPulseInterval;
		int          conditionPulseNumber;
		std::string  memo;
	};

public:
	static void duplicationCheck(const int groupId, const int recType);
	static void insertDatabase(const PmsImitationDAO::DataSet &motionInfo);
	static void insertDatabaseExec(const PmsImitationDAO::DataSet &motionInfo);
	static void insert(sql::Connection *con, const PmsImitationDAO::DataSet &motionInfo);
};

#endif //EMBODIED_BRAIN_IMITATION_DAO_H
