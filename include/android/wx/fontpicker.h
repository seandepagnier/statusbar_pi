
#include <wx/button.h>
#include <wx/fontdlg.h>

class WXDLLIMPEXP_FWD_CORE wxFontPickerEvent;

extern WXDLLIMPEXP_DATA_CORE(const char) wxFontPickerWidgetNameStr[];
extern WXDLLIMPEXP_DATA_CORE(const char) wxFontPickerCtrlNameStr[];

#define wxFNTP_FONTDESC_AS_LABEL      0x0008

// uses the currently selected font to draw the label of the button
#define wxFNTP_USEFONT_FOR_LABEL      0x0010

#define wxFONTBTN_DEFAULT_STYLE \
    (wxFNTP_FONTDESC_AS_LABEL | wxFNTP_USEFONT_FOR_LABEL)

// ----------------------------------------------------------------------------
// wxFontPickerCtrl specific flags
// ----------------------------------------------------------------------------

#define wxFNTP_USE_TEXTCTRL       (wxPB_USE_TEXTCTRL)
#define wxFNTP_DEFAULT_STYLE      (wxFNTP_FONTDESC_AS_LABEL|wxFNTP_USEFONT_FOR_LABEL)

// not a style but rather the default value of the maximum pointsize allowed
#define wxFNTP_MAXPOINT_SIZE      100


// ----------------------------------------------------------------------------
// wxFontPickerCtrl: platform-independent class which embeds the
// platform-dependent wxFontPickerWidget andm if wxFNTP_USE_TEXTCTRL style is
// used, a textctrl next to it.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxFontPickerCtrl : public wxButton
{
public:
    wxFontPickerCtrl(wxWindow *parent,
                     wxWindowID id,
                     const wxFont& initial = wxNullFont,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxFNTP_DEFAULT_STYLE,
                     const wxValidator& validator = wxDefaultValidator,
                     const wxString& name = wxFontPickerCtrlNameStr)
        : wxButton(parent, id, "Set Font", pos, size, 0, validator, name),
        m_font(initial),
        m_nMaxPointSize(wxFNTP_MAXPOINT_SIZE),
        m_bUsed(false)
    {
        Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxFontPickerCtrl::SetFontDlg ), NULL, this );
    }
    virtual ~wxFontPickerCtrl() {}
public:         // public API
    void SetFontDlg(wxCommandEvent &event)
    {
        wxFontData init_font_data;
        init_font_data.SetInitialFont(m_font);
        wxFontDialog dlg(GetParent(), init_font_data);
        dlg.Centre();
        if (dlg.ShowModal() != wxID_CANCEL) {
            wxFontData font_data = dlg.GetFontData();
            m_font = font_data.GetChosenFont();
            m_bUsed = true;
        }
        SetFont(m_font);
    }

    // get the font chosen
    wxFont GetSelectedFont() const
        { return m_font; }

    // sets currently displayed font
    void SetSelectedFont(const wxFont& f) { m_font = f; SetFont(f); }

    // set/get the max pointsize
    void SetMaxPointSize(unsigned int max)
        { m_nMaxPointSize=max; }
    unsigned int GetMaxPointSize() const
        { return m_nMaxPointSize; }

    // event handler for our picker
    void OnFontChange(wxFontPickerEvent &);

    bool m_bUsed;
private:
    wxFont m_font;
    // the maximum pointsize allowed to the user
    unsigned int m_nMaxPointSize;
    DECLARE_DYNAMIC_CLASS(wxFontPickerCtrl)
};


// ----------------------------------------------------------------------------
// wxFontPickerEvent: used by wxFontPickerCtrl only
// ----------------------------------------------------------------------------

wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_FONTPICKER_CHANGED, wxFontPickerEvent );

class WXDLLIMPEXP_CORE wxFontPickerEvent : public wxCommandEvent
{
public:
    wxFontPickerEvent() {}
    wxFontPickerEvent(wxObject *generator, int id, const wxFont &f)
        : wxCommandEvent(wxEVT_FONTPICKER_CHANGED, id),
          m_font(f)
    {
        SetEventObject(generator);
    }

    wxFont GetFont() const { return m_font; }
    void SetFont(const wxFont &c) { m_font = c; }

    // default copy ctor, assignment operator and dtor are ok
    virtual wxEvent *Clone() const { return new wxFontPickerEvent(*this); }

private:
    wxFont m_font;

    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxFontPickerEvent)
};

// ----------------------------------------------------------------------------
// event types and macros
// ----------------------------------------------------------------------------

typedef void (wxEvtHandler::*wxFontPickerEventFunction)(wxFontPickerEvent&);

#define wxFontPickerEventHandler(func) \
    wxEVENT_HANDLER_CAST(wxFontPickerEventFunction, func)

#define EVT_FONTPICKER_CHANGED(id, fn) \
    wx__DECLARE_EVT1(wxEVT_FONTPICKER_CHANGED, id, wxFontPickerEventHandler(fn))

// old wxEVT_COMMAND_* constants
#define wxEVT_COMMAND_FONTPICKER_CHANGED   wxEVT_FONTPICKER_CHANGED

