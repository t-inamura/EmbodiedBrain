#pragma warning(disable:4251)

#include <list>
#include <iostream>
#include <windows.h> 
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <embodied_brain/common/param.h>
#include <embodied_brain/database/table/perception_neuron_dao.h>

/*
 * 収録IDの重複チェック（データベースに既に同一動作IDのレコードがあった場合、exceptionをthrowする）
 */
void PerceptionNeuronDAO::duplicationCheck(const int recId)
{
	sql::mysql::MySQL_Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *rs;

	driver = sql::mysql::get_driver_instance();

	try
	{
		//DB接続
		con = driver->connect("tcp://" + Param::getDbHost() + ":" + std::to_string(Param::getDbPort()), Param::getDbUser(), Param::getDbPass());

		con->setSchema(Param::getDbSchema());

		//モデル動作情報サマリテーブルから、指定動作IDの件数を取得するSQL
		sql::SQLString selectSQL = "SELECT COUNT(*) AS cnt FROM perception_neuron_motions_summary WHERE rec_id=" + std::to_string(recId) + ";";

		stmt = con->createStatement();
		rs = stmt->executeQuery(selectSQL);

		if (rs->next())
		{
			//指定動作IDの件数が0件でなかった場合は、エラー
			if (rs->getInt("cnt") != 0)
			{
				throw std::exception(("データベース上に同じ収録ID(" + std::to_string(recId) + ")の情報が存在します。処理終了します。").c_str());
			}
		}

		delete rs;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException & ex)
	{
		std::cout << "db connection error:" << ex.what() << std::endl;
		std::cout << "Can't connect DB" << std::endl;

		std::string inputKey;

		while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
		{
			std::cout << "DBに接続できませんでした。処理続行しますか？(y/n)："; std::cin >> inputKey;
		}

		if (inputKey.compare("n") == 0)
		{
			throw std::exception("DBに接続できないため処理中止します。");
		}
	}
}


/*
 * データベースから動作データを取得する
 */
