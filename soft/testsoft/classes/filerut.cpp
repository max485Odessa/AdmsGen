#include "filerut.hpp"
#include "windows.h"
#include <vcl.h>


bool CheckDirectory_E (char *ldirnames)
{
bool rv = false;
if (ldirnames)
    {
    unsigned long atrbt = GetFileAttributes(ldirnames);
    if (atrbt != 0xFFFFFFFF)
        {
        if (atrbt & FILE_ATTRIBUTE_DIRECTORY) rv = true;
        }
    }
return rv;
}


bool CreateDirectory_E (char *lPath)
{
bool rv = CreateDirectory (lPath, NULL);
return rv;
}


bool DeleteFile_E (char *lFileName)
{
return ::DeleteFile (lFileName);
}


HANDLE OpenFileToRead_E (char *lpFileName)
{
return (HANDLE)::CreateFile(lpFileName,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
}


HANDLE OpenFile_E (char *lpFileName)
{
return (HANDLE)::CreateFile (lpFileName, GENERIC_WRITE | GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);  // GENERIC_READ GENERIC_WRITE |
}



HANDLE CreateFile_E (char *lpFileName)
{
return (HANDLE)::CreateFile(lpFileName,GENERIC_WRITE|GENERIC_READ,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
}


DWORD SeekFile_E (HANDLE handl,long seeks,DWORD modes)
{
// FILE_BEGIN , FILE_CURRENT , FILE_END
long hpar=0;
DWORD rv = ::SetFilePointer(handl,seeks,&hpar,modes);
return rv;
}







DWORD GetSizeFile_E (HANDLE handl)
{
DWORD curpointer = SeekFile_E(handl,0,FILE_CURRENT);
DWORD rv = SeekFile_E(handl,0,FILE_END);
SeekFile_E(handl,curpointer,FILE_BEGIN);
return rv;
}



void CloseFile_E (HANDLE handl)
{
if (handl != INVALID_HANDLE_VALUE) ::CloseHandle (handl);
}


unsigned long GetLenFile_E (char *lpFilename)
{
unsigned long sizess=0;
if (lpFilename)
        {
        HANDLE hhnm = ::CreateFile (lpFilename,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
        if (hhnm!=INVALID_HANDLE_VALUE)
                {
                sizess=GetSizeFile_E (hhnm);
                CloseFile_E (hhnm);
                }
        }
return sizess;
}




DWORD ReadFileE (void *lpReadRam,unsigned int sizesf,HANDLE handl)
{
DWORD CC1=0;
if (handl && handl!=INVALID_HANDLE_VALUE && lpReadRam)
        {
        ::ReadFile(handl,lpReadRam,sizesf,&CC1,0);
        }
return CC1;
}



DWORD WriteFile_E (char *lpWriteAdr,unsigned int sizesf,HANDLE handl)
{
DWORD CC1=0;
if (handl && handl!=INVALID_HANDLE_VALUE && lpWriteAdr)
        {
        ::WriteFile(handl,lpWriteAdr,sizesf,&CC1,0);
        }
return CC1;
}





unsigned int SaveFile_E (char *lpFileName,char *lpRam,unsigned int sizes)
{
unsigned int rv=0;
if (lpFileName && lpRam)
        {
        HANDLE handwr = CreateFile_E (lpFileName);
        if (handwr != INVALID_HANDLE_VALUE)
                {
                rv = WriteFile_E (lpRam,sizes,handwr);
                CloseFile_E (handwr);
                }
        }
return rv;
}



unsigned int LoadFile_E (char *lpFileName,char *lpRam,unsigned int sizes)
{
unsigned int rv=0;
if (lpFileName && lpRam && sizes)
        {
        HANDLE handwr = OpenFile_E (lpFileName);
        if (handwr!=INVALID_HANDLE_VALUE)
                {
                rv = ReadFileE (lpRam,sizes,handwr);
                CloseFile_E (handwr);
                }
        }
return rv;
}



bool RenameFile_E(LPSTR szFullPath, LPSTR szBackup)
{
return ::RenameFile (szFullPath,szBackup);
}



bool MoveFile_E (LPSTR lpExistingFileName, LPSTR lpNewFileName)
{
return ::MoveFile (lpExistingFileName, lpNewFileName);
}



