/* $Id: Text.h,v 1.2 2011-03-31 08:15:57 okamoto Exp $ */ 
#ifndef Text_h
#define Text_h

#include "Encode.h"

#include <string>

/**
 * @brief テキストデータクラス
 *
 * シミュレーション世界内でやりとりされるテキストデータクラス
 *
 */
    
class Text
{
private:
	Encode m_encode;
	std::string m_text;
public:
	/**
	 * @brief コンストラクタ
	 * @param enc エンコード
	 * @param text テキスト文字列
	 */
	Text(Encode enc, const char *text) : m_encode(enc){
		if (text) {
			m_text = text;
		}
	}

	//! エンコードを取得する
	Encode getEncode() { return m_encode; }
	//! テキスト文字列を取得する
	const char *getString() { return m_text.length() > 0? m_text.c_str(): NULL; }
};


#endif // Text_h
 
