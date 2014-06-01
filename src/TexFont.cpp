/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  texture OpenGL text rendering built from wxFont
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 Sean D'Epagnier                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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
 **************************************************************************/

#ifdef ocpnUSE_GL

#include <wx/wx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "TexFont.h"

void TexFont::Build( wxFont &font, bool blur, bool luminance )
{
    /* avoid rebuilding if the parameters are the same */
    if(font == m_font && blur == m_blur)
        return;
    
    m_font = font;
    m_blur = blur;

    wxBitmap bmp(256, 256);
    wxMemoryDC dc(bmp);

    dc.SetFont( font );

    int maxglyphw = 0, maxglyphh = 0;
    for( int i = MIN_GLYPH; i < MAX_GLYPH; i++ ) {
        wxCoord gw, gh;
        wxString text;
        if(i == DEGREE_GLYPH)
            text = _T("°");
        else
            text = wxString::Format(_T("%c"), i);
        wxCoord descent, exlead;
        dc.GetTextExtent( text, &gw, &gh, &descent, &exlead, &font ); // measure the text

        tgi[i].width = gw;
        tgi[i].height = gh;

        tgi[i].advance = gw;

        maxglyphw = wxMax(gw, maxglyphw);
        maxglyphh = wxMax(gh, maxglyphh);
    }

    /* add extra pixel to give a border between rows of characters
       without this, in some cases a faint line can be see on the edge
       from the character above */
    maxglyphh++;

    int w = COLS_GLYPHS * maxglyphw;
    int h = ROWS_GLYPHS * maxglyphh;

    wxASSERT(w < 2048 && h < 2048);

    /* make power of 2 */
    for(tex_w = 1; tex_w < w; tex_w *= 2);
    for(tex_h = 1; tex_h < h; tex_h *= 2);

    wxBitmap tbmp(tex_w, tex_h);
    dc.SelectObject(tbmp);

    /* fill bitmap with black */
    dc.SetBackground( wxBrush( wxColour( 0, 0, 0 ) ) );
    dc.Clear();
        
    /* draw the text white */
    dc.SetTextForeground( wxColour( 255, 255, 255 ) );

    int row = 0, col = 0;
    for( int i = MIN_GLYPH; i < MAX_GLYPH; i++ ) {
        if(col == COLS_GLYPHS) {
            col = 0;
            row++;
        }

        tgi[i].x = col * maxglyphw;
        tgi[i].y = row * maxglyphh;

        wxString text;
        if(i == DEGREE_GLYPH)
            text = _T("°");
        else
            text = wxString::Format(_T("%c"), i);

        dc.DrawText(text, tgi[i].x, tgi[i].y );
        col++;
    }

    wxImage image = tbmp.ConvertToImage();

    GLuint format, internalformat;
    int stride;

    if(luminance) {
        format = GL_LUMINANCE_ALPHA;
        stride = 2;
    } else {
        format = GL_ALPHA;
        stride = 1;
    }

    internalformat = format;

    if( m_blur )
        image = image.Blur(1);

    unsigned char *imgdata = image.GetData();
    unsigned char *teximage = (unsigned char *) malloc( stride * tex_w * tex_h );

    for( int j = 0; j < tex_w*tex_h; j++ )
        for( int k = 0; k < stride; k++ )
            teximage[j * stride + k] = imgdata[3*j];

    if(texobj)
        Delete();

    glGenTextures( 1, &texobj );
    glBindTexture( GL_TEXTURE_2D, texobj );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glTexImage2D( GL_TEXTURE_2D, 0, internalformat, tex_w, tex_h, 0,
                  format, GL_UNSIGNED_BYTE, teximage );

    free(teximage);
}

void TexFont::Delete( )
{
    glDeleteTextures(1, &texobj);
    texobj = 0;
}

void TexFont::GetTextExtent(const char *string, int len, int *width, int *height)
{
    int w=0, h=0;

    for(int i = 0; i < len; i++ ) {
        unsigned char c = string[i];
        if(c == '\n') {
            h += tgi[(int)'A'].height;
            continue;
        }
        if(c == 0xc2 && (unsigned char)string[i+1] == 0xb0) {
            c = DEGREE_GLYPH;
            i++;
        }
        if( c < MIN_GLYPH || c >= MAX_GLYPH)
            continue;

        TexGlyphInfo &tgisi = tgi[c];

        w += tgisi.advance;
        if(tgisi.height > h)
            h = tgisi.height;
    }
    if(width) *width = w;
    if(height) *height = h;
}

void TexFont::GetTextExtent(const wxString &string, int *width, int *height)
{
    GetTextExtent(string.ToUTF8(), string.size(), width, height);
}

void TexFont::RenderGlyph( int c )
{
    if( c < MIN_GLYPH || c >= MAX_GLYPH)
        return;

    TexGlyphInfo &tgic = tgi[c];

    int x = tgic.x, y = tgic.y;
    float w = tgic.width, h = tgic.height;
    float tx1 = (float)x / tex_w;
    float tx2 = (float)(x + w) / tex_w;
    float ty1 = (float)y / tex_h;
    float ty2 = (float)(y + h) / tex_h;

    glBegin( GL_QUADS );

    glTexCoord2f( tx1, ty1 );  glVertex2i( 0, 0 );
    glTexCoord2f( tx2, ty1 );  glVertex2i( w, 0 );
    glTexCoord2f( tx2, ty2 );  glVertex2i( w, h );
    glTexCoord2f( tx1, ty2 );  glVertex2i( 0, h );

    glEnd();
    glTranslatef( tgic.advance, 0.0, 0.0 );
}

void TexFont::RenderString( const char *string, int x, int y )
{
    glPushMatrix();
    glTranslatef(x, y, 0);

    glPushMatrix();
    glBindTexture( GL_TEXTURE_2D, texobj);

    for( int i = 0; string[i]; i++ ) {
        if(string[i] == '\n') {
            glPopMatrix();
            glTranslatef(0, tgi[(int)'A'].height, 0);
            glPushMatrix();
            continue;
        }
        /* degree symbol */
        if((unsigned char)string[i] == 0xc2 &&
           (unsigned char)string[i+1] == 0xb0) {
            RenderGlyph( DEGREE_GLYPH );
            i++;
            continue;
        }
        RenderGlyph( string[i] );
    }

    glPopMatrix();
    glPopMatrix();
}

void TexFont::RenderString( const wxString &string, int x, int y )
{
    RenderString((const char*)string.ToUTF8(), x, y);
}

#endif     //#ifdef ocpnUSE_GL