/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Status Bar Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier
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

#include "wx/wx.h"
#include <wx/graphics.h>
#include <wx/fontdlg.h>

#include "plugingl/pidc.h"
#include "StatusbarUI.h"
#include "plugingl/TexFont.h"
#include "statusbar_pi.h"
#include "icons.h"

# if !defined(NAN)
# define NAN std::numeric_limits<double>::quiet_NaN ()
# endif

#ifdef USE_ANDROID_GLES2    
#include "qdebug.h"
#endif

#ifdef __OCPN__ANDROID__
#define MIN_FONT_SIZE 20
#else
#define MIN_FONT_SIZE 8
#endif

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new statusbar_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

void StatusbarPrefsDialog::LoadConfig()
{
    StatusbarConfig &c = m_statusbar_pi.m_config;

    m_colourPicker->SetColour(c.color);
    m_cbInvertBackground->SetValue(c.invertbackground);
    m_cbBlur->SetValue(c.blur);
    m_sTransparency->SetValue(255-c.color.Alpha());
    m_colourPickerBG->SetColour(c.bgcolor);
    m_sTransparencyBG->SetValue(255-c.bgcolor.Alpha());

    m_sXPosition->SetValue(c.XPosition);
    m_sYPosition->SetValue(c.YPosition);

    m_tDisplayString->SetValue(c.DisplayString);
}

void StatusbarPrefsDialog::SaveConfig()
{
    StatusbarConfig &c = m_statusbar_pi.m_config;

    c.color = m_colourPicker->GetColour();
    c.color.Set(c.color.Red(), c.color.Green(), c.color.Blue(), 255-m_sTransparency->GetValue());

    c.invertbackground = m_cbInvertBackground->GetValue();
    c.bgcolor = m_colourPickerBG->GetColour();
    c.bgcolor.Set(c.bgcolor.Red(), c.bgcolor.Green(), c.bgcolor.Blue(), 255-m_sTransparencyBG->GetValue());
    c.blur = m_cbBlur->GetValue();
    
    c.XPosition = m_sXPosition->GetValue();
    c.YPosition = m_sYPosition->GetValue();

#ifdef __OCPN__ANDROID__
//    if(m_fontPicker->m_bUsed)
#endif
//        c.font = m_fontPicker->GetSelectedFont();
    if(c.font.GetPointSize() < MIN_FONT_SIZE)
        c.font.SetPointSize(MIN_FONT_SIZE);

    c.DisplayString = m_tDisplayString->GetValue();
}

//-----------------------------------------------------------------------------
//
//    StatusBar PlugIn Implementation
//
//-----------------------------------------------------------------------------

statusbar_pi::statusbar_pi(void *ppimgr)
    :opencpn_plugin_111(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
}

int statusbar_pi::Init(void)
{
    AddLocaleCatalog( _T("opencpn-statusbar_pi") );

    // This is bugged, need to fix in core program so we know the scheme at start
    m_ColorScheme = PI_GLOBAL_COLOR_SCHEME_RGB;
    LoadConfig();

    m_TimeRefreshTimer.Connect(wxEVT_TIMER, wxTimerEventHandler
                               ( statusbar_pi::OnRefreshTimer ), NULL, this);
    m_DateRefreshTimer.Connect(wxEVT_TIMER, wxTimerEventHandler
                               ( statusbar_pi::OnRefreshTimer ), NULL, this);

    m_PreferencesDialog = NULL;

    return (WANTS_OVERLAY_CALLBACK    |
            WANTS_OPENGL_OVERLAY_CALLBACK    |
            WANTS_CURSOR_LATLON       |
            WANTS_CONFIG              |
//            WANTS_NMEA_SENTENCES |
            WANTS_NMEA_EVENTS |
            WANTS_PREFERENCES
            );
}

bool statusbar_pi::DeInit(void)
{
    SaveConfig();
    return true;
}

int statusbar_pi::GetAPIVersionMajor()
{
    return MY_API_VERSION_MAJOR;
}

int statusbar_pi::GetAPIVersionMinor()
{
    return MY_API_VERSION_MINOR;
}

int statusbar_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int statusbar_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

