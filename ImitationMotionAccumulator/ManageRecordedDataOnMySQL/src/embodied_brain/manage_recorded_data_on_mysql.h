#ifndef EMBODIED_BRAIN_MANAGE_RECORDED_DATA_ON_MYSQL_H
#define EMBODIED_BRAIN_MANAGE_RECORDED_DATA_ON_MYSQL_H

/*
 * データベース管理クラス
 */
class ManageRecordedDataOnMySQL
{
public:
	static const int         MAX_LINE = 100;
	static const std::string DETAIL_TBL;
	static const std::string SUMMARY_TBL;
	static const std::string IMITATION_TBL;

	void run();
};


#endif //EMBODIED_BRAIN_MANAGE_RECORDED_DATA_ON_MYSQL_H
