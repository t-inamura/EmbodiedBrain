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


const std::string DatabaseDAO::DETAIL_TBL    = "perception_neuron_motions_time_series";
const std::string DatabaseDAO::SUMMARY_TBL   = "perception_neuron_motions_summary";
const std::string DatabaseDAO::SWITCHING_TBL = "msw_recording_info";


/*
 * データベース管理実行
 */
void ManageRecordedDataOnMySQL::run()
{
	try
	{
		typedef boost::char_separator<char> BOOST_CHAR_SEP;
		typedef boost::tokenizer< BOOST_CHAR_SEP > BOOST_TOKENIZER;

		boost::regex regexSelectPnPattern       ("select pn");
		boost::regex regexSelectPnWithNumPattern("select pn +[0-9]{1,9}");
		boost::regex regexSelectSwPattern       ("select sw");
		boost::regex regexSelectSwWithNumPattern("select sw +[0-9]{1,9}");
		boost::regex regexUpdateMidPattern      ("update pn +[0-9]{1,9} +[0-9]{1,9}");
		//boost::regex regexUpdateMidPattern      ("update pn +[0-9]{1,9} +rid +[0-9]{1,9}");
		//boost::regex regexUpdateMemoPattern     ("update pn +[0-9]{1,9} +memo .+");
		boost::regex regexDeletePattern         ("delete pn +[0-9]{1,9}");
		boost::regex regexHelpPattern           ("h");
		boost::regex regexExitPattern           ("q");

		DatabaseDAO databaseDAO;

		char inputLineAry[DatabaseDAO::MAX_LINE];
		BOOST_CHAR_SEP sep(" ");

		std::string inputLine;

		// 使い方表示
		this->printHelp();

		while (true)
		{
			std::cout << std::endl;
			std::cout << "> コマンドを入力してください（h :help）" << std::endl;
			std::cout << "> "; std::cin.getline(inputLineAry, DatabaseDAO::MAX_LINE);

			std::string inputLine = inputLineAry;

			boost::algorithm::trim(inputLine);

			/*
			 * select pn
			 */
			if (regex_match(inputLine, regexSelectPnPattern))
			{
				databaseDAO.select();
			}
			/*
			 * select pn XXX
			 */
			else if (regex_match(inputLine, regexSelectPnWithNumPattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);
				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++; it++;
				int printNum = stoi(*it);

				databaseDAO.select(printNum);
			}
			/*
			 * select sw
			 */
			else if (regex_match(inputLine, regexSelectSwPattern))
			{
				databaseDAO.selectSwitching();
			}
			/*
			 * select sw XXX
			 */
			else if (regex_match(inputLine, regexSelectSwWithNumPattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);
				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++; it++;
				int printNum = stoi(*it);

				databaseDAO.selectSwitching(printNum);
			}
			/*
			 * update pn RRR NNN
			 */
			else if (regex_match(inputLine, regexUpdateMidPattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);
				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++; it++;
				std::string recId = *it;
				it++; 
				std::string newRecId = *it;

				// 事前エラーチェック
				if (databaseDAO.selectCount(DatabaseDAO::SUMMARY_TBL, "rec_id", recId) == 0)
				{
					std::cout << "更新元のrec_id(" + recId + ")が存在しません。処理終了します。" << std::endl;
					continue;
				}

				if (databaseDAO.selectCount(DatabaseDAO::SUMMARY_TBL, "rec_id", newRecId) != 0)
				{
					std::cout << "更新先のrec_id(" + newRecId + ")が" + DatabaseDAO::SUMMARY_TBL + "に既に存在します。処理終了します。" << std::endl;
					continue;
				}

				std::cout << "時系列データのレコード数は、" << databaseDAO.selectCount(DatabaseDAO::DETAIL_TBL, "rec_id", recId) << "件です。" << std::endl;


				char inputYN[DatabaseDAO::MAX_LINE];
				std::string inputKey = "";

				while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
				{
					std::cout << "> rec_id=[" << recId << "] new rec_id=[" << newRecId << "]として更新しますが宜しいですか？(y/n)：";
					std::cin.getline(inputYN, DatabaseDAO::MAX_LINE);
					inputKey = inputYN;
				}

				if (inputKey.compare("y") == 0)
				{
					int cnt;

					// 動作切替・収録情報テーブル更新
					cnt = databaseDAO.updateAndDelete("UPDATE " + DatabaseDAO::SWITCHING_TBL + " SET rec_id=" + newRecId + " WHERE rec_id=" + recId);
					std::cout << DatabaseDAO::SWITCHING_TBL+"のrec_id="+recId+"のレコードを" << cnt << "件更新しました。" << std::endl;

					cnt = databaseDAO.updateAndDelete("UPDATE " + DatabaseDAO::SWITCHING_TBL + " SET original_rec_id=" + newRecId + " WHERE original_rec_id=" + recId);
					std::cout << DatabaseDAO::SWITCHING_TBL+"のoriginal_rec_id="+recId+"のレコードを" << cnt << "件更新しました。" << std::endl;


					// Perception Neuron動作サマリテーブル更新
					cnt = databaseDAO.updateAndDelete("UPDATE " + DatabaseDAO::SUMMARY_TBL + " SET rec_id=" + newRecId + " WHERE rec_id=" + recId);
					std::cout << DatabaseDAO::SUMMARY_TBL+"のレコードを" << cnt << "件更新しました。" << std::endl;


					// Perception Neuron動作時系列テーブル更新
//					std::cout << DatabaseDAO::DETAIL_TBL+"を更新中です。" << std::endl;
					cnt = databaseDAO.updateAndDelete("UPDATE " + DatabaseDAO::DETAIL_TBL + " SET rec_id=" + newRecId + " WHERE rec_id=" + recId);
					std::cout << DatabaseDAO::DETAIL_TBL+"のレコードを" << cnt << "件更新しました。" << std::endl;
				}
			}
			//メモの更新に関しては、動作切替・収録情報テーブルにしても、Perception Neuron動作サマリテーブルにしても１行しかないため、MySQL Workbenchで直接編集してもらっても大差ない。
			///*
			// * update RRR memo NNN
			// */
			//else if (regex_match(inputLine, regexUpdateMemoPattern))
			//{
			//	BOOST_TOKENIZER tokens(inputLine, sep);
			//	boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

			//	it++; 
			//	std::string recId = *it;
			//	it++; it++;
			//	std::string newMemo;

			//	while(it!=tokens.end())
			//	{
			//		newMemo += (*it) + " ";
			//		it++;
			//	}
			//	boost::trim(newMemo);

			//	if (databaseDAO.selectCount(DatabaseDAO::SUMMARY_TBL, "rec_id", recId) == 0)
			//	{
			//		std::cout << "対象のrec_id(" + recId + ")が存在しません。処理終了します。" << std::endl;
			//		continue;
			//	}

			//	char inputYN[DatabaseDAO::MAX_LINE];
			//	std::string inputKey = "";

			//	while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
			//	{
			//		std::cout << "> rec_id=[" << recId << "] new memo=[" << newMemo << "]として" + DatabaseDAO::SUMMARY_TBL + "を更新しますが宜しいですか？(y/n)：";
			//		std::cin.getline(inputYN, DatabaseDAO::MAX_LINE);
			//		inputKey = inputYN;
			//	}

			//	if (inputKey.compare("y") == 0)
			//	{
			//		databaseDAO.updateAndDelete("UPDATE " + DatabaseDAO::SUMMARY_TBL + " SET memo='" + newMemo + "' WHERE rec_id=" + recId);
			//		std::cout << DatabaseDAO::SUMMARY_TBL+"のmemoを更新しました。" << std::endl;
			//	}
			//}
			/*
			 * delete pn RRR
			 */
			else if (regex_match(inputLine, regexDeletePattern))
			{
				BOOST_TOKENIZER tokens(inputLine, sep);
				boost::tokenizer< BOOST_CHAR_SEP >::iterator it = tokens.begin();

				it++; it++;
				std::string recId = *it;

				// 事前エラーチェック
				if (databaseDAO.selectCount(DatabaseDAO::SUMMARY_TBL, "rec_id", recId) == 0)
				{
					std::cout << "削除対象のrec_id(" + recId + ")が存在しません。処理終了します。" << std::endl;
					continue;
				}
				if (databaseDAO.selectCount(DatabaseDAO::SWITCHING_TBL, "original_rec_id", recId) != 0)
				{
					std::cout << "削除対象のrec_id(" + recId + ")を手本動作として使用している収録があります。処理終了します。" << std::endl;
					continue;
				}

				std::cout << "時系列データのレコード数は、" << databaseDAO.selectCount(DatabaseDAO::DETAIL_TBL, "rec_id", recId) << "件です。" << std::endl;


				char inputYN[DatabaseDAO::MAX_LINE];
				std::string inputKey = "";

				while (inputKey.compare("y") != 0 && inputKey.compare("n") != 0)
				{
					std::cout << "> rec_id=[" << recId << "]のレコードを削除しますが宜しいですか？(y/n)：";
					std::cin.getline(inputYN, DatabaseDAO::MAX_LINE);
					inputKey = inputYN;
				}

				if (inputKey.compare("y") == 0)
				{
					int cnt;

					// 真似情報テーブル更新
					cnt = databaseDAO.updateAndDelete("DELETE FROM " + DatabaseDAO::SWITCHING_TBL + " WHERE rec_id=" + recId);
					std::cout << DatabaseDAO::SWITCHING_TBL+"のレコードを" << cnt << "件削除しました。" << std::endl;


					// Perception Neuron動作サマリテーブル更新
					cnt = databaseDAO.updateAndDelete("DELETE FROM " + DatabaseDAO::SUMMARY_TBL + " WHERE rec_id=" + recId);
					std::cout << DatabaseDAO::SUMMARY_TBL+"のレコードを" << cnt << "件削除しました。" << std::endl;


					// Perception Neuron動作時系列テーブル更新
//					std::cout << DatabaseDAO::DETAIL_TBL+"を削除中です。" << std::endl;
					cnt = databaseDAO.updateAndDelete("DELETE FROM " + DatabaseDAO::DETAIL_TBL + " WHERE rec_id=" + recId);
					std::cout << DatabaseDAO::DETAIL_TBL+"のレコードを" << cnt << "件削除しました。" << std::endl;
				}
			}
			/*
			 * help
			 */
			else if (regex_match(inputLine, regexHelpPattern))
			{
				this->printHelp();
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


/*
 * ヘルプ表示
 */
void ManageRecordedDataOnMySQL::printHelp()
{
	std::string printStr = "\n"
		"　■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n"
		"　■ 動作切替情報関連テーブル操作方法                             ■\n"
		"　■   （現状、Perception Neuron関連テーブルのみ変更・削除可能）  ■\n"
		"　■                                                              ■\n"
		"　■   select pn          ：PerceptionNeuron表示(全て)            ■\n"
		"　■   select pn XXX      ：PerceptionNeuron表示(指定件数XXXずつ) ■\n"
		"　■   select sw          ：動作切替情報表示(全て)                ■\n"
		"　■   select sw XXX      ：動作切替情報表示(指定件数XXXずつ)     ■\n"
		"　■   update pn RRR NNN  ：PerceptionNeuron 変更(rec_id)         ■\n"
//		"　■   update pn RRR memo NNN：変更(memo)                         ■\n"
		"　■   delete pn RRR      ：PerceptionNeuron 削除                 ■\n"
		"　■   h                  ：本操作方法表示                        ■\n"
		"　■   q                  ：終了                                  ■\n"
		"　■                                                              ■\n"
		"　■     (RRR:rec_id, NNN:new rec_id)                             ■\n"
		"　■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";

	std::cout << printStr;
}


