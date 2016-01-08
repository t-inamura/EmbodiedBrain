///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef SIGVERSE_SENSE_OF_SELF_GUI_GUI_FRAME
#define SIGVERSE_SENSE_OF_SELF_GUI_GUI_FRAME

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText_select_avatar;
		wxButton* m_button_mirror_therapy;
		wxStaticText* m_staticText_reverse_mode;
		wxChoice* m_choice_reverse_mode;
		wxStaticText* m_staticText_delay_time;
		wxTextCtrl* m_textCtrl_delay_time;
		wxButton* m_button_long_arm;
		wxButton* m_button_short_arm;
		wxButton* m_button_robot_arm;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMainFrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMirrorTherapyClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReverseModeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLongArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShortArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRobotArmClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Mirror Therapy Change Avatar Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif // SIGVERSE_SENSE_OF_SELF_GUI_GUI_FRAME
