﻿#include <sigverse/tools/LinkageGraspGUI/Param.h>
#include <sigverse/tools/LinkageGraspGUI/gui/LinkageGraspGUIMainFrame.h>

/*
 * constructor
 */
LinkageGraspGUIMainFrame::LinkageGraspGUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv )
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
void LinkageGraspGUIMainFrame::OnMainFrameClose( wxCloseEvent &event )
{
	// 接続中の全コントローラと切断する
	avatarController.disconnectFromAllController();

	Destroy();
}

void LinkageGraspGUIMainFrame::OnLinkageGraspClick( wxCommandEvent& event )
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

void LinkageGraspGUIMainFrame::OnReverseModeChoice( wxCommandEvent& event )
{
// TODO: Implement OnReverseModeChoice
}

void LinkageGraspGUIMainFrame::OnLongArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:right_arm_long_man;");
}

void LinkageGraspGUIMainFrame::OnShortArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:right_arm_short_man;");
}

void LinkageGraspGUIMainFrame::OnRobotArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:right_arm_robot_man;");
}
