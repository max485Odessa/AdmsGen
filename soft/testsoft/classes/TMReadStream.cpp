#include "TMReadStream.hpp"
#include "textrut.hpp"
#include "filerut.hpp"
#include "TMWriteStream.hpp"



TMCreateReadStream::TMCreateReadStream()
{
GlobalFileSize=0;
GlobalIndexBuferRead=0;
LocalIndexBuferRead=0;
BuferDataSize=0;
FlagEndBX=0;
Handll=INVALID_HANDLE_VALUE;
memset(lpBufer,0,sizeof(lpBufer));
}



unsigned int TMCreateReadStream::GetCurrentASCIIString (unsigned char *lpDest,unsigned int sizebufs)
{
unsigned int rv=0;
if (lpDest && sizebufs)
	{
	unsigned char datab;
	while (!FlagEndBX && sizebufs)
		{
		datab=ReadByteBX();
		if (!FlagEndBX)
			{
			if (datab==13)
				{
				ReadByteBX(); // 10
				break;
				}
			}
		else
			{
			break;
			}
		rv++;
		lpDest[0]=datab;
		lpDest++;
		sizebufs--;
		}
	if (sizebufs) lpDest[0]=0;
	}
return rv;
}



bool TMCreateReadStream::NextStr()
{
bool rv=0;
unsigned char datab;
while (!FlagEndBX)
	{
	datab=ReadByteBX();
	if (!FlagEndBX)
		{
		if (datab==13)
			{
			ReadByteBX(); // 10
			if (!FlagEndBX) rv=1;
			break;
			}
		}
	}
return rv;
}



unsigned long TMCreateReadStream::GetStrings (TDString &Dest)
{
unsigned long rvcnt = 0;
char datab;
Dest = "";
while (!FlagEndBX)
    {
    datab = ReadByteBX ();
    if (!datab || FlagEndBX)
        {
        break;
        }
    else
        {
        if (datab == 13) rvcnt++;
        Dest = Dest + datab;
        }
    }
return rvcnt;
}



unsigned int TMCreateReadStream::GetSelASCIIString (unsigned char *lpDest, unsigned int sizebufs, unsigned int strIndx)
{
unsigned int rv=0;
if (lpDest && sizebufs && strIndx)
	{
	strIndx--;
	SetReadIndex (0);
	unsigned int IndxFStr = 0;
	bool err = 0;
	while (IndxFStr < strIndx)
		{
		if (!NextStr())
			{
			err = 1;
			break;
			}
		IndxFStr++;
		}
	if (!err)
        {
        rv = GetCurrentASCIIString (lpDest, sizebufs);
        }
	}
return rv;
}




unsigned int TMCreateReadStream::ReadDataToNewFile (char *lpFileName,unsigned int wrsize)
{
unsigned int rv=0;
if (lpFileName && wrsize)
	{
	TMCreateWrireStream wrstrm;
	if (wrstrm.CreateStream(lpFileName))
		{
		char curbyted;
		while (wrsize && !FlagEndBX)
			{
			curbyted=ReadByteBX();
			if (!wrstrm.WriteByte (curbyted)) break;
			wrsize--;
			rv++;
			}
		wrstrm.CloseStream ();
		}
	}
return rv;
}


