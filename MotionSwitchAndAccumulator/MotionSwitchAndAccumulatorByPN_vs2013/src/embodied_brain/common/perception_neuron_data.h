#ifndef EMBODIED_BRAIN_PERCEPTION_NEURON_DATA_H
#define EMBODIED_BRAIN_PERCEPTION_NEURON_DATA_H

#include <mutex>
#include "NeuronDataReader.h"
#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>
#include <embodied_brain/common/param.h>
#include <embodied_brain/database/perception_neuron_db.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>
#include <embodied_brain/database/table/pms_imitation_dao.h>
#include <embodied_brain/avatar/avatar_controller.h>

/*
 * 共通データクラス
 */
class PerceptionNeuronData
{
protected :
	PerceptionNeuronSensorData latestSensorData;

	std::mutex mtx4LatestSensorData;

public:
	PerceptionNeuronData();
	~PerceptionNeuronData();

	/////@brief Callback function for receiving BVH data.
	//static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	///@brief Update BVH data.
//	void updateBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);
	void updateBvhData(BvhDataHeader* header, float* data);

	PerceptionNeuronSensorData getLatestSensorData();

	int run(int argc, char **argv);
};


#endif // EMBODIED_BRAIN_PERCEPTION_NEURON_DATA_H 
