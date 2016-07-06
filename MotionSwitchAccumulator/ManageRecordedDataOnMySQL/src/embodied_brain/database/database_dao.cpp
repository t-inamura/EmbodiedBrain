#pragma warning(disable:4251)

#include <embodied_brain/common/Param.h>
#include <embodied_brain/manage_recorded_data_on_mysql.h>
#include <embodied_brain/database/database_dao.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <windows.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>


/*
 * 指定テーブル、指定収録IDのレコード件数を表示
 */
int DatabaseDAO::selectCount(const std::string &tableName, const std::string &columnName, const std::string &recId)
{
	try
	{
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		driver = sql::mysql::get_driver_instance();

		con = driver->connect("tcp://" + Param::getDbHost() + ":" + std::to_string((long long)Param::getDbPort()), Param::getDbUser(), Param::getDbPass());
		con->setSchema(Param::getDbSchema());

		sql::SQLString selectSQL = "select count(*) from " + tableName + " WHERE "+columnName+"=" + recId;

		stmt = con->createStatement();
		rs = stmt->executeQuery(selectSQL);

		if (rs->next())
		{
			return rs->getInt(1);
		}
		else
		{
			return 0;
		}

		delete rs;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException & ex)
	{
		std::cout << "select count中にエラーが発生しました。" << std::endl;
		std::cout << "db connection error:" << ex.what() << std::endl;
		return -1;
	}
}


/*
 * 指定件数ずつ表示
 */
void DatabaseDAO::select(const int printNum)
{
	try
	{
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		driver = sql::mysql::get_driver_instance();

		con = driver->connect("tcp://" + Param::getDbHost() + ":" + std::to_string((long long)Param::getDbPort()), Param::getDbUser(), Param::getDbPass());
		con->setSchema(Param::getDbSchema());

		sql::SQLString selectSQL = 
			"select sum_t.rec_id, sum_t.rec_total_time, sum_t.rec_start_time, sum_t.memo, detail_t.data_num "
			"from "
			"(select * from " + SUMMARY_TBL + ") sum_t "
			"left join "
			"(select rec_id, count(rec_id) as data_num from " + DETAIL_TBL + " group by rec_id) detail_t "
			"on sum_t.rec_id = detail_t.rec_id ";

		stmt = con->createStatement();
		rs = stmt->executeQuery(selectSQL);
		
		std::cout << " RecId  |Time[s]| StartTime           | Memo                                   |Data_num " << std::endl;

		int cnt = 0;

		while(rs->next())
		{
//			std::ostringstream groupIdStr;
//
//			if (!rs->isNull("group_id"))
//			{
//				groupIdStr << std::setfill(' ') << std::setw(7) << std::right << rs->getInt("group_id");
//			}
//			else
//			{
//				groupIdStr << "   -   ";
//			}

			std::cout <<
				std::setfill(' ') << std::setw(7) << std::right << rs->getInt("rec_id") << " | " <<
				std::setfill(' ') << std::setw(5) << std::right << rs->getInt("rec_total_time") << " | " <<
				rs->getString("rec_start_time") << " | " <<
				std::setfill(' ') << std::setw(38) << std::left << rs->getString("memo") << " | " << 
				std::setfill(' ') << std::setw(6) << std::right << rs->getInt("data_num") << //" | " << 
//				groupIdStr.str() << 
				std::endl;

			cnt++;

			if (cnt == printNum)
			{
				char inputYN[MAX_LINE];

				std::cout << "> press Enter"; std::cin.getline(inputYN, MAX_LINE);

				cnt = 0;
			}
		}

		delete rs;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException & ex)
	{
		std::cout << "select中にエラーが発生しました。" << std::endl;
		std::cout << "db connection error:" << ex.what() << std::endl;
	}
}


/*
 * 全件表示
 */
void DatabaseDAO::select()
{
	this->select(99999);
}


/*
 * 指定件数ずつ表示（動作切替・収録情報）
 */
void DatabaseDAO::selectSwitching(const int printNum)
{
	try
	{
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		driver = sql::mysql::get_driver_instance();

		con = driver->connect("tcp://" + Param::getDbHost() + ":" + std::to_string((long long)Param::getDbPort()), Param::getDbUser(), Param::getDbPass());
		con->setSchema(Param::getDbSchema());

		sql::SQLString selectSQL = "select * from " + SWITCHING_TBL;

		stmt = con->createStatement();
		rs = stmt->executeQuery(selectSQL);
		
		std::cout << " GroupId | BeforeRecId | AfterRecId  | FakeRecId   | Memo                                   " << std::endl;

		int cnt = 0;

		while(rs->next())
		{
			std::cout <<
				std::setfill(' ') << std::setw(8)  << std::right << rs->getInt("group_id")      << " | " <<
				std::setfill(' ') << std::setw(12) << std::right << rs->getInt("before_rec_id") << " | " <<
				std::setfill(' ') << std::setw(12) << std::right << rs->getInt("after_rec_id")  << " | " <<
				std::setfill(' ') << std::setw(12) << std::right << rs->getInt("fake_rec_id")   << " | " <<
				std::setfill(' ') << std::setw(38) << std::left  << rs->getString("memo")  << 
				std::endl;

			cnt++;

			if (cnt == printNum)
			{
				char inputYN[MAX_LINE];

				std::cout << "> press Enter"; std::cin.getline(inputYN, MAX_LINE);

				cnt = 0;
			}
		}

		delete rs;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException & ex)
	{
		std::cout << "select中にエラーが発生しました。" << std::endl;
		std::cout << "db connection error:" << ex.what() << std::endl;
	}
}


/*
 * 全件表示（真似情報）
 */
void DatabaseDAO::selectSwitching()
{
	this->selectSwitching(99999);
}



/*
 * 更新/削除
 */
int DatabaseDAO::updateAndDelete(const std::string &sqlStr)
{
	try
	{
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		int cnt;

		driver = sql::mysql::get_driver_instance();

		con = driver->connect("tcp://" + Param::getDbHost() + ":" + std::to_string((long long)Param::getDbPort()), Param::getDbUser(), Param::getDbPass());
		con->setSchema(Param::getDbSchema());
		con->setAutoCommit(false);

		stmt = con->createStatement();
		cnt = stmt->executeUpdate(sqlStr);

		con->commit();

		return cnt;

		delete stmt;
		delete con;
	}
	catch (sql::SQLException & ex)
	{
		std::cout << "update or delete中にエラーが発生しました。" << std::endl;
		std::cout << "db connection error:" << ex.what() << std::endl;
		return -1;
	}
}


