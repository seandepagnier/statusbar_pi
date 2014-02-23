/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Status Bar Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier
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


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <typeinfo>

#include "StatusbarUI.h"
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




//---------------------------------------------------------------------------------------------------------
//
//    StatusBar PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

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

    //    And load the configuration items
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
  It also uses excessive cpu to operate (under gtk).\n\
\n\
The statusbar plugin improves on some of these difficulties.\
  Best used with OpenGL enabled (requires some basic OpenGL extensions).\n");
}

const int num_font_chars = 256;

struct font_char
{
    int w, h;
    unsigned int tex;
    bool newline;

    void Build(wxFont &font, int i, bool blur) {
        wxString text = wxString::Format(_T("%c"), i);
        wxBitmap tbmp(200, 200);
        wxMemoryDC dc(tbmp);

        dc.SetFont( font );

        dc.GetTextExtent(text, &w, &h, NULL, NULL, &font);

        newline = false;
        if(i == ' ')
            w /= 2; /* skinny spaces to make display have more option */
        if(i == '\n') {
            h /= 3;
            newline = true;
        } else
        if(!w || !h)
            tex = 0;
        else {

            if(tex)
                glDeleteTextures(1, &tex);

        glGenTextures(1, &tex);

        wxBitmap bmp( w, h );
        dc.SelectObject( bmp );
        
        /* fill bitmap with black */
        dc.SetBackground( wxBrush( wxColour( 0, 0, 0 ) ) );
        dc.Clear();
        
        /* draw the text white */
        dc.SetTextForeground( wxColour( 255, 255, 255 ) );
        dc.DrawText(text, 0, 0 );
        dc.SelectObject( wxNullBitmap );
    
        wxImage image = bmp.ConvertToImage();

        if(blur)
            image = image.Blur(1);

        char *data = new char[2 * w * h];
        unsigned char *imgdata = image.GetData();
        for( int i = 0; i < w*h; i++ ) {
            data[2*i+0] = imgdata[3*i+0];
            data[2*i+1] = imgdata[3*i+0];
        }
        
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, tex);
        glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE_ALPHA, w, h, 0,
                     GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
        delete [] data;
        }
    }

    void Render() {
        if(newline) { /* newline */
            glPopMatrix();
            glTranslated(0, h, 0);
            glPushMatrix();
            return;
        }

        if(!tex)
            return;
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, tex);

        glBegin( GL_QUADS );
        glTexCoord2f( 0, 0 ), glVertex2i( 0, -h );
        glTexCoord2f( w, 0 ), glVertex2i( w, -h );
        glTexCoord2f( w, h ), glVertex2i( w,  0 );
        glTexCoord2f( 0, h ), glVertex2i( 0,  0 );
        glEnd();
        
        glTranslated(w, 0, 0);
    }

} g_fontTex[num_font_chars];


wxString DefaultString = _T("Ship %02A %2.4B %D   %02E %2.4F %H   SOG %.2I kts  COG %03J    \
%02O %2.4P %R   %02S %2.4T %V   %03W  %.0X NMi    Scale %Z");
void StatusbarPrefsDialog::OnBuiltinString( wxCommandEvent& event )
{
    wxString OwnshipString = _T("Ship %02A %2.4B %D   %02E %2.4F %H   SOG %.2I kts  COG %03J");
    wxString MultilineString = _T("%02A %2.2B%D  %02E %2.2F%H  %.1I kts %03J\
\\n%02O %2.2P%R %02S %2.2T%V %03W %.2X NMi %03.a");
    
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

wxString statusbar_pi::RenderString(PlugIn_ViewPort *vp)
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
            case 'I': value = lastfix.Sog; break;
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
                value = dist;
            } break;
            case 'Y': {
                value = DistGreatCircle_Plugin(lastfix.Lat, lastfix.Lon, m_cursor_lat, m_cursor_lon);
            } break;

            case 'Z':
                outputtext += wxString::Format(_T(" %.0f (%3.1fx)"),
                                               vp->chart_scale, vp->view_scale_ppm*265 );
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
                    lpfps = .1 * fps + .9 * fps;
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
            }
        } else
            outputtext += text[i];

    return outputtext;
}

bool statusbar_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    wxString outputtext = RenderString(vp);
    wxWindow *parent_window = GetOCPNCanvasWindow();
    
    dc.SetTextForeground(m_PreferencesDialog->m_colourPicker->GetColour());
    dc.SetBackgroundMode( wxTRANSPARENT );
    dc.SetFont(m_PreferencesDialog->m_fontPicker->GetSelectedFont());

    int xp = m_PreferencesDialog->m_sXPosition->GetValue();
    int yp = parent_window->GetSize().y - m_PreferencesDialog->m_sYPosition->GetValue();

    dc.DrawText(outputtext, xp, yp);

    return true;
}

bool statusbar_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    wxString outputtext = RenderString(vp);
    wxWindow *parent_window = GetOCPNCanvasWindow();

    glEnable(GL_TEXTURE_RECTANGLE_ARB);

    glEnable( GL_BLEND );

    if(m_PreferencesDialog->m_cbInvertBackground->GetValue()) {
        glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

        glPushMatrix();
        glTranslated(m_PreferencesDialog->m_sXPosition->GetValue(),
                     parent_window->GetSize().y - m_PreferencesDialog->m_sYPosition->GetValue(), 0);
        DrawString(outputtext);
        glPopMatrix();
    }

    wxColour color = m_PreferencesDialog->m_colourPicker->GetColour();
    int alpha = 255 - m_PreferencesDialog->m_sTransparency->GetValue();
    glColor4ub(color.Red(), color.Green(), color.Blue(), alpha);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glPushMatrix();
    glTranslated(m_PreferencesDialog->m_sXPosition->GetValue(),
                 parent_window->GetSize().y - m_PreferencesDialog->m_sYPosition->GetValue(), 0);
    DrawString(outputtext);
    glPopMatrix();

    glDisable( GL_TEXTURE_RECTANGLE_ARB );
    glDisable( GL_BLEND );

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

bool statusbar_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pConfig;

    if(!pConf)
        return false;

    pConf->SetPath( _T("/PlugIns/StatusBar") );

    wxString colorstr = m_PreferencesDialog->m_colourPicker->GetColour().GetAsString();
    pConf->Read( _T("Color"), &colorstr, colorstr );
    m_PreferencesDialog->m_colourPicker->SetColour(wxColour(colorstr));
    
    int fontsize = 18;
    pConf->Read( _T("FontSize"), &fontsize, fontsize );
    int fontweight = wxNORMAL;
    pConf->Read( _T("FontWeight"), &fontweight, fontweight );
    wxString fontfacename;
    pConf->Read( _T("FontFaceName"), &fontfacename, fontfacename );
    
    wxFont font(fontsize, wxDEFAULT, wxNORMAL, fontweight, false, fontfacename);
    m_PreferencesDialog->m_fontPicker->SetSelectedFont(font);
    
    bool invertbackground = true;
    pConf->Read( _T("InvertBackground"), &invertbackground, invertbackground );
    m_PreferencesDialog->m_cbInvertBackground->SetValue(invertbackground);

    bool blur = true;
    pConf->Read( _T("Blur"), &blur, blur );
    m_PreferencesDialog->m_cbBlur->SetValue(blur);
    
    int transparency = 96;
    pConf->Read( _T("Transparency"), &transparency, transparency );
    m_PreferencesDialog->m_sTransparency->SetValue(transparency);
    
    int XPosition = 0;
    pConf->Read( _T("XPosition"), &XPosition, XPosition );
    m_PreferencesDialog->m_sXPosition->SetValue(XPosition);

    int YPosition = 0;
    pConf->Read( _T("YPosition"), &YPosition, YPosition );
    m_PreferencesDialog->m_sYPosition->SetValue(YPosition);
    
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

    pConf->Write( _T("Color"), m_PreferencesDialog->m_colourPicker->GetColour().GetAsString() );

    wxFont font = m_PreferencesDialog->m_fontPicker->GetSelectedFont();
    pConf->Write( _T("FontSize"), font.GetPointSize() );
    pConf->Write( _T("FontWeight"), font.GetWeight() );
    pConf->Write( _T("FontFaceName"), font.GetFaceName() );
    
    pConf->Write( _T("InvertBackground"), m_PreferencesDialog->m_cbInvertBackground->GetValue() );
    pConf->Write( _T("Blur"), m_PreferencesDialog->m_cbBlur->GetValue() );
    pConf->Write( _T("Transparency"), m_PreferencesDialog->m_sTransparency->GetValue() );
    pConf->Write( _T("XPosition"), m_PreferencesDialog->m_sXPosition->GetValue() );
    pConf->Write( _T("YPosition"), m_PreferencesDialog->m_sYPosition->GetValue() );
    
    pConf->Write( _T("DisplayString"), m_PreferencesDialog->m_tDisplayString->GetValue() );
    
    return true;
}

void statusbar_pi::BuildFont()
{
    wxFont font = m_PreferencesDialog->m_fontPicker->GetSelectedFont();
    bool blur = m_PreferencesDialog->m_cbBlur->GetValue();

    if(font == m_font && blur == m_blur)
        return;

    m_font = font;
    m_blur = blur;

    for(int t=0; t<num_font_chars; t++)
        g_fontTex[t].Build(font, t, blur);
}

void statusbar_pi::DrawString(wxString str)
{
    BuildFont();

    glPushMatrix();
    glPushMatrix();
    for(unsigned int i=0; i<str.length(); i++)
        g_fontTex[str.GetChar(i)].Render();
    glPopMatrix();
    glPopMatrix();
}
