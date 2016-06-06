/*
 * データベース管理クラス
 */
#include <embodied_brain/manage_recorded_data_on_mysql.h>
#include <embodied_brain/database/database_dao.h>

#include <iostream>
#include <string>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>


const int         ManageRecordedDataOnMySQL::MAX_LINE      = 100;
const std::string ManageRecordedDataOnMySQL::DETAIL_TBL    = "perception_neuron_motions_time_series";
const std::string ManageRecordedDataOnMySQL::SUMMARY_TBL   = "perception_neuron_motions_summary";
const std::string ManageRecordedDataOnMySQL::IMITATION_TBL = "pms_imitation_info";


/*
 * データベース管理実行
 */
void ManageRecordedDataOnMySQL::run()
{
	try
	{
		typedef boost::char_separator<char> BOOST_CHAR_SEP;
		typedef boost::tokenizer< BOOST_CHAR_SEP > BOOST_TOKENIZER;

		boost::regex regexSelectPattern       ("l");
		boost::regex regexSelectWithNumPattern("l +[0-9]{1,9}");
		boost::regex regexUpdateMidPattern    ("update +[0-9]{1,9} +mid +[0-9]{1,9}");
		boost::regex regexUpdateMemoPattern   ("update +[0-9]{1,9} +memo .+");
		boost::regex regexDeletePattern       ("delete +[0-9]{1,9}");
		boost::regex regexHelpPattern         ("h");
		boost::regex regexExitPattern         ("q");

		DatabaseDAO databaseDAO;

		char inputLineAry[MAX_LINE];
		BOOST_CHAR_SEP sep(" ");

		std::string inputLine;

		databaseDAO.printHelp();

		while (true)
		{
			std::cout << "> コマンドを入力してください" << std::endl;
			std::cout << "> "; std::cin.getline(inputLineAry, MAX_LINE);

			std::string inputLine = inputLineAry;

			boost::algorithm::trim(inputLine);

			/*
			 * select
			 */
			if (regex_match(inputLine, regexSelectPattern))
			{
				databaseDAO.select();
			}
			/*
			 * select XXX
			 */
			else if (regex_match(inputLine, regexSelectWithNumPattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);

				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++; 
				int printNum = stoi(*it);

				databaseDAO.select(printNum);
			}
			/*
			 * update MMM mid NNN
			 */
			else if (regex_match(inputLine, regexUpdateMidPattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);

				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++; 
				std::string motionId = *it;
				it++; it++;
				std::string newMotionId = *it;

				if (databaseDAO.selectCount(SUMMARY_TBL, motionId) == 0)
				{
					std::cout << "更新元のmotion_id(" + motionId + ")が存在しません。処理終了します。" << std::endl;
					continue;
				}

				if (databaseDAO.selectCount(SUMMARY_TBL, newMotionId) != 0)
				{
					std::cout << "更新先のmotion_id(" + newMotionId + ")が" + SUMMARY_TBL + "に既に存在します。処理終了します。" << std::endl;
					continue;
				}

				if (databaseDAO.selectCount(DETAIL_TBL, newMotionId) != 0)
				{
					std::cout << "更新先のmotion_id(" + newMotionId + ")が" + DETAIL_TBL + "に既に存在します。処理終了します。" << std::endl;
					continue;
				}


				std::cout << DETAIL_TBL+"中の更新対象レコード数は、" << databaseDAO.selectCount(DETAIL_TBL, motionId) << "件です。" << std::endl;


				char inputYN[MAX_LINE];
				std::string inputKey = "";

				while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
				{
					std::cout << "> motion_id=[" << motionId << "] new motion_id=[" << newMotionId << "]として" + SUMMARY_TBL + "と" + DETAIL_TBL + "を更新しますが宜しいですか？(y/n)：";
					std::cin.getline(inputYN, MAX_LINE);
					inputKey = inputYN;
				}

				if (inputKey.compare("y") == 0)
				{
					databaseDAO.updateAndDelete("UPDATE " + SUMMARY_TBL + " SET motion_id=" + newMotionId + " WHERE motion_id=" + motionId);

					std::cout << SUMMARY_TBL+"のmotion_idを更新しました。" << std::endl;


					std::cout << DETAIL_TBL+"を更新中です。" << std::endl;

					int cnt = databaseDAO.updateAndDelete("UPDATE " + DETAIL_TBL + " SET motion_id=" + newMotionId + " WHERE motion_id=" + motionId);

					std::cout << DETAIL_TBL+"のmotion_id="+motionId+"のレコードを" << cnt << "件更新しました。" << std::endl;
				}
			}
			/*
			 * update MMM memo NNN
			 */
			else if (regex_match(inputLine, regexUpdateMemoPattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);

				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++; 
				std::string motionId = *it;
				it++; it++;
				std::string newMemo;

				while(it!=tokens.end())
				{
					newMemo += (*it) + " ";
					it++;
				}
				boost::trim(newMemo);

				if (databaseDAO.selectCount(SUMMARY_TBL, motionId) == 0)
				{
					std::cout << "対象のmotion_id(" + motionId + ")が存在しません。処理終了します。" << std::endl;
					continue;
				}

				char inputYN[MAX_LINE];
				std::string inputKey = "";

				while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
				{
					std::cout << "> motion_id=[" << motionId << "] new memo=[" << newMemo << "]として" + SUMMARY_TBL + "を更新しますが宜しいですか？(y/n)：";
					std::cin.getline(inputYN, MAX_LINE);
					inputKey = inputYN;
				}

				if (inputKey.compare("y") == 0)
				{
					databaseDAO.updateAndDelete("UPDATE " + SUMMARY_TBL + " SET memo='" + newMemo + "' WHERE motion_id=" + motionId);

					std::cout << SUMMARY_TBL+"のmemoを更新しました。" << std::endl;
				}
			}
			/*
			 * delete MMM
			 */
			else if (regex_match(inputLine, regexDeletePattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);

				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++;
				std::string motionId = *it;

				if (databaseDAO.selectCount(SUMMARY_TBL, motionId) == 0)
				{
					std::cout << "削除対象のmotion_id(" + motionId + ")が存在しません。処理終了します。" << std::endl;
					continue;
				}

				std::cout << DETAIL_TBL+"中の削除対象レコード数は、" << databaseDAO.selectCount(DETAIL_TBL, motionId) << "件です。" << std::endl;


				char inputYN[MAX_LINE];
				std::string inputKey = "";

				while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
				{
					std::cout << "> " + SUMMARY_TBL + "と" + DETAIL_TBL + "からmotion_id=[" << motionId << "]のレコードを削除しますが宜しいですか？(y/n)：";
					std::cin.getline(inputYN, MAX_LINE);
					inputKey = inputYN;
				}

				if (inputKey.compare("y") == 0)
				{
					databaseDAO.updateAndDelete("DELETE FROM " + SUMMARY_TBL + " WHERE motion_id=" + motionId);

					std::cout << SUMMARY_TBL+"のレコードを削除しました。" << std::endl;


					std::cout << DETAIL_TBL+"を削除中です。" << std::endl;

					int cnt = databaseDAO.updateAndDelete("DELETE FROM " + DETAIL_TBL + " WHERE motion_id=" + motionId);

					std::cout << DETAIL_TBL+"のmotion_id="+motionId+"のレコードを" << cnt << "件削除しました。" << std::endl;
				}
			}
			/*
			 * help
			 */
			else if (regex_match(inputLine, regexHelpPattern))
			{
				databaseDAO.printHelp();
			}
			/*
			 * exit
			 */
			else if (regex_match(inputLine, regexExitPattern))
			{
				std::cout << "終了します";
				break;
			}
			/*
			 * FORMAT ERROR
			 */
			else
			{
				std::cout << "書式不正です" << inputLine << std::endl;
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}


}

