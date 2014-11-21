/* $Id: ViewImage.h,v 1.5 2012-02-07 05:41:34 yahara Exp $ */ 
#ifndef ViewImage_h
#define ViewImage_h

#include <stdlib.h>

#include "ViewImageInfo.h"

/**
 * @brief 視覚データクラス
 */
class ViewImage
{
 private:
        ViewImageInfo	m_info;
	char *		m_buf;
	int		m_buflen;
	
	double m_fov;
	double m_ar;   //aspect ratio
public:
	/**
	 *  @brief コンストラクタ
	 *
	 *  @param info 視覚データ情報
	 */
	ViewImage(const ViewImageInfo &info)
		: m_info(info), m_buf(0), m_buflen(0)
	{
		m_buflen = calcBufferSize(info);
		m_buf = new char [m_buflen];
	}

	//! デストラクタ
	~ViewImage() {
		if (m_buf) {
			delete [] m_buf; m_buf = 0;
		}
	}

private:
	// sekikawa(2007/10/12)
	int getWidthBytes(int width, int bytesPerOnePixel);

	int	calcBufferSize(const ViewImageInfo &info);

public:
	//! 視覚データ情報の取得
	const ViewImageInfo & getInfo() { return m_info; }

	//! 画像の幅(pixel)の取得
	int  getWidth() const { return m_info.getWidth(); }
	//! 画像の高さ(pixel)の取得
	int  getHeight() const { return m_info.getHeight(); }
	//! 画像データの取得
	char * getBuffer() const { return m_buf; }
	//! 画像データ設定
	void setBuffer(char *buf) { m_buf = buf; }
	//! 画像データ長の取得
	int   getBufferLength() const { return m_buflen; }
	//! 視野角設定
	void setFOVy(double fov){ m_fov = fov;} 
	//! アスペクト比設定
	void setAspectRatio(double ar){ m_ar = ar;}
	//! 視野角取得
	double getFOVy(){ return m_fov;} 
	//! アスペクト比取得
	double getAspectRatio(){ return m_ar;}

	//virtual void setDimension(int n){} 

	/**
	 * @brief 画像データを Windows ビットマップ形式で保存する
	 *
	 * @param fname 保存先ファイル名
	 */
	
	bool	saveAsWindowsBMP(const char *fname);

#ifdef WIN32
	// sekikawa(2007/10/12)
	// convert RGBA format to BGR and turn y-axis upside down
	void setBitImageAsWindowsBMP(unsigned char *bitImage);
#endif

#if (defined _DEBUG || defined UNIT_TEST || defined IRWAS_TEST_CLIENT)
	static ViewImage *createSample();
#endif
};

/*
class DepthImage : public ViewImage
{
 public:
 DepthImage(const ViewImageInfo &info)
   : ViewImage(info)
  {
  }
  //! ピクセルの位置からdepthデータ取得
  unsigned char getDepthFromPixel(int w, int h=0);

  //! ピクセルの位置から距離データ取得
  double getDistanceFromPixel(int w, int h=0);

  //! 方向（角度）から距離データ取得
  unsigned char getDepthFromAngle(double theta, double phi=0);

  void setDimension(int dim){m_dim = dim;}
 private:

  //dimension
  int m_dim;

};
*/
#endif // ViewImage_h
 
