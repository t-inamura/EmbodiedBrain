///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <sigverse/tools/SenseOfSelfGUI/gui/SenseOfSelfGUIFrame.h>

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,-1 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 218, 248, 233 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText_select_avatar = new wxStaticText( this, wxID_ANY, wxT("Select Avatar"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_select_avatar->Wrap( -1 );
	bSizer1->Add( m_staticText_select_avatar, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button_mirror_therapy = new wxButton( this, wxID_ANY, wxT("Mirror Therapy"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button_mirror_therapy, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer111;
	fgSizer111 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer111->SetFlexibleDirection( wxBOTH );
	fgSizer111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText_reverse_mode = new wxStaticText( this, wxID_ANY, wxT("Reverse Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_reverse_mode->Wrap( -1 );
	fgSizer111->Add( m_staticText_reverse_mode, 0, wxALL, 5 );
	
	wxString m_choice_reverse_modeChoices[] = { wxT("Right hand to Left"), wxT("Left hand to Right"), wxT("No reverse") };
	int m_choice_reverse_modeNChoices = sizeof( m_choice_reverse_modeChoices ) / sizeof( wxString );
	m_choice_reverse_mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_reverse_modeNChoices, m_choice_reverse_modeChoices, 0 );
	m_choice_reverse_mode->SetSelection( 0 );
	fgSizer111->Add( m_choice_reverse_mode, 0, wxALL, 5 );
	
	m_staticText_delay_time = new wxStaticText( this, wxID_ANY, wxT("Delay Time [ms]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_delay_time->Wrap( -1 );
	fgSizer111->Add( m_staticText_delay_time, 0, wxALL, 5 );
	
	m_textCtrl_delay_time = new wxTextCtrl( this, wxID_ANY, wxT("1000"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer111->Add( m_textCtrl_delay_time, 0, wxALL, 5 );
	
	
	fgSizer11->Add( fgSizer111, 1, wxLEFT, 20 );
	
	m_button_long_arm = new wxButton( this, wxID_ANY, wxT("Long Arm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button_long_arm, 0, wxALL, 5 );
	
	
	fgSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button_short_arm = new wxButton( this, wxID_ANY, wxT("Short Arm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button_short_arm, 0, wxALL, 5 );
	
	
	fgSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button_robot_arm = new wxButton( this, wxID_ANY, wxT("Robot Arm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button_robot_arm, 0, wxALL, 5 );
	
	
	fgSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( fgSizer11, 1, wxALL, 10 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	m_button_mirror_therapy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorTherapyClick ), NULL, this );
	m_choice_reverse_mode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnReverseModeChoice ), NULL, this );
	m_button_long_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnLongArmClick ), NULL, this );
	m_button_short_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnShortArmClick ), NULL, this );
	m_button_robot_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnRobotArmClick ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	m_button_mirror_therapy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMirrorTherapyClick ), NULL, this );
	m_choice_reverse_mode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnReverseModeChoice ), NULL, this );
	m_button_long_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnLongArmClick ), NULL, this );
	m_button_short_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnShortArmClick ), NULL, this );
	m_button_robot_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnRobotArmClick ), NULL, this );
	
}
