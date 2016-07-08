#ifndef EMBODIED_BRAIN_MOTION_SWITCH_ACCUMULATOR_H
#define EMBODIED_BRAIN_MOTION_SWITCH_ACCUMULATOR_H

#include <mutex>
#include "NeuronDataReader.h"
#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>
#include <embodied_brain/common/param.h>
#include <embodied_brain/common/perception_neuron_data.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>
#include <embodied_brain/database/table/msw_recording_info_dao.h>
#include <embodied_brain/avatar/avatar_controller.h>

/*
 * 動作切替・収録クラス
 */
class MotionSwitchAccumulator : public PerceptionNeuronDevice
{
protected :
	// 切替前の動作IDは切替後動作ID＋100万とする
	static const int additionalId4before = 1000000;

	std::list<PerceptionNeuronDAO::TimeSeries_t> getMotionDataFromDBorFile(const std::string &recIdStr);

	void accumulateMotionData4RecFake();
	void accumulateMotionData4Experiment(AvatarController &avatarController);
	PerceptionNeuronDAO::DataSet accumulateMotionDataBeforeSwitching(AvatarController &avatarController);
	PerceptionNeuronDAO::DataSet accumulateMotionDataAfterSwitching(AvatarController &avatarController);

	void setMotionDataBeforeSwitching(PerceptionNeuronDAO::DataSet &motionSet, const std::map<int, PerceptionNeuronSensorData> &accumulatedDataMap);
	void setMotionDataAfterSwitching(PerceptionNeuronDAO::DataSet &motionSet, const std::map<int, PerceptionNeuronSensorData> &accumulatedDataMap);

//	PerceptionNeuronSensorData latestSensorData;
	std::map<int, PerceptionNeuronSensorData> accumulatedDataMapBeforeSwitching;
	std::map<int, PerceptionNeuronSensorData> accumulatedDataMapAfterSwitching;

	PerceptionNeuronData *perceptionNeuronData;

public:
	///@brief Callback function for receiving BVH data.
	static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	int run(int argc, char **argv);
};


#endif // EMBODIED_BRAIN_MOTION_SWITCH_ACCUMULATOR_H 
