/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Status Bar Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _STATUSBARPI_H_
#define _STATUSBARPI_H_

#include <wx/wx.h>

#include "version.h"

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    9

#define ABOUT_AUTHOR_URL "http://seandepagnier.users.sourceforge.net"

#include <wx/fileconf.h>
#include "ocpn_plugin.h"
#include "defs.h"

#define STATUSBAR_TOOL_POSITION -1          // Request default positioning of toolbar tool

class StatusbarPrefsDialog : public StatusbarPrefsDialogBase
{
public:
StatusbarPrefsDialog( wxWindow *parent ) : StatusbarPrefsDialogBase( parent ) {}
    void Refresh() { RequestRefresh(GetOCPNCanvasWindow()); }
    virtual void Refresh( wxColourPickerEvent& event ) { Refresh(); }
    virtual void Refresh( wxScrollEvent& event ) { Refresh(); }
    virtual void Refresh( wxCommandEvent& event ) { Refresh(); }
    virtual void Refresh( wxSpinEvent& event ) { Refresh(); }
    virtual void Refresh( wxFontPickerEvent& event ) { Refresh(); }
    void OnBuiltinString( wxCommandEvent& event );
    void OnDisplayStringInfo( wxCommandEvent& event );
    void OnAboutAuthor( wxCommandEvent& event );
};

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

class statusbar_pi : public wxEvtHandler, public opencpn_plugin_19
{
public:
    statusbar_pi( void *ppimgr );

//  The required PlugIn Methods
    int Init( void );
    bool DeInit( void );

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

    wxString StatusString(PlugIn_ViewPort *vp);

    bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
    bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
    void SetCursorLatLon(double lat, double lon);
    void SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix);
    void ShowPreferencesDialog( wxWindow* parent );
    void SetColorScheme(PI_ColorScheme cs);

private:
    int GetYPosition();

    bool LoadConfig( void );
    bool SaveConfig( void );

    void BuildFont();
    wxString ColorSchemeName();

    void OnRefreshTimer( wxTimerEvent & );

    wxDateTime m_LastRefreshTime;
    wxTimer m_TimeRefreshTimer, m_DateRefreshTimer;

    wxFileConfig     *m_pConfig;

    StatusbarPrefsDialog *m_PreferencesDialog;

    double m_cursor_lat, m_cursor_lon;
    PlugIn_Position_Fix_Ex lastfix;

    TexFont m_texfont;

    PI_ColorScheme m_ColorScheme;
};

#endif
