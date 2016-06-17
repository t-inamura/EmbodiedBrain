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
	
	mm_staticText_mirror_therapy = new wxStaticText( this, wxID_ANY, wxT("Mirror Therapy"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_mirror_therapy->Wrap( -1 );
	bSizerTop->Add( mm_staticText_mirror_therapy, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	bSizer11->SetMinSize( wxSize( 200,-1 ) ); 
	mm_button_mirror_therapy = new wxButton( this, wxID_ANY, wxT("Mirror Therapy !"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer11->Add( mm_button_mirror_therapy, 0, wxALL, 5 );
	
	
	fgSizer1->Add( bSizer11, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mm_staticText_reverse_mode = new wxStaticText( this, wxID_ANY, wxT("Reverse Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_reverse_mode->Wrap( -1 );
	fgSizer12->Add( mm_staticText_reverse_mode, 0, wxALL, 5 );
	
	wxString mm_choice_reverse_modeChoices[] = { wxT("Right hand to Left"), wxT("Left hand to Right"), wxT("No reverse") };
	int mm_choice_reverse_modeNChoices = sizeof( mm_choice_reverse_modeChoices ) / sizeof( wxString );
	mm_choice_reverse_mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mm_choice_reverse_modeNChoices, mm_choice_reverse_modeChoices, 0 );
	mm_choice_reverse_mode->SetSelection( 0 );
	fgSizer12->Add( mm_choice_reverse_mode, 0, wxALL, 5 );
	
	mm_staticText_delay_time = new wxStaticText( this, wxID_ANY, wxT("Delay Time [ms]"), wxDefaultPosition, wxDefaultSize, 0 );
	mm_staticText_delay_time->Wrap( -1 );
	fgSizer12->Add( mm_staticText_delay_time, 0, wxALL, 5 );
	
	mm_textCtrl_delay_time = new wxTextCtrl( this, wxID_ANY, wxT("1000"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( mm_textCtrl_delay_time, 0, wxALL, 5 );
	
	
	fgSizer1->Add( fgSizer12, 1, wxLEFT, 5 );
	
	mm_button_long_arm = new wxButton( this, wxID_ANY, wxT("Long Arm !"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( mm_button_long_arm, 0, wxALL, 5 );
	
	
	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	mm_button_short_arm = new wxButton( this, wxID_ANY, wxT("Short Arm !"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( mm_button_short_arm, 0, wxALL, 5 );
	
	
	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	mm_button_robot_arm = new wxButton( this, wxID_ANY, wxT("Robot Arm !"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( mm_button_robot_arm, 0, wxALL, 5 );
	
	
	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizerTop->Add( fgSizer1, 1, wxALL, 10 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerTop->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	lm_staticText_linkage_controller = new wxStaticText( this, wxID_ANY, wxT("Linkage Controller"), wxDefaultPosition, wxDefaultSize, 0 );
	lm_staticText_linkage_controller->Wrap( -1 );
	bSizerTop->Add( lm_staticText_linkage_controller, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->SetMinSize( wxSize( 200,-1 ) ); 
	lm_button_change = new wxButton( this, wxID_ANY, wxT("Linkage !"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( lm_button_change, 0, wxALL, 5 );
	
	
	fgSizer2->Add( bSizer21, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	lm_staticText_limb_mode = new wxStaticText( this, wxID_ANY, wxT("Limb mode"), wxDefaultPosition, wxDefaultSize, 0 );
	lm_staticText_limb_mode->Wrap( -1 );
	fgSizer22->Add( lm_staticText_limb_mode, 0, wxALL, 5 );
	
	wxString lm_choice_limb_modeChoices[] = { wxT("HAND"), wxT("FOOT") };
	int lm_choice_limb_modeNChoices = sizeof( lm_choice_limb_modeChoices ) / sizeof( wxString );
	lm_choice_limb_mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, lm_choice_limb_modeNChoices, lm_choice_limb_modeChoices, 0 );
	lm_choice_limb_mode->SetSelection( 0 );
	lm_choice_limb_mode->Enable( false );
	
	fgSizer22->Add( lm_choice_limb_mode, 0, wxALL, 5 );
	
	lm_staticText_grasp_mode = new wxStaticText( this, wxID_ANY, wxT("Grasp Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	lm_staticText_grasp_mode->Wrap( -1 );
	fgSizer22->Add( lm_staticText_grasp_mode, 0, wxALL, 5 );
	
	wxString lm_choice_grasp_modeChoices[] = { wxT("SANDWICH"), wxT("GRASP_RIGHT"), wxT("GRASP_LEFT") };
	int lm_choice_grasp_modeNChoices = sizeof( lm_choice_grasp_modeChoices ) / sizeof( wxString );
	lm_choice_grasp_mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, lm_choice_grasp_modeNChoices, lm_choice_grasp_modeChoices, 0 );
	lm_choice_grasp_mode->SetSelection( 0 );
	fgSizer22->Add( lm_choice_grasp_mode, 0, wxALL, 5 );
	
	lm_staticText_reverse_mode = new wxStaticText( this, wxID_ANY, wxT("Reverse Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	lm_staticText_reverse_mode->Wrap( -1 );
	fgSizer22->Add( lm_staticText_reverse_mode, 0, wxALL, 5 );
	
	wxString lm_choice_reverse_modeChoices[] = { wxT("Right to Left"), wxT("Left to Right"), wxT("Not reverse") };
	int lm_choice_reverse_modeNChoices = sizeof( lm_choice_reverse_modeChoices ) / sizeof( wxString );
	lm_choice_reverse_mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, lm_choice_reverse_modeNChoices, lm_choice_reverse_modeChoices, 0 );
	lm_choice_reverse_mode->SetSelection( 0 );
	fgSizer22->Add( lm_choice_reverse_mode, 0, wxALL, 5 );
	
	lm_staticText_fixed_waist = new wxStaticText( this, wxID_ANY, wxT("Fixed Waist"), wxDefaultPosition, wxDefaultSize, 0 );
	lm_staticText_fixed_waist->Wrap( -1 );
	fgSizer22->Add( lm_staticText_fixed_waist, 0, wxALL, 5 );
	
	wxString lm_choice_fixed_waistChoices[] = { wxT("false"), wxT("true") };
	int lm_choice_fixed_waistNChoices = sizeof( lm_choice_fixed_waistChoices ) / sizeof( wxString );
	lm_choice_fixed_waist = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, lm_choice_fixed_waistNChoices, lm_choice_fixed_waistChoices, 0 );
	lm_choice_fixed_waist->SetSelection( 1 );
	fgSizer22->Add( lm_choice_fixed_waist, 0, wxALL, 5 );
	
	
	fgSizer2->Add( fgSizer22, 1, wxLEFT, 5 );
	
	
	bSizerTop->Add( fgSizer2, 1, wxALL, 10 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerTop->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	sm_staticText_sense_of_self = new wxStaticText( this, wxID_ANY, wxT("Sense Of Self"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_staticText_sense_of_self->Wrap( -1 );
	bSizerTop->Add( sm_staticText_sense_of_self, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	bSizer31->SetMinSize( wxSize( 200,-1 ) ); 
	sm_button_middle_arm = new wxButton( this, wxID_ANY, wxT("Middle Arm !"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( sm_button_middle_arm, 0, wxALL, 5 );
	
	sm_button_long_arm1 = new wxButton( this, wxID_ANY, wxT("Long Arm !"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( sm_button_long_arm1, 0, wxALL, 5 );
	
	sm_button_short_arm1 = new wxButton( this, wxID_ANY, wxT("Short Arm !"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( sm_button_short_arm1, 0, wxALL, 5 );
	
	
	fgSizer3->Add( bSizer31, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer32;
	fgSizer32 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer32->SetFlexibleDirection( wxBOTH );
	fgSizer32->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	sm_button_add_desk_height = new wxButton( this, wxID_ANY, wxT("Add Desk height"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_button_add_desk_height->Enable( false );
	
	fgSizer32->Add( sm_button_add_desk_height, 0, wxALL, 5 );
	
	sm_staticText_desk_height_dy = new wxStaticText( this, wxID_ANY, wxT("(dy)="), wxDefaultPosition, wxDefaultSize, 0 );
	sm_staticText_desk_height_dy->Wrap( -1 );
	fgSizer32->Add( sm_staticText_desk_height_dy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sm_textCtrl_desk_height_dy = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_textCtrl_desk_height_dy->Enable( false );
	
	fgSizer32->Add( sm_textCtrl_desk_height_dy, 0, wxALL, 5 );
	
	
	fgSizer32->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sm_button_add_plate_position = new wxButton( this, wxID_ANY, wxT("Add Plate position"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_button_add_plate_position->Enable( false );
	
	fgSizer32->Add( sm_button_add_plate_position, 0, wxALL, 5 );
	
	sm_staticText_plate_position_dx_dz = new wxStaticText( this, wxID_ANY, wxT("(dx,dz)="), wxDefaultPosition, wxDefaultSize, 0 );
	sm_staticText_plate_position_dx_dz->Wrap( -1 );
	fgSizer32->Add( sm_staticText_plate_position_dx_dz, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sm_textCtrl_plate_position_dx = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_textCtrl_plate_position_dx->Enable( false );
	
	fgSizer32->Add( sm_textCtrl_plate_position_dx, 0, wxALL, 5 );
	
	sm_textCtrl_plate_position_dz = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	sm_textCtrl_plate_position_dz->Enable( false );
	
	fgSizer32->Add( sm_textCtrl_plate_position_dz, 0, wxALL, 5 );
	
	
	fgSizer3->Add( fgSizer32, 1, wxEXPAND, 5 );
	
	
	bSizerTop->Add( fgSizer3, 1, wxALL, 10 );
	
	
	this->SetSizer( bSizerTop );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	mm_button_mirror_therapy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorMirrorTherapyClick ), NULL, this );
	mm_button_long_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorLongArmClick ), NULL, this );
	mm_button_short_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorShortArmClick ), NULL, this );
	mm_button_robot_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorRobotArmClick ), NULL, this );
	lm_button_change->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnLinkageChangeClick ), NULL, this );
	sm_button_middle_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosMiddleArmClick ), NULL, this );
	sm_button_long_arm1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosLongArmClick ), NULL, this );
	sm_button_short_arm1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosShortArmClick ), NULL, this );
	sm_button_add_desk_height->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosAddDeskHeightClick ), NULL, this );
	sm_button_add_plate_position->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosAddPlatePositionClick ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	mm_button_mirror_therapy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorMirrorTherapyClick ), NULL, this );
	mm_button_long_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorLongArmClick ), NULL, this );
	mm_button_short_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorShortArmClick ), NULL, this );
	mm_button_robot_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorRobotArmClick ), NULL, this );
	lm_button_change->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnLinkageChangeClick ), NULL, this );
	sm_button_middle_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosMiddleArmClick ), NULL, this );
	sm_button_long_arm1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosLongArmClick ), NULL, this );
	sm_button_short_arm1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosShortArmClick ), NULL, this );
	sm_button_add_desk_height->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosAddDeskHeightClick ), NULL, this );
	sm_button_add_plate_position->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSosAddPlatePositionClick ), NULL, this );
	
}
