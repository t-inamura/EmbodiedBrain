#ifndef EMBODIED_BRAIN_PERCEPTION_NEURON_DAO_H
#define EMBODIED_BRAIN_PERCEPTION_NEURON_DAO_H

#pragma warning(disable:4251)

#include <string>
#include <time.h>
#include <list>
#include <cppconn/prepared_statement.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>

/*
 * MySQL操作クラス
 */
class PerceptionNeuronDAO
{
public:
	struct Summary_t
	{
		int     recId;
		int     userId;
		int     recTotalTime;
		tm      recStartTime;
		int     recInterval;
		std::string  memo;

		/*
		 * startTimeを(%Y-%m-%d %H:%M:%S)フォーマットで返す
		 */
		std::string getRecStartTimeStr() const
		{
			char startTimeStr[20];

			strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", &recStartTime);

			return startTimeStr;
		}
	};

	struct TimeSeries_t
	{
		int              recId;
		int              elapsedTime;
		SigCmn::Vector3  hips_pos;
		PerceptionNeuronSensorData::PerceptionNeuronBVHJoint links[NeuronBVH::BonesType::BonesTypeCount];
	};

	struct DataSet
	{
		Summary_t summary;
		std::list<PerceptionNeuronDAO::TimeSeries_t> timeSeries;
	};

protected:
	static void setJointPosition(PerceptionNeuronSensorData::PerceptionNeuronBVHJoint &joint, const NeuronBVH::BonesType &bonesType, const double x, const double y, const double z);
	static void setPreparedStatementLinkValue(sql::PreparedStatement *pstmt, const int preparedStatementIndex, const PerceptionNeuronSensorData::PerceptionNeuronBVHJoint link);

public:
	static void duplicationCheck(const int recId);
	static int  selectMotionData(std::list<PerceptionNeuronDAO::TimeSeries_t> &motionInfo, const std::string &recId);
	static void insertDatabase(const PerceptionNeuronDAO::DataSet &motionInfo);
	static void insertDatabaseExec(const PerceptionNeuronDAO::DataSet &motionInfo);
	static void insert(sql::Connection *con, const PerceptionNeuronDAO::DataSet &motionInfo);
};

#endif //EMBODIED_BRAIN_PERCEPTION_NEURON_DAO_H
