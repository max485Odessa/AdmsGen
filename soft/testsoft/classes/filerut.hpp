#ifndef Filerut_HPP_3
#define Filerut_HPP_3

#include <windows.h>

bool DeleteFile_E (char *lFileName);
bool CheckDirectory_E (char *ldirnames);
bool RenameFile_E(LPSTR szFullPath, LPSTR szBackup);
bool MoveFile_E (LPSTR lpEexFn, LPSTR lpNewFileName);
unsigned long GetLenFile_E (char *lpFilename);
HANDLE OpenFile_E (char *lpFileName);
HANDLE OpenFileToRead_E (char *lpFileName);
void CloseFile_E (HANDLE handl);
HANDLE CreateFile_E (char *lpFileName);
DWORD SeekFile_E (HANDLE handl,long seeks,DWORD modes);
DWORD GetSizeFile_E (HANDLE handl);
DWORD ReadFileE (void *lpReadRam,unsigned int sizesf,HANDLE handl);
DWORD WriteFile_E (char *lpWriteAdr,unsigned int sizesf,HANDLE handl);
unsigned int SaveFile_E (char *lpFileName,char *lpRam,unsigned int sizes);
unsigned int LoadFile_E (char *lpFileName,char *lpRam,unsigned int sizes);
bool CreateDirectory_E (char *lPath);

#endif


