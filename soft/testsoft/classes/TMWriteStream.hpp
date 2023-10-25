#ifndef TMCrWrite1
#define TMCrWrite1

#include <vcl.h>
#include "textrut.hpp"

class TMCreateWrireStream{
public:
        TMCreateWrireStream();
        ~TMCreateWrireStream();
        unsigned char CreateStream (TDString FileName);
        unsigned char CreateStream (char *lpFileName);
        unsigned char CreateStreamLen (char *lpFileName, unsigned long sizes);
        unsigned char OpenStream (TDString FileName);
        unsigned char OpenOrCreateStream (TDString FileName);
        static unsigned long CopyFile (char *lpInputFN, char *lpOutFN);

        unsigned char SetOffsetBegin (unsigned long offsetlong);

        unsigned char WriteByte (unsigned char data);
        unsigned char WriteShort (unsigned short datas);
        unsigned char WriteLong (unsigned long datas);
        char WriteString (TDString mstr);
        char WriteASCIIZ (char *lpStr);
	char WriteOnlyCharsetStringAddNM (char *lpmstr);
	char WriteOnlyCharsetString (char *lpmstr);
        unsigned char WriteHexByte(unsigned char data);
        char WriteTextIniStringParam (char *lpNameVariable,char *lpParam);
        char WriteTextIniIntParam (char *lpNameVariable,int paramint);

        unsigned int WriteFileCoding (char *lpFileName,char *lpPassCode);
        unsigned int WriteFileCodingPink (char *lpFileName,char *lpPassCode);
        
		
        unsigned int WriteBlock (char *lpRam, int counterb);
        unsigned int WriteFileX (char *lpFileName);

        unsigned char CloseStream ();
        unsigned int GlobalFileSize;
        
private:
        unsigned char lpBufer[32768];
        unsigned int IndexDataBufer;
        unsigned char flagdata;
        HANDLE Handll;
        unsigned char WriteBufer();
};


#endif
