#ifndef EMBODIED_BRAIN_IMITATION_MOTION_ACCUMULATOR_H
#define EMBODIED_BRAIN_IMITATION_MOTION_ACCUMULATOR_H

#include <mutex>
#include "NeuronDataReader.h"
#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>
#include <embodied_brain/common/param.h>
#include <embodied_brain/database/perception_neuron_db.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>
#include <embodied_brain/database/table/pms_imitation_dao.h>

/*
 * PSMove操作履歴蓄積プログラム(アバター描画側)クラス
 */
class ImitationMotionAccumulator : public PerceptionNeuronDevice
{
protected :
//	void initialize(const int argMotionId, const std::string &memo, const int argMaxRecordTime);

	std::list<PerceptionNeuronDAO::TimeSeries_t> getMotionDataFromDBorFile(const std::string &recIdStr);

	void accumulateMotionData();
	void accumulate(const int elapsedTime);

	PerceptionNeuronSensorData latestSensorData;
	std::map<int, PerceptionNeuronSensorData> accumulatingSensorDataMap;

	
	std::list<PerceptionNeuronDAO::TimeSeries_t> originalPerceptionNeuronTimeSeries;

//	PerceptionNeuronDAO::DataSet imitationMotionData;

	//PerceptionNeuronDAO imitationMotionData;

	//Summary_t summary;
	//std::list<TimeSeries_t> timeSeries;



	//int    recId;
	//double maxTime;

	////MySQL関連
	//PerceptionNeuronDAO perceptionNeuronDAO;

	std::mutex mtx4LatestSensorData;

//	//モデル動作情報テーブルクラス
//	MotionInfoOfModelDAO motionInfoOfModelDAO;
//
//	アバター操作クラス
//	AvatarController avatarController;
//
//	std::list<AvatarController::MotionInfo> getMotionData(const std::string &motionIdStr);
//
public:
	///@brief Callback function for receiving BVH data.
	static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	///@brief Update BVH data.
	void updateBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	int run(int argc, char **argv);
};


#endif // EMBODIED_BRAIN_IMITATION_MOTION_ACCUMULATOR_H 
