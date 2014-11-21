/* $Id: ViewImageInfo.h,v 1.3 2011-09-16 03:54:26 okamoto Exp $ */ 
#ifndef ViewImageInfo_h
#define ViewImageInfo_h

#include <assert.h>

typedef unsigned short ImageDataType;

enum {
	IMAGE_DATA_TYPE_ANY = 0,
	IMAGE_DATA_WINDOWS_BMP,
};


typedef unsigned short ColorBitType;

enum {
	COLORBIT_ANY = 0,
	COLORBIT_24,
	DEPTHBIT_8,
};

enum ImageDataSize {
	IMAGE_320X240 = 0,
	IMAGE_320X1,
};

/**
 * @brief 画像データ情報クラス
 */
class ViewImageInfo
{
private:
	ImageDataType	m_dataType;
	ColorBitType	m_cbType;
	int		m_width;
	int		m_height;

public:
	/**
	 * @brief コンストラクタ
	 *
	 * @param dataType 画像データタイプ
	 * @param cbType   カラービットタイプ
	 * @param sz       画像サイズ
	 */
	ViewImageInfo(ImageDataType dataType, ColorBitType cbType, ImageDataSize sz) : m_dataType(dataType), m_cbType(cbType)
	{
		
		switch(sz) {
		case IMAGE_320X240:
			m_width = 320; m_height = 240;
			break;
		case IMAGE_320X1:
			m_width = 320; m_height = 1;
			break;
		default:
			assert(0);
			break;
		}
	}
	/**
	 * @brief コンストラクタ
	 *
	 * @param dataType 画像データタイプ
	 * @param cbType   カラービットタイプ
	 * @param w        幅(pixel)
	 * @param h        高さ(pixel)
	 */
	ViewImageInfo(ImageDataType dataType, ColorBitType cbType, int w, int h)
		: m_dataType(dataType), m_cbType(cbType), m_width(w), m_height(h)
	{
	}
	//! コピーコンストラクタ
	ViewImageInfo(const ViewImageInfo &o)
		: m_dataType(o.m_dataType),
		  m_cbType(o.m_cbType),
		  m_width(o.m_width), m_height(o.m_height)
	{
	}
	//! 画像データ形式(BMP etc...)を得る
	ImageDataType getDataType() const { return m_dataType; }
	//! カラータイプを得る
	ColorBitType  getColorBitType() const { return m_cbType; }
	//! 画像の幅(pixel)を得る
	int	      getWidth() const { return m_width; }
	//! 画像の高さ(pixel)を得る
	int	      getHeight() const { return m_height; }

	//! 1ピクセルあたりのデータサイズを得る
	int	getBytesPerOnePixel() const {
		int b;
		switch(m_cbType) {
			case COLORBIT_24:
				b = 3; break;
			case DEPTHBIT_8:
				b = 1; break;
			default:
				assert(0);	// error
				b = 0; break;
		}

		return b;
	}
};


#endif // ViewImageInfo_h
 