wxBitmap *statusbar_pi::GetPlugInBitmap()
{
    return _img_statusbar;
}

wxString statusbar_pi::GetCommonName()
{
    return _("StatusBar");
}

wxString statusbar_pi::GetShortDescription()
{
    return _("StatusBar Plugin is an optional replacement for the builtin statusbar");
}

wxString statusbar_pi::GetLongDescription()
{
    return _("StatusBar plugin replaces builtin statusbar\n\
The builtin status bar (disable from the User Interface tab)\n\
is very limited in it's configuration options and \
can be very difficult to read.\n\
\n\
The statusbar plugin improves on some of these difficulties.\n\
  Best used with OpenGL enabled (requires some basic OpenGL extensions).\n");
}

wxString DefaultString = _T("Ship %02A %2.4B %D   %02E %2.4F %H   SOG %.2I  COG %03J    \
%02O %2.4P %R   %02S %2.4T %V   %03W  %.1X    Scale %Z");


void StatusbarPrefsDialog::OnFont( wxCommandEvent& event )
{
    StatusbarConfig &c = m_statusbar_pi.m_config;
    wxFontData init_font_data;
    init_font_data.SetInitialFont(c.font);
    wxFontDialog dlg(GetParent(), init_font_data);
    dlg.Centre();
    if (dlg.ShowModal() != wxID_CANCEL) {
        wxFontData font_data = dlg.GetFontData();
        c.font = font_data.GetChosenFont();
        Refresh();
    }
}

void StatusbarPrefsDialog::OnBuiltinString( wxCommandEvent& event )
{
    wxString OwnshipString = _T("Ship %02A %2.4B %D   %02E %2.4F %H   SOG %.2I  COG %03J");
    wxString MultilineString = _T("%02A %2.2B%D  %02E %2.2F%H  %.1I %03J\
\\n%02O %2.2P%R %02S %2.2T%V %03W %.2X %03.a");
    
    switch(event.GetSelection()) {
    case 0: break;
    case 1:  m_tDisplayString->SetValue(DefaultString); break;
    case 2:  m_tDisplayString->SetValue(OwnshipString); break;
    case 3:  m_tDisplayString->SetValue(MultilineString); break;
    }

    m_cBuiltinString->SetSelection(0);
}

void StatusbarPrefsDialog::OnDisplayStringInfo( wxCommandEvent& event )
{
    wxMessageDialog dlg( GetOCPNCanvasWindow(), _("\
Display String can include formats eg: \"%03.0E\" gives ship longitude \
the format specifier 03.0 gives how many places to round to, and to use leading 0's etc..\n\
The following are formats:\n\
%A ship lat degrees    %B ship lat minutes   %C ship lat seconds  %D ship N/S\n\
%E ship lon degrees    %F ship lon minutes   %G ship lon seconds  %H ship E/W\n\
%I ship sog            %J ship cog           %K ship heading      %L ship heading rate\n\
%O cursor lat degrees    %P cursor lat minutes   %Q cursor lat seconds   %R cursor N/S\n\
%S cursor lon degrees    %T cursor lon minutes   %U cursor lon seconds   %V cursor E/W\n\
%W from ship bearing to cursor\n\
%X distance to cursor mercator  %Y distance to cursor great circle  %Z chart scale\n\
%a viewport orientation angle\n\
%f frames rendered per second\n\
%d Date    %t Time    %z Time Zone\n\
%% print a percent"), _("Statusbar Information"), wxOK | wxICON_INFORMATION );
    dlg.ShowModal();
}

void StatusbarPrefsDialog::OnAboutAuthor( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser(_T(ABOUT_AUTHOR_URL));
}

double Minutes(double degrees)
{
    return 60*fabs(degrees - trunc(degrees));
}

double Seconds(double degrees)
{
    double minutes = Minutes(degrees);
    return 60*fabs(minutes - trunc(minutes));
}

