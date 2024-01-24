Statusbar Plugin for OpenCPN
=======================================

The status bar plugin provides customizable, easier to read output text
and lower cpu usage than the builtin statusbar.

Compiling
=========

* git clone git://github.com/seandepagnier/statusbar_pi.git

Under windows, you must find the file "opencpn.lib" (Visual Studio) or "libopencpn.dll.a" (mingw) which is built in the build directory after compiling opencpn.  This file must be copied to the plugin directory.

Build as normally:

* cd ..
* cd build
* cmake ..
* make
* make install

For OSX standalone build in statusbar_pi directory:
* mkdir build
* cd build
* cmake ..
* make
* make create-pkg

License
=======
The plugin code is licensed under the terms of the GPL v3+
