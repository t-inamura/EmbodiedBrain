#ifndef EMBODIED_BRAIN_FILE_MANAGER_H
#define EMBODIED_BRAIN_FILE_MANAGER_H

#include <time.h>
#include <ctime>
#include <windows.h>
#include <embodied_brain/common/param.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>
#include <embodied_brain/database/table/msw_recording_info_dao.h>

/*
 * ファイル管理クラス
 */
class FileManager
{
private:
	void setJointPosition(PerceptionNeuronSensorData::PerceptionNeuronBVHJoint &joint, const NeuronBVH::BonesType &bonesType, const std::vector<std::string> &vecStr, const int index);

public:
	int  getMotionData(std::list<PerceptionNeuronDAO::TimeSeries_t> &timeSeries, const std::string &fileName, const std::string &motionId);

	void outputDataFile(const PerceptionNeuronDAO::DataSet &motionDataBeforeSwitching, const PerceptionNeuronDAO::DataSet &motionDataAfterSwitching, const MswRecordingInfoDAO::DataSet &recordingInfo);
	void outputDataFile(const PerceptionNeuronDAO::DataSet &motionData);

	void outputDataFilePerceptionNeuron(const PerceptionNeuronDAO::DataSet &motionData);
	void outputDataFileSwitching(const MswRecordingInfoDAO::DataSet &recordingInfo);
	std::string getJointStr(const SigCmn::Vector3 &rotation);
};

#endif // EMBODIED_BRAIN_FILE_MANAGER_H 
