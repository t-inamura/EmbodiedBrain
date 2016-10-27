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
#include <embodied_brain/database/table/msw_recording_info_dao.h>

/*
 * IDの重複チェック
 */
bool MswRecordingInfoDAO::duplicationCheck(const int recId)
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
		sql::SQLString selectSQL = "SELECT COUNT(*) AS cnt FROM motion_switching_recording_info WHERE after_switching_rec_id=" + std::to_string(recId) + " ;";

		stmt = con->createStatement();
		rs = stmt->executeQuery(selectSQL);

		if (rs->next())
		{
			//指定IDの件数が0件でなかった場合は、エラー
			if (rs->getInt("cnt") != 0)
			{
				std::cout << "データベース上に同じ主キー(after_switching_rec_id=" + std::to_string(recId) + ")の情報が存在します。処理終了します。" << std::endl;
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
void MswRecordingInfoDAO::insertDatabase(const MswRecordingInfoDAO::DataSet &motionInfo)
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
			MswRecordingInfoDAO::insertDatabaseExec(motionInfo);
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
void MswRecordingInfoDAO::insertDatabaseExec(const MswRecordingInfoDAO::DataSet &motionInfo)
{
	std::cout << "◆データベース(動作切替実験_収録情報関連)蓄積　－開始－◆" << std::endl;

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
	MswRecordingInfoDAO::insert(con, motionInfo);

	time(&end_time);

	fprintf(stdout, "INSERTにかかった時間： %3.0f[s] \n", difftime(end_time, start_time));

	con->close();

	std::cout << "◆データベース(動作切替実験_収録情報関連)蓄積　－終了－◆" << std::endl << std::endl;
}


/*
 * データベースへの情報追加（クエリ発行）
 */
void MswRecordingInfoDAO::insert(sql::Connection *con, const MswRecordingInfoDAO::DataSet &motionInfo)
{
	/*
	 * 収録情報テーブルへのINSERT処理（1件のみ）
	 */
	sql::Statement *stmt;

	sql::SQLString insertQuery
		=
		"INSERT INTO motion_switching_recording_info ("
			"after_switching_rec_id, "
			"serial_number, "
			"before_switching_rec_id, "
			"fake_rec_id, "
			"memo, "
			"number_of_iterations, "
			"smoothing_type, "
			"frames_number_for_delay, "
			"invert_flg, "
			"invert_fake_flg "
		")VALUES ("
			+ std::to_string(motionInfo.afterSwitchingRecId) + ","
			+ std::to_string(motionInfo.seialNumber) + ","
			+ std::to_string(motionInfo.beforeSwitchingRecId) + ","
			+ std::to_string(motionInfo.fakeRecId) + ","
			+ "'" + motionInfo.memo + "'" + ","
			+ std::to_string(motionInfo.numberOfIterations) + ","
			+ std::to_string(motionInfo.smoothingType) + ","
			+ std::to_string(motionInfo.framesNumberForDelay) + ","
			+ std::to_string(motionInfo.invertFlg) + ","
			+ std::to_string(motionInfo.invertFakeFlg) + ","
		") ";

	stmt = con->createStatement();
	stmt->executeUpdate(insertQuery);
	con->commit();

	std::cout << "動作切替実験_収録情報テーブルへのINSERT終了" << std::endl;

	stmt->close();
}

