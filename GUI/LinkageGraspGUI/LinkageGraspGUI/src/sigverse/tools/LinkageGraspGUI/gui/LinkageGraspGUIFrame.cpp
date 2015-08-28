///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <sigverse/tools/LinkageGraspGUI/gui/LinkageGraspGUIFrame.h>

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,-1 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 218, 248, 233 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText_change_mode = new wxStaticText( this, wxID_ANY, wxT("Change mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_change_mode->Wrap( -1 );
	bSizer1->Add( m_staticText_change_mode, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer111;
	fgSizer111 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer111->SetFlexibleDirection( wxBOTH );
	fgSizer111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText_limb_mode = new wxStaticText( this, wxID_ANY, wxT("Limb mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_limb_mode->Wrap( -1 );
	fgSizer111->Add( m_staticText_limb_mode, 0, wxALL, 5 );
	
	wxString m_choice_limb_modeChoices[] = { wxT("HAND"), wxT("FOOT") };
	int m_choice_limb_modeNChoices = sizeof( m_choice_limb_modeChoices ) / sizeof( wxString );
	m_choice_limb_mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_limb_modeNChoices, m_choice_limb_modeChoices, 0 );
	m_choice_limb_mode->SetSelection( 0 );
	fgSizer111->Add( m_choice_limb_mode, 0, wxALL, 5 );
	
	m_staticText_reverse_mode = new wxStaticText( this, wxID_ANY, wxT("Reverse Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_reverse_mode->Wrap( -1 );
	fgSizer111->Add( m_staticText_reverse_mode, 0, wxALL, 5 );
	
	wxString m_choice_reverse_modeChoices[] = { wxT("Right to Left"), wxT("Left to Right") };
	int m_choice_reverse_modeNChoices = sizeof( m_choice_reverse_modeChoices ) / sizeof( wxString );
	m_choice_reverse_mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_reverse_modeNChoices, m_choice_reverse_modeChoices, 0 );
	m_choice_reverse_mode->SetSelection( 0 );
	fgSizer111->Add( m_choice_reverse_mode, 0, wxALL, 5 );
	
	
	fgSizer11->Add( fgSizer111, 1, wxLEFT, 20 );
	
	
	bSizer1->Add( fgSizer11, 1, wxALL, 10 );
	
	m_button_change = new wxButton( this, wxID_ANY, wxT("Change!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_button_change, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	m_choice_limb_mode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnLimbModeChoice ), NULL, this );
	m_choice_reverse_mode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnReverseModeChoice ), NULL, this );
	m_button_change->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnChangeClick ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
	m_choice_limb_mode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnLimbModeChoice ), NULL, this );
	m_choice_reverse_mode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrame::OnReverseModeChoice ), NULL, this );
	m_button_change->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnChangeClick ), NULL, this );
	
}