wxString statusbar_pi::StatusString(PlugIn_ViewPort *vp)
{
    StatusbarConfig &c = m_config;

    wxString text = c.DisplayString;
    wxString outputtext;

    double brg = NAN, dist = NAN;
    for(unsigned int i = 0; i< text.length(); i++)
        if(text[i] == '\\') {
            if(++i >= text.length()) break;
            if(text[i] == '\\')
                outputtext += '\\';
            else if(text[i] == 'n')
                outputtext += '\n';
        } else
        if(text[i] == '%') {
            if(++i >= text.length()) break;
            wxString ipart, fpart;
            while(i < text.length()) {
                if(text[i] == '.')
                    break;
                if(text[i] >= '0' && text[i] <= '9')
                    ipart += text[i];
                else goto done;
                if(++i >= text.length()) break;
            }
            if(++i >= text.length()) break;
            while(i < text.length()) {
                if(text[i] >= '0' && text[i] <= '9')
                    fpart += text[i];
                else break;
                if(++i >= text.length()) break;
            }
        done:            
            wxString fmt = _T("%") + ipart + _T(".") + fpart + _T("f");

            bool degree = false;
            wxString units;
            double value = NAN;
            switch((char)text[i]) {
            case 'A': value = fabs(trunc(lastfix.Lat)); break;
            case 'B': value = Minutes(lastfix.Lat); break;
            case 'C': value = Seconds(lastfix.Lat); break;
            case 'D': outputtext += (lastfix.Lat > 0) ? 'N' : 'S'; break;
            case 'E': value = fabs(trunc(lastfix.Lon)); break;
            case 'F': value = Minutes(lastfix.Lon); break;
            case 'G': value = Seconds(lastfix.Lon); break;
            case 'H': outputtext += (lastfix.Lon > 0) ? 'E' : 'W'; break;
            case 'I':
                value = toUsrSpeed_Plugin(lastfix.Sog);
                units = getUsrSpeedUnit_Plugin(); break;
            case 'J': value = lastfix.Cog; degree=true; break;
            case 'K': value = lastfix.Hdt; degree=true; break;
            case 'L': value = lastfix.Hdm; degree=true; break;

            case 'O': value = fabs(trunc(m_cursor_lat)); break;
            case 'P': value = Minutes(m_cursor_lat); break;
            case 'Q': value = Seconds(m_cursor_lat); break;
            case 'R': outputtext += (m_cursor_lat > 0) ? 'N' : 'S'; break;
            case 'S': value = fabs(trunc(m_cursor_lon)); break;
            case 'T': value = Minutes(m_cursor_lon); break;
            case 'U': value = Seconds(m_cursor_lon); break;
            case 'V': outputtext += (m_cursor_lon > 0) ? 'E' : 'W'; break;

#if (API_VERSION_MAJOR == 1) && (API_VERSION_MINOR < 10)
#define DistanceBearingMercator_Plugin DistanceBearingMercator
#define DistGreatCircle_Plugin DistGreatCircle
#endif
            case 'W': {
                if(wxIsNaN(brg))
                    DistanceBearingMercator_Plugin(m_cursor_lat, m_cursor_lon,
                                                   lastfix.Lat, lastfix.Lon, &brg, &dist);
                value = brg;
                degree = true;
            } break;
            case 'X': {
                if(wxIsNaN(dist))
                    DistanceBearingMercator_Plugin(lastfix.Lat, lastfix.Lon,
                                                   m_cursor_lat, m_cursor_lon, &brg, &dist);
                value = toUsrDistance_Plugin(dist);
                units = getUsrDistanceUnit_Plugin();
            } break;
            case 'Y': {
                double gcdist = toUsrDistance_Plugin(
                    DistGreatCircle_Plugin(lastfix.Lat, lastfix.Lon, m_cursor_lat, m_cursor_lon));
                value = toUsrDistance_Plugin(gcdist);
                units = getUsrDistanceUnit_Plugin();
            } break;

            case 'Z':
                outputtext += wxString::Format(_T(" %.0f (%3.1fx)"),
                                               vp->chart_scale, 265*vp->view_scale_ppm );
                break;

            case 'a':
                value = vp->rotation * 180 / M_PI;
                break;
            case 'f':
            {
                wxDateTime now = wxDateTime::UNow();
                static wxDateTime last;
                if(last.IsValid()) {
                    static float lpfps;
                    int ms = (now - last).GetMilliseconds().ToLong();
                    if(ms < 1)
                        ms = 1;
                    float fps = 1000.0 / ms;
                    lpfps = .03 * fps + .97 * lpfps;
                    value = lpfps;
                }
                last = now;
            } break;
            case 'd':
                outputtext += wxDateTime::Now().FormatDate();
                // force refresh at midnight
                if(!m_DateRefreshTimer.IsRunning())
                    m_DateRefreshTimer.Start
                        (1000 * (24*60*60 - (wxDateTime::Now() -
                                             wxDateTime::Today()).GetSeconds().ToLong()));
                break;
            case 't':
                outputtext += wxDateTime::Now().Format(_T("%H:%M"));
                m_TimeRefreshTimer.Start(1000 * 60);
                break;
            case 'z':
                outputtext += wxDateTime::Now().Format(_T("%Z"));
                break;
            case '%': outputtext += _T("%"); break;
            }

            if(!wxIsNaN(value)) {
                long fparti;
                fpart.ToLong(&fparti);
                if(fparti == 0)
                    value = trunc(value);
                outputtext += wxString::Format(fmt, value);
                if(degree)
                    outputtext += _T("°");
                if(units.size())
                    outputtext += _T(" ") + units;
            }
        } else
            outputtext += text[i];

    return outputtext;
}

