﻿#include <sigverse/tools/LinkageGraspGUI/Param.h>
#include <sigverse/tools/LinkageGraspGUI/gui/LinkageGraspGUIMainFrame.h>

LinkageGraspGUIMainFrame::LinkageGraspGUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv )
:
MainFrame( parent )
{
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

void LinkageGraspGUIMainFrame::OnMainFrameClose( wxCloseEvent& event )
{
	// Disconnect from all controllers.
	avatarController.disconnectFromAllController();

	Destroy();
}

void LinkageGraspGUIMainFrame::OnLimbModeChoice( wxCommandEvent& event )
{
	// No process.
}

void LinkageGraspGUIMainFrame::OnReverseModeChoice( wxCommandEvent& event )
{
	// No process.
}

void LinkageGraspGUIMainFrame::OnChangeClick( wxCommandEvent& event )
{
	std::string limbModeStr;

	int limbModeNo = m_choice_limb_mode->GetSelection();

	switch (limbModeNo)
	{
		case 0 : { limbModeStr = "HAND"; break; }
		case 1 : { limbModeStr = "FOOT"; break; }
	}

	std::string reverseModeStr;

	int reverseModeNo = m_choice_reverse_mode->GetSelection();

	switch (reverseModeNo)
	{
		case 0 : { reverseModeStr = "LEFT";  break; }
		case 1 : { reverseModeStr = "RIGHT"; break; }
	}

	//Send message to SIGVerse controller.
	avatarController.sendMessageToController("LIMB_MODE:"+limbModeStr+";REVERSE_MODE:"+reverseModeStr+";");
}
