/* $Id: RecvSound.h,v 1.2 2011-03-31 08:16:17 okamoto Exp $ */ 
#ifndef RecvSound_h
#define RecvSound_h

#include <string>
#include <string.h>

#include "Controller.h"

class RawSound;

/** 
 * @brief コントローラ onRecvSound イベントクラス
 */

class RecvSoundEvent : public ControllerEvent
{
private:
	typedef std::string S;
private:
	S		m_caller;
	RawSound *	m_sound;
	RawSound *	m_soundTmp;
	int		m_prevSeq;
	char *		m_curr;
private:
	void	free_();
public:
	RecvSoundEvent() :
		ControllerEvent(),
		m_sound(0), m_soundTmp(0),
		m_prevSeq(-1), m_curr(0) {;}
	~RecvSoundEvent() { free_(); }
	bool	set(int packetNum, int seq, char *data, int n);
	/**
	 * @brief 送信元エージェント名を取得する
	 */
	const char *getCaller() { return m_caller.c_str(); }
	/**
	 * @brief 音声データを取得する
	 */
	RawSound   *getRawSound() { return m_sound; }
	/**
	 * @brief 音声データを取得し、破棄責任を放棄させる
	 */
	RawSound   *releaseRawSound() {
		RawSound *tmp = m_sound;
		m_sound = 0;
		return tmp;
	}
};

#endif // RecvSound_h
 
