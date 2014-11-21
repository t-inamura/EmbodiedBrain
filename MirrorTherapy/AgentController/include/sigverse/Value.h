/* $Id: Value.h,v 1.3 2012-06-11 05:59:25 okamoto Exp $ */ 
#ifndef Value_h
#define Value_h

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

enum {
	VALUE_TYPE_NOT_SET = -1,
	VALUE_TYPE_STRING,
	VALUE_TYPE_INT,
	VALUE_TYPE_UINT,
	VALUE_TYPE_LONG,
	VALUE_TYPE_ULONG,
	VALUE_TYPE_FLOAT,
	VALUE_TYPE_DOUBLE,
	VALUE_TYPE_BOOL,
	VALUE_TYPE_NUM,
};

typedef short ValueType;
/**
 * @brief 属性値クラス
 */
class Value
{
private:
	ValueType	m_type;
public:
	/**
	 * @brief コンストラクタ
	 * @param t 属性値の種類
	 */
	Value(ValueType t) : m_type(t) {;}

	//! 属性値の種類
	ValueType	type() const { return m_type; }
	//! バイナリ化したときのデータサイズの取得
	virtual short	binaryLength() const = 0;
	//! バイナリ化したデータの取得
	virtual char *  binary()  = 0;

	//! デストラクタ
	virtual ~Value() {}

	//! 与えられたオブジェクトの内容をコピーする
	virtual void copy(const Value &o) = 0;

	//! 属性の種類を文字列として取得する
	virtual const char *getTypeString() const = 0;

	//! 属性値をdoule型の値として取得する
	virtual double getDouble() const = 0;

	//! double型の値を設定する
	virtual void setDouble(double v) = 0;


	//! 属性値をint型の値として取得する
	virtual int getInt() const = 0;
	//! int型の値を設定する
	virtual void setInt(int v) = 0; 

	//! 属性値を文字列として取得する
	virtual const char *getString() const = 0;

	//! 文字列値を設定する
	virtual void setString(const char *) = 0;

	//! 属性値をbool型の値として取得する
	virtual bool getBool() const = 0;
	//! bool型の値を設定する
	virtual void setBool(bool) = 0;

	//! オブジェクトのクローンを作成する
	virtual Value *clone() = 0;

	//! バイナリデータから属性値を復元する
	static Value *decode(char *data);
};

/**
 * @brief bool型属性値クラス
 */
class BoolValue : public Value
{
private:
	typedef Value Super;
private:
	enum { DATASIZE = sizeof(ValueType) + sizeof(short), };
private:
	bool	m_value;
public:
	//! コンストラクタ
	BoolValue() : Value(VALUE_TYPE_BOOL) {;}
#ifdef UNIT_TEST
	BoolValue(bool b) : Value(VALUE_TYPE_BOOL) {
		setBool(b);
	}
#endif

private:
	double getDouble() const { return (double)m_value; }
	int getInt() const { return (int)m_value; }
	const char *getString() const;
	const char *getTypeString() const;

	void setDouble(double v);
	void setInt(int v);
	
	void setString(const char *str);

	void copy(const Value &o)
	{
		m_value = o.getBool();
	}
	bool getBool() const { return m_value; }
	void setBool(bool b) { m_value = b; }

	short	binaryLength() const
	{
		return DATASIZE;
	}
	char *  binary();

	Value *clone();
};

/**
 * @brief double型属性値クラス
 */
class DoubleValue : public Value
{
	enum {
		DATASIZE = sizeof(double) + sizeof(ValueType),
	};
		
	typedef Value Super;
private:
	double	m_value;

public:
	//! コンストラクタ
	DoubleValue() : Super(VALUE_TYPE_DOUBLE), m_value(0.0) {;}
#ifdef UNIT_TEST
	DoubleValue(double v) : Super(VALUE_TYPE_DOUBLE), m_value(v) {;}
#endif
private:
	double getDouble() const { return m_value; }
	int getInt() const { return (int)m_value; }

	const char *getString() const;
	const char *getTypeString() const;

	void setDouble(double v) { m_value = v; }
	void setInt(int v) { m_value = (double)v; }
	void setString(const char *s);

	bool getBool() const { return false; }
	void setBool(bool b) { ; }

	short	binaryLength() const {
		return DATASIZE;
	}
	char *  binary();
	void copy(const Value &o);

	Value *clone();
};

/**
 * @brief 文字列型属性値クラス
 */
class StringValue : public Value
{
private:
	std::string	m_str;
	char	*m_buf;
	int	m_bufsize;
public:
	//! コンストラクタ
	StringValue() : Value(VALUE_TYPE_STRING), m_buf(0), m_bufsize(0) {;}
	StringValue(const char *str) : Value(VALUE_TYPE_STRING), m_buf(0), m_bufsize(0)
	{
		setString(str);
	}
	//! デストラクタ
	~StringValue() {
		delete [] m_buf; m_buf = 0;
	}

private:
	double getDouble() const { assert(0); return 0.0; }
	int getInt() const { assert(0); return 0; }
	const char *getString() const { return m_str.c_str(); }
	const char *getTypeString() const {
		static const char *type = "string";
		return type;
	}

	void setDouble(double v) { assert(0); }
	void setInt(int v) { assert(0); }
	void setString(const char *str) { m_str = str; }

	bool getBool() const  { return false;}
	void setBool(bool b) {; }

	short	binaryLength() const;
	char *  binary();
	void copy(const Value &o);

	Value *clone();
};

#endif // Value_h
 