int  PerceptionNeuronDAO::selectMotionData(std::list<PerceptionNeuronDAO::TimeSeries_t> &timeSeries, const std::string &recId)
{
	try
	{
		DWORD startTime, endTime;
		double diffTime;

		std::cout << "◆モデル動作データ取得　－開始－◆" << std::endl;

		startTime = timeGetTime();
	
		/*
		 * データベースコネクション
		 */
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;

		driver = sql::mysql::get_driver_instance();


		std::stringstream ss; ss << Param::getDbPort(); std::string dbPortStr = ss.str();

		con = driver->connect("tcp://" + Param::getDbHost() + ":" + dbPortStr, Param::getDbUser(), Param::getDbPass());
		con->setSchema(Param::getDbSchema());
	//	con->setAutoCommit(false);

		/*
		 * モデル動作情報テーブルからのSELECT
		 */
		sql::Statement *stmt;
		sql::ResultSet *rs;

		stmt = con->createStatement();
		rs = stmt->executeQuery("SELECT * FROM perception_neuron_motions_time_series WHERE rec_id = " + recId + " ORDER BY elapsed_time ASC ");

		while(rs->next())
		{
			PerceptionNeuronDAO::TimeSeries_t motion;

			motion.recId       = rs->getInt("rec_id");
			motion.elapsedTime = rs->getInt("elapsed_time");
			motion.hips_pos.x  = (float)(rs->getDouble("hips_pos_x"));
			motion.hips_pos.y  = (float)(rs->getDouble("hips_pos_y"));
			motion.hips_pos.z  = (float)(rs->getDouble("hips_pos_z"));

			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::Hips],              NeuronBVH::BonesType::Hips,              rs->getDouble("hips_rot_x"),                 rs->getDouble("hips_rot_y"),                 rs->getDouble("hips_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightUpLeg],        NeuronBVH::BonesType::RightUpLeg,        rs->getDouble("right_up_leg_rot_x"),         rs->getDouble("right_up_leg_rot_y"),         rs->getDouble("right_up_leg_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightLeg],          NeuronBVH::BonesType::RightLeg,          rs->getDouble("right_leg_rot_x"),            rs->getDouble("right_leg_rot_y"),            rs->getDouble("right_leg_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightFoot],         NeuronBVH::BonesType::RightFoot,         rs->getDouble("right_foot_rot_x"),           rs->getDouble("right_foot_rot_y"),           rs->getDouble("right_foot_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftUpLeg],         NeuronBVH::BonesType::LeftUpLeg,         rs->getDouble("left_up_leg_rot_x"),          rs->getDouble("left_up_leg_rot_y"),          rs->getDouble("left_up_leg_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftLeg],           NeuronBVH::BonesType::LeftLeg,           rs->getDouble("left_leg_rot_x"),             rs->getDouble("left_leg_rot_y"),             rs->getDouble("left_leg_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftFoot],          NeuronBVH::BonesType::LeftFoot,          rs->getDouble("left_foot_rot_x"),            rs->getDouble("left_foot_rot_y"),            rs->getDouble("left_foot_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::Spine],             NeuronBVH::BonesType::Spine,             rs->getDouble("spine_rot_x"),                rs->getDouble("spine_rot_y"),                rs->getDouble("spine_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::Spine1],            NeuronBVH::BonesType::Spine1,            rs->getDouble("spine1_rot_x"),               rs->getDouble("spine1_rot_y"),               rs->getDouble("spine1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::Spine2],            NeuronBVH::BonesType::Spine2,            rs->getDouble("spine2_rot_x"),               rs->getDouble("spine2_rot_y"),               rs->getDouble("spine2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::Spine3],            NeuronBVH::BonesType::Spine3,            rs->getDouble("spine3_rot_x"),               rs->getDouble("spine3_rot_y"),               rs->getDouble("spine3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::Neck],              NeuronBVH::BonesType::Neck,              rs->getDouble("neck_rot_x"),                 rs->getDouble("neck_rot_y"),                 rs->getDouble("neck_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::Head],              NeuronBVH::BonesType::Head,              rs->getDouble("head_rot_x"),                 rs->getDouble("head_rot_y"),                 rs->getDouble("head_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightShoulder],     NeuronBVH::BonesType::RightShoulder,     rs->getDouble("right_shoulder_rot_x"),       rs->getDouble("right_shoulder_rot_y"),       rs->getDouble("right_shoulder_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightArm],          NeuronBVH::BonesType::RightArm,          rs->getDouble("right_arm_rot_x"),            rs->getDouble("right_arm_rot_y"),            rs->getDouble("right_arm_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightForeArm],      NeuronBVH::BonesType::RightForeArm,      rs->getDouble("right_fore_arm_rot_x"),       rs->getDouble("right_fore_arm_rot_y"),       rs->getDouble("right_fore_arm_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHand],         NeuronBVH::BonesType::RightHand,         rs->getDouble("right_hand_rot_x"),           rs->getDouble("right_hand_rot_y"),           rs->getDouble("right_hand_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandThumb1],   NeuronBVH::BonesType::RightHandThumb1,   rs->getDouble("right_hand_thumb1_rot_x"),    rs->getDouble("right_hand_thumb1_rot_y"),    rs->getDouble("right_hand_thumb1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandThumb2],   NeuronBVH::BonesType::RightHandThumb2,   rs->getDouble("right_hand_thumb2_rot_x"),    rs->getDouble("right_hand_thumb2_rot_y"),    rs->getDouble("right_hand_thumb2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandThumb3],   NeuronBVH::BonesType::RightHandThumb3,   rs->getDouble("right_hand_thumb3_rot_x"),    rs->getDouble("right_hand_thumb3_rot_y"),    rs->getDouble("right_hand_thumb3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandIndex],  NeuronBVH::BonesType::RightInHandIndex,  rs->getDouble("right_in_hand_index_rot_x"),  rs->getDouble("right_in_hand_index_rot_y"),  rs->getDouble("right_in_hand_index_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandIndex1],   NeuronBVH::BonesType::RightHandIndex1,   rs->getDouble("right_hand_index1_rot_x"),    rs->getDouble("right_hand_index1_rot_y"),    rs->getDouble("right_hand_index1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandIndex2],   NeuronBVH::BonesType::RightHandIndex2,   rs->getDouble("right_hand_index2_rot_x"),    rs->getDouble("right_hand_index2_rot_y"),    rs->getDouble("right_hand_index2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandIndex3],   NeuronBVH::BonesType::RightHandIndex3,   rs->getDouble("right_hand_index3_rot_x"),    rs->getDouble("right_hand_index3_rot_y"),    rs->getDouble("right_hand_index3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandMiddle], NeuronBVH::BonesType::RightInHandMiddle, rs->getDouble("right_in_hand_middle_rot_x"), rs->getDouble("right_in_hand_middle_rot_y"), rs->getDouble("right_in_hand_middle_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandMiddle1],  NeuronBVH::BonesType::RightHandMiddle1,  rs->getDouble("right_hand_middle1_rot_x"),   rs->getDouble("right_hand_middle1_rot_y"),   rs->getDouble("right_hand_middle1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandMiddle2],  NeuronBVH::BonesType::RightHandMiddle2,  rs->getDouble("right_hand_middle2_rot_x"),   rs->getDouble("right_hand_middle2_rot_y"),   rs->getDouble("right_hand_middle2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandMiddle3],  NeuronBVH::BonesType::RightHandMiddle3,  rs->getDouble("right_hand_middle3_rot_x"),   rs->getDouble("right_hand_middle3_rot_y"),   rs->getDouble("right_hand_middle3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandRing],   NeuronBVH::BonesType::RightInHandRing,   rs->getDouble("right_in_hand_ring_rot_x"),   rs->getDouble("right_in_hand_ring_rot_y"),   rs->getDouble("right_in_hand_ring_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandRing1],    NeuronBVH::BonesType::RightHandRing1,    rs->getDouble("right_hand_ring1_rot_x"),     rs->getDouble("right_hand_ring1_rot_y"),     rs->getDouble("right_hand_ring1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandRing2],    NeuronBVH::BonesType::RightHandRing2,    rs->getDouble("right_hand_ring2_rot_x"),     rs->getDouble("right_hand_ring2_rot_y"),     rs->getDouble("right_hand_ring2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandRing3],    NeuronBVH::BonesType::RightHandRing3,    rs->getDouble("right_hand_ring3_rot_x"),     rs->getDouble("right_hand_ring3_rot_y"),     rs->getDouble("right_hand_ring3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightInHandPinky],  NeuronBVH::BonesType::RightInHandPinky,  rs->getDouble("right_in_hand_pinky_rot_x"),  rs->getDouble("right_in_hand_pinky_rot_y"),  rs->getDouble("right_in_hand_pinky_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandPinky1],   NeuronBVH::BonesType::RightHandPinky1,   rs->getDouble("right_hand_pinky1_rot_x"),    rs->getDouble("right_hand_pinky1_rot_y"),    rs->getDouble("right_hand_pinky1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandPinky2],   NeuronBVH::BonesType::RightHandPinky2,   rs->getDouble("right_hand_pinky2_rot_x"),    rs->getDouble("right_hand_pinky2_rot_y"),    rs->getDouble("right_hand_pinky2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::RightHandPinky3],   NeuronBVH::BonesType::RightHandPinky3,   rs->getDouble("right_hand_pinky3_rot_x"),    rs->getDouble("right_hand_pinky3_rot_y"),    rs->getDouble("right_hand_pinky3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftShoulder],      NeuronBVH::BonesType::LeftShoulder,      rs->getDouble("left_shoulder_rot_x"),        rs->getDouble("left_shoulder_rot_y"),        rs->getDouble("left_shoulder_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftArm],           NeuronBVH::BonesType::LeftArm,           rs->getDouble("left_arm_rot_x"),             rs->getDouble("left_arm_rot_y"),             rs->getDouble("left_arm_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftForeArm],       NeuronBVH::BonesType::LeftForeArm,       rs->getDouble("left_fore_arm_rot_x"),        rs->getDouble("left_fore_arm_rot_y"),        rs->getDouble("left_fore_arm_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHand],          NeuronBVH::BonesType::LeftHand,          rs->getDouble("left_hand_rot_x"),            rs->getDouble("left_hand_rot_y"),            rs->getDouble("left_hand_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandThumb1],    NeuronBVH::BonesType::LeftHandThumb1,    rs->getDouble("left_hand_thumb1_rot_x"),     rs->getDouble("left_hand_thumb1_rot_y"),     rs->getDouble("left_hand_thumb1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandThumb2],    NeuronBVH::BonesType::LeftHandThumb2,    rs->getDouble("left_hand_thumb2_rot_x"),     rs->getDouble("left_hand_thumb2_rot_y"),     rs->getDouble("left_hand_thumb2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandThumb3],    NeuronBVH::BonesType::LeftHandThumb3,    rs->getDouble("left_hand_thumb3_rot_x"),     rs->getDouble("left_hand_thumb3_rot_y"),     rs->getDouble("left_hand_thumb3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandIndex],   NeuronBVH::BonesType::LeftInHandIndex,   rs->getDouble("left_in_hand_index_rot_x"),   rs->getDouble("left_in_hand_index_rot_y"),   rs->getDouble("left_in_hand_index_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandIndex1],    NeuronBVH::BonesType::LeftHandIndex1,    rs->getDouble("left_hand_index1_rot_x"),     rs->getDouble("left_hand_index1_rot_y"),     rs->getDouble("left_hand_index1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandIndex2],    NeuronBVH::BonesType::LeftHandIndex2,    rs->getDouble("left_hand_index2_rot_x"),     rs->getDouble("left_hand_index2_rot_y"),     rs->getDouble("left_hand_index2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandIndex3],    NeuronBVH::BonesType::LeftHandIndex3,    rs->getDouble("left_hand_index3_rot_x"),     rs->getDouble("left_hand_index3_rot_y"),     rs->getDouble("left_hand_index3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandMiddle],  NeuronBVH::BonesType::LeftInHandMiddle,  rs->getDouble("left_in_hand_middle_rot_x"),  rs->getDouble("left_in_hand_middle_rot_y"),  rs->getDouble("left_in_hand_middle_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandMiddle1],   NeuronBVH::BonesType::LeftHandMiddle1,   rs->getDouble("left_hand_middle1_rot_x"),    rs->getDouble("left_hand_middle1_rot_y"),    rs->getDouble("left_hand_middle1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandMiddle2],   NeuronBVH::BonesType::LeftHandMiddle2,   rs->getDouble("left_hand_middle2_rot_x"),    rs->getDouble("left_hand_middle2_rot_y"),    rs->getDouble("left_hand_middle2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandMiddle3],   NeuronBVH::BonesType::LeftHandMiddle3,   rs->getDouble("left_hand_middle3_rot_x"),    rs->getDouble("left_hand_middle3_rot_y"),    rs->getDouble("left_hand_middle3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandRing],    NeuronBVH::BonesType::LeftInHandRing,    rs->getDouble("left_in_hand_ring_rot_x"),    rs->getDouble("left_in_hand_ring_rot_y"),    rs->getDouble("left_in_hand_ring_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandRing1],     NeuronBVH::BonesType::LeftHandRing1,     rs->getDouble("left_hand_ring1_rot_x"),      rs->getDouble("left_hand_ring1_rot_y"),      rs->getDouble("left_hand_ring1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandRing2],     NeuronBVH::BonesType::LeftHandRing2,     rs->getDouble("left_hand_ring2_rot_x"),      rs->getDouble("left_hand_ring2_rot_y"),      rs->getDouble("left_hand_ring2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandRing3],     NeuronBVH::BonesType::LeftHandRing3,     rs->getDouble("left_hand_ring3_rot_x"),      rs->getDouble("left_hand_ring3_rot_y"),      rs->getDouble("left_hand_ring3_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftInHandPinky],   NeuronBVH::BonesType::LeftInHandPinky,   rs->getDouble("left_in_hand_pinky_rot_x"),   rs->getDouble("left_in_hand_pinky_rot_y"),   rs->getDouble("left_in_hand_pinky_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandPinky1],    NeuronBVH::BonesType::LeftHandPinky1,    rs->getDouble("left_hand_pinky1_rot_x"),     rs->getDouble("left_hand_pinky1_rot_y"),     rs->getDouble("left_hand_pinky1_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandPinky2],    NeuronBVH::BonesType::LeftHandPinky2,    rs->getDouble("left_hand_pinky2_rot_x"),     rs->getDouble("left_hand_pinky2_rot_y"),     rs->getDouble("left_hand_pinky2_rot_z"));
			PerceptionNeuronDAO::setJointPosition(motion.links[NeuronBVH::BonesType::LeftHandPinky3],    NeuronBVH::BonesType::LeftHandPinky3,    rs->getDouble("left_hand_pinky3_rot_x"),     rs->getDouble("left_hand_pinky3_rot_y"),     rs->getDouble("left_hand_pinky3_rot_z"));

			//動作情報リストに追加
			timeSeries.push_back(motion);
		}

		endTime = timeGetTime();

		diffTime = (endTime-startTime)/1000.0;

		std::cout << "レコード数＝" << timeSeries.size() << std::endl;

		stmt->close();
		con->close();

		delete rs;
		delete stmt;
		delete con;

		fprintf(stdout, "SELECTにかかった時間： %8.3f[s] \n", diffTime);

		std::cout << "◆モデル動作データ取得　－終了－◆" << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return timeSeries.size();
}


/*
 * MySQLから取得した関節座標データ(x,y,z)を、SIGVerse送信用関節オブジェクトに設定する
 */
void PerceptionNeuronDAO::setJointPosition(PerceptionNeuronSensorData::PerceptionNeuronBVHJoint &joint, const NeuronBVH::BonesType &bonesType, const double x, const double y, const double z)
{
	joint.jointType = bonesType;

	joint.rotation.x = (float)(x);
	joint.rotation.y = (float)(y);
	joint.rotation.z = (float)(z);
}




/*
 * データベースへのモーションデータの蓄積
 */
void PerceptionNeuronDAO::insertDatabase(const PerceptionNeuronDAO::DataSet &motionInfo)
{
	try
	{
		std::string inputKey;

		while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
		{
			std::cout << "収録データをデータベースに蓄積しますか？(y/n)："; std::cin >> inputKey;
		}

		if (inputKey.compare("y") == 0)
		{
			PerceptionNeuronDAO::insertDatabaseExec(motionInfo);
		}
		else
		{
			std::cout << "◆データベースに蓄積しません◆" << std::endl << std::endl;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}


/*
 * データベースへのモーションデータの蓄積（実行部）
 */
void PerceptionNeuronDAO::insertDatabaseExec(const PerceptionNeuronDAO::DataSet &motionInfo)
{
	std::cout << "◆データベース(Perception Neuron関連)蓄積　－開始－◆" << std::endl;

	sql::mysql::MySQL_Driver *driver;
	sql::Connection *con;

	driver = sql::mysql::get_driver_instance();

	//DB接続
	con = driver->connect("tcp://" + Param::getDbHost() + ":" + std::to_string(Param::getDbPort()), Param::getDbUser(), Param::getDbPass());
	con->setSchema(Param::getDbSchema());
	con->setAutoCommit(false);

	time_t start_time, end_time;

	time(&start_time);

	//Insert into the General tables.
	PerceptionNeuronDAO::insert(con, motionInfo);

	time(&end_time);

	fprintf(stdout, "INSERTにかかった時間： %3.0f[s] \n", difftime(end_time, start_time));

	con->close();

	std::cout << "◆データベース(Perception Neuron関連)蓄積　－終了－◆" << std::endl << std::endl;
}


/*
 * データベースへのモーションデータの蓄積（汎用テーブル用）
 */
void PerceptionNeuronDAO::insert(sql::Connection *con, const PerceptionNeuronDAO::DataSet &motionInfo)
{
	/*
	 * Perception Neuron動作サマリ情報テーブルへのINSERT処理（1件のみ）
	 */
	sql::Statement *stmt;

	sql::SQLString insertQuery
		=
		"INSERT INTO perception_neuron_motions_summary (rec_id, user_id, rec_total_time, rec_start_time, rec_interval, memo) VALUES ("
		+ std::to_string(motionInfo.summary.recId) + ","
		+ std::to_string(motionInfo.summary.userId) + ","
		+ std::to_string(motionInfo.summary.recTotalTime) + ","
		+ "'" + motionInfo.summary.getRecStartTimeStr() + "'" + ","
		+ std::to_string(motionInfo.summary.recInterval) + ","
		+ "'" + motionInfo.summary.memo + "'" +
		") ";

	stmt = con->createStatement();
	stmt->executeUpdate(insertQuery);

	std::cout << "Perception Neuron動作サマリ情報テーブルへのINSERT終了" << std::endl;


	/*
	 * Perception Neuron動作時系列情報テーブルへのINSERT処理（1件以上）
	 */
	sql::PreparedStatement *pstmt;

	sql::SQLString insertQueryPStmt
		=
		"INSERT INTO perception_neuron_motions_time_series VALUES ("
		"?,?,"                                                                  // rec_id, elapsed_time
		"?,?,?,"                                                                // hips_pos_x, hips_pos_y, hips_pos_z
		"?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?," // 10
		"?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?," // 20
		"?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?," // 30
		"?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?," // 40
		"?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?," // 50
		"?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?, ?,?,?"         // 59
		")";

//	std::cout << "SQLdetail:" << insertQueryPStmt << std::endl;

	pstmt = con->prepareStatement(insertQueryPStmt);

	int jointDataNum = 3;

	std::list<PerceptionNeuronDAO::TimeSeries_t>::const_iterator it = motionInfo.timeSeries.begin();

	//PreparedStatementを使用して複数件のレコードをINSERT
	while (it != motionInfo.timeSeries.end())
	{
		pstmt->setInt(1, motionInfo.summary.recId);
		pstmt->setInt(2, (*it).elapsedTime);

		pstmt->setDouble(3, (*it).hips_pos.x);
		pstmt->setDouble(4, (*it).hips_pos.y);
		pstmt->setDouble(5, (*it).hips_pos.z);

		int index = 6;

		for (int linkNo = 0; linkNo < NeuronBVH::BonesType::BonesTypeCount; linkNo++)
		{
			PerceptionNeuronDAO::setPreparedStatementLinkValue(pstmt, index, (*it).links[linkNo]);
			index += jointDataNum;
		}

		pstmt->executeUpdate();

		it++;
	}

	//commit
	con->commit();

	std::cout << "Perception Neuron動作時系列情報テーブルへのINSERT終了" << std::endl;


	stmt->close();
	pstmt->close();
}



/*
 * PreparedStatementの1つのLinkに値をセットする
 */
void PerceptionNeuronDAO::setPreparedStatementLinkValue(sql::PreparedStatement *pstmt, const int preparedStatementIndex, const PerceptionNeuronSensorData::PerceptionNeuronBVHJoint link)
{
	pstmt->setDouble(preparedStatementIndex + 0, link.rotation.y);
	pstmt->setDouble(preparedStatementIndex + 1, link.rotation.x);
	pstmt->setDouble(preparedStatementIndex + 2, link.rotation.z);
}

