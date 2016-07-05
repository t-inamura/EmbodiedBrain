///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ImitationMotionAccumulatorGUIFrame.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,-1 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 218, 248, 233 ) );
	
	wxBoxSizer* bSizerTop;
	bSizerTop = new wxBoxSizer( wxVERTICAL );
	
	mm_button_update_config = new wxButton( this, wxID_ANY, wxT("Update Config file !"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizerTop->Add( mm_button_update_config, 0, wxALIGN_CENTER|wxALL, 10 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerTop->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_imitation = new wxStaticText( this, wxID_ANY, wxT("Imitation"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_imitation->Wrap( -1 );
	mm_staticText_imitation->SetFont( wxFont( 15, 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer3->Add( mm_staticText_imitation, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxString m_radioBox_modeChoices[] = { wxT("RecOrigin"), wxT("Experiment") };
	int m_radioBox_modeNChoices = sizeof( m_radioBox_modeChoices ) / sizeof( wxString );
	m_radioBox_mode = new wxRadioBox( this, wxID_ANY, wxT("mode"), wxDefaultPosition, wxDefaultSize, m_radioBox_modeNChoices, m_radioBox_modeChoices, 1, wxRA_SPECIFY_COLS );
	m_radioBox_mode->SetSelection( 0 );
	fgSizer1->Add( m_radioBox_mode, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_rec_id = new wxStaticText( this, wxID_ANY, wxT("rec_id"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_rec_id->Wrap( -1 );
	fgSizer11->Add( mm_staticText_rec_id, 0, wxALL, 5 );
	
	mm_textCtrl_rec_id = new wxTextCtrl( this, wxID_ANY, wxT("10001"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_rec_id->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer11->Add( mm_textCtrl_rec_id, 0, wxALL, 5 );
	
	mm_staticText_user_id = new wxStaticText( this, wxID_ANY, wxT("user_id"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_user_id->Wrap( -1 );
	fgSizer11->Add( mm_staticText_user_id, 0, wxALL, 5 );
	
	mm_textCtrl_user_id = new wxTextCtrl( this, wxID_ANY, wxT("901"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_user_id->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer11->Add( mm_textCtrl_user_id, 0, wxALL, 5 );
	
	mm_staticText_origin_max_time = new wxStaticText( this, wxID_ANY, wxT("origin_max_time"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_origin_max_time->Wrap( -1 );
	fgSizer11->Add( mm_staticText_origin_max_time, 0, wxALL, 5 );
	
	mm_textCtrl_origin_max_time = new wxTextCtrl( this, wxID_ANY, wxT("999"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_origin_max_time->Enable( true );
	mm_textCtrl_origin_max_time->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer11->Add( mm_textCtrl_origin_max_time, 0, wxALL, 5 );
	
	mm_staticText_imitation_group_id = new wxStaticText( this, wxID_ANY, wxT("imitation_group_id"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_imitation_group_id->Wrap( -1 );
	fgSizer11->Add( mm_staticText_imitation_group_id, 0, wxALL, 5 );
	
	mm_textCtrl_imitation_group_id = new wxTextCtrl( this, wxID_ANY, wxT("201"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_imitation_group_id->Enable( false );
	mm_textCtrl_imitation_group_id->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer11->Add( mm_textCtrl_imitation_group_id, 0, wxALL, 5 );
	
	mm_staticText_imitation_rec_type = new wxStaticText( this, wxID_ANY, wxT("imitation_rec_type"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_imitation_rec_type->Wrap( -1 );
	fgSizer11->Add( mm_staticText_imitation_rec_type, 0, wxALL, 5 );
	
	imitation_rec_type = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	imitation_rec_type->Enable( false );
	imitation_rec_type->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer11->Add( imitation_rec_type, 0, wxALL, 5 );
	
	mm_staticText_imitation_origin_rec_id = new wxStaticText( this, wxID_ANY, wxT("imitation_origin_rec_id"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_imitation_origin_rec_id->Wrap( -1 );
	fgSizer11->Add( mm_staticText_imitation_origin_rec_id, 0, wxALL, 5 );
	
	imitation_imitation_origin_rec_id = new wxTextCtrl( this, wxID_ANY, wxT("10001"), wxDefaultPosition, wxDefaultSize, 0 );
	imitation_imitation_origin_rec_id->Enable( false );
	imitation_imitation_origin_rec_id->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer11->Add( imitation_imitation_origin_rec_id, 0, wxALL, 5 );
	
	
	fgSizer1->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer121;
	fgSizer121 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer121->SetFlexibleDirection( wxBOTH );
	fgSizer121->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_database_parameter = new wxStaticText( this, wxID_ANY, wxT("Database Parameter"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_database_parameter->Wrap( -1 );
	fgSizer121->Add( mm_staticText_database_parameter, 0, wxALIGN_RIGHT, 5 );
	
	
	fgSizer121->Add( 0, 0, 1, wxEXPAND, 5 );
	
	mm_staticText_db_perception_neuron_memo1 = new wxStaticText( this, wxID_ANY, wxT("db_perception_neuron_memo"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_db_perception_neuron_memo1->Wrap( -1 );
	fgSizer121->Add( mm_staticText_db_perception_neuron_memo1, 0, wxALL, 5 );
	
	mm_textCtrl_db_perception_neuron_memo = new wxTextCtrl( this, wxID_ANY, wxT("PNmemo"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_db_perception_neuron_memo->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer121->Add( mm_textCtrl_db_perception_neuron_memo, 0, wxALL, 5 );
	
	mm_staticText_db_imitation_condition_pulse_power = new wxStaticText( this, wxID_ANY, wxT("db_imitation_condition_pulse_power"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_db_imitation_condition_pulse_power->Wrap( -1 );
	fgSizer121->Add( mm_staticText_db_imitation_condition_pulse_power, 0, wxALL, 5 );
	
	mm_textCtrl_db_imitation_condition_pulse_power = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_db_imitation_condition_pulse_power->Enable( false );
	mm_textCtrl_db_imitation_condition_pulse_power->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer121->Add( mm_textCtrl_db_imitation_condition_pulse_power, 0, wxALL, 5 );
	
	mm_staticText_db_imitation_condition_pulse_frequency = new wxStaticText( this, wxID_ANY, wxT("db_imitation_condition_pulse_frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_db_imitation_condition_pulse_frequency->Wrap( -1 );
	fgSizer121->Add( mm_staticText_db_imitation_condition_pulse_frequency, 0, wxALL, 5 );
	
	mm_textCtrl_db_imitation_condition_pulse_frequency = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_db_imitation_condition_pulse_frequency->Enable( false );
	mm_textCtrl_db_imitation_condition_pulse_frequency->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer121->Add( mm_textCtrl_db_imitation_condition_pulse_frequency, 0, wxALL, 5 );
	
	mm_staticText_db_imitation_condition_pulse_duration = new wxStaticText( this, wxID_ANY, wxT("db_imitation_condition_pulse_duration"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_db_imitation_condition_pulse_duration->Wrap( -1 );
	fgSizer121->Add( mm_staticText_db_imitation_condition_pulse_duration, 0, wxALL, 5 );
	
	mm_textCtrl_db_imitation_condition_pulse_duration = new wxTextCtrl( this, wxID_ANY, wxT("30"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_db_imitation_condition_pulse_duration->Enable( false );
	mm_textCtrl_db_imitation_condition_pulse_duration->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer121->Add( mm_textCtrl_db_imitation_condition_pulse_duration, 0, wxALL, 5 );
	
	mm_staticText_db_imitation_condition_pulse_interval = new wxStaticText( this, wxID_ANY, wxT("db_imitation_condition_pulse_interval"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_db_imitation_condition_pulse_interval->Wrap( -1 );
	fgSizer121->Add( mm_staticText_db_imitation_condition_pulse_interval, 0, wxALL, 5 );
	
	mm_textCtrl_db_imitation_condition_pulse_interval = new wxTextCtrl( this, wxID_ANY, wxT("40"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_db_imitation_condition_pulse_interval->Enable( false );
	mm_textCtrl_db_imitation_condition_pulse_interval->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer121->Add( mm_textCtrl_db_imitation_condition_pulse_interval, 0, wxALL, 5 );
	
	mm_staticText_db_imitation_condition_pulse_number = new wxStaticText( this, wxID_ANY, wxT("db_imitation_condition_pulse_number"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_db_imitation_condition_pulse_number->Wrap( -1 );
	fgSizer121->Add( mm_staticText_db_imitation_condition_pulse_number, 0, wxALL, 5 );
	
	mm_textCtrl_db_imitation_condition_pulse_number = new wxTextCtrl( this, wxID_ANY, wxT("50"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_db_imitation_condition_pulse_number->Enable( false );
	mm_textCtrl_db_imitation_condition_pulse_number->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer121->Add( mm_textCtrl_db_imitation_condition_pulse_number, 0, wxALL, 5 );
	
	mm_staticText_db_imitation_memo = new wxStaticText( this, wxID_ANY, wxT("db_imitation_memo"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_db_imitation_memo->Wrap( -1 );
	fgSizer121->Add( mm_staticText_db_imitation_memo, 0, wxALL, 5 );
	
	mm_textCtrl_db_imitation_memo = new wxTextCtrl( this, wxID_ANY, wxT("IMmemo"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_db_imitation_memo->Enable( false );
	mm_textCtrl_db_imitation_memo->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer121->Add( mm_textCtrl_db_imitation_memo, 0, wxALL, 5 );
	
	
	fgSizer1->Add( fgSizer121, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( fgSizer1, 1, wxALL, 10 );
	
	wxFlexGridSizer* fgSizer111;
	fgSizer111 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer111->SetFlexibleDirection( wxBOTH );
	fgSizer111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_accum_interval = new wxStaticText( this, wxID_ANY, wxT("accum_interval"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_accum_interval->Wrap( -1 );
	fgSizer111->Add( mm_staticText_accum_interval, 0, wxALL, 5 );
	
	mm_textCtrl_accum_interval = new wxTextCtrl( this, wxID_ANY, wxT("30"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_accum_interval->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer111->Add( mm_textCtrl_accum_interval, 0, wxALL, 5 );
	
	mm_staticText_motion_data_file_path = new wxStaticText( this, wxID_ANY, wxT("motion_data_file_path"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_motion_data_file_path->Wrap( -1 );
	fgSizer111->Add( mm_staticText_motion_data_file_path, 0, wxALL, 5 );
	
	mm_textCtrl_motion_data_file_path = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_motion_data_file_path->SetMinSize( wxSize( 600,-1 ) );
	
	fgSizer111->Add( mm_textCtrl_motion_data_file_path, 0, wxALL, 5 );
	
	
	fgSizer3->Add( fgSizer111, 1, wxEXPAND, 5 );
	
	
	bSizerTop->Add( fgSizer3, 1, wxALL, 10 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerTop->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	lm_staticText_database = new wxStaticText( this, wxID_ANY, wxT("Database"), wxDefaultPosition, wxDefaultSize, 0 );
	lm_staticText_database->Wrap( -1 );
	lm_staticText_database->SetFont( wxFont( 15, 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer2->Add( lm_staticText_database, 0, wxALL, 5 );
	
	sm_staticText_perception_neuron = new wxStaticText( this, wxID_ANY, wxT("Perception Neuron"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_staticText_perception_neuron->Wrap( -1 );
	sm_staticText_perception_neuron->SetFont( wxFont( 15, 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer2->Add( sm_staticText_perception_neuron, 0, wxALL, 5 );
	
	sm_staticText_sigverse = new wxStaticText( this, wxID_ANY, wxT("SIGVerse"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_staticText_sigverse->Wrap( -1 );
	sm_staticText_sigverse->SetFont( wxFont( 15, 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer2->Add( sm_staticText_sigverse, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_host = new wxStaticText( this, wxID_ANY, wxT("host"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_host->Wrap( -1 );
	fgSizer21->Add( mm_staticText_host, 0, wxALL, 5 );
	
	mm_textCtrl_host = new wxTextCtrl( this, wxID_ANY, wxT("socio2.iir.nii.ac.jp"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_host->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer21->Add( mm_textCtrl_host, 0, wxALL, 5 );
	
	mm_staticText_port = new wxStaticText( this, wxID_ANY, wxT("port"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_port->Wrap( -1 );
	fgSizer21->Add( mm_staticText_port, 0, wxALL, 5 );
	
	mm_textCtrl_port = new wxTextCtrl( this, wxID_ANY, wxT("3306"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_port->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer21->Add( mm_textCtrl_port, 0, wxALL, 5 );
	
	mm_staticText_schema = new wxStaticText( this, wxID_ANY, wxT("schema"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_schema->Wrap( -1 );
	fgSizer21->Add( mm_staticText_schema, 0, wxALL, 5 );
	
	mm_textCtrl_schema = new wxTextCtrl( this, wxID_ANY, wxT("PMS"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_schema->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer21->Add( mm_textCtrl_schema, 0, wxALL, 5 );
	
	mm_staticText_user = new wxStaticText( this, wxID_ANY, wxT("user"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_user->Wrap( -1 );
	fgSizer21->Add( mm_staticText_user, 0, wxALL, 5 );
	
	mm_textCtrl_user = new wxTextCtrl( this, wxID_ANY, wxT("pmsnossl"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_user->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer21->Add( mm_textCtrl_user, 0, wxALL, 5 );
	
	mm_staticText_pass = new wxStaticText( this, wxID_ANY, wxT("pass"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_pass->Wrap( -1 );
	fgSizer21->Add( mm_staticText_pass, 0, wxALL, 5 );
	
	mm_textCtrl_pass = new wxTextCtrl( this, wxID_ANY, wxT("pms2630"), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	mm_textCtrl_pass->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer21->Add( mm_textCtrl_pass, 0, wxALL, 5 );
	
	
	fgSizer2->Add( fgSizer21, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer31->SetFlexibleDirection( wxBOTH );
	fgSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_data_type = new wxStaticText( this, wxID_ANY, wxT("data_type"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_data_type->Wrap( -1 );
	fgSizer31->Add( mm_staticText_data_type, 0, wxALL, 5 );
	
	wxString mm_choice_data_typeChoices[] = { wxT("BVH"), wxT("CALC") };
	int mm_choice_data_typeNChoices = sizeof( mm_choice_data_typeChoices ) / sizeof( wxString );
	mm_choice_data_type = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mm_choice_data_typeNChoices, mm_choice_data_typeChoices, 0 );
	mm_choice_data_type->SetSelection( 0 );
	mm_choice_data_type->Enable( false );
	
	fgSizer31->Add( mm_choice_data_type, 0, wxALL, 5 );
	
	mm_staticText_bvh_ip_address = new wxStaticText( this, wxID_ANY, wxT("bvh_ip_address"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_bvh_ip_address->Wrap( -1 );
	fgSizer31->Add( mm_staticText_bvh_ip_address, 0, wxALL, 5 );
	
	mm_textCtrl_bvh_ip_address = new wxTextCtrl( this, wxID_ANY, wxT("127.0.0.1"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_bvh_ip_address->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer31->Add( mm_textCtrl_bvh_ip_address, 0, wxALL, 5 );
	
	mm_staticText_bvh_port = new wxStaticText( this, wxID_ANY, wxT("bvh_port"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_bvh_port->Wrap( -1 );
	fgSizer31->Add( mm_staticText_bvh_port, 0, wxALL, 5 );
	
	mm_textCtrl_bvh_port = new wxTextCtrl( this, wxID_ANY, wxT("7001"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_bvh_port->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer31->Add( mm_textCtrl_bvh_port, 0, wxALL, 5 );
	
	mm_staticText_calc_ip_address = new wxStaticText( this, wxID_ANY, wxT("calc_ip_address"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_calc_ip_address->Wrap( -1 );
	fgSizer31->Add( mm_staticText_calc_ip_address, 0, wxALL, 5 );
	
	mm_textCtrl_calc_ip_address = new wxTextCtrl( this, wxID_ANY, wxT("127.0.0.1"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	mm_textCtrl_calc_ip_address->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer31->Add( mm_textCtrl_calc_ip_address, 0, wxALL, 5 );
	
	mm_staticText_calc_port = new wxStaticText( this, wxID_ANY, wxT("calc_port"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_calc_port->Wrap( -1 );
	fgSizer31->Add( mm_staticText_calc_port, 0, wxALL, 5 );
	
	mm_textCtrl_calc_port = new wxTextCtrl( this, wxID_ANY, wxT("7012"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	mm_textCtrl_calc_port->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer31->Add( mm_textCtrl_calc_port, 0, wxALL, 5 );
	
	
	fgSizer2->Add( fgSizer31, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer41;
	fgSizer41 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer41->SetFlexibleDirection( wxBOTH );
	fgSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_avatar_disp_interval = new wxStaticText( this, wxID_ANY, wxT("avatar_disp_interval [ms]"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_avatar_disp_interval->Wrap( -1 );
	fgSizer41->Add( mm_staticText_avatar_disp_interval, 0, wxALL, 5 );
	
	mm_textCtrl_avatar_disp_interval = new wxTextCtrl( this, wxID_ANY, wxT("33"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_avatar_disp_interval->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer41->Add( mm_textCtrl_avatar_disp_interval, 0, wxALL, 5 );
	
	mm_staticText_service_name = new wxStaticText( this, wxID_ANY, wxT("service_name"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_service_name->Wrap( -1 );
	fgSizer41->Add( mm_staticText_service_name, 0, wxALL, 5 );
	
	mm_textCtrl_service_name = new wxTextCtrl( this, wxID_ANY, wxT("SVC_PERCEPTION_NEURON"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_service_name->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer41->Add( mm_textCtrl_service_name, 0, wxALL, 5 );
	
	mm_staticText_device_type = new wxStaticText( this, wxID_ANY, wxT("device_type"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_device_type->Wrap( -1 );
	fgSizer41->Add( mm_staticText_device_type, 0, wxALL, 5 );
	
	mm_textCtrl_device_type = new wxTextCtrl( this, wxID_ANY, wxT("PERCEPTION_NEURON"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_device_type->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer41->Add( mm_textCtrl_device_type, 0, wxALL, 5 );
	
	mm_staticText_device_unique_id = new wxStaticText( this, wxID_ANY, wxT("device_unique_id"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_device_unique_id->Wrap( -1 );
	fgSizer41->Add( mm_staticText_device_unique_id, 0, wxALL, 5 );
	
	mm_textCtrl_device_unique_id = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_textCtrl_device_unique_id->SetMinSize( wxSize( 200,-1 ) );
	
	fgSizer41->Add( mm_textCtrl_device_unique_id, 0, wxALL, 5 );
	
	
	fgSizer2->Add( fgSizer41, 1, wxEXPAND, 5 );
	
	
	bSizerTop->Add( fgSizer2, 1, wxALL, 10 );
	
	
	this->SetSizer( bSizerTop );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	mm_button_update_config->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorMirrorTherapyClick ), NULL, this );
	m_radioBox_mode->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( MainFrame::OnImitationModeSelect ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	mm_button_update_config->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorMirrorTherapyClick ), NULL, this );
	m_radioBox_mode->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( MainFrame::OnImitationModeSelect ), NULL, this );
	
}
