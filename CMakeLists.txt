##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
# ---------------------------------------------------------------------------
# Frontend Author: Jon Gough   Frontend2: v1.0.160.3
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
# Frontend2 Author: Jon Gough   v1.0.161					 
# ---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##


## ----- Modify section below to include all the details for your plugin ----- ##

set(CMLOC "CMakeLists: ")


# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
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

set(VERSION_MAJOR "0")
set(VERSION_MINOR "7")
set(VERSION_PATCH "6")
set(VERSION_TWEAK "4")
set(VERSION_DATE "15/04/2021")  # DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
#Now set in plugin configure, needed to build flatpak, also locally.
#set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried.
# set(GIT_USER "rgleason")
# set(GIT_REPOSITORY_NAME "weather_routing_pi")

## Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
#set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Ricks's customization
set(CLOUDSMITH_BASE_REPOSITORY "statusbar")  #without the _pi
# Set the following to use OpenCPN cloudsmith repositories instead of own repositories
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
set(USE_LOCAL_GLU TRUE)
# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")    #not operational???
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)      #not operational ????

option(USE_GL "Enable OpenGL support" ON)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##

project(${PACKAGE})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ===========================================================================
include("PluginSetup")
# ===========================================================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- 

set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
# ===========================================================================
include("PluginConfigure")
# ===========================================================================
## ----- do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements for source, headers, etc. ----- ##

add_definitions(-DUSE_S57)

IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "${CMLOC}SVG support enabled...")
    IF(NOT QT_ANDROID)
        ADD_DEFINITIONS(-DPLUGIN_USE_SVG)
    ENDIF(NOT QT_ANDROID)
ENDIF(PLUGIN_USE_SVG)

#======== Below from cmakelists.txt  weather_routing_pi
# ----Remember to Add your project Set(SRCS and set(HDRS  etc..
 
SET(SRCS
    src/statusbar_pi.cpp
    src/StatusbarUI.cpp
#    src/android/wx/fontpicker.cpp
    src/icons.cpp
	src/TexFont.cpp
)

SET(HDRS
     include/defs.h
	 include/icons.h
	 include/statusbar_pi.h
	 include/StatusbarUI.h
	 include/TexFont.h
)

add_definitions(-DPLUGIN_USE_SVG)

# =================================================
SET(SRC ${SRCS} ${HDRS} )
# =================================================

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)									
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include/GL)

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/ocpn-api)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)

if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)

##=================================================
target_sources(${PACKAGE_NAME} PUBLIC ${SRC})
##=================================================

add_definitions(-DTIXML_USE_STL)

#if(UNIX AND NOT APPLE AND NOT QT_ANDROID)
#    if(PI_JSON_SCHEMA_VALIDATOR)
        # Change OS/ABI to be UNIX - System V to make it run with OCPN
#        set(lib_name lib${PACKAGE_NAME}.so)
#        message(STATUS "${CMLOC}lib-name: ${lib_name}")
#        find_file(elfedit elfedit)
#        if(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#            message(STATUS "${CMLOC}Will ensure OS/ABI is set to 0 to match OCPN")
#            add_custom_command(
#                TARGET ${PACKAGE_NAME}
#                POST_BUILD
#                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                DEPENDS ${PACKAGE_NAME}
#                COMMENT " Running post build action on ${lib_name}."
#                COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) ')
#        else(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#            message(STATUS "${CMLOC}Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
#        endif(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#    endif(PI_JSON_SCHEMA_VALIDATOR)
#endif(UNIX AND NOT APPLE AND NOT QT_ANDROID)

## ----- Change above to match project requirements ----- ##

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/plugingl/include)
add_subdirectory(libs/plugingl)
target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)


## ----- do not change next section - needed to configure build process ----- ##
# ===========================================================================
include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")
# ===========================================================================
## ----- do not change section above - needed to configure build process ----- ##
##
message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
