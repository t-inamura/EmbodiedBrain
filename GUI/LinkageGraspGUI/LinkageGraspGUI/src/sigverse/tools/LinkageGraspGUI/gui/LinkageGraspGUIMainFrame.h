﻿#ifndef SIGVERSE_LINKAGE_GRASP_GUI_MAIN_FRAME_H
#define SIGVERSE_LINKAGE_GRASP_GUI_MAIN_FRAME_H

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include <sigverse/tools/LinkageGraspGUI/AvatarController.h>
#include "LinkageGraspGUIFrame.h"

#include <wx/wxprec.h>
#include <wx/cmdline.h>

//// end generated include

/** Implementing MainFrame */
class LinkageGraspGUIMainFrame : public MainFrame
{
	protected:
		// Handlers for MainFrame events.
		void OnMainFrameClose( wxCloseEvent& event );
		void OnLimbModeChoice( wxCommandEvent& event );
		void OnGraspModeChoice( wxCommandEvent& event );
		void OnReverseModeChoice( wxCommandEvent& event );
		void OnChangeClick( wxCommandEvent& event );

		AvatarController avatarController;

	public:
		/** Constructor */
		LinkageGraspGUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv );
	//// end generated class members
	
};

#endif // SIGVERSE_LINKAGE_GRASP_GUI_MAIN_FRAME_H
