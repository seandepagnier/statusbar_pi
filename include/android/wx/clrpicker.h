/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Color Picker hack for android
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2018 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 *
 */

#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/colordlg.h>
#include <wx/colourdata.h>

extern WXDLLIMPEXP_DATA_CORE(const char) wxColourPickerWidgetNameStr[];

class WXDLLIMPEXP_FWD_CORE wxColourPickerEvent;

wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_COLOURPICKER_CHANGED, wxColourPickerEvent );

class WXDLLIMPEXP_CORE wxColourPickerEvent : public wxCommandEvent
{
public:
    wxColourPickerEvent() {}
    wxColourPickerEvent(wxObject *generator, int id, const wxColour &col)
        : wxCommandEvent(wxEVT_COLOURPICKER_CHANGED, id),
          m_colour(col)
    {
        SetEventObject(generator);
    }

    wxColour GetColour() const { return m_colour; }
    void SetColour(const wxColour &c) { m_colour = c; }


    // default copy ctor, assignment operator and dtor are ok
    virtual wxEvent *Clone() const { return new wxColourPickerEvent(*this); }

private:
    wxColour m_colour;

    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxColourPickerEvent)
};

typedef void (wxEvtHandler::*wxColourPickerEventFunction)(wxColourPickerEvent&);

#define wxColourPickerEventHandler(func) \
    wxEVENT_HANDLER_CAST(wxColourPickerEventFunction, func)

#define EVT_COLOURPICKER_CHANGED(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COLOURPICKER_CHANGED, id, wxColourPickerEventHandler(fn))
#define wxEVT_COMMAND_COLOURPICKER_CHANGED wxEVT_COLOURPICKER_CHANGED

enum {  wxCLRP_DEFAULT_STYLE };
    
class WXDLLIMPEXP_CORE wxColourPickerCtrl : public wxButton
{
public:
wxColourPickerCtrl(wxWindow *parent,
                   wxWindowID id,
                   const wxColour& initial = *wxBLACK,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxCLRP_DEFAULT_STYLE,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxColourPickerWidgetNameStr)
    : wxButton(parent, id, " ", pos, size, 0, validator, name)
    {
        SetColour(initial);
	Connect( wxEVT_PAINT, wxPaintEventHandler( wxColourPickerCtrl::OnPaint ), NULL, this );
        Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxColourPickerCtrl::SetColorDlg ), NULL, this );
    }

    void OnPaint( wxPaintEvent &event )
    {
        wxPaintDC dc( this );
        wxColour c(m_color.Red(), m_color.Green(), m_color.Blue());
        dc.SetBackground(c);
        dc.Clear();
    }
    
    void SetColorDlg(wxCommandEvent &event)
    {
        wxColourData color;
        wxColour c = GetColour();
        color.SetColour(c);
        wxGetColourFromUser(GetParent(), c, "", &color);
        SetColour(color.GetColour());
    }

    virtual void SetColour(const wxColour &col)
    {
        m_color = col;
        // fire the colour-changed event
        //wxColourPickerEvent event(this, GetId(), GetColour());
        //HandleWindowEvent(event);
    }

    virtual wxColour GetColour()
    {
        return m_color;
    }

    virtual ~wxColourPickerCtrl() {}
private:
    wxColour m_color;
};
