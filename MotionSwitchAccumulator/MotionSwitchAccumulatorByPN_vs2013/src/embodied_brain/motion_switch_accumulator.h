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

	// 偽動作のSerial Numberは 0固定
	static const int fakeMotionSerialNumber = 0;

	//// 偽動作IDの再生順キュー
	//std::queue<int> fakeIdPlayOrderQueue;

	void initFakeIdPlayOrderList(int *fakeIdPlayOrderList);
	void inputValues();

	std::list<PerceptionNeuronDAO::TimeSeries_t> getMotionDataFromDBorFile(const int fakeRecId, const int serialNumber);

	void accumulateMotionData4RecFake(const int serialNumber);
	void accumulateMotionData4Experiment(AvatarController &avatarController, const int serialNumber, const int fakeRecId);
	PerceptionNeuronDAO::DataSet accumulateMotionDataBeforeSwitching(AvatarController &avatarController, const int serialNumber);
	PerceptionNeuronDAO::DataSet accumulateMotionDataAfterSwitching(AvatarController &avatarController, const int serialNumber);

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
