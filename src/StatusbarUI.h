///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/clrpicker.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class StatusbarPrefsDialogBase
///////////////////////////////////////////////////////////////////////////////
class StatusbarPrefsDialogBase : public wxDialog
{
	private:
		wxStaticText* m_staticText18;

	protected:
		wxStaticText* m_staticText7;
		wxButton* m_FontPicker;
		wxChoice* m_cBuiltinString;
		wxButton* m_bAboutAuthor;

		// Virtual event handlers, override them in your derived class
		virtual void Refresh( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void Refresh( wxScrollEvent& event ) { event.Skip(); }
		virtual void Refresh( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnFont( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBuiltinString( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDisplayStringInfo( wxCommandEvent& event ) { event.Skip(); }
		virtual void Refresh( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutAuthor( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxFlexGridSizer* fgSizer6;
		wxStaticText* m_staticText15;
		wxColourPickerCtrl* m_colourPicker;
		wxSlider* m_sTransparency;
		wxColourPickerCtrl* m_colourPickerBG;
		wxSlider* m_sTransparencyBG;
		wxStaticText* m_staticText19;
		wxSpinCtrl* m_sXPosition;
		wxStaticText* m_staticText20;
		wxSpinCtrl* m_sYPosition;
		wxStaticText* m_staticText16;
		wxButton* m_bInfo;
		wxStaticText* m_staticText17;
		wxTextCtrl* m_tDisplayString;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;

		StatusbarPrefsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Status Bar Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 337,280 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

		~StatusbarPrefsDialogBase();

};