unsigned int TMCreateReadStream::ReadDataToRamAndDecodingPink (char *lpOutRamAdr,char *lpInput,char *lpPassworde,unsigned int wrsize)
{
unsigned int rv=0;
if (lpOutRamAdr && lpInput && lpPassworde && wrsize)
	{
	if (1)
		{
		unsigned char *TechPass="Bruliante-Incorparates";
		unsigned char *lpCurTechPass=TechPass;
		unsigned char *lpCurPassdata=lpPassworde;
		//unsigned int LenTechPass=GetLenStr(lpCurTechPass);
		unsigned int LenPassword=GetLenStr(lpPassworde);
		unsigned char curbyted;
                unsigned char curbcod=234,tmpvvv;
		unsigned char maskleft=1;
		unsigned char maskright=128;
		while (wrsize)
			{
			curbyted=lpInput[0];
                        lpInput++;
                        curbyted=curbyted ^ curbcod;
                        curbyted=curbyted ^ TechPass[(curbcod & 0x0F)];

			if (LenPassword)
				{
				curbyted=curbyted ^ lpCurPassdata[0];
				lpCurPassdata++;
				if (!lpCurPassdata[0]) lpCurPassdata=lpPassworde;
				}
			curbyted=curbyted ^ lpCurTechPass[0];
			lpCurTechPass++;
			if (!lpCurTechPass[0]) lpCurTechPass=TechPass;
			curbyted=curbyted ^ maskright;
			curbyted=curbyted ^ maskleft;

			maskright=maskright >> 1;
			if (!maskright) maskright=128;
			maskleft=maskleft <<1;
			if (!maskleft) maskleft=1;

			curbyted=curbyted ^ (unsigned char)rv;

			// ??????????? ????????? ----------------
                        lpOutRamAdr[0] = curbyted;
                        lpOutRamAdr++;
                        curbcod=curbyted;
			wrsize--;
			rv++;
			}
		}
	}
return rv;
}



unsigned int TMCreateReadStream::ReadDataToNewFileAndDecodingPink (char *lpFileName,char *lpPassworde,unsigned int wrsize)
{
unsigned int rv=0;
if (lpFileName && lpPassworde && wrsize && !FlagEndBX)
	{
	TMCreateWrireStream wrstrm;
	if (wrstrm.CreateStream(lpFileName))
		{
		unsigned char *TechPass="Bruliante-Incorparates";
		unsigned char *lpCurTechPass=TechPass;
		unsigned char *lpCurPassdata=lpPassworde;
		//unsigned int LenTechPass=GetLenStr(lpCurTechPass);
		unsigned int LenPassword=GetLenStr(lpPassworde);
		unsigned char curbyted;
        unsigned char curbcod=234,tmpvvv;
		unsigned char maskleft=1;
		unsigned char maskright=128;
		while (wrsize && !FlagEndBX)
			{
			curbyted=ReadByteBX();
                        //tmpvvv=curbyted;
                        curbyted=curbyted ^ curbcod;
                        curbyted=curbyted ^ TechPass[(curbcod & 0x0F)];

			if (LenPassword)
				{
				curbyted=curbyted ^ lpCurPassdata[0];
				lpCurPassdata++;
				if (!lpCurPassdata[0]) lpCurPassdata=lpPassworde;
				}
			curbyted=curbyted ^ lpCurTechPass[0];
			lpCurTechPass++;
			if (!lpCurTechPass[0]) lpCurTechPass=TechPass;
			curbyted=curbyted ^ maskright;
			curbyted=curbyted ^ maskleft;

			maskright=maskright >> 1;
			if (!maskright) maskright=128;
			maskleft=maskleft <<1;
			if (!maskleft) maskleft=1;

			curbyted=curbyted ^ (unsigned char)rv;

			// ??????????? ????????? ----------------
			if (!wrstrm.WriteByte (curbyted)) break;
                        curbcod=curbyted;
			wrsize--;
			rv++;
			}
		wrstrm.CloseStream ();
		}
	}
return rv;
}





