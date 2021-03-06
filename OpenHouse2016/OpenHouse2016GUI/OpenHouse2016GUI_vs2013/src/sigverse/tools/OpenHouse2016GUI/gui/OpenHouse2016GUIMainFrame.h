#ifndef SIGVERSE_OpenHouse2016GUIMainFrame
#define SIGVERSE_OpenHouse2016GUIMainFrame

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include <sigverse/tools/OpenHouse2016GUI/AvatarController.h>
#include "OpenHouse2016GUIFrame.h"

#include <wx/wxprec.h>
#include <wx/cmdline.h>

//// end generated include

/** Implementing MainFrame */
class OpenHouse2016GUIMainFrame : public MainFrame
{
	protected:
		// Handlers for MainFrame events.
		void OnMainFrameClose( wxCloseEvent& event );
		void OnMirrorMirrorTherapyClick( wxCommandEvent& event );
		void OnMirrorLongArmClick( wxCommandEvent& event );
		void OnMirrorShortArmClick( wxCommandEvent& event );
		void OnMirrorRobotArmClick( wxCommandEvent& event );
		void OnLinkageChangeClick( wxCommandEvent& event );
		void OnSosMiddleArmClick( wxCommandEvent& event );
		void OnSosLongArmClick( wxCommandEvent& event );
		void OnSosShortArmClick( wxCommandEvent& event );
		void OnSosAddDeskHeightClick( wxCommandEvent& event );
		void OnSosAddPlatePositionClick( wxCommandEvent& event );

		AvatarController avatarController;
	public:
		/** Constructor */
		OpenHouse2016GUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv );
	//// end generated class members
};

#endif // SIGVERSE_OpenHouse2016GUIMainFrame
