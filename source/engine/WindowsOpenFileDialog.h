#ifndef _WINDOWSOPENFILEDIALOG_H_
#define _WINDOWSOPENFILEDIALOG_H_

#include <windows.h>
#include <commdlg.h>
#include <tchar.h>

class OpenFileDialog
{
    public:
        OpenFileDialog(void);
        ~OpenFileDialog();

        TCHAR* _defaultExtension;
        TCHAR* _fileName;
        TCHAR* _filter;
        int _filterIndex;
        int _flags;
        TCHAR* _initialDir;
        HWND _owner;
        TCHAR* _title;

        bool ShowDialog();
};

#endif // _WINDOWSOPENFILEDIALOG_H_
