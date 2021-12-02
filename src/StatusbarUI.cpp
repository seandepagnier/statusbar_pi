///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May 19 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////
#include "wx/wx.h"
#include "StatusbarUI.h"
#include "ocpn_plugin.h"

///////////////////////////////////////////////////////////////////////////

StatusbarPrefsDialogBase::StatusbarPrefsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    wxFont *qFont = GetOCPNScaledFont_PlugIn(_("Dialog"));
    SetFont( *qFont );

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Current Color Scheme") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	fgSizer6 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer6->AddGrowableCol( 3 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText15 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer6->Add( m_staticText15, 0, wxALL, 5 );

	m_colourPicker = new wxColourPickerCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxColour( 50, 0, 103 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer6->Add( m_colourPicker, 0, wxALL, 5 );

	m_staticText18 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, _("Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer6->Add( m_staticText18, 0, wxALL, 5 );

	m_sTransparency = new wxSlider( sbSizer1->GetStaticBox(), wxID_ANY, 64, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_sTransparency->SetMinSize( wxSize( 60,-1 ) );

	fgSizer6->Add( m_sTransparency, 0, wxALL|wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, _("Background Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer6->Add( m_staticText7, 0, wxALL, 5 );

	m_colourPickerBG = new wxColourPickerCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxColour( 56, 228, 28 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer6->Add( m_colourPickerBG, 0, wxALL, 5 );

	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, _("Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer6->Add( m_staticText8, 0, wxALL, 5 );

	m_sTransparencyBG = new wxSlider( sbSizer1->GetStaticBox(), wxID_ANY, 180, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer6->Add( m_sTransparencyBG, 0, wxALL|wxEXPAND, 5 );


	fgSizer61->Add( fgSizer6, 1, wxEXPAND, 5 );


	sbSizer1->Add( fgSizer61, 1, wxEXPAND, 5 );


	fgSizer7->Add( sbSizer1, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText19 = new wxStaticText( this, wxID_ANY, _("X Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer3->Add( m_staticText19, 0, wxALL, 5 );

	m_sXPosition = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1000, 10000, 0 );
	fgSizer3->Add( m_sXPosition, 0, wxALL, 5 );

	m_staticText20 = new wxStaticText( this, wxID_ANY, _("Y Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer3->Add( m_staticText20, 0, wxALL, 5 );

	m_sYPosition = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1000, 10000, 20 );
	fgSizer3->Add( m_sYPosition, 0, wxALL, 5 );

	m_staticText16 = new wxStaticText( this, wxID_ANY, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer3->Add( m_staticText16, 0, wxALL, 5 );

	m_FontPicker = new wxButton( this, wxID_ANY, _("Set Font"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_FontPicker, 0, wxALL, 5 );

	wxString m_cBuiltinStringChoices[] = { _("Select"), _("Default"), _("OwnShip"), _("Multiline") };
	int m_cBuiltinStringNChoices = sizeof( m_cBuiltinStringChoices ) / sizeof( wxString );
	m_cBuiltinString = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cBuiltinStringNChoices, m_cBuiltinStringChoices, 0 );
	m_cBuiltinString->SetSelection( 0 );
	fgSizer3->Add( m_cBuiltinString, 0, wxALL, 5 );

	m_bInfo = new wxButton( this, wxID_ANY, _("Info"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_bInfo, 0, wxALL, 5 );


	fgSizer7->Add( fgSizer3, 1, wxEXPAND, 5 );

	m_staticText17 = new wxStaticText( this, wxID_ANY, _("Display String"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( 500 );
	fgSizer7->Add( m_staticText17, 0, wxALL, 5 );

	m_tDisplayString = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_tDisplayString, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bAboutAuthor = new wxButton( this, wxID_ANY, _("About Author"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_bAboutAuthor, 0, wxALL, 5 );

	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2->Realize();

	fgSizer4->Add( m_sdbSizer2, 1, wxEXPAND, 5 );


	fgSizer7->Add( fgSizer4, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer7 );
	this->Layout();
	fgSizer7->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_colourPicker->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_colourPickerBG->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sXPosition->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sYPosition->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_FontPicker->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnFont ), NULL, this );
	m_cBuiltinString->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnBuiltinString ), NULL, this );
	m_bInfo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnDisplayStringInfo ), NULL, this );
	m_tDisplayString->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_bAboutAuthor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnAboutAuthor ), NULL, this );
}

StatusbarPrefsDialogBase::~StatusbarPrefsDialogBase()
{
	// Disconnect Events
	m_colourPicker->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparency->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_colourPickerBG->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sTransparencyBG->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sXPosition->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_sYPosition->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_FontPicker->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnFont ), NULL, this );
	m_cBuiltinString->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnBuiltinString ), NULL, this );
	m_bInfo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnDisplayStringInfo ), NULL, this );
	m_tDisplayString->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( StatusbarPrefsDialogBase::Refresh ), NULL, this );
	m_bAboutAuthor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatusbarPrefsDialogBase::OnAboutAuthor ), NULL, this );

}
