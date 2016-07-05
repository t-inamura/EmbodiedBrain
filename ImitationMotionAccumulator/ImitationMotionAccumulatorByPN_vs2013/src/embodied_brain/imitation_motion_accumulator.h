#ifndef EMBODIED_BRAIN_IMITATION_MOTION_ACCUMULATOR_H
#define EMBODIED_BRAIN_IMITATION_MOTION_ACCUMULATOR_H

#include <mutex>
#include "NeuronDataReader.h"
#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>
#include <embodied_brain/common/param.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>
#include <embodied_brain/database/table/pms_imitation_dao.h>
#include <embodied_brain/avatar/avatar_controller.h>

/*
 * 真似動作収録クラス
 */
class ImitationMotionAccumulator : public PerceptionNeuronDevice
{
protected :
//	void initialize(const int argMotionId, const std::string &memo, const int argMaxRecordTime);

	std::list<PerceptionNeuronDAO::TimeSeries_t> getMotionDataFromDBorFile(const std::string &recIdStr);

	void accumulateMotionData(AvatarController &avatarController);
	void accumulate(const int elapsedTime);

	PerceptionNeuronSensorData latestSensorData;
	std::map<int, PerceptionNeuronSensorData> accumulatingSensorDataMap;

	std::mutex mtx4LatestSensorData;

public:
	///@brief Callback function for receiving BVH data.
	static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	///@brief Update BVH data.
	void updateBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	int run(int argc, char **argv);
};


#endif // EMBODIED_BRAIN_IMITATION_MOTION_ACCUMULATOR_H 
