///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef SIGVERSE_OPEN_HOUSE_2016_GUI_FRAME
#define SIGVERSE_OPEN_HOUSE_2016_GUI_FRAME

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* mm_staticText_mirror_therapy;
		wxButton* mm_button_mirror_therapy;
		wxStaticText* mm_staticText_reverse_mode;
		wxChoice* mm_choice_reverse_mode;
		wxStaticText* mm_staticText_delay_time;
		wxTextCtrl* mm_textCtrl_delay_time;
		wxButton* mm_button_long_arm;
		wxButton* mm_button_short_arm;
		wxButton* mm_button_robot_arm;
		wxStaticLine* m_staticline1;
		wxStaticText* lm_staticText_linkage_controller;
		wxButton* lm_button_change;
		wxStaticText* lm_staticText_limb_mode;
		wxChoice* lm_choice_limb_mode;
		wxStaticText* lm_staticText_grasp_mode;
		wxChoice* lm_choice_grasp_mode;
		wxStaticText* lm_staticText_reverse_mode;
		wxChoice* lm_choice_reverse_mode;
		wxStaticText* lm_staticText_fixed_waist;
		wxChoice* lm_choice_fixed_waist;
		wxStaticLine* m_staticline2;
		wxStaticText* sm_staticText_sense_of_self;
		wxButton* sm_button_middle_arm;
		wxButton* sm_button_long_arm1;
		wxButton* sm_button_short_arm1;
		wxButton* sm_button_add_desk_height;
		wxStaticText* sm_staticText_desk_height_dy;
		wxTextCtrl* sm_textCtrl_desk_height_dy;
		wxButton* sm_button_add_plate_position;
		wxStaticText* sm_staticText_plate_position_dx_dz;
		wxTextCtrl* sm_textCtrl_plate_position_dx;
		wxTextCtrl* sm_textCtrl_plate_position_dz;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMainFrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMirrorMirrorTherapyClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMirrorLongArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMirrorShortArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMirrorRobotArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLinkageChangeClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSosMiddleArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSosLongArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSosShortArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSosAddDeskHeightClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSosAddPlatePositionClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Open House 2016 Change Avatar Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 718,775 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif //SIGVERSE_OPEN_HOUSE_2016_GUI_FRAME
