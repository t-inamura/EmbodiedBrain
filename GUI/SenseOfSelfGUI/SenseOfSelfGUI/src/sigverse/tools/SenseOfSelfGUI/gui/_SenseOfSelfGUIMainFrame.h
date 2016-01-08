﻿#ifndef SIGVERSE_SENSE_OF_SELF_GUI_GUI_MAIN_FRAME
#define SIGVERSE_SENSE_OF_SELF_GUI_GUI_MAIN_FRAME

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include <sigverse/tools/SenseOfSelfGUI/gui/SenseOfSelfGUIFrame.h>
#include <sigverse/tools/SenseOfSelfGUI/AvatarController.h>

#include <wx/wxprec.h>
#include <wx/cmdline.h>

//// end generated include

/** Implementing MainFrame */
class SenseOfSelfGUIMainFrame : public MainFrame
{
	protected:
		// Handlers for MainFrame events.
		void OnMainFrameClose( wxCloseEvent& event );
		void OnMirrorTherapyClick( wxCommandEvent& event );
		void OnReverseModeChoice( wxCommandEvent& event );
		void OnLongArmClick( wxCommandEvent& event );
		void OnShortArmClick( wxCommandEvent& event );
		void OnRobotArmClick( wxCommandEvent& event );

		AvatarController avatarController;

	public:
		/** Constructor */
		SenseOfSelfGUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv );
	//// end generated class members
	
};

#endif // SIGVERSE_SENSE_OF_SELF_GUI_GUI_MAIN_FRAME