unsigned int TMCreateReadStream::ReadDataToNewFileAndDecoding (char *lpFileName,char *lpPassworde,unsigned int wrsize)
{
unsigned int rv=0;
if (lpFileName && lpPassworde && wrsize && !FlagEndBX)
	{
	TMCreateWrireStream wrstrm;
	if (wrstrm.CreateStream(lpFileName))
		{
		char *TechPass="Bruliante-Incorparates";
		char *lpCurTechPass=TechPass;
		char *lpCurPassdata=lpPassworde;
		//unsigned int LenTechPass=GetLenStr(lpCurTechPass);
		unsigned int LenPassword=GetLenStr(lpPassworde);
		unsigned char curbyted;
		unsigned char maskleft=1;
		unsigned char maskright=128;
		while (wrsize && !FlagEndBX)
			{
			curbyted=ReadByteBX();

			if (LenPassword)
				{
				curbyted=curbyted ^ lpCurPassdata[0];
				lpCurPassdata++;
				if (!lpCurPassdata[0]) lpCurPassdata=lpPassworde;
				}
			curbyted=curbyted ^ lpCurTechPass[0];
			lpCurTechPass++;
			if (!lpCurTechPass[0]) lpCurTechPass=TechPass;
			curbyted=curbyted ^ maskright;
			curbyted=curbyted ^ maskleft;

			maskright=maskright >> 1;
			if (!maskright) maskright=128;
			maskleft=maskleft <<1;
			if (!maskleft) maskleft=1;

			curbyted=curbyted ^ (unsigned char)rv;

			// ??????????? ????????? ----------------
			if (!wrstrm.WriteByte (curbyted)) break;
			wrsize--;
			rv++;
			}
		wrstrm.CloseStream ();
		}
	}
return rv;
}




TMCreateReadStream::~TMCreateReadStream()
{
CloseStream();
}



unsigned char TMCreateReadStream::OpenStream (char *filename)
{
unsigned char rv=0;
CloseStream();
Handll=OpenFile_E (filename);
if (Handll!=INVALID_HANDLE_VALUE)
        {
        GlobalFileSize=GetSizeFile_E (Handll);
        if (GlobalFileSize)
                {
                BuferDataSize=ReadFileE ((void*)&lpBufer,sizeof(lpBufer),Handll);

                FlagEndBX=0;
                rv=1;
                }
        else
                {
                CloseFile_E(Handll);
                Handll=INVALID_HANDLE_VALUE;
                }
        }
LocalIndexBuferRead=0;
GlobalIndexBuferRead=0;
return rv;
}






void TMCreateReadStream::CloseStream()
{
if (Handll!=INVALID_HANDLE_VALUE)
        {
        CloseFile_E(Handll);
        }
GlobalFileSize=0;
GlobalIndexBuferRead=0;
LocalIndexBuferRead=0;
BuferDataSize=0;
FlagEndBX=1;
Handll=INVALID_HANDLE_VALUE;
}



unsigned int TMCreateReadStream::GetFileSize()
{
if (Handll!=INVALID_HANDLE_VALUE)
        {
        return GlobalFileSize;
        }
return 0;
}



unsigned char TMCreateReadStream::SetReadIndex(unsigned int ofsets)
{
unsigned char rv=0;
if (Handll!=INVALID_HANDLE_VALUE)
        {
        if (ofsets<GlobalFileSize)
                {
                SeekFile_E (Handll,ofsets,FILE_BEGIN);
                GlobalIndexBuferRead=ofsets;
                ReadNextDataBufer();
                rv=1;
                }
        }
return rv;
}



unsigned char TMCreateReadStream::AddReadIndex(int indexs)
{
return SetReadIndex((GlobalIndexBuferRead+indexs));
}


unsigned char TMCreateReadStream::ReadByteBX()
{
unsigned char data1=0;
if (Handll!=INVALID_HANDLE_VALUE && !FlagEndBX)
        {

        if (LocalIndexBuferRead<BuferDataSize)
                {
                data1=lpBufer[LocalIndexBuferRead];
                LocalIndexBuferRead++;
                }
        else
                {
                ReadNextDataBufer();
                if (BuferDataSize)
                        {
                        data1=lpBufer[LocalIndexBuferRead];
                        LocalIndexBuferRead++;
                        }
                else
                        {
                        FlagEndBX=1;
                        }
                }
        }
return data1;
}



unsigned short TMCreateReadStream::ReadWordBX()
{
unsigned short rv=0,data0,data1;
if (!FlagEndBX)
        {
        data0=ReadByteBX();
        if (!FlagEndBX)
                {
                data1=ReadByteBX();
                if (!FlagEndBX)
                        {
                        rv=(data1<<8)|data0;
                        }
                }
        }
return rv;
}



