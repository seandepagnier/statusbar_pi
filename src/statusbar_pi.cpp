/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Status Bar Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Sean D'Epagnier
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

#include <wx/wx.h>

#include "StatusbarUI.h"
#include "TexFont.h"
#include "statusbar_pi.h"
#include "icons.h"

# if !defined(NAN)
# define NAN std::numeric_limits<double>::quiet_NaN ()
# endif

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new statusbar_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


//-----------------------------------------------------------------------------
//
//    StatusBar PlugIn Implementation
//
//-----------------------------------------------------------------------------

statusbar_pi::statusbar_pi(void *ppimgr)
    :opencpn_plugin_19(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
}

int statusbar_pi::Init(void)
{
    m_PreferencesDialog = new StatusbarPrefsDialog(GetOCPNCanvasWindow());
    AddLocaleCatalog( _T("opencpn-statusbar_pi") );

    //    Get a pointer to the opencpn configuration object
    m_pConfig = GetOCPNConfigObject();

    // This is bugged, need to fix in core program so we know the scheme at start
    m_ColorScheme = PI_GLOBAL_COLOR_SCHEME_RGB;
    LoadConfig();

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
The statusbar plugin improves on some of these difficulties.\
  Best used with OpenGL enabled (requires some basic OpenGL extensions).\n");
}

wxString DefaultString = _T("Ship %02A %2.4B %D   %02E %2.4F %H   SOG %.2I  COG %03J    \
%02O %2.4P %R   %02S %2.4T %V   %03W  %.1X    Scale %Z");
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
    wxString text = m_PreferencesDialog->m_tDisplayString->GetValue();
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
            switch(text[i]) {
            case 'A': value = fabs(lastfix.Lat); break;
            case 'B': value = Minutes(lastfix.Lat); break;
            case 'C': value = Seconds(lastfix.Lat); break;
            case 'D': outputtext += (lastfix.Lat > 0) ? 'N' : 'S'; break;
            case 'E': value = fabs(lastfix.Lon); break;
            case 'F': value = Minutes(lastfix.Lon); break;
            case 'G': value = Seconds(lastfix.Lon); break;
            case 'H': outputtext += (lastfix.Lon > 0) ? 'E' : 'W'; break;
            case 'I':
                value = toUsrSpeed_Plugin(lastfix.Sog);
                units = getUsrSpeedUnit_Plugin(); break;
            case 'J': value = lastfix.Cog; degree=true; break;
            case 'K': value = lastfix.Hdt; degree=true; break;
            case 'L': value = lastfix.Hdm; degree=true; break;

            case 'O': value = fabs(m_cursor_lat); break;
            case 'P': value = Minutes(m_cursor_lat); break;
            case 'Q': value = Seconds(m_cursor_lat); break;
            case 'R': outputtext += (m_cursor_lat > 0) ? 'N' : 'S'; break;
            case 'S': value = fabs(m_cursor_lon); break;
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
                    float fps = 1000.0 / (now - last).GetMilliseconds().ToLong();
                    lpfps = .03 * fps + .97 * lpfps;
                    value = lpfps;
                }
                last = now;
            } break;
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
    wxString outputtext = StatusString(vp);
    wxWindow *parent_window = GetOCPNCanvasWindow();
    
    int px = m_PreferencesDialog->m_sXPosition->GetValue();
    int py = parent_window->GetSize().y - m_PreferencesDialog->m_sYPosition->GetValue();

    int width, height;
    dc.SetFont(m_PreferencesDialog->m_fontPicker->GetSelectedFont());
    dc.GetTextExtent(outputtext, &width, &height);
    py -= height;

    wxColour color = m_PreferencesDialog->m_colourPickerBG->GetColour();
    int alpha = 255 - m_PreferencesDialog->m_sTransparencyBG->GetValue();

    if(alpha) {
#if wxUSE_GRAPHICS_CONTEXT
        wxWindowDC *wdc = dynamic_cast<wxWindowDC*>(&dc);
        wxGraphicsContext *pgc = wxGraphicsContext::Create( *wdc );
        pgc->SetBrush(wxColour(color.Red(), color.Green(), color.Blue(), alpha));
        pgc->DrawRectangle(px, py, width, height);
#else
        dc.SetTextBackground(color);
        dc.SetBackgroundMode( wxSOLID );
#endif
    }

    dc.SetTextForeground(m_PreferencesDialog->m_colourPicker->GetColour());
    dc.DrawText(outputtext, px, py);

    return true;
}

bool statusbar_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    wxString outputtext = StatusString(vp);
    wxWindow *parent_window = GetOCPNCanvasWindow();

    BuildFont();

    int px = m_PreferencesDialog->m_sXPosition->GetValue();
    int py = parent_window->GetSize().y - m_PreferencesDialog->m_sYPosition->GetValue();

    int height;
    m_texfont.GetTextExtent(outputtext, 0, &height);
    py -= height;

    glEnable( GL_BLEND );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    wxColour color = m_PreferencesDialog->m_colourPickerBG->GetColour();
    int alpha = 255 - m_PreferencesDialog->m_sTransparencyBG->GetValue();

    glColor4ub(color.Red(), color.Green(), color.Blue(), alpha);
    m_texfont.RenderString(outputtext, px, py);

    glEnable( GL_TEXTURE_2D );
    if(m_PreferencesDialog->m_cbInvertBackground->GetValue()) {
        glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

        m_texfont.RenderString(outputtext, px, py);
    }

    color = m_PreferencesDialog->m_colourPicker->GetColour();
    alpha = 255 - m_PreferencesDialog->m_sTransparency->GetValue();
    glColor4ub(color.Red(), color.Green(), color.Blue(), alpha);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    m_texfont.RenderString(outputtext, px, py);

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );

    return true;
}

