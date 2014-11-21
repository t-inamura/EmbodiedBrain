/* $Id: RawSound.h,v 1.2 2011-03-31 08:15:57 okamoto Exp $ */ 
#ifndef RawSound_h
#define RawSound_h

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

enum {
	RAW_SOUND_LITTLE_ENDIAN = 0,
	RAW_SOUND_BIG_ENDIAN,
};

typedef unsigned short RawSoundEndian;

//! 音声データヘッダクラス
class RawSoundHeader
{
private:
	int 		m_channels;
	unsigned 	m_samplingRate;		// [Hz]
	unsigned 	m_bitsPerSample;	// Byte/sec
	RawSoundEndian  m_endian;
public:
	/**
	 * @brief コンストラクタ
	 *
	 * @param channels     チャンネル数(モノラル:1,ステレオ:2)
	 * @param samplingRate サンプリングレート
	 * @param bitsPerSample サンプルあたりビット数
	 * @param endian        データエンディアン
	 */
	RawSoundHeader(int channels, unsigned samplingRate, unsigned bitsPerSample, RawSoundEndian endian) :
		m_channels(channels), m_samplingRate(samplingRate),
		m_bitsPerSample(bitsPerSample), m_endian(endian) {;}

	//! コンストラクタ
	RawSoundHeader() : m_channels(0), m_samplingRate(0), m_bitsPerSample(0), m_endian(0) {}

	//! チャンネル数を得る
	int		getChannelNum() { return m_channels; }
	//! サンプリングレートを得る
	unsigned 	getSamplingRate() { return m_samplingRate; }
	//! サンプルあたりのビット数を得る
	unsigned	getBitPerSample() { return m_bitsPerSample; }
	//! データエンディアンを得る
	RawSoundEndian  getEndian() { return m_endian; }
};

//! 音声データクラス
class RawSound
{
private:
	RawSoundHeader	m_header;
	int		m_datalen;
	char *		m_data;	
public:
	/**
	 * @brief コンストラクタ
	 *
	 * @param h 音声データヘッダ
	 * @param datalen 音声データ長
	 *
	 */
	RawSound(RawSoundHeader &h, int datalen)
		: m_header(h), m_datalen(datalen)
	{
#ifdef WIN32
	    m_data = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (DWORD)datalen);
#else
		m_data = new char[datalen];
#endif
	}

	//! コンストラクタ
	RawSound() : m_datalen(0), m_data(NULL) {}
	//! デストラクタ
	~RawSound()	{ freeWaveData(); }

	//! 音声データヘッダの取得
	RawSoundHeader &getHeader() { return m_header; }

	//! 音声データ長の取得
	int		getDataLen() { return m_datalen; }
	//! 音声データの取得
	char *		getData() { return m_data; }

	/**
	 * @brief WAV形式ファイルをロードする
	 * @param waveFile WAVファイル名
	 */
	bool loadWaveFile(const char *waveFile);
	/**
	 * @brief WAV形式データをロードする
	 * @param data    WAVデータ
	 * @param datalen WAVデータ長
	 */
	bool loadWaveData(char *data, int datalen);
	//! 再生する
	bool play();
	//! WAVデータメモリを解放する
	void freeWaveData();
};

#endif // RawSound_h
