/* $Id: Controller.h,v 1.1.1.1 2011-03-25 02:18:50 okamoto Exp $ */ 
#ifndef CommController_Controller_h
#define CommController_Controller_h

/**
 * @brief コントローライベントクラス
 */
class ControllerEvent
{
protected:
	double m_currTime; //!< 現在のシミュレーション時間

public:
	//! 現在のシミュレーション時間を得る
	double	time() { return m_currTime; }

	//! 現在のシミュレーション時間を設定する
	void setTime(double time) { m_currTime = time; } 
	
};

#endif // CommController_Controller_h
 
