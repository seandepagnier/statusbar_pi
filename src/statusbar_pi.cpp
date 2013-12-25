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
#include "statusbar_pi.h"
#include "icons.h"

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
It is also useful to customize the display\n");
}

bool statusbar_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    return true;
}

bool statusbar_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    wxString text;

    double brg, dist;
    DistanceBearingMercator_Plugin(lastfix.Lat, lastfix.Lon, cur_lat, cur_lon, &brg, &dist);
//    double gcdist = DistGreatCircle_Plugin(lastfix.Lat, lastfix.Lon, cur_lat, cur_lon);


    text.Printf(_T("Ship %02.0f %05.2f %c  %02.0f %05.2f %c  SOG %03.1f kts  COG %03.0f    \
%03.0f° %.1f %s"),
                fabs(lastfix.Lat), 60*fabs(lastfix.Lat - trunc(lastfix.Lat)), lastfix.Lat >= 0 ? 'N' : 'S',
                fabs(lastfix.Lon), 60*fabs(lastfix.Lon - trunc(lastfix.Lon)), lastfix.Lon >= 0 ? 'E' : 'W',
                lastfix.Sog, lastfix.Cog,
                brg, dist, "NMi");
    int fontsize = 20;

    wxBitmap tbmp(200, 200);
    wxMemoryDC dc(tbmp);

    wxFont font( fontsize, wxDEFAULT, wxNORMAL, wxBOLD );
    dc.SetFont( font );

    int w, h;
    dc.GetTextExtent(text, &w, &h, NULL, NULL, &font);

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

    image = image.Blur(1);

    char *data = new char[2 * w * h];
    unsigned char *imgdata = image.GetData();
    for( int i = 0; i < w*h; i++ ) {
        data[2*i+0] = imgdata[3*i+0];
        data[2*i+1] = imgdata[3*i+0];
    }

    glColor4ub(60, 150, 0, 200);

    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE_ALPHA, w, h, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
    delete [] data;

    glEnable(GL_TEXTURE_RECTANGLE_ARB);

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    m_parent_window = GetOCPNCanvasWindow();
    int x = 0, y = m_parent_window->GetSize().y - h - fontsize;
    glBegin( GL_QUADS );
    glTexCoord2f( 0, 0 ), glVertex2i( x, y );
    glTexCoord2f( w, 0 ), glVertex2i( x + w, y );
    glTexCoord2f( w, h ), glVertex2i( x + w, y + h );
    glTexCoord2f( 0, h ), glVertex2i( x, y + h );
    glEnd();

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

}


bool statusbar_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pConfig;

    if(pConf)
    {
        pConf->SetPath( _T("/PlugIns/StatusBar") );

        return true;
    }
    else
        return false;
}

bool statusbar_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pConfig;

    if(pConf)
    {
        pConf->SetPath( _T("/PlugIns/StatusBar") );

        return true;
    }
    else
        return false;
}

