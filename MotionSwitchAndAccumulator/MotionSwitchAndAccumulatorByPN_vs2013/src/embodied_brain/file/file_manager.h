#ifndef EMBODIED_BRAIN_FILE_MANAGER_H
#define EMBODIED_BRAIN_FILE_MANAGER_H

#include <time.h>
#include <ctime>
#include <windows.h>
#include <embodied_brain/common/param.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>
#include <embodied_brain/database/table/pms_imitation_dao.h>

/*
 * ファイル管理クラス
 */
class FileManager
{
private:
	void setJointPosition(PerceptionNeuronSensorData::PerceptionNeuronBVHJoint &joint, const NeuronBVH::BonesType &bonesType, const std::vector<std::string> &vecStr, const int index);

public:
	int  getMotionData(std::list<PerceptionNeuronDAO::TimeSeries_t> &timeSeries, const std::string &fileName, const std::string &motionId);

	void outputDataFile(const PerceptionNeuronDAO::DataSet &motionData, const PmsImitationDAO::DataSet &imitationInfo);
	void outputDataFileExec(const PerceptionNeuronDAO::DataSet &motionData, const PmsImitationDAO::DataSet &imitationInfo);
	std::string getJointStr(const SigCmn::Vector3 &rotation);
};

#endif // EMBODIED_BRAIN_FILE_MANAGER_H 
