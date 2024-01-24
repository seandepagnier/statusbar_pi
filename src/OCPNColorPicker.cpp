/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN Color Picker Control
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) 2022 by David S. Register                               *
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <OCPNColorPicker.h>
#include <wx/colordlg.h>

#ifdef __OCPN__ANDROID__
unsigned int androidColorPicker(unsigned int initialColor);
#endif

// ============================================================================
// PI_OCPNColourPickerCtrl implementation
// ============================================================================

BEGIN_EVENT_TABLE(PI_OCPNColourPickerCtrl, wxButton)
#ifdef __WXMSW__
EVT_PAINT(PI_OCPNColourPickerCtrl::OnPaint)
#endif
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// PI_OCPNColourPickerCtrl
// ----------------------------------------------------------------------------

PI_OCPNColourPickerCtrl::PI_OCPNColourPickerCtrl(wxWindow *parent, wxWindowID id,
                                           const wxColour &initial,
                                           const wxPoint &pos,
                                           const wxSize &size, long style,
                                           const wxValidator &validator,
                                           const wxString &name) {
  Create(parent, id, initial, pos, size, style, validator, name);
}

bool PI_OCPNColourPickerCtrl::Create(wxWindow *parent, wxWindowID id,
                                  const wxColour &col, const wxPoint &pos,
                                  const wxSize &size, long style,
                                  const wxValidator &validator,
                                  const wxString &name) {
  m_bitmap = wxBitmap(60, 13);

  // create this button
  if (!wxBitmapButton::Create(parent, id, m_bitmap, pos, size,
                              style | wxBU_AUTODRAW, validator, name)) {
    wxFAIL_MSG(wxT("PI_OCPNColourPickerCtrl creation failed"));
    return false;
  }

  // and handle user clicks on it
  Connect(GetId(), wxEVT_BUTTON,
          wxCommandEventHandler(PI_OCPNColourPickerCtrl::OnButtonClick), NULL,
          this);

  m_colour = col;
  UpdateColour();
  InitColourData();

  return true;
}

void PI_OCPNColourPickerCtrl::InitColourData() {
#if 0
    ms_data.SetChooseFull(true);
    unsigned char grey = 0;
    for (int i = 0; i < 16; i++, grey += 16)
    {
        // fill with grey tones the custom colors palette
        wxColour colour(grey, grey, grey);
        ms_data.SetCustomColour(i, colour);
    }
#endif
}

void PI_OCPNColourPickerCtrl::OnButtonClick(wxCommandEvent &WXUNUSED(ev)) {
#ifdef __OCPN__ANDROID__
  unsigned int cco = 0;
  cco |= 0xff;
  cco = cco << 8;
  cco |= m_colour.Red();
  cco = cco << 8;
  cco |= m_colour.Green();
  cco = cco << 8;
  cco |= m_colour.Blue();
  unsigned int cc = androidColorPicker(cco);

  wxColour cnew;
  unsigned char blue = (unsigned char)cc % 256;
  unsigned char green = (unsigned char)(cc >> 8) % 256;
  ;
  unsigned char red = (unsigned char)(cc >> 16) % 256;
  cnew.Set(red, green, blue);

  SetColour(cnew);

#else
  // update the wxColouData to be shown in the dialog
  ms_data.SetColour(m_colour);

  // create the colour dialog and display it
  wxColourDialog dlg(this, &ms_data);
  if (dlg.ShowModal() == wxID_OK) {
    ms_data = dlg.GetColourData();
    SetColour(ms_data.GetColour());
  }
#endif
}

void PI_OCPNColourPickerCtrl::UpdateColour() {
#ifndef __OCPN__ANDROID__
  SetBitmapLabel(wxBitmap());
#endif

  wxMemoryDC dc(m_bitmap);
  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.SetBrush(wxBrush(m_colour));
  dc.DrawRectangle(0, 0, m_bitmap.GetWidth(), m_bitmap.GetHeight());

  dc.SelectObject(wxNullBitmap);
  SetBitmapLabel(m_bitmap);
}

void PI_OCPNColourPickerCtrl::SetColour(wxColour &c) {
  m_colour = c;
  m_bitmap = wxBitmap(GetSize().x - 10, GetSize().y - 10);
  UpdateColour();
}

wxColour PI_OCPNColourPickerCtrl::GetColour(void) { return m_colour; }

wxSize PI_OCPNColourPickerCtrl::DoGetBestSize() const {
  wxSize sz(wxBitmapButton::DoGetBestSize());
#ifdef __OCPN__ANDROID__
  sz.y += 12;
#else
  sz.y += 6;
#endif
  sz.x += 30;
  if (HasFlag(wxCLRP_SHOW_LABEL)) return sz;

  // if we have no label, then make this button a square
  // (like e.g. native GTK version of this control) ???
  // sz.SetWidth(sz.GetHeight());
  return sz;
}

void PI_OCPNColourPickerCtrl::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);

  int offset_x = (GetSize().x - m_bitmap.GetWidth()) / 2;
  int offset_y = (GetSize().y - m_bitmap.GetHeight()) / 2;

  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.SetBrush(wxBrush(m_colour));
  dc.DrawRectangle(offset_x, offset_y, m_bitmap.GetWidth(),
                   m_bitmap.GetHeight());

  event.Skip();
}

#ifdef __OCPN__ANDROID__

#include <QtAndroidExtras/QAndroidJniObject>

extern JavaVM *java_vm;         // found in androidUtil.cpp, accidentally exported....

