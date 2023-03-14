##---------------------------------------------------------------------------
## Author:      Sean d'Epagnier
##***************************************************************************
## *   Copyright (C) 2010 by Sean d'Epagnier                              *
## *   License:     GPLv3+
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
##---------------------------------------------------------------------------
# Frontend2 Author: Jon Gough  
# Frontend2: v1.0.230.0  05/03/2023  # DD/MM/YYYY format
#---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##


## ----- Modify section below to include all the details for your plugin ----- ##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.5.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
# Need cmake 3.13 to implement this, but raspbian builds are at 3.7.2 and bionic at 3.10 and xenial builds at 3.5.1
#    cmake_policy(SET CMP0076 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions
set(VERBOSE_NAME "StatusBar")
set(COMMON_NAME "StatusBar")
set(TITLE_NAME "StatusBar")
set(PACKAGE_CONTACT "Sean D'Epagnier")
set(PACKAGE "statusbar")
set(SHORT_DESCRIPTION "StatusBar Plugin is an optional replacement for the builtin statusbar")
set(LONG_DESCRIPTION "The builtin status bar (disable from the User Interface tab) has limited configuration options and can be very difficult to read. The statusbar plugin improves on some of these difficulties. Best used with OpenGL enabled (requires some basic OpenGL extensions).")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "4")
set(VERSION_TWEAK "0")
set(VERSION_DATE "14/03/2023")  # DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
#Now set in plugin configure, needed to build flatpak, also locally.
#set(GIT_REPOSITORY_SERVER "github.com")

# Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
# set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(CLOUDSMITH_BASE_REPOSITORY "statusbar")  #without the _pi


set(CLOUDSMITH_USER "opencpn")
#set(CLOUDSMITH_USER "rick-gleason")

set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/statusbar.html")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

## ----- Modify section above to include all the details for your plugin ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Use local version of GLU library
#set(USE_LOCAL_GLU TRUE)
# Prefer libGL.so to libOpenGL.so, see CMP0072
#set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
#set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" ON)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##

project(${PACKAGE})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# =======================================================
include("PluginSetup")
# =======================================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##

## ----- Modify section below if there are special requirements for the plugin -----

set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
# ========================================================
include("PluginConfigure")
# ========================================================
## ----- do not change section above - needed to configure build process ----- ##

## ----- Change below to match project requirements for source, headers, etc. ----- ##

add_definitions(-DUSE_S57)


#======== Below from cmakelists.txt  weather_routing_pi
# ----Remember to Add your project Set(SRCS and set(HDRS  etc..

SET(SRCS
    src/statusbar_pi.cpp
    src/StatusbarUI.cpp
    src/icons.cpp
    src/TexFont.cpp
    src/OCPNColorPicker.cpp
)

SET(HDRS
     include/defs.h
	 include/icons.h
	 include/statusbar_pi.h
	 include/StatusbarUI.h
	 include/TexFont.h
	 include/OCPNColorPicker.h
)

set(LIBHDRS
    libs/GL/gl.h
    libs/GL/gl_private.h
    libs/GL/glext.h
    libs/GL/glu.h
    libs/wxJSON/include/json_defs.h
    libs/wxJSON/include/jsonreader.h
    libs/wxJSON/include/jsonval.h
    libs/wxJSON/include/jsonwriter.h)
add_definitions(-DPLUGIN_USE_SVG)

set(EXTINCLUDE_DIR ${EXTINCLUDE_DIR} extinclude libs/ocpn-api/)

##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================

# Not doing JSON Schema Validation

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/${EXTINCLUDE_DIR})
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/GL)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/ocpn-api)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wxJSON/include)



# ----- Change below to match project requirements for android build ----- ##

if(QT_ANDROID)
    include_directories( ${PROJECT_SOURCE_DIR}/extinclude/GLES2)
endif(QT_ANDROID)

##
## ----- Change above to match project requirements for android build ----- ##
##

##=================================================
target_sources(${PACKAGE_NAME} PUBLIC ${SRCS} ${HDRS})
##=================================================
#target_sources(${PACKAGE_NAME} PUBLIC ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${LIBSSRC})

if(NOT OCPN_FLATPAK_CONFIG)
    # Build environment not available when flatpak is being configured so following statements will not work
    message(STATUS "${CMLOC}Adding target link libraries to ${PACKAGE_NAME}")

#    add_subdirectory(libs/tinyxml)
#    target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)
endif(NOT OCPN_FLATPAK_CONFIG)

add_definitions(-DTIXML_USE_STL)

# Not doing JSON Schema Validation

## ----- Change above to match project requirements ----- ##
##
## ----- do not change next section - needed to configure build process ----- ##
##

# Needed for android builds
if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/plugingl/include)
add_subdirectory(libs/plugingl)
target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

## ----- do not change next section - needed to configure build process ----- ##
# Needed for all builds
# ========================================================
include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")
# ========================================================

## ----- do not change section above - needed to configure build process ----- ##
##
message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
