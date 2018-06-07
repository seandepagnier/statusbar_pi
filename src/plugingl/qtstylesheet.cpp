#ifdef __OCPN__ANDROID__
#include <qdebug.h>

QString qtStyleSheet = "\
QScrollBar:horizontal {\
border: 0px solid grey;\
background-color: rgb(240, 240, 240);\
height: 35px;\
margin: 0px 1px 0 1px;\
}\
QScrollBar::handle:horizontal {\
background-color: rgb(200, 200, 200);\
min-width: 20px;\
border-radius: 10px;\
}\
QScrollBar::add-line:horizontal {\
border: 0px solid grey;\
background: #32CC99;\
width: 0px;\
subcontrol-position: right;\
subcontrol-origin: margin;\
}\
QScrollBar::sub-line:horizontal {\
border: 0px solid grey;\
background: #32CC99;\
width: 0px;\
subcontrol-position: left;\
subcontrol-origin: margin;\
}\
QScrollBar:vertical {\
border: 0px solid grey;\
background-color: rgb(240, 240, 240);\
width: 35px;\
margin: 1px 0px 1px 0px;\
}\
QScrollBar::handle:vertical {\
background-color: rgb(200, 200, 200);\
min-height: 20px;\
border-radius: 10px;\
}\
QScrollBar::add-line:vertical {\
border: 0px solid grey;\
background: #32CC99;\
height: 0px;\
subcontrol-position: top;\
subcontrol-origin: margin;\
}\
QScrollBar::sub-line:vertical {\
border: 0px solid grey;\
background: #32CC99;\
height: 0px;\
subcontrol-position: bottom;\
subcontrol-origin: margin;\
}\
QCheckBox {\
spacing: 25px;\
}\
QCheckBox::indicator {\
width: 30px;\
height: 30px;\
}\
QTreeWidget QScrollBar:vertical {\
    border: 0px solid grey;\
    background-color: rgb(240, 240, 240);\
    width: 35px;\
    margin: 1px 0px 1px 0px;\
}\
QTreeWidget QScrollBar::handle:vertical {\
    background-color: rgb(200, 200, 200);\
    min-height: 20px;\
    border-radius: 10px;\
}\
QTreeWidget QScrollBar::add-line:vertical {\
    border: 0px solid grey;\
    background: #32CC99;\
    height: 0px;\
    subcontrol-position: top;\
    subcontrol-origin: margin;\
}\
\
QTreeWidget QScrollBar::sub-line:vertical {\
    border: 0px solid grey;\
    background: #32CC99;\
    height: 0px;\
    subcontrol-position: bottom;\
    subcontrol-origin: margin;\
}\
\
QTreeWidget QScrollBar:horizontal {\
    border: 0px solid grey;\
    background-color: rgb(240, 240, 240);\
    height: 35px;\
    margin: 0px 1px 0 1px;\
}\
QTreeWidget QScrollBar::handle:horizontal {\
    background-color: rgb(200, 200, 200);\
    min-width: 20px;\
    border-radius: 10px;\
}\
QTreeWidget QScrollBar::add-line:horizontal {\
    border: 0px solid grey;\
    background: #32CC99;\
    width: 0px;\
    subcontrol-position: right;\
    subcontrol-origin: margin;\
}\
QTreeWidget QScrollBar::sub-line:horizontal {\
    border: 0px solid grey;\
    background: #32CC99;\
    width: 0px;\
    subcontrol-position: left;\
    subcontrol-origin: margin;\
}\
QScrollBar::handle:horizontal {\
background-color: rgb(200, 200, 200);\
min-width: 20px;\
border-radius: 10px;\
}\
QScrollBar::add-line:horizontal {\
border: 0px solid grey;\
background: #32CC99;\
width: 0px;\
subcontrol-position: right;\
subcontrol-origin: margin;\
}\
QScrollBar::sub-line:horizontal {\
border: 0px solid grey;\
background: #32CC99;\
width: 0px;\
subcontrol-position: left;\
subcontrol-origin: margin;\
}\
QScrollBar:vertical {\
border: 0px solid grey;\
background-color: rgb(240, 240, 240);\
width: 35px;\
margin: 1px 0px 1px 0px;\
}\
QScrollBar::handle:vertical {\
background-color: rgb(200, 200, 200);\
min-height: 20px;\
border-radius: 10px;\
}\
QScrollBar::add-line:vertical {\
border: 0px solid grey;\
background: #32CC99;\
height: 0px;\
subcontrol-position: top;\
subcontrol-origin: margin;\
}\
QScrollBar::sub-line:vertical {\
border: 0px solid grey;\
background: #32CC99;\
height: 0px;\
subcontrol-position: bottom;\
subcontrol-origin: margin;\
}\
QSlider::groove { border: 1px solid #999999;  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E6E6E6, stop:1 #EEEEEE); } \
QSlider::handle { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #7cb0e9, stop:1 #7cb0e9); border: 1px solid #5c5c5c; \
border-radius: 3px; width: 25px; height: 30px; } \
}\
QCheckBox {\
spacing: 25px;\
}\
QCheckBox::indicator {\
font-size: 16px;\
width: 30px;\
height: 30px;\
}\
QRadioButton {\
    font-size: 16px;\
} \
QPushButton {\
    font-size: 16px;\
} \
QLineEdit {\
    font-size: 16px;\
} \
QComboBox {\
    font-size: 16px;\
} \
QTreeWidget::item {\
    border: 0px solid grey;\
    height: 25px;\
    font-size: 25px;\
}\
";
#endif
