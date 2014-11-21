/* $Id: Attribute.h,v 1.2 2011-03-31 08:15:56 okamoto Exp $ */ 
#ifndef Attribute_h
#define Attribute_h

#include <string>
#include "Value.h"

/**
 * @brief 属性クラス
 *
 * エージェントの属性を表す
 */
   
class Attribute
{
	typedef std::string S;
private:
	S	m_name;
	Value * m_value;
	S	m_group;
	char  * m_buf;
	int	m_bufsize;
	int	m_voffset;
private:
	int	binarySize() const;
	void	free_();
public:
	/**
	 * @brief コンストラクタ
	 * @param n 属性名
	 * @param v 値
	 */
	Attribute(S n, Value *v) : m_name(n), m_value(v), m_buf(0), m_bufsize(0) {;}
	/**
	 * @brief コンストラクタ
	 * @param n 属性名
	 * @param v 属性値
	 * @param g 属性グループ名
	 */
	Attribute(S n, Value *v, S g) : m_name(n), m_value(v), m_group(g), m_buf(0), m_bufsize(0)  {;}
	/**
	 * @brief デフォルトコンストラクタ
	 */
	Attribute() : m_buf(0), m_bufsize(0), m_voffset(0) {;}
	/**
	 * @brief コピーコンストラクタ
	 */
	Attribute(const Attribute &o);
	
	/**
	 * @brief デストラクタ
	 */
	~Attribute() { free_(); }

	/**
	 * @brief 属性名を得る
	 */
	const char *name() const { return m_name.c_str(); }
	/**
	 * @brief 属性グループ名を得る
	 */
	const char *group() const { return m_group.c_str(); }
	/**
	 * @brief 属性値を得る
	 */
	Value & value() const { return *m_value; }

	/**
	 * @brief double型の属性値を得る
	 */
	double	getDouble() const { return value().getDouble(); }

	/**
	 * @brief オブジェクトの内容を文字列化する
	 */ 
	const char *toString();

	/**
	 * @brief オブジェクトの内容をバイナリ化する
	 * @param n バイナリデータサイズ
	 * @return バイナリデータ
	 */ 
	char *	toBinary(int &n);
	/**
	 * @brief バイナリデータから復元する
	 * @param data バイナリ化された属性データ
	 * @param n    バイナリデータサイズ
	 */
	int	setBinary(char *data, int n);
	/**
	 * @brief ダンプメソッド(デバッグ用)
	 */
	void	dump();
};

#endif // Attribute_h
 
