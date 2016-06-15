#include "SenseOfSelfGUIMainFrame.h"
#include <sigverse/tools/SenseOfSelfGUI/Param.h>

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

void SenseOfSelfGUIMainFrame::OnMainFrameClose( wxCloseEvent& event )
{
	// 接続中の全コントローラと切断する
	avatarController.disconnectFromAllController();

	Destroy();
}

void SenseOfSelfGUIMainFrame::OnMiddleArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:arm_middle_man;");
}

void SenseOfSelfGUIMainFrame::OnLongArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:arm_long_man;");
}

void SenseOfSelfGUIMainFrame::OnShortArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("AVATAR:arm_short_man;");
}

void SenseOfSelfGUIMainFrame::OnAddDeskHeightClick( wxCommandEvent& event )
{
	std::string dyStr = m_textCtrl_desk_height_dy->GetValue();

	avatarController.sendMessageToController("DESK:"+dyStr+";");
}

void SenseOfSelfGUIMainFrame::OnAddPlatePositionClick( wxCommandEvent& event )
{
	std::string dxStr = m_textCtrl_plate_position_dx->GetValue();
	std::string dzStr = m_textCtrl_plate_position_dz->GetValue();

	avatarController.sendMessageToController("PLATE:"+dxStr+","+dzStr+";");
}
