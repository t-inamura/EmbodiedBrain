///*
// * データベース操作クラス
// */
//#include <time.h>
//#include <mysql.h>
//
//#include "scope/database/DatabaseDAO.h"
//
//DatabaseDAO::DatabaseDAO(){}
//
//DatabaseDAO::~DatabaseDAO(){}
//
///*
// * LONG型のBINDを行う
// */
//void DatabaseDAO::bindMySQLTypeLong(MYSQL_BIND *bind, int *value)
//{
//	bind->buffer_type = MYSQL_TYPE_LONG;
//	bind->buffer      = value;
//	bind->is_null     = 0;
//	bind->length      = 0;
//}
//
////void this->bind_MYSQL_TYPE_SHORT(MYSQL_BIND *bind, short *value)
////{
////	bind->buffer_type = MYSQL_TYPE_SHORT;
////	bind->buffer      = value;
////	bind->is_null     = 0;
////	bind->length      = 0;
////}
//
///*
// * SHORT型のBINDを行う
// */
//void DatabaseDAO::bindMySQLTypeShort(MYSQL_BIND *bind, int *value) //int型をセットしている
//{
//	bind->buffer_type = MYSQL_TYPE_SHORT;
//	bind->buffer      = value;
//	bind->is_null     = 0;
//	bind->length      = 0;
//}
//
///*
// * TINY型のBINDを行う
// */
//void DatabaseDAO::bindMySQLTypeTiny(MYSQL_BIND *bind, int *value) //int型をセットしている
//{
//	bind->buffer_type = MYSQL_TYPE_TINY;
//	bind->buffer      = value;
//	bind->is_null     = 0;
//	bind->length      = 0;
//}
//
////void MySQLOperator::bind_MYSQL_TYPE_TINY(MYSQL_BIND *bind, bool *value)
////{
////	signed char *tmp = 0;
////
////	if(*value) { *tmp = 1; }
////
////	bind->buffer_type = MYSQL_TYPE_TINY;
////	bind->buffer      = (char *)tmp;
//////	bind->is_null     = 0;
//////	bind->length      = 0;
////	bind->is_unsigned = true;
////}
//
///*
// * FLOAT型のBINDを行う
// */
//void DatabaseDAO::bindMySQLTypeFloat(MYSQL_BIND *bind, float *value)
//{
//	bind->buffer_type = MYSQL_TYPE_FLOAT;
//	bind->buffer      = value;
//	bind->is_null     = 0;
//	bind->length      = 0;
//}
//
////void MySQLOperator::bind_MYSQL_TYPE_TIMESTAMP(MYSQL_BIND *bind, MYSQL_TIME *value)
////{
////	// valueの設定方法
////	// value.year        = 2014;
////	// value.month       = 10;
////	// value.day         = 22;
////	// value.hour        = 18;
////	// value.minute      = 31;
////	// value.second      = 0;
////	// value.second_part = 0;
////
////	bind->buffer_type = MYSQL_TYPE_TIMESTAMP;
////	bind->buffer      = value;
////	bind->is_null     = 0;
////	bind->length      = 0;
////}
//