void statusbar_pi::SetCursorLatLon(double lat, double lon)
{
    m_cursor_lat = lat, m_cursor_lon = lon;
}

void statusbar_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    lastfix = pfix;
}

void statusbar_pi::ShowPreferencesDialog( wxWindow* parent )
{
    for(int i=0; i<4; i++)
        parent = parent->GetParent();

    wxDialog *dlg = dynamic_cast<wxDialog*>(parent);
    if(dlg)
        dlg->EndModal(wxID_OK);

    m_PreferencesDialog->Show();
}

void statusbar_pi::SetColorScheme(PI_ColorScheme cs)
{
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

bool statusbar_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pConfig;

    if(!pConf)
        return false;

    pConf->SetPath( _T("/PlugIns/StatusBar") );

    wxString colorstr = m_PreferencesDialog->m_colourPicker->GetColour().GetAsString();
    pConf->Read( _T("Color")+ColorSchemeName(), &colorstr, colorstr );
    m_PreferencesDialog->m_colourPicker->SetColour(wxColour(colorstr));
        
    bool invertbackground = true;
    pConf->Read( _T("InvertBackground")+ColorSchemeName(), &invertbackground, invertbackground );
    m_PreferencesDialog->m_cbInvertBackground->SetValue(invertbackground);

    bool blur = true;
    pConf->Read( _T("Blur")+ColorSchemeName(), &blur, blur );
    m_PreferencesDialog->m_cbBlur->SetValue(blur);
    
    int transparency = 96;
    pConf->Read( _T("Transparency")+ColorSchemeName(), &transparency, transparency );
    m_PreferencesDialog->m_sTransparency->SetValue(transparency);

    wxString colorstrbg = m_PreferencesDialog->m_colourPickerBG
        ->GetColour().GetAsString();
    pConf->Read( _T("ColorBG")+ColorSchemeName(), &colorstrbg, colorstrbg );
    m_PreferencesDialog->m_colourPickerBG->SetColour(wxColour(colorstrbg));

    int transparencybg = 180;
    pConf->Read( _T("TransparencyBG")+ColorSchemeName(), &transparencybg, transparencybg );
    m_PreferencesDialog->m_sTransparencyBG->SetValue(transparencybg);
    
    int XPosition = 0;
    pConf->Read( _T("XPosition"), &XPosition, XPosition );
    m_PreferencesDialog->m_sXPosition->SetValue(XPosition);

    int YPosition = 0;
    pConf->Read( _T("YPosition"), &YPosition, YPosition );
    m_PreferencesDialog->m_sYPosition->SetValue(YPosition);
    
    int fontsize = 18;
    pConf->Read( _T("FontSize"), &fontsize, fontsize );
    int fontweight = wxNORMAL;
    pConf->Read( _T("FontWeight"), &fontweight, fontweight );
    wxString fontfacename;
    pConf->Read( _T("FontFaceName"), &fontfacename, fontfacename );
    
    wxFont font(fontsize, wxDEFAULT, wxNORMAL, fontweight, false, fontfacename);
    m_PreferencesDialog->m_fontPicker->SetSelectedFont(font);

    wxString DisplayString = DefaultString;
    pConf->Read( _T("DisplayString"), &DisplayString, DisplayString );
    m_PreferencesDialog->m_tDisplayString->SetValue(DisplayString);
    
    return true;
}

bool statusbar_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pConfig;

    if(!pConf)
        return false;

    pConf->SetPath( _T("/PlugIns/StatusBar") );
    
    pConf->Write( _T("Color")+ColorSchemeName(),
                  m_PreferencesDialog->m_colourPicker->GetColour().GetAsString() );
    pConf->Write( _T("InvertBackground")+ColorSchemeName(),
                  m_PreferencesDialog->m_cbInvertBackground->GetValue() );
    pConf->Write( _T("ColorBG")+ColorSchemeName(), m_PreferencesDialog->m_colourPickerBG
                  ->GetColour().GetAsString() );
    pConf->Write( _T("TransparencyBG")+ColorSchemeName(), m_PreferencesDialog
                  ->m_sTransparencyBG->GetValue() );
    pConf->Write( _T("Blur")+ColorSchemeName(), m_PreferencesDialog->m_cbBlur->GetValue() );
    pConf->Write( _T("Transparency")+ColorSchemeName(),
                  m_PreferencesDialog->m_sTransparency->GetValue());

    pConf->Write( _T("XPosition"), m_PreferencesDialog->m_sXPosition->GetValue() );
    pConf->Write( _T("YPosition"), m_PreferencesDialog->m_sYPosition->GetValue() );
    wxFont font = m_PreferencesDialog->m_fontPicker->GetSelectedFont();
    pConf->Write( _T("FontSize"), font.GetPointSize() );
    pConf->Write( _T("FontWeight"), font.GetWeight() );
    pConf->Write( _T("FontFaceName"), font.GetFaceName() );
    
    pConf->Write( _T("DisplayString"), m_PreferencesDialog->m_tDisplayString->GetValue() );
    
    return true;
}

void statusbar_pi::BuildFont()
{
    wxFont font = m_PreferencesDialog->m_fontPicker->GetSelectedFont();
    bool blur = m_PreferencesDialog->m_cbBlur->GetValue();

    m_texfont.Build(font, blur);
}
