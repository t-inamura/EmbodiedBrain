#include <SIGService/SIGService.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>
#include <embodied_brain/common/perception_neuron_data.h>

#include <embodied_brain/common/param.h>
#include <embodied_brain/file/file_manager.h>

#include <cstddef>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <sstream>
#include <conio.h>
#include <limits>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>




/*
 * Perception Neuronから動作情報を受信する、コールバック関数
 */
PerceptionNeuronData::PerceptionNeuronData()
{
	this->latestSensorData.dataType     = PerceptionNeuronSensorData::DataTypeEnum::BVH;
	this->latestSensorData.bvhData.data = (float *)calloc(10000, sizeof(float));  //多めに領域確保
}

PerceptionNeuronData::~PerceptionNeuronData()
{
	if (this->latestSensorData.bvhData.data)
	{
		std::free(this->latestSensorData.bvhData.data);
	}
}

///*
// * Perception Neuronから動作情報を受信する、コールバック関数
// */
//void __stdcall PerceptionNeuronData::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
//{
//	PerceptionNeuronData* pthis = (PerceptionNeuronData*)customedObj;
//
//	pthis->updateBvhData(customedObj, sender, header, data);
//}


/*
 * Perception Neuronから受信した最新の動作情報を、共有メモリに保存する
 */
void PerceptionNeuronData::updateBvhData(BvhDataHeader* header, float* data)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(this->mtx4LatestSensorData);

	this->latestSensorData.bvhData.avatarIndex   = header->AvatarIndex;
	this->latestSensorData.bvhData.avatarName    = std::string((char*)header->AvatarName);
	this->latestSensorData.bvhData.withDisp      = (header->WithDisp==1);
	this->latestSensorData.bvhData.withReference = (header->WithReference==1);
	this->latestSensorData.bvhData.frameIndex    = header->FrameIndex;
	this->latestSensorData.bvhData.dataCount     = header->DataCount;

	memcpy(this->latestSensorData.bvhData.data, data, header->DataCount * sizeof(float));
}






/*
 * 最新の動作情報を共有メモリに保存
 */
PerceptionNeuronSensorData PerceptionNeuronData::getLatestSensorData()
{
	// スレッド排他制御
	std::lock_guard<std::mutex> lock(this->mtx4LatestSensorData);

	PerceptionNeuronSensorData sensorData;

	sensorData.bvhData.data = (float *)malloc(this->latestSensorData.bvhData.dataCount * sizeof(float));
	sensorData.bvhData.dataCount = this->latestSensorData.bvhData.dataCount;
	memcpy(sensorData.bvhData.data, this->latestSensorData.bvhData.data, this->latestSensorData.bvhData.dataCount * sizeof(float));

	return sensorData;
}
