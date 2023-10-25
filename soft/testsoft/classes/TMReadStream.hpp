#ifndef TMCrRead1
#define TMCrRead1

#include <vcl.h>
#include "textrut.hpp"

class TMCreateReadStream{
public:
        TMCreateReadStream();
        ~TMCreateReadStream();
        unsigned char OpenStream (char *filename);
        void CloseStream ();
        unsigned int GetFileSize ();
        unsigned char AddReadIndex (int index);
        unsigned char SetReadIndex (unsigned int index);
        unsigned int GetCurrentIndex ();
        unsigned int ReadData (unsigned char *lpAdr,int size);
        unsigned int GetString (char *lpString, unsigned int maxsizesb);
        bool GetNextString (TDString &Dststr);
        bool GetAllText (TDString &Dststr);
        unsigned char DecIndex ();
        unsigned int ReadDataToNewFile (char *lpFileName, unsigned int wrsize);
        unsigned int ReadDataToNewFileAndDecoding (char *lpFileName, char *lpPassworde, unsigned int wrsize);
        unsigned int ReadDataToNewFileAndDecodingPink (char *lpFileName, char *lpPassworde, unsigned int wrsize);
        unsigned int ReadDataToRamAndDecodingPink (char *lpOutRamAdr, char *lpInput, char *lpPassworde, unsigned int wrsize);
        unsigned int GetCurrentASCIIString (unsigned char *lpDest, unsigned int sizebufs);
        unsigned int GetSelASCIIString (unsigned char *lpDest, unsigned int sizebufs, unsigned int strIndx);
        unsigned long GetStrings (TDString &Dest);
        char CompareString (TDString names);
        unsigned char ReadByteBX ();
        unsigned short ReadWordBX ();
        unsigned int ReadDwordBX ();
        unsigned char lpBufer[32768];
        unsigned int GlobalFileSize;

private:

        void ReadNextDataBufer ();
        bool NextStr ();
        unsigned char FlagEndBX;
        unsigned int GlobalIndexBuferRead;
        unsigned int LocalIndexBuferRead;
        unsigned int BuferDataSize;
        HANDLE Handll;
};

#endif
