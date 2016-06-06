/*
 * データベース(MySQL)に収録された各種データを簡易なコマンドでメンテナンスする
 *
 * ＜流れ＞
 * 1. コンソールからのコマンド入力で、メンテナンスしたいテーブルを選択させる
 *    　メンテナンス対象テーブルは以下
 *    　　モデル動作情報関連テーブル(m_motion_info_of_model, m_motion_info_of_model_summary)
 *    　　PSMove操作履歴関連テーブル(t_psmove_operation_history, t_psmove_operation_history_summary)
 * 2. 指定したテーブルについて、情報表示、更新、削除 等をコンソールからの簡易なコマンド入力で行う
 *
 * ＜引数＞
 * なし
 * 
 * ＜その他＞
 * ・設定ファイル名は、config.ini
 */
#include <embodied_brain/common/Param.h>
#include <embodied_brain/manage_recorded_data_on_mysql.h>

#include <iostream>

const std::string Param::PARAM_FILE_NAME = "ManageRecordedDataOnMySQL.ini";

std::string Param::dbHost;
int         Param::dbPort;
std::string Param::dbSchema;
std::string Param::dbUser;
std::string Param::dbPass;

/*
 * メイン
 */
int main(int argc, char **argv)
{
	try
	{
		//コンフィグファイルからデータベース関連のパラメータを取得する
		Param::readConfigFile();


		ManageRecordedDataOnMySQL manageRecordedDataOnMySQL;

		manageRecordedDataOnMySQL.run();
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

