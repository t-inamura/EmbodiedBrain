///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __IMITATIONMOTIONACCUMULATORGUIFRAME_H__
#define __IMITATIONMOTIONACCUMULATORGUIFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxButton* mm_button_update_config;
		wxStaticLine* m_staticline1;
		wxStaticText* mm_staticText_imitation;
		wxRadioBox* m_radioBox_mode;
		wxStaticText* mm_staticText_rec_id;
		wxTextCtrl* mm_textCtrl_rec_id;
		wxStaticText* mm_staticText_user_id;
		wxTextCtrl* mm_textCtrl_user_id;
		wxStaticText* mm_staticText_origin_max_time;
		wxTextCtrl* mm_textCtrl_origin_max_time;
		wxStaticText* mm_staticText_imitation_group_id;
		wxTextCtrl* mm_textCtrl_imitation_group_id;
		wxStaticText* mm_staticText_imitation_rec_type;
		wxTextCtrl* imitation_rec_type;
		wxStaticText* mm_staticText_imitation_origin_rec_id;
		wxTextCtrl* imitation_imitation_origin_rec_id;
		wxStaticText* mm_staticText_database_parameter;
		wxStaticText* mm_staticText_db_perception_neuron_memo1;
		wxTextCtrl* mm_textCtrl_db_perception_neuron_memo;
		wxStaticText* mm_staticText_db_imitation_condition_pulse_power;
		wxTextCtrl* mm_textCtrl_db_imitation_condition_pulse_power;
		wxStaticText* mm_staticText_db_imitation_condition_pulse_frequency;
		wxTextCtrl* mm_textCtrl_db_imitation_condition_pulse_frequency;
		wxStaticText* mm_staticText_db_imitation_condition_pulse_duration;
		wxTextCtrl* mm_textCtrl_db_imitation_condition_pulse_duration;
		wxStaticText* mm_staticText_db_imitation_condition_pulse_interval;
		wxTextCtrl* mm_textCtrl_db_imitation_condition_pulse_interval;
		wxStaticText* mm_staticText_db_imitation_condition_pulse_number;
		wxTextCtrl* mm_textCtrl_db_imitation_condition_pulse_number;
		wxStaticText* mm_staticText_db_imitation_memo;
		wxTextCtrl* mm_textCtrl_db_imitation_memo;
		wxStaticText* mm_staticText_accum_interval;
		wxTextCtrl* mm_textCtrl_accum_interval;
		wxStaticText* mm_staticText_motion_data_file_path;
		wxTextCtrl* mm_textCtrl_motion_data_file_path;
		wxStaticLine* m_staticline2;
		wxStaticText* lm_staticText_database;
		wxStaticText* sm_staticText_perception_neuron;
		wxStaticText* sm_staticText_sigverse;
		wxStaticText* mm_staticText_host;
		wxTextCtrl* mm_textCtrl_host;
		wxStaticText* mm_staticText_port;
		wxTextCtrl* mm_textCtrl_port;
		wxStaticText* mm_staticText_schema;
		wxTextCtrl* mm_textCtrl_schema;
		wxStaticText* mm_staticText_user;
		wxTextCtrl* mm_textCtrl_user;
		wxStaticText* mm_staticText_pass;
		wxTextCtrl* mm_textCtrl_pass;
		wxStaticText* mm_staticText_data_type;
		wxChoice* mm_choice_data_type;
		wxStaticText* mm_staticText_bvh_ip_address;
		wxTextCtrl* mm_textCtrl_bvh_ip_address;
		wxStaticText* mm_staticText_bvh_port;
		wxTextCtrl* mm_textCtrl_bvh_port;
		wxStaticText* mm_staticText_calc_ip_address;
		wxTextCtrl* mm_textCtrl_calc_ip_address;
		wxStaticText* mm_staticText_calc_port;
		wxTextCtrl* mm_textCtrl_calc_port;
		wxStaticText* mm_staticText_avatar_disp_interval;
		wxTextCtrl* mm_textCtrl_avatar_disp_interval;
		wxStaticText* mm_staticText_service_name;
		wxTextCtrl* mm_textCtrl_service_name;
		wxStaticText* mm_staticText_device_type;
		wxTextCtrl* mm_textCtrl_device_type;
		wxStaticText* mm_staticText_device_unique_id;
		wxTextCtrl* mm_textCtrl_device_unique_id;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMainFrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMirrorMirrorTherapyClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImitationModeSelect( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Imitation Motion Accumulator Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1044,825 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif //__IMITATIONMOTIONACCUMULATORGUIFRAME_H__
