///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef SIGVERSE_LINKAGE_GRASP_GUI_FRAME_H
#define SIGVERSE_LINKAGE_GRASP_GUI_FRAME_H

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText_change_mode;
		wxStaticText* m_staticText_limb_mode;
		wxChoice* m_choice_limb_mode;
		wxStaticText* m_staticText_grasp_mode;
		wxChoice* m_choice_grasp_mode;
		wxStaticText* m_staticText_reverse_mode;
		wxChoice* m_choice_reverse_mode;
		wxButton* m_button_change;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMainFrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnLimbModeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGraspModeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReverseModeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChangeClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Linkage Grasp Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,203 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif //SIGVERSE_LINKAGE_GRASP_GUI_FRAME_H