bool CheckPendingJNIException() {
  JNIEnv *jenv;

  if (java_vm->GetEnv((void **)&jenv, JNI_VERSION_1_6) != JNI_OK) return true;

  if ((jenv)->ExceptionCheck() == JNI_TRUE) {
    // Handle exception here.
    (jenv)->ExceptionDescribe();  // writes to logcat
    (jenv)->ExceptionClear();

    return false;  // There was a pending exception, but cleared OK
                   // interesting discussion:
                   // http://blog.httrack.com/blog/2013/08/23/catching-posix-signals-on-android/
  }

  return false;
}

wxString callActivityMethod_is(const char *method, int parm) {
  if (CheckPendingJNIException()) return _T("NOK");
  JNIEnv *jenv;

  wxString return_string;
  QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod(
      "org/qtproject/qt5/android/QtNative", "activity",
      "()Landroid/app/Activity;");

  if (!activity.isValid()) {
    // qDebug() << "Activity is not valid";
    return return_string;
  }

  //  Call the desired method
  QAndroidJniObject data =
      activity.callObjectMethod(method, "(I)Ljava/lang/String;", parm);
  if (CheckPendingJNIException()) return _T("NOK");

  jstring s = data.object<jstring>();

  //  Need a Java environment to decode the resulting string
  if (java_vm->GetEnv((void **)&jenv, JNI_VERSION_1_6) != JNI_OK) {
    // qDebug() << "GetEnv failed.";
  } else {
    const char *ret_string = (jenv)->GetStringUTFChars(s, NULL);
    return_string = wxString(ret_string, wxConvUTF8);
  }

  return return_string;
}


#define ANDROID_EVENT_TIMER 4389
#define ACTION_NONE -1
#define ACTION_COLORDIALOG_END 4

class PI_androidUtilHandler : public wxEvtHandler {
public:
  PI_androidUtilHandler();
  ~PI_androidUtilHandler() {}

  void onTimerEvent(wxTimerEvent &event);

  wxString GetStringResult() { return m_stringResult; }

  wxTimer m_eventTimer;
  int m_action;
  bool m_done;
  wxString m_stringResult;

  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(PI_androidUtilHandler, wxEvtHandler)
EVT_TIMER(ANDROID_EVENT_TIMER, PI_androidUtilHandler::onTimerEvent)
END_EVENT_TABLE()

PI_androidUtilHandler::PI_androidUtilHandler() {
  m_eventTimer.SetOwner(this, ANDROID_EVENT_TIMER);
}


void PI_androidUtilHandler::onTimerEvent(wxTimerEvent &event) {

    // Poll the upstream Android side to detect end of ColorPicker dialog
      // qDebug() << "colorpicker poll";
      //  Get a reference to the running FileChooser
      QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod(
          "org/qtproject/qt5/android/QtNative", "activity",
          "()Landroid/app/Activity;");

      if (!activity.isValid()) {
        // qDebug() << "onTimerEvent : Activity is not valid";
        return;
      }

      //  Call the method which tracks the completion of the Intent.
      QAndroidJniObject data = activity.callObjectMethod(
          "isColorPickerDialogFinished", "()Ljava/lang/String;");

      jstring s = data.object<jstring>();

      JNIEnv *jenv;

      //  Need a Java environment to decode the resulting string
      if (java_vm->GetEnv((void **)&jenv, JNI_VERSION_1_6) != JNI_OK) {
        // qDebug() << "GetEnv failed.";
      } else {
        // The string coming back will be one of:
        //  "no"   ......Dialog not done yet.
        //  "cancel:"   .. user cancelled Dialog.
        //  "color: ".
        if (!s) {
          //qDebug() << "isColorPickerDialogFinished returned null";
        } else if ((jenv)->GetStringLength(s)) {
          const char *ret_string = (jenv)->GetStringUTFChars(s, NULL);
          // qDebug() << "isColorPickerDialogFinished returned " << ret_string;
          if (!strncmp(ret_string, "cancel:", 7)) {
            m_done = true;
            m_stringResult = _T("cancel:");
          } else if (!strncmp(ret_string, "color:", 6)) {
            m_done = true;
            m_stringResult = wxString(ret_string, wxConvUTF8);
          }
        }
      }
}

unsigned int androidColorPicker(unsigned int initialColor) {

  PI_androidUtilHandler handler;
  handler.m_eventTimer.Stop();
  handler.m_done = false;

  wxString val = callActivityMethod_is("doColorPickerDialog", initialColor);

    if (val == _T("OK")) {
      // qDebug() << "ResultOK, starting spin loop";
      handler.m_action = ACTION_COLORDIALOG_END;
      handler.m_eventTimer.Start(1000, wxTIMER_CONTINUOUS);

      //  Spin, waiting for result
      while (!handler.m_done) {
        wxMilliSleep(50);
        wxSafeYield(NULL, true);
      }

      // qDebug() << "out of spin loop";
      handler.m_action = ACTION_NONE;
      handler.m_eventTimer.Stop();

      wxString tresult = handler.GetStringResult();
      if (tresult.StartsWith(_T("cancel:"))) {
        // qDebug() << "Cancel1";
        return initialColor;
      } else if (tresult.StartsWith(_T("color:"))) {
        wxString color = tresult.AfterFirst(':');
        long a;
        color.ToLong(&a);
        unsigned int b = a;

        return b;
      }
    }
    else {
      //qDebug() << "Result NOT OK";
    }

    return 0;
}


#endif

