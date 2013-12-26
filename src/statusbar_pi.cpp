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
    return _("StatusBar replaces builtin statusbar");
}

wxString statusbar_pi::GetLongDescription()
{
    return _("StatusBar replaces builtin statusbar because it is\n\
very inefficient (on linux and gtk) and also very difficult to read.\n\
It is also possible to customize the display,\
  Requires OpenGL and some extensions.\n");
}

bool statusbar_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    return false;
}

const int num_font_chars = 256;

struct font_char
{
    int w, h;
    unsigned int tex;

    void Build(wxFont &font, int i, bool blur) {

        wxString text = wxString::Format(_T("%c"), i);
        wxBitmap tbmp(200, 200);
        wxMemoryDC dc(tbmp);

        dc.SetFont( font );

        dc.GetTextExtent(text, &w, &h, NULL, NULL, &font);

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

void StatusbarPrefsDialog::OnDisplayStringInfo( wxCommandEvent& event )
{
    wxMessageDialog dlg( GetOCPNCanvasWindow(), _("\
Display String\n\
the following are formats:\n\
%1 ship lat lon\n\
%2 ship sog\n\
%3 ship cog\n\
%4 ship heading\n\
%5 ship heading rate\n\
%6 cursor lat lon\n\
%7 bearing to cursor\n\
%8 distance to cursor mercator\n\
%9 distance to cursor great circle\n\
%A chart scale\n\
%% print a percent"), _("Statusbar Information"), wxOK | wxICON_INFORMATION );
    dlg.ShowModal();
}

wxString LLString(double lat, double lon)
{
    return wxString::Format(_T("%02.0f %05.2f %c  %02.0f %05.2f %c"),
                            trunc(fabs(lat)),
                            60*fabs(lat - trunc(lat)), lat >= 0 ? 'N' : 'S',
                            trunc(fabs(lon)),
                            60*fabs(lon - trunc(lon)), lon >= 0 ? 'E' : 'W');
}

bool statusbar_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    wxString text = m_PreferencesDialog->m_tDisplayString->GetValue();
    wxString outputtext;

    double brg = NAN, dist = NAN;
    for(unsigned int i = 0; i< text.length(); i++)
        if(text[i] == '%') {
            i++;
            switch(text[i++]) {
            case '1': outputtext += LLString(lastfix.Lat, lastfix.Lon); break;
            case '2': outputtext += wxString::Format(_T("%03.1f"), lastfix.Sog); break;
            case '3': outputtext += wxString::Format(_T("%03.0f°"), lastfix.Cog); break;
            case '4': outputtext += wxString::Format(_T("%03.0f°"), lastfix.Hdt); break;
            case '5': outputtext += wxString::Format(_T("%03.0f°"), lastfix.Hdm); break;
            case '6': outputtext += LLString(cur_lat, cur_lon); break;
            case '7': {
                if(wxIsNaN(brg))
                    DistanceBearingMercator_Plugin(cur_lat, cur_lon, lastfix.Lat, lastfix.Lon, &brg, &dist);
                outputtext += wxString::Format(_T("%03.0f°"), brg);
            } break;
            case '8': {
                if(wxIsNaN(dist))
                    DistanceBearingMercator_Plugin(lastfix.Lat, lastfix.Lon, cur_lat, cur_lon, &brg, &dist);
                outputtext += wxString::Format(_T("%.1f"), dist);
            } break;
            case '9': {
                double gcdist = DistGreatCircle_Plugin(lastfix.Lat, lastfix.Lon, cur_lat, cur_lon);
                outputtext += wxString::Format(_T("%.1f"), gcdist);
            } break;
            case 'A':
                outputtext += wxString::Format(_T(" %.0f (%3.1fx)"),
                                               vp->chart_scale, vp->view_scale_ppm*265 );
                break;
            case '%': outputtext += _T("%"); break;
            }
        } else
            outputtext += text[i];

    wxColour color = m_PreferencesDialog->m_colourPicker->GetColour();
    int alpha = 255 - m_PreferencesDialog->m_sTransparency->GetValue();
    glColor4ub(color.Red(), color.Green(), color.Blue(), alpha);

    glEnable(GL_TEXTURE_RECTANGLE_ARB);

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    wxWindow *parent_window = GetOCPNCanvasWindow();

    glPushMatrix();
    glTranslated(m_PreferencesDialog->m_sXPosition->GetValue(),
                 parent_window->GetSize().y - m_PreferencesDialog->m_sYPosition->GetValue(), 0);

    DrawString(outputtext);
    glPopMatrix();

    glDisable( GL_BLEND );
    glDisable(GL_TEXTURE_RECTANGLE_ARB);

    return true;
}

void statusbar_pi::SetCursorLatLon(double lat, double lon)
{
    cur_lat = lat, cur_lon = lon;
}

void statusbar_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    lastfix = pfix;
}

void statusbar_pi::ShowPreferencesDialog( wxWindow* parent )
{
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
    
    wxFont font(fontsize, wxDEFAULT, wxNORMAL, fontweight, fontfacename);
    m_PreferencesDialog->m_fontPicker->SetSelectedFont(font);
    
    bool blur = true;
    pConf->Read( _T("Blur"), &blur, blur );
    m_PreferencesDialog->m_cbBlur->SetValue(blur);
    
    int transparency = 64;
    pConf->Read( _T("Transparency"), &transparency, transparency );
    m_PreferencesDialog->m_sTransparency->SetValue(transparency);
    
    int XPosition = 0;
    pConf->Read( _T("XPosition"), &XPosition, XPosition );
    m_PreferencesDialog->m_sXPosition->SetValue(XPosition);

    int YPosition = 0;
    pConf->Read( _T("YPosition"), &YPosition, YPosition );
    m_PreferencesDialog->m_sYPosition->SetValue(YPosition);
    
    wxString DisplayString = m_PreferencesDialog->m_tDisplayString->GetValue();
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
    for(unsigned int i=0; i<str.length(); i++)
        g_fontTex[str.GetChar(i)].Render();
    glPopMatrix();
}