unsigned int TMCreateReadStream::ReadDwordBX()
{
unsigned int data0,data1,data2,data3,rv=0;
if (!FlagEndBX)
        {
        data0=ReadByteBX();
        if (!FlagEndBX)
        	{
        	data1=ReadByteBX();
        	if (!FlagEndBX)
        		{
        		data2=ReadByteBX();
        		if (!FlagEndBX)
        			{
        			data3=ReadByteBX();
        			if (!FlagEndBX)
        				{
        				rv=(data3<<24)|(data2<<16)|(data1<<8) |data0;
        				}
        			}
        		}
	        }
        }
return rv;
}



unsigned int TMCreateReadStream::GetCurrentIndex()
{
return GlobalIndexBuferRead+LocalIndexBuferRead;
}



unsigned char TMCreateReadStream::DecIndex()
{
return SetReadIndex((GetCurrentIndex()-1));
}



unsigned int TMCreateReadStream::GetString(char *lpString,unsigned int maxsizesb)
{
if (!lpString || !maxsizesb) return 0;
unsigned int cntr=0,sizesd=0;
char datab,flagwr=0;
while (cntr<32768)
        {
        datab=ReadByteBX();
        if (FlagEndBX) break;
        if (datab>31)
                {
                flagwr=1;
                if (sizesd<maxsizesb)
                        {
                        lpString[sizesd]=datab;
                        sizesd++;
                        }
                else
                        {
                        break;
                        }
                }
        else
                {
                if (flagwr) break;
                }
        cntr++;
        }
lpString[sizesd]=0;
return sizesd;
}



bool TMCreateReadStream::GetAllText (TDString &Dststr)
{
bool rv = false;
Dststr = "";
char datab;
while (true)
    {
    datab = ReadByteBX();
    if (!datab || FlagEndBX)
        {
        break;
        }
    if (datab < ' ')
        {
        if (datab != 9 && datab != 10 && datab != 13) datab = ' ';
        }
    Dststr = Dststr + datab;
    }
rv = true;
return rv;
}



bool TMCreateReadStream::GetNextString (TDString &Dststr)
{
bool rv = false;
Dststr = "";
char datab;
while (!FlagEndBX)
        {
        datab = ReadByteBX();
        if (!datab || FlagEndBX)
            {
            break;
            }
        if (datab > 31)
            {
            Dststr = Dststr + datab;
            }
        else
            {
            if (datab == 13)
                {
                rv = true;
                break;
                }
            }
        }
return rv;
}




void TMCreateReadStream::ReadNextDataBufer()
{
GlobalIndexBuferRead=SeekFile_E (Handll,0,FILE_CURRENT);
BuferDataSize=ReadFileE (&lpBufer,sizeof(lpBufer),Handll);
LocalIndexBuferRead=0;
if (BuferDataSize) FlagEndBX=0;
}



unsigned int TMCreateReadStream::ReadData(unsigned char *lpAdr,int size)
{
if (!lpAdr || !size) return 0;
int lind=0;
char data1;
while(lind<size)
        {
        data1=ReadByteBX();
        if (FlagEndBX) break;
        lpAdr[lind]=data1;
        lind++;
        }
return lind;
}


char TMCreateReadStream::CompareString(TDString names)
{
char data1=0,data2=0;
if (names=="") return -1;
char *Names = (char*)names.c_str();
unsigned int indexstr=0;
unsigned int TGlobIndex,TLocIndex;
TGlobIndex=GlobalIndexBuferRead;
TLocIndex=LocalIndexBuferRead;
while(1)
        {
        data1=Names[indexstr];
        if (!data1)
                {
                data2=0;
                break;
                }
        data2=ReadByteBX();
        if (data1>data2)
                {
                data2=1;
                break;
                }
        if (data1<data2)
                {
                data2=-1;
                break;
                }
        indexstr++;
        }
if (TGlobIndex!=GlobalIndexBuferRead)
        {
        SetReadIndex(TGlobIndex);
        }
else
        {
        LocalIndexBuferRead=TLocIndex;
        }
return data2;
}



