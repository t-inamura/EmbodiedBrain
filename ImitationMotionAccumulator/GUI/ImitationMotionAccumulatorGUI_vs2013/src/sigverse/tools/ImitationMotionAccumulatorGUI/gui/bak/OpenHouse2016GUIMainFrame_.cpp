#include <sigverse/tools/ImitationMotionAccumulatorGUI/Param.h>
#include <sigverse/tools/ImitationMotionAccumulatorGUI/gui/ImitationMotionAccumulatorGUIMainFrame.h>

ImitationMotionAccumulatorGUIMainFrame::ImitationMotionAccumulatorGUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv )
:
MainFrame( parent )
{
	std::cout << "test" << std::endl;

	std::string ipAddress = argv[1].ToStdString();
	int portNum = atoi(argv[2].ToStdString().c_str());

	/*
	 * Read parameter from configuration file.
	 */
	Param::readConfigFile();

	/*
	 * Connect to SIGServer.
	 */
	avatarController.connectSIGServer(ipAddress, portNum);

	/*
	 * Start checking received data from SIGService.
	 */
	avatarController.checkRecvSIGServiceData();
}

void ImitationMotionAccumulatorGUIMainFrame::OnMainFrameClose( wxCloseEvent& event )
{
	// Disconnect from all controllers.
	avatarController.disconnectFromAllController();

	Destroy();
}



void ImitationMotionAccumulatorGUIMainFrame::OnMirrorMirrorTherapyClick( wxCommandEvent& event )
{
	std::string reverseModeStr;

	int reverseModeNo = mm_choice_reverse_mode->GetSelection();

	switch (reverseModeNo)
	{
		case 0 : { reverseModeStr = "LEFTHAND";  break; }
		case 1 : { reverseModeStr = "RIGHTHAND"; break; }
		case 2 : { reverseModeStr = "NOREVERSE"; break; }
	}
	std::string delayTimeStr = mm_textCtrl_delay_time->GetValue();

	avatarController.sendMessageToController("ROOM:MirrorTherapy;AVATAR:mirror_therapy_man;REVERSE:" + reverseModeStr + ";DELAY:" + delayTimeStr + ";");
}

void ImitationMotionAccumulatorGUIMainFrame::OnMirrorLongArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("ROOM:MirrorTherapy;AVATAR:mirror_arm_long_man;");
}

void ImitationMotionAccumulatorGUIMainFrame::OnMirrorShortArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("ROOM:MirrorTherapy;AVATAR:mirror_arm_short_man;");
}

void ImitationMotionAccumulatorGUIMainFrame::OnMirrorRobotArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("ROOM:MirrorTherapy;AVATAR:mirror_right_arm_robot_man;");
}



void ImitationMotionAccumulatorGUIMainFrame::OnLinkageChangeClick( wxCommandEvent& event )
{
	// Limb mode
	std::string limbModeStr;

	int limbModeNo = lm_choice_limb_mode->GetSelection();

	switch (limbModeNo)
	{
		case 0 : { limbModeStr = "HAND"; break; }
		case 1 : { limbModeStr = "FOOT"; break; }
	}

	// Grasp mode
	std::string graspModeStr;

	int graspModeNo = lm_choice_grasp_mode->GetSelection();

	switch (graspModeNo)
	{
		case 0 : { graspModeStr = "SANDWICH";   break; }
		case 1 : { graspModeStr = "GRASP_RIGHT"; break; }
		case 2 : { graspModeStr = "GRASP_LEFT";  break; }
	}

	// Reverse mode
	std::string reverseModeStr;

	int reverseModeNo = lm_choice_reverse_mode->GetSelection();

	switch (reverseModeNo)
	{
		case 0 : { reverseModeStr = "LEFT";        break; }
		case 1 : { reverseModeStr = "RIGHT";       break; }
		case 2 : { reverseModeStr = "NOT_REVERSE"; break; }
	}

	// Fixed Waist
	std::string fixedWaistStr;

	int fixedWaistNo = lm_choice_fixed_waist->GetSelection();

	switch (fixedWaistNo)
	{
		case 0 : { fixedWaistStr = "false"; break; }
		case 1 : { fixedWaistStr = "true"; break; }
	}

	//Send message to SIGVerse controller.
	avatarController.sendMessageToController("ROOM:Linkage;AVATAR:linkage_man;LIMB_MODE:"+limbModeStr+";GRASP_MODE:"+graspModeStr+";REVERSE_MODE:"+reverseModeStr+";FIXED_WAIST:"+fixedWaistStr+";");
}



void ImitationMotionAccumulatorGUIMainFrame::OnSosMiddleArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("ROOM:SenseOfSelf;AVATAR:sos_arm_middle_man;");
}

void ImitationMotionAccumulatorGUIMainFrame::OnSosLongArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("ROOM:SenseOfSelf;AVATAR:sos_arm_long_man;");
}

void ImitationMotionAccumulatorGUIMainFrame::OnSosShortArmClick( wxCommandEvent& event )
{
	avatarController.sendMessageToController("ROOM:SenseOfSelf;AVATAR:sos_arm_short_man;");
}

void ImitationMotionAccumulatorGUIMainFrame::OnSosAddDeskHeightClick( wxCommandEvent& event )
{
	std::string dyStr = sm_textCtrl_desk_height_dy->GetValue();

	avatarController.sendMessageToController("ROOM:SenseOfSelf;DESK:"+dyStr+";");
}

void ImitationMotionAccumulatorGUIMainFrame::OnSosAddPlatePositionClick( wxCommandEvent& event )
{
	std::string dxStr = sm_textCtrl_plate_position_dx->GetValue();
	std::string dzStr = sm_textCtrl_plate_position_dz->GetValue();

	avatarController.sendMessageToController("ROOM:SenseOfSelf;PLATE:"+dxStr+","+dzStr+";");
}


