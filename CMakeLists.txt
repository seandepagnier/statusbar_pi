##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## ***************************************************************************
## *   Copyright (C) 2013 by Sean D'Epagnier                                 *
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 3 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   Free Software Foundation, Inc.,                                       *
## *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
## ***************************************************************************


# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(statusbar_pi)
SET(PACKAGE_NAME statusbar_pi)
SET(VERBOSE_NAME Statusbar)
SET(TITLE_NAME Statusbar)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "5")

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_STATUSBAR
    src/statusbar_pi.cpp
    src/TexFont.cpp
    src/StatusbarUI.cpp
    src/icons.cpp
)


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_STATUSBAR} )

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
