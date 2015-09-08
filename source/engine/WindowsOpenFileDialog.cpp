// Terrain Viewer
//
// Windows Open File Dialog, file: WindowsOpenFileDialog.cpp
//
// - Implementation of the Windows Open File Dialog class.
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
#include "WindowsOpenFileDialog.h"

OpenFileDialog::OpenFileDialog(void)
{
    _defaultExtension = 0;
    _fileName = new TCHAR[MAX_PATH];
    _filter = 0;
    _filterIndex = 0;
    _flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    _initialDir = 0;
    _owner = 0;
    _title = 0;
}

OpenFileDialog::~OpenFileDialog()
{
    delete _fileName;
}

bool OpenFileDialog::ShowDialog()
{
    OPENFILENAME ofn ;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = _owner;
    ofn.lpstrDefExt = _defaultExtension;
    ofn.lpstrFile = _fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = _filter;
    ofn.nFilterIndex = _filterIndex;
    ofn.lpstrInitialDir = _initialDir;
    ofn.lpstrTitle = _title;
    ofn.Flags = _flags;

    bool result = (GetOpenFileName(&ofn) == TRUE);

    return result;
}