bool statusbar_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    piDC pidc(dc);
    Render(pidc, vp);
    return true;
}

bool statusbar_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    piDC pidc;
    glEnable( GL_BLEND );
    pidc.SetVP(vp);
    Render(pidc, vp);
    glDisable( GL_BLEND );
    return true;
} 

void statusbar_pi::Render(piDC &dc, PlugIn_ViewPort *vp)
{
    m_LastRefreshTime = wxDateTime::UNow();

    wxString outputtext = StatusString(vp);
    wxWindow *parent_window = GetOCPNCanvasWindow();

    StatusbarConfig &c = m_config;
        
    int px = c.XPosition;
    int py = parent_window->GetSize().y - GetYPosition();

    int width, height;
    dc.SetFont(c.font);
    dc.GetTextExtent(outputtext, &width, &height);
    py -= height;

    wxColour color = c.bgcolor;

#if wxUSE_GRAPHICS_CONTEXT
    int alpha = c.bgcolor.Alpha();
    if(alpha && dc.GetDC()) {
        wxGraphicsContext *pgc = NULL;
        wxMemoryDC *pmdc = wxDynamicCast(dc.GetDC(), wxMemoryDC);
        if( pmdc ) pgc = wxGraphicsContext::Create( *pmdc );
        else {
            wxClientDC *pcdc = wxDynamicCast(dc.GetDC(), wxClientDC);
            if( pcdc ) pgc = wxGraphicsContext::Create( *pcdc );
        }
        if(pgc) {
            pgc->SetBrush(wxColour(color.Red(), color.Green(), color.Blue(), alpha));
            pgc->DrawRectangle(px, py, width, height);
	    delete pgc;
        }
    }
#else
    else
        dc.SetTextBackground(color);
//        dc.SetBackgroundMode( wxSOLID );
#endif

    dc.SetTextForeground(c.color);
    dc.DrawText(outputtext, px, py);
}

void statusbar_pi::SetCursorLatLon(double lat, double lon)
{
    m_cursor_lat = lat, m_cursor_lon = lon;

    /* refresh but not too fast as it consumes much cpu */
    if(!m_LastRefreshTime.IsValid() ||
       (wxDateTime::UNow() - m_LastRefreshTime).GetMilliseconds().ToLong() > 400)
        RequestRefresh(GetOCPNCanvasWindow());
}

void statusbar_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    lastfix = pfix;
}

void statusbar_pi::ShowPreferencesDialog( wxWindow* parent )
{
    SaveConfig();
    if(!m_PreferencesDialog) {
        m_PreferencesDialog = new StatusbarPrefsDialog(GetOCPNCanvasWindow(), *this);
        m_PreferencesDialog->LoadConfig();
#ifdef __OCPN__ANDROID__
        m_PreferencesDialog->GetHandle()->setStyleSheet( qtStyleSheet);
#endif
    }

#ifdef __OCPN__ANDROID__
    m_PreferencesDialog->ShowModal();
    m_PreferencesDialog->SaveConfig();
#else
    m_PreferencesDialog->Show();
#endif
}

