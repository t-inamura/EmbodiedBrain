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
#include <embodied_brain/database/table/pms_imitation_dao.h>

/*
 * IDの重複チェック
 */
bool PmsImitationDAO::duplicationCheck(const int groupId, const int recType)
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

		//真似情報テーブルから、指定IDの件数を取得するSQL
		sql::SQLString selectSQL = "SELECT COUNT(*) AS cnt FROM pms_imitation_info WHERE group_id=" + std::to_string(groupId) + " AND rec_type=" + std::to_string(recType) + ";";

		stmt = con->createStatement();
		rs = stmt->executeQuery(selectSQL);

		if (rs->next())
		{
			//指定IDの件数が0件でなかった場合は、エラー
			if (rs->getInt("cnt") != 0)
			{
				std::cout << "データベース上に同じ主キー(group_id=" + std::to_string(groupId) + ",rec_type="+std::to_string(recType)+")の情報が存在します。処理終了します。" << std::endl;
				return false;
			}
		}

		delete rs;
		delete stmt;
		delete con;

		return true;
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
		else
		{
			return true;
		}
	}
	return false;
}



/*
 * データベースへのレコード追加
 */
void PmsImitationDAO::insertDatabase(const PmsImitationDAO::DataSet &motionInfo)
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
			PmsImitationDAO::insertDatabaseExec(motionInfo);
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
 * データベースへの情報追加（実行部）
 */
void PmsImitationDAO::insertDatabaseExec(const PmsImitationDAO::DataSet &motionInfo)
{
	std::cout << "◆データベース(PMS情報関連)蓄積　－開始－◆" << std::endl;

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
	PmsImitationDAO::insert(con, motionInfo);

	time(&end_time);

	fprintf(stdout, "INSERTにかかった時間： %3.0f[s] \n", difftime(end_time, start_time));

	con->close();

	std::cout << "◆データベース(PMS情報関連)蓄積　－終了－◆" << std::endl << std::endl;
}


/*
 * データベースへの情報追加（クエリ発行）
 */
void PmsImitationDAO::insert(sql::Connection *con, const PmsImitationDAO::DataSet &motionInfo)
{
	/*
	 * 真似情報テーブルへのINSERT処理（1件のみ）
	 */
	sql::Statement *stmt;

	sql::SQLString insertQuery
		=
		"INSERT INTO pms_imitation_info ("
			"group_id, "
			"rec_type, "
			"rec_id, "
			"original_rec_id, "
			"condition_pulse_power, "
			"condition_pulse_frequency, "
			"condition_pulse_duration, "
			"condition_pulse_interval, "
			"condition_pulse_number, "
			"memo "
		")VALUES ("
			+ std::to_string(motionInfo.groupId) + ","
			+ std::to_string(motionInfo.recType) + ","
			+ std::to_string(motionInfo.recId) + ","
			+ std::to_string(motionInfo.originalRecId) + ","
			+ std::to_string(motionInfo.conditionPulsePower) + ","
			+ std::to_string(motionInfo.conditionPulseFrequency) + ","
			+ std::to_string(motionInfo.conditionPulseDuration) + ","
			+ std::to_string(motionInfo.conditionPulseInterval) + ","
			+ std::to_string(motionInfo.conditionPulseNumber) + ","
			+ "'" + motionInfo.memo + "'" +
		") ";

	stmt = con->createStatement();
	stmt->executeUpdate(insertQuery);
	con->commit();

	std::cout << "PMS実験_真似情報テーブルへのINSERT終了" << std::endl;

	stmt->close();
}

