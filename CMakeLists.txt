##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## ***************************************************************************
## *   Copyright (C) 2013 by Sean D'Epagnier                                 *
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
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
# This should be 2.8.0 to have FindGTK2 module
IF (COMMAND cmake_policy)
    CMAKE_POLICY(SET CMP0003 OLD)
    CMAKE_POLICY(SET CMP0005 OLD)
    CMAKE_POLICY(SET CMP0011 OLD)
ENDIF (COMMAND cmake_policy)

PROJECT(statusbar_pi)

SET(PACKAGE_NAME statusbar_pi)
SET(PLUGIN_SOURCE_DIR .)
MESSAGE (STATUS "")
MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

#SET(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

# require proper c++
#ADD_DEFINITIONS( "-Wall -ansi -pedantic -Wno-variadic-macros" )
#TODO: Should we use  -fno-stack-protector
#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT WIN32 AND NOT APPLE)
    ADD_DEFINITIONS( "-Wall -O0 -g -fexceptions -fvisibility=hidden" )
    SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
ENDIF(NOT WIN32 AND NOT APPLE)

IF( APPLE )
   ADD_DEFINITIONS( "-O3 -fexceptions -fvisibility=hidden" )
   ADD_DEFINITIONS("-Wno-unused -Wno-overloaded-virtual")
   SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
ENDIF( APPLE )


# Add some definitions to satisfy MS
IF(WIN32)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(WIN32)


#SET(wxWidgets_USE_LIBS base core net xml html adv aui)
#SET(BUILD_SHARED_LIBS TRUE)
#SET( wxWidgets_USE_DEBUG OFF)
#SET( wxWidgets_USE_UNICODE ON)
#FIND_PACKAGE(wxWidgets REQUIRED)

INCLUDE(${wxWidgets_USE_FILE})

FIND_PACKAGE(Gettext REQUIRED)

# For convenience we define the sources as a variable. You can add
# header files and cpp/c files and CMake will sort them out


SET(SRC_STATUSBAR
    src/statusbar_pi.h
    src/statusbar_pi.cpp
    src/icons.cpp
    src/icons.h
)


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_STATUSBAR} )

IF(WIN32)
    SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${OPENCPN_IMPORT_LIB} )
    ADD_DEPENDENCIES(${PACKAGE_NAME} ${PARENT})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} )


IF(APPLE)
    INSTALL(TARGETS ${PACKAGE_NAME} BUNDLE LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/OpenCPN.app/Contents/PlugIns)
ENDIF(APPLE)

IF(UNIX AND NOT APPLE)
    INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME LIBRARY DESTINATION ${PREFIX_PLUGINS})
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
    INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION "plugins")
ENDIF(WIN32)

INCLUDE("../PluginLocalization.cmake")
PLUGIN_LOCALIZATION()
