///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __STATUSBARUI_H__
#define __STATUSBARUI_H__

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
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/spinctrl.h>
#include <wx/fontpicker.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class StatusbarPrefsDialogBase
///////////////////////////////////////////////////////////////////////////////
class StatusbarPrefsDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxChoice* m_cBuiltinString;
		wxButton* m_bAboutAuthor;
		
		// Virtual event handlers, overide them in your derived class
		virtual void Refresh( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void Refresh( wxScrollEvent& event ) { event.Skip(); }
		virtual void Refresh( wxCommandEvent& event ) { event.Skip(); }
		virtual void Refresh( wxSpinEvent& event ) { event.Skip(); }
		virtual void Refresh( wxFontPickerEvent& event ) { event.Skip(); }
		virtual void OnBuiltinString( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDisplayStringInfo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutAuthor( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxStaticText* m_staticText15;
		wxColourPickerCtrl* m_colourPicker;
		wxStaticText* m_staticText18;
		wxSlider* m_sTransparency;
		wxCheckBox* m_cbInvertBackground;
		wxColourPickerCtrl* m_colourPickerBG;
		wxSlider* m_sTransparencyBG;
		wxCheckBox* m_cbBlur;
		wxStaticText* m_staticText19;
		wxSpinCtrl* m_sXPosition;
		wxStaticText* m_staticText20;
		wxSpinCtrl* m_sYPosition;
		wxStaticText* m_staticText16;
		wxFontPickerCtrl* m_fontPicker;
		wxButton* m_bInfo;
		wxStaticText* m_staticText17;
		wxTextCtrl* m_tDisplayString;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		
		StatusbarPrefsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Status Bar Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE ); 
		~StatusbarPrefsDialogBase();
	
};

#endif //__STATUSBARUI_H__
