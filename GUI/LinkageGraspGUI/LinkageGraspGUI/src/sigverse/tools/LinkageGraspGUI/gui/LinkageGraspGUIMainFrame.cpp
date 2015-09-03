#include <sigverse/tools/LinkageGraspGUI/Param.h>
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

void LinkageGraspGUIMainFrame::OnGraspModeChoice( wxCommandEvent& event )
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

	std::string graspModeStr;

	int graspModeNo = m_choice_grasp_mode->GetSelection();

	switch (graspModeNo)
	{
		case 0 : { graspModeStr = "SANDWICH";   break; }
		case 1 : { graspModeStr = "GRASP_RIGHT"; break; }
		case 2 : { graspModeStr = "GRASP_LEFT";  break; }
	}

	std::string reverseModeStr;

	int reverseModeNo = m_choice_reverse_mode->GetSelection();

	switch (reverseModeNo)
	{
		case 0 : { reverseModeStr = "LEFT";        break; }
		case 1 : { reverseModeStr = "RIGHT";       break; }
		case 2 : { reverseModeStr = "NOT_REVERSE"; break; }
	}

	//Send message to SIGVerse controller.
	avatarController.sendMessageToController("LIMB_MODE:"+limbModeStr+";GRASP_MODE:"+graspModeStr+";REVERSE_MODE:"+reverseModeStr+";");
}