void statusbar_pi::SetColorScheme(PI_ColorScheme cs)
{
    if(m_PreferencesDialog)
        DimeWindow(m_PreferencesDialog);
    SaveConfig();
    m_ColorScheme = cs;
    LoadConfig();
}

wxString statusbar_pi::ColorSchemeName()
{
    switch(m_ColorScheme) {
    case PI_GLOBAL_COLOR_SCHEME_RGB:   return _T("RGB");
    case PI_GLOBAL_COLOR_SCHEME_DAY:   return _T("DAY");
    case PI_GLOBAL_COLOR_SCHEME_DUSK:  return _T("DUSK");
    case PI_GLOBAL_COLOR_SCHEME_NIGHT: return _T("NIGHT");
    default: return _T("UNKNOWN");
    }
}

int statusbar_pi::GetYPosition()
{
    StatusbarConfig &c = m_config;
    if(c.YPosition == -100)
        c.YPosition = GetChartbarHeight();

    return c.YPosition;
}

bool statusbar_pi::LoadConfig(void)
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return false;

    StatusbarConfig &c = m_config;

    pConf->SetPath( _T("/PlugIns/StatusBar") );

    wxString colorstr = "rgba(50, 0, 103, 1.000)";
    pConf->Read( _T("Color")+ColorSchemeName(), &colorstr, colorstr );
    c.color = wxColour(colorstr);

    pConf->Read( _T("InvertBackground")+ColorSchemeName(), &c.invertbackground, true );
    pConf->Read( _T("Blur")+ColorSchemeName(), &m_config.blur, false );

    wxString colorstrbg = "rgba(56, 228, 28, 1.000)";
    pConf->Read( _T("ColorBG")+ColorSchemeName(), &colorstrbg, colorstrbg );
    c.bgcolor = wxColour(colorstrbg);
    
    pConf->Read( _T("XPosition"), &c.XPosition, 0 );
    pConf->Read( _T("YPosition"), &c.YPosition, -100 );

    int fontsize, fontweight;
    wxString fontfacename;
    pConf->Read( _T("FontSize"), &fontsize, 18 );
    pConf->Read( _T("FontWeight"), &fontweight, wxFONTWEIGHT_NORMAL);
    pConf->Read( _T("FontFaceName"), &fontfacename, wxEmptyString );
    if(fontsize < MIN_FONT_SIZE)
        fontsize = MIN_FONT_SIZE;

    c.font = wxFont(fontsize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, (wxFontWeight)fontweight, false, fontfacename);

    pConf->Read( _T("DisplayString"), &c.DisplayString, DefaultString );

    return true;
}

static wxString ColorFormat(wxColour c)
{
    return wxString::Format(wxT("rgba(%d, %d, %d, %s)"),
                            c.Red(), c.Green(), c.Blue(),
                            wxString::FromCDouble(c.Alpha() / 255., 3));
}

bool statusbar_pi::SaveConfig(void)
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return false;

    StatusbarConfig &c = m_config;

    pConf->SetPath( _T("/PlugIns/StatusBar") );

    pConf->Write( _T("Color")+ColorSchemeName(), ColorFormat(c.color) );
    pConf->Write( _T("InvertBackground")+ColorSchemeName(), c.invertbackground);
    pConf->Write( _T("ColorBG")+ColorSchemeName(), ColorFormat(c.bgcolor) );
    pConf->Write( _T("Blur")+ColorSchemeName(), c.blur );

    pConf->Write( _T("XPosition"), c.XPosition );
    pConf->Write( _T("YPosition"), c.YPosition );

    pConf->Write( _T("FontSize"), c.font.GetPointSize() );
    pConf->Write( _T("FontWeight"), (int)c.font.GetWeight() );
    pConf->Write( _T("FontFaceName"), c.font.GetFaceName() );

    pConf->Write( _T("DisplayString"), c.DisplayString );
    
    return true;
}

void statusbar_pi::OnRefreshTimer( wxTimerEvent & )
{
    RequestRefresh(GetOCPNCanvasWindow());
}
