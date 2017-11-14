// Terrain Viewer
//
// Build Settings, file: BuildSettings.hpp
//
// - The build settings file is used to specify macros used on
//	 compilation. The macro WINDOWS_BUILD must be defined when
//   compiling under Visual Studio.
//
// Copyright (c) César González Segura 2015, ETSE-UV
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library.
//
#ifndef _BUILDSETTINGS_HPP_
#define _BUILDSETTINGS_HPP_

// Enable for Visual Studio building
#define WINDOWS_BUILD

#ifdef WINDOWS_BUILD
/*
  This macro changes the application manifest
  to use modern visual styles (in Windows Vista
  and newer).
*/
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#endif // _BUILDSETTINGS_HPP_