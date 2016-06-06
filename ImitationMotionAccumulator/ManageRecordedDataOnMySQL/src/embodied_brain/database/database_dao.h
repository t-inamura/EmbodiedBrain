#ifndef EMBODIED_BRAIN_DATABASE_DAO_H
#define EMBODIED_BRAIN_DATABASE_DAO_H

#include <string>

/*
 * モデル動作情報関連テーブル管理クラス
 */
class DatabaseDAO
{
private:
	static const int         MAX_LINE;

public:
	int  selectCount(const std::string &tableName, const std::string &motionId);
	void select(int printNum);
	void select();
	int  updateAndDelete(const std::string &sqlStr);
	void printHelp();
};


#endif //EMBODIED_BRAIN_DATABASE_DAO_H
