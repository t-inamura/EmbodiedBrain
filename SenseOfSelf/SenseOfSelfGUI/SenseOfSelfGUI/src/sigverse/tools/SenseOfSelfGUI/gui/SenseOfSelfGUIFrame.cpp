///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "SenseOfSelfGUIFrame.h"

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
	fgSizer11 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button_middle_arm = new wxButton( this, wxID_ANY, wxT("Middle Arm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button_middle_arm, 0, wxALL, 5 );
	
	m_button_long_arm = new wxButton( this, wxID_ANY, wxT("Long Arm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button_long_arm, 0, wxALL, 5 );
	
	m_button_short_arm = new wxButton( this, wxID_ANY, wxT("Short Arm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button_short_arm, 0, wxALL, 5 );
	
	
	bSizer1->Add( fgSizer11, 1, wxALL, 10 );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button_add_desk_height = new wxButton( this, wxID_ANY, wxT("Add Desk height"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_button_add_desk_height, 0, wxALL, 5 );
	
	m_staticText_desk_height_dy = new wxStaticText( this, wxID_ANY, wxT("(dy)="), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_desk_height_dy->Wrap( -1 );
	fgSizer12->Add( m_staticText_desk_height_dy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_textCtrl_desk_height_dy = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_textCtrl_desk_height_dy, 0, wxALL, 5 );
	
	
	fgSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button_add_plate_position = new wxButton( this, wxID_ANY, wxT("Add Plate position"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_button_add_plate_position, 0, wxALL, 5 );
	
	m_staticText_plate_position_dx_dz = new wxStaticText( this, wxID_ANY, wxT("(dx,dz)="), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_plate_position_dx_dz->Wrap( -1 );
	fgSizer12->Add( m_staticText_plate_position_dx_dz, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_textCtrl_plate_position_dx = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_textCtrl_plate_position_dx, 0, wxALL, 5 );
	
	m_textCtrl_plate_position_dz = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_textCtrl_plate_position_dz, 0, wxALL, 5 );
	
	
	bSizer1->Add( fgSizer12, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	m_button_middle_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMiddleArmClick ), NULL, this );
	m_button_long_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnLongArmClick ), NULL, this );
	m_button_short_arm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnShortArmClick ), NULL, this );
	m_button_add_desk_height->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnAddDeskHeightClick ), NULL, this );
	m_button_add_plate_position->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnAddPlatePositionClick ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	m_button_middle_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnMiddleArmClick ), NULL, this );
	m_button_long_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnLongArmClick ), NULL, this );
	m_button_short_arm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnShortArmClick ), NULL, this );
	m_button_add_desk_height->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnAddDeskHeightClick ), NULL, this );
	m_button_add_plate_position->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnAddPlatePositionClick ), NULL, this );
	
}
