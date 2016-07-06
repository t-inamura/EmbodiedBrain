#ifndef EMBODIED_BRAIN_DATABASE_DAO_H
#define EMBODIED_BRAIN_DATABASE_DAO_H

#include <string>

/*
 * 動作切替・収録情報関連テーブル管理クラス
 */
class DatabaseDAO
{
public:
	static const int         MAX_LINE     = 100;
	static const std::string DETAIL_TBL;
	static const std::string SUMMARY_TBL;
	static const std::string SWITCHING_TBL;

	int  selectCount(const std::string &tableName, const std::string &columnName, const std::string &recId);
	void select(int printNum);
	void select();
	void selectSwitching(int printNum);
	void selectSwitching();
	int  updateAndDelete(const std::string &sqlStr);
};


#endif //EMBODIED_BRAIN_DATABASE_DAO_H
