///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SENSEOFSELFGUIFRAME_H__
#define __SENSEOFSELFGUIFRAME_H__

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
#include <wx/textctrl.h>
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
		wxButton* m_button_middle_arm;
		wxButton* m_button_long_arm;
		wxButton* m_button_short_arm;
		wxButton* m_button_add_desk_height;
		wxStaticText* m_staticText_desk_height_dy;
		wxTextCtrl* m_textCtrl_desk_height_dy;
		wxButton* m_button_add_plate_position;
		wxStaticText* m_staticText_plate_position_dx_dz;
		wxTextCtrl* m_textCtrl_plate_position_dx;
		wxTextCtrl* m_textCtrl_plate_position_dz;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMainFrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMiddleArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLongArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShortArmClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddDeskHeightClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddPlatePositionClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sense Of Self Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif //__SENSEOFSELFGUIFRAME_H__
