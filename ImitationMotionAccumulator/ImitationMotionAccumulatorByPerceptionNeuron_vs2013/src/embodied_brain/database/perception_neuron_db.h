//#ifndef EMBODIED_BRAIN_PERCEPTION_NEURON_DB_H
//#define EMBODIED_BRAIN_PERCEPTION_NEURON_DB_H
//
//#include <string>
//#include <time.h>
//#include <list>
//#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
//
///*
// * 共通クラス
// */
//class PerceptionNeuronDB
//{
//public:
//	struct Summary_t
//	{
//		int     recId;
//		int     userId;
//		int     recTotalTime;
//		tm      recStartTime;
//		int     recInterval;
//		std::string  memo;
//
//		/*
//		 * startTimeを(%Y-%m-%d %H:%M:%S)フォーマットで返す
//		 */
//		std::string getRecStartTimeStr() const
//		{
//			char startTimeStr[20];
//
//			strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", &recStartTime);
//
//			return startTimeStr;
//		}
//	};
//
//	struct TimeSeries_t
//	{
//		int              recId;
//		int              elapsedTime;
//		SigCmn::Vector3  hips_pos;
//		PerceptionNeuronSensorData::PerceptionNeuronBVHJoint links[NeuronBVH::BonesType::BonesTypeCount];
//	};
//
//	Summary_t summary;
//	std::list<TimeSeries_t> timeSeries;
//};
//
//#endif //EMBODIED_BRAIN_PERCEPTION_NEURON_DB_H
