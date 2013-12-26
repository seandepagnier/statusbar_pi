///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "StatusbarUI.h"

///////////////////////////////////////////////////////////////////////////

StatusbarPrefsDialogBase::StatusbarPrefsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer7->AddGrowableCol( 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer6->Add( m_staticText15, 0, wxALL, 5 );
	
	m_colourPicker = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 50, 0, 103 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer6->Add( m_colourPicker, 0, wxALL, 5 );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer6->Add( m_staticText16, 0, wxALL, 5 );
	
	m_fontPicker = new wxFontPickerCtrl( this, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE );
	m_fontPicker->SetMaxPointSize( 100 ); 
	fgSizer6->Add( m_fontPicker, 0, wxALL, 5 );
	
	m_checkBox4 = new wxCheckBox( this, wxID_ANY, _("N/A option"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_checkBox4, 0, wxALL, 5 );
	
	m_cbBlur = new wxCheckBox( this, wxID_ANY, _("Blur"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbBlur, 0, wxALL, 5 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer6->Add( m_staticText18, 0, wxALL, 5 );
	
	m_sTransparency = new wxSlider( this, wxID_ANY, 64, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer6->Add( m_sTransparency, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, _("X Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer6->Add( m_staticText19, 0, wxALL, 5 );
	
	m_sXPosition = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1000, 10000, 0 );
	fgSizer6->Add( m_sXPosition, 0, wxALL, 5 );
	
	m_staticText20 = new wxStaticText( this, wxID_ANY, _("Y Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer6->Add( m_staticText20, 0, wxALL, 5 );
	
	m_sYPosition = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1000, 10000, 0 );
	fgSizer6->Add( m_sYPosition, 0, wxALL, 5 );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, _("Display String"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( 500 );
	fgSizer6->Add( m_staticText17, 0, wxALL, 5 );
	
	m_bInfo = new wxButton( this, wxID_ANY, _("Info"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_bInfo, 0, wxALL, 5 );
	
	
	fgSizer7->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	m_tDisplayString = new wxTextCtrl( this, wxID_ANY, _("Ship %1   SOG %2  COG %3   %7  %8  NMi  %A"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_tDisplayString, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2->Realize();
	
	fgSizer7->Add( m_sdbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bInfo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnDisplayStringInfo ), NULL, this );
}

StatusbarPrefsDialogBase::~StatusbarPrefsDialogBase()
{
	// Disconnect Events
	m_bInfo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnDisplayStringInfo ), NULL, this );
	
}
