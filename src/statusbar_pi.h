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

#ifndef _STATUSBARPI_H_
#define _STATUSBARPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    1

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    9

#include <wx/fileconf.h>
#include "../../../include/ocpn_plugin.h"

#define STATUSBAR_TOOL_POSITION -1          // Request default positioning of toolbar tool

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

class statusbar_pi : public opencpn_plugin_19
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

    bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
    bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
    void SetCursorLatLon(double lat, double lon);
    void SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix);
    void ShowPreferencesDialog( wxWindow* parent );

private:
    wxWindow *m_parent_window;
    wxFileConfig     *m_pConfig;

    double cur_lat, cur_lon;
    PlugIn_Position_Fix_Ex lastfix;

    bool LoadConfig( void );
    bool SaveConfig( void );
};

#endif
