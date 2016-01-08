#include <sigverse/tools/SenseOfSelfGUI/Param.h>
#include <sigverse/tools/SenseOfSelfGUI/gui/SenseOfSelfGUIMainFrame.h>

/*
 * constructor
 */
SenseOfSelfGUIMainFrame::SenseOfSelfGUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv )
:
MainFrame( parent )
{
	std::string ipAddress = argv[1].ToStdString();
	int portNum = atoi(argv[2].ToStdString().c_str());

	/*
	 * パラメータファイル読み取り
	 */
	Param::readConfigFile();

	/*
	 * SIGServer への接続
	 */
	avatarController.connectSIGServer(ipAddress, portNum);

	/*
	 * SIGServiceからのデータチェック開始
	 */
	avatarController.checkRecvSIGServiceData();
}

/*
 * 画面が閉じられたときの処理
 */
void SenseOfSelfGUIMainFrame::OnMainFrameClose( wxCloseEvent &event )
{
	// 接続中の全コントローラと切断する
	avatarController.disconnectFromAllController();

	Destroy();
}

void SenseOfSelfGUIMainFrame::OnMirrorTherapyClick( wxCommandEvent& event )
{
	std::string reverseModeStr;

	int reverseModeNo = m_choice_reverse_mode->GetSelection();

	switch (reverseModeNo)
	{
		case 0 : { reverseModeStr = "LEFTHAND";  break; }
		case 1 : { reverseModeStr = "RIGHTHAND"; break; }
		case 2 : { reverseModeStr = "NOREVERSE"; break; }
	}
	std::string delayTimeStr = m_textCtrl_delay_time->GetValue();

	avatarController.sendMessageToController("AVATAR:mirror_therapy_man;REVERSE:"+reverseModeStr+";DELAY:"+delayTimeStr+";");
}

void SenseOfSelfGUIMainFrame::OnReverseModeChoice( wxCommandEvent& event )
{
// TODO: Implement OnReverseModeChoice
}

void SenseOfSelfGUIMainFrame::OnLongArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:right_arm_long_man;");
}

void SenseOfSelfGUIMainFrame::OnShortArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:right_arm_short_man;");
}

void SenseOfSelfGUIMainFrame::OnRobotArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:right_arm_robot_man;");
}
