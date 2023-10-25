typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
#include "stdlib.h"

//#include "vcl.h"

#include "textrut.hpp"
#include <Registry.hpp>

#define C_MULTDV_L32_AMOUNT 10
#define C_MULTDV_L64_AMOUNT 20
const static unsigned long tmpMultData[C_MULTDV_L32_AMOUNT] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
const static uint64_t tmpMultData64[C_MULTDV_L64_AMOUNT] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,  10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000,1000000000000000000};

TDString GenIPBroadcastAdress (TDString IpAdrStr, TDString IpSubNemtMask)
{
TDString rv = "";

unsigned long Ndig_ip[4];
unsigned long Ndig_mask[4];
unsigned long NdigBroad[4];
char *lInStr_adr = (char*)IpAdrStr.c_str();
char *lInStr_mask = (char*)IpSubNemtMask.c_str();
unsigned char CIndx = 0;
TDString outstr_adr, outstr_msk;
if (CreateWordStringFromIndxLine (lInStr_adr, outstr_adr, 5, '.') == 3 && CreateWordStringFromIndxLine (lInStr_mask, outstr_msk, 5, '.') == 3)
    {
    while (CIndx < 4)
        {
        if (CIndx != CreateWordStringFromIndxLine (lInStr_adr, outstr_adr, CIndx, '.')) break;
        if (!ConvertStrToULong ((unsigned char *)outstr_adr.c_str(),&Ndig_ip[CIndx])) break;
        if (CIndx != CreateWordStringFromIndxLine (lInStr_mask, outstr_msk, CIndx, '.')) break;
        if (!ConvertStrToULong ((unsigned char *)outstr_msk.c_str(),&Ndig_mask[CIndx])) break;
        NdigBroad[CIndx] = (255 ^ (Ndig_mask[CIndx] & 255)) | (Ndig_ip[CIndx] & 255);
        CIndx++;
        }
    if (CIndx == 4)
        {
        CIndx = 0;
        while (CIndx < 4)
            {
            rv = rv + IntToStr(NdigBroad[CIndx]).c_str();
            if (CIndx < 3) rv = rv + '.';
            CIndx++;
            }
        }
    }
return rv;
}



bool str_compare (char *lStr1, char *lStr2, unsigned long size)
{
	bool rv = false;
	if (size)
		{
		while (size)
			{
			if (*lStr1++ != *lStr2++) break;
			size--;
			}
		if (!size) rv = true;
		}
	return rv;
}




bool ParseHDR (TDString inp_str, GETHDR *lparse_out)
{
bool rv = false;
char *lStr_start = (char*)inp_str.c_str(), *lindx;
unsigned long len_all = GetLenStr (lStr_start);
lindx = lStr_start;
TDString cmd_str = "", get_str = "", param_str = "", host_ip_str = "", host_port_str = "", conect_mode_str = "" ;
unsigned long len_cmd, len_get, len_param, len_ip, len_port,len_mode;
len_cmd = CreateWordStringFromIndxLineAndCF (lStr_start, cmd_str, 0,' ');
len_get = CreateWordStringFromIndxLineAndCF (lStr_start, get_str, 1,' ');
len_port = CreateWordStringFromIndxLineAndCF (lStr_start, host_port_str, 4,' ');
len_mode = CreateWordStringFromIndxLineAndCF (lStr_start, conect_mode_str, 6,' ');

if (cmd_str == "GET")
    {
    TDString Ip_str = "";
    TDString Port_str = "";
    char *lHIPPORT = (char*)host_port_str.c_str();
    CreateWordStringFromIndxLineAndCF (lHIPPORT, Ip_str, 0,':');
    CreateWordStringFromIndxLineAndCF (lHIPPORT, Port_str, 1,':');
    lparse_out->getparam = get_str;
    lparse_out->host_ip = Ip_str;
    lparse_out->host_port = Port_str;
    lparse_out->connectmod = conect_mode_str;
    rv = true;
    }
return rv;
}






TDString GetShortName (TDString str, unsigned long maxlen)
{
TDString rv = "";
if (maxlen)
    {
    unsigned long len = str.length();
    if (len <= maxlen)
        {
        rv = str;
        }
    else
        {
        //unsigned long Mid = len / 2;
        char *lSrc = (char*)str.c_str();
        unsigned long Scnt = 0;
        if (maxlen > 1)
            {
            if (maxlen < 5)
                {
                Scnt = maxlen - 1;
                }
            else
                {
                if (maxlen < 10)
                    {
                    Scnt = maxlen - 2;
                    }
                else
                    {
                    Scnt = maxlen - 3;
                    }
                }
            }
        while (maxlen)
            {
            if (!Scnt)
                {
                rv = rv + '.';
                }
            else
                {
                rv = rv + lSrc[0];
                Scnt--;
                }
            maxlen--;
            }
        }
    }
return rv;
}



TDString GetShortStringKBytes (unsigned long sizebyte, bool F_str)
{
TDString strRv = "";
TDString laststr = "";
if (sizebyte > 1024)
    {
    float Val = sizebyte;
    Val = Val / 1024;
    strRv = Float_To_Str (Val, 1);
    laststr = " Kb.";
    }
else
    {
    strRv = IntToStr (sizebyte).c_str();
    laststr = " bytes";
    }
if (F_str)
    {
    strRv = strRv + laststr;
    }
return strRv;
}



bool GetBitMasiv (TEX::BUFPAR *lInp, unsigned long BitN, bool &GetValue)
{
bool rv = false;
if (lInp && lInp->lpRam && lInp->Sizes)
    {
    unsigned long ByteOffsr = BitN / 8;
    unsigned char BitOffs = BitN % 8;
    unsigned char Mask = 128;
    Mask = Mask >> BitOffs;
    if (lInp->Sizes > ByteOffsr)
        {
        unsigned char *lRam = (unsigned char*)lInp->lpRam;
        rv = true;
        if (lRam[ByteOffsr] & Mask)
            {
            GetValue = true;
            }
        else
            {
            GetValue = false;
            }
        }
    }
return rv;
}



bool SetBitMasiv (TEX::BUFPAR *lDst, unsigned long BitN, bool valdat)
{
bool rv = false;
if (lDst && lDst->lpRam && lDst->Sizes)
    {
    unsigned long ByteOffsr = BitN / 8;
    unsigned char BitOffs = BitN % 8;
    unsigned char Mask = 128;
    Mask = Mask >> BitOffs;
    if (lDst->Sizes > ByteOffsr)
        {
        unsigned char *lRam = (unsigned char*)lDst->lpRam;
        rv = true;
        if (valdat)
            {
            lRam[ByteOffsr] |= Mask;
            }
        else
            {
            lRam[ByteOffsr] = lRam[ByteOffsr] & (255 - Mask);
            }
        }
    }
return rv;
}


void SwapBytes (char *buf, unsigned long size)
{
if (size >= 2 && !(size & 1))
    {
    unsigned long ix_inc = 0;
    unsigned long ix_dec = size - 1;
    unsigned long cicles = size / 2;
    unsigned char dat;
    while (cicles)
        {
        dat = buf[ix_inc];
        buf[ix_inc] = buf[ix_dec];
        buf[ix_dec] = dat;
        ix_inc++;
        ix_dec--;
        cicles--;
        }
    }
}



unsigned short SwapShort (unsigned short datas)
{
unsigned short rv = datas;
SwapBytes ((char*)&rv, sizeof(rv));
return rv;
}



unsigned long SwapLong (unsigned long datas)
{
unsigned long rv = datas;
SwapBytes ((char*)&rv, sizeof(rv));
return rv;
}



uint64_t SwapI64 (uint64_t datas)
{
uint64_t rv = datas;
SwapBytes ((char*)&rv, sizeof(rv));
return rv;
}



unsigned long sqrt_cpu_newton (long L)
{
    unsigned long rslt = (unsigned long)L;
    long div = L;
    if (L <= 0) return 0;
    while (L)
    {
        div = (L / div + div) / 2;
        if (rslt > (unsigned long)div) rslt = (unsigned long)div;
            else break;
    }
return rslt;
}



void FileNameChangeExt (TDString &FileName, TDString &NewExt)
{
TDString str1 = "";
char *lTxt = (char*)FileName.c_str();
char dat;
while (true)
    {
    dat = lTxt[0];
    if (!dat || dat == '.')
        {
        str1 = str1 + '.';
        break;
        }
    str1 = str1 + dat;
    lTxt++;
    }
FileName = str1 + NewExt;
}







void ExtractURLandPath (char *lpFullUrl, TDString &UlrOut, TDString &PathOut)
{
UlrOut = "";
PathOut = "";
if (lpFullUrl)
  {
  char F_end_url = 0;
  char tmpd;
  while (1)
    {
    tmpd = lpFullUrl[0];
    if (!tmpd) break;
    if (!F_end_url)
      {
      if (tmpd == '/')
        {
        PathOut = PathOut + tmpd;
        F_end_url = 1;
        }
      else
        {
        UlrOut = UlrOut + tmpd;
        }
      }
    else
      {
      PathOut = PathOut + tmpd;
      }
    lpFullUrl++;
    }
  }
}




extern void ByteToHEX (unsigned char *lpRams,unsigned char datas);
extern void GetIPAdress (TDString &outstr,unsigned char *lpIPAdr);
extern void GetMACAdress (TDString &outstr,unsigned char *lpMACAdr);


void ExtractIPAdress (TDString InpDataStr, TEX::IPDIGSTRING *Dsst)
{
TDString OutStr;
long rv =  CreateWordStringFromIndxLine ((char*)InpDataStr.c_str(), OutStr, 5, '.');;
if (rv == 3)
    {
    unsigned char IndxDst = 0;
    int Dig;
    while (IndxDst < 4)
        {
        rv = CreateWordStringFromIndxLine ((char*)InpDataStr.c_str(), OutStr, IndxDst, '.');
        Dig = ConvertStringToInt ((char*)OutStr.c_str());
        if (Dig < 0) Dig = 0;
        if (Dig > 255) Dig = 255;
        Dsst->ip.IPADR [IndxDst] = Dig;
        IndxDst++;
        }
    }
}


unsigned char ConvNDataToASCII (unsigned char datas)
{
unsigned char rv;
if (datas >= 10)
   {
   rv = 'A';
   rv = rv + datas;
   }
else
    {
    rv = 48 + datas;
    }
return rv;
}



unsigned long GetCntDelimCharsFrString (char *lpStrName, char DelimCh)
{
unsigned long rvcnt = 0;
char Dt;
while (1)
  {
  Dt = lpStrName[0];
  if (!Dt) break;
  if (Dt == DelimCh) rvcnt++;
  lpStrName++;
  }
return rvcnt;
}





char FullExtractFilenamePath (char *lpFPFileName, TDString &OutDiskName, TDString &OutPathNames, TDString &OutFileName)
{
char rv = 0;
OutDiskName = "";
OutPathNames = "";
OutFileName = "";
if (lpFPFileName)
  {
  unsigned long LenSize = GetLenStr (lpFPFileName);
  if (LenSize >= 2)
    {
    if (lpFPFileName[1] == ':')
      {
      OutDiskName = OutDiskName + lpFPFileName[0];
      lpFPFileName = lpFPFileName + 2;
      LenSize = LenSize - 2;
      if (LenSize > 1)
        {
        char CurDelim = lpFPFileName[0];
        rv = CurDelim;
        if (CurDelim == '\\' || CurDelim == '/')
          {
          LenSize--;
          lpFPFileName++;
          unsigned long cnt_delim = GetCntDelimCharsFrString (lpFPFileName,CurDelim);
          if (cnt_delim)
            {
            // есть имена папок
            TDString TmpPath;
            while (cnt_delim)
              {
              TmpPath = "";
              lpFPFileName = lpGetNextStrToDelim (TmpPath,lpFPFileName,CurDelim);
              OutPathNames = OutPathNames + TmpPath.c_str() + CurDelim;
              cnt_delim--;
              }
            }
          OutFileName = CreateSubString_cor (lpFPFileName).c_str();
          }
        }
      }
    }
  }
return rv;
}





TDString Float_To_Str(float datas,unsigned char ndig)
{
TDString strRv = "";
strRv = FloatToStrF (datas,ffFixed,7,ndig).c_str(); // ffGeneral
return strRv;
}


// возвращает количество папок в пути
unsigned long GetCountDirectory (TDString filenamepaths)
{
unsigned long rvcnt = 0;
char *lpFname = (char*)filenamepaths.c_str();
//unsigned long lnstr = GetLenStr (lpFname);
char datss;
unsigned long indxfnd = 0;
long curindxs;
TDString str1;
while (1)
        {
        curindxs = CreateWordStringFromIndxLine (lpFname,str1,indxfnd,'\\');
        if (str1 == "") break;
        if (curindxs == -1 || (unsigned long)curindxs != indxfnd) break;
        if (FindByte ((char*)str1.c_str(), ':') ==0 &&  FindByte ((char*)str1.c_str(), '.')==0 )
                {
                rvcnt++;
                MessageBox(0,str1.c_str(),"",MB_OK);
                }
        indxfnd++;
        }
return rvcnt;
}







bool ExtractDirectoty (TDString InpPaths, TDString &SdirOut, unsigned long Indxsbdr)
{
bool rv = 0;
TDString outestr1 = "";
SdirOut = "";
long curindxs = CreateWordStringFromIndxLine ((char*)InpPaths.c_str(),outestr1,Indxsbdr,'\\');
if (curindxs != -1 && (unsigned long)curindxs == Indxsbdr)
        {
        SdirOut = outestr1;
        rv = 1;
        }
return rv;
}






unsigned long CreateAllDirectories (TDString fuldirname)
{
unsigned long rv = 0;
if (fuldirname != "")
        {
        char *lpStr = (char*)fuldirname.c_str();
        TDString BaseName = ""; BaseName += lpStr[0];
        BaseName = BaseName + ":\\";
        unsigned long indssx = 1;
        long rvindx;
        TDString str1;
        while (1)
                {
                if (!ExtractDirectoty (fuldirname,str1,indssx)) break;
                BaseName = BaseName + str1 + '\\';
                CreateDirectory(BaseName.c_str(),NULL);
                //MessageBox(0,"",BaseName.c_str(),MB_OK);
                indssx++;
                }
        rv = indssx;
        }
return rv;
}


TDString GetCurentEXEFileName (HINSTANCE hInstance)
{
char buffer[MAX_PATH];
GetModuleFileName(hInstance,buffer,MAX_PATH);
return  (TDString)buffer;
}




// возвращает путь где стартовала программа
TDString GetExecPath ()
{
TDString CurentPath = "";
CurentPath = GetCurrentDir().c_str();
char *lpFlStr = (char*)CurentPath.c_str();
unsigned long szln = GetLenStr (lpFlStr);

if (szln)
        {
        if (lpFlStr[szln-1] != '\\')
                {
                CurentPath = CurentPath + '\\';
                }
        }
return CurentPath;
}




bool InstallXServices (TDString SfullName, TDString DfileName, TDString strNameRegedit)
{
bool rv = 0;
TDString DestDir = ExtractFilePath(DfileName.c_str()).c_str();
CreateAllDirectories (DestDir);
CopyFile(SfullName.c_str(),DfileName.c_str(),0);
TRegistry *reg = new TRegistry(KEY_ALL_ACCESS);
if (reg)
        {
        if (reg->CreateKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"))
                {
                if (reg->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\", 1))
                        {
                        reg->WriteString(strNameRegedit.c_str(),DfileName.c_str());
                        rv = 1;
                        }
                reg->CloseKey();
                }
        delete reg;
        }
return rv;
}



bool RemoveXServices (TDString strNameRegedit)
{
bool rv = 0;
TRegistry *reg = new TRegistry(KEY_ALL_ACCESS);
if (reg)
        {
        if (reg->CreateKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"))
                {
                if (reg->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\", 1))
                        {
                        reg->DeleteValue(strNameRegedit.c_str());
                        //reg->WriteString(strNameRegedit,DfileName);
                        rv = 1;
                        }
                reg->CloseKey();
                }
        delete reg;
        }
return rv;
}



TDString DeleteCharFromString (TDString str1, char cutdat)
{
TDString strrv = "";
if (str1 != "")
        {
        char *lpRam = (char*)str1.c_str();
        unsigned long szln = GetLenStr(lpRam);
        unsigned long curIndxs = 0;
        char dt;
        while (curIndxs < szln)
                {
                dt = lpRam[curIndxs];
                if (!dt) break;
                if (dt == cutdat)
                        {
                        // сдвинуть данные
                        unsigned long sszs = szln - curIndxs - 1;
                        char *lpDest = &lpRam[curIndxs];
                        while (sszs)
                                {
                                lpDest[0] = lpDest[1];
                                lpDest++;
                                sszs--;
                                }
                        lpDest[0] = 0;
                        }
                else
                        {
                        curIndxs++;
                        }
                }
        strrv = (TDString) lpRam;
        }
return strrv;
}



void CopyCharsToString (TDString &DestString, char *lpString, unsigned long sizess)
{
unsigned char SavDat = lpString[sizess];
lpString[sizess] = 0;
DestString = lpString;
lpString[sizess] = SavDat;
}



TDString GetCurrentDirectoryProgram ()
{
TDString rv = "";
static char BuferDirNames[2048];
unsigned long Sizes = GetCurrentDirectory(2048,BuferDirNames);
if (Sizes)
        {
        if (BuferDirNames[Sizes] != '\\') BuferDirNames[Sizes] = '\\';
        BuferDirNames[Sizes+1] = 0;
        rv = (TDString)BuferDirNames;
        }

return rv;
}






// формирует строку из текста, находит данные между разделителями, меняет перевод строки на пробелы
bool CreateStringFrom2Delims(char *lpRam, TDString &outstr, char Bdelin, char Edelim)
{
bool rv = 0;
if (lpRam)
        {
        bool F_F_delim = 0;             // флаг - первый разделитель обнаружен
        unsigned long CntBdelim = 0;    // счетчик входных разделителей
        outstr = "";
        char tmpb;
        while (1)
                {
                tmpb = lpRam[0];
                if (!tmpb)
                        {
                        rv = 0;         // ошибка - не найден завершающий разделитель
                        break;
                        }
                if (tmpb == Bdelin)
                        {
                        if (F_F_delim)
                                {
                                outstr = outstr + tmpb;
                                }
                        else
                                {
                                F_F_delim = 1;
                                }
                        CntBdelim++;
                        }
                else
                        {
                        if (F_F_delim)
                                {
                                if (tmpb == Edelim)
                                        {
                                        CntBdelim--;
                                        if (!CntBdelim)
                                                {
                                                // формирование строки завершенно
                                                rv = 1;
                                                break;
                                                }
                                        else
                                                {
                                                outstr = outstr + tmpb;
                                                }
                                        }
                                else
                                        {
                                        // замена символов перевода строк и табудяции на пробелы
                                        if (tmpb < 32) tmpb = 32;
                                        outstr = outstr + tmpb;
                                        }
                                }
                        }
                lpRam++;
                }
        }
return rv;
}









void AddARPTable (char *lpIpAdr, char *lpMacAdr)
{
TDString lmac = "", lip = "", cmdstr = "";
GetMACAdress (lmac,lpMacAdr);
GetIPAdress (lip,(unsigned char*)lpIpAdr);
cmdstr = "-s " + lip + " " +  lmac;
char *lpStr = (char*)cmdstr.c_str();
//ShellExecute(0,0,"C:\\autonica\\7z.exe",lpStr, NULL, SW_SHOWNORMAL);
MessageBox(0,lpStr,"",MB_OK);
ShellExecute(0,0,"arp.exe ",lpStr, NULL, SW_SHOWNORMAL);
}



void ChangeFileName_Ext (TDString &InpStr, TDString ExtName)
{
TDString CurFname = CutExtFileName (InpStr);
CurFname = CurFname + "."+ExtName;
InpStr = CurFname;
}




unsigned char ConvertHexVal(unsigned char datas)
{
unsigned char rv=0;
while (1)
        {
        if (datas>=97 && datas<=102)
                {
                rv=10+(datas-97);
                break;
                }
        if (datas>=65 && datas<=70)
                {
                rv=10+(datas-65);
                break;
                }
        if (datas>=48 && datas<=57)
                {
                rv=datas-48;
                break;
                }
        break;
        }
return rv;
}




bool CheckHexVal (unsigned char datas)
{
bool rv = 0;
while (1)
        {
        if (datas>=97 && datas<=102)
                {
                rv = 1;
                break;
                }
        if (datas>=65 && datas<=70)
                {
                rv = 1;
                break;
                }
        if (datas>=48 && datas<=57)
                {
                rv = 1;
                break;
                }
        break;
        }
return rv;
}





void GetIPAdress (TDString &outstr, unsigned char *lpIPAdr)
{
outstr = "";
outstr = IntToStr(lpIPAdr[0]).c_str();
outstr = outstr+".";
outstr = outstr + IntToStr(lpIPAdr[1]).c_str();
outstr = outstr+".";
outstr = outstr + IntToStr(lpIPAdr[2]).c_str();
outstr = outstr+".";
outstr = outstr + IntToStr(lpIPAdr[3]).c_str();
}



void GetIPAdress (TDString &outstr, TEX::IPDIGSTRING *Inp)
{
GetIPAdress (outstr, Inp->ip.IPADR);
}





void GetMACAdress (TDString &outstr,unsigned char *lpMACAdr)
{
unsigned char Indxlclmc = 0;
outstr = "";
char buferr[3];
char *lpHd = buferr;
buferr[2] = 0;

while (Indxlclmc < 6)
        {
        ByteToHEX (lpHd,lpMACAdr[Indxlclmc]);
        outstr = outstr + (TDString)lpHd;
        if (Indxlclmc < 5)
                {
                outstr = outstr+"-";
                }
        Indxlclmc++;
        }
}





unsigned char ConvertASCIIDecToVal (uchar *lpRams)
{
unsigned char rv=(lpRams[0]-48)*10;
rv=rv+(lpRams[1]-48);
return rv;
}





unsigned char ConvertHex2Val(uchar *lpRams)
{
uchar rv=ConvertHexVal((lpRams[0]<<4) & 0xF0);
rv=rv+ConvertHexVal(lpRams[1] & 0x0F);
return rv;
}



char ConvBinToASCIIHex (unsigned char datas)
{
char rv = 0;
datas = datas & 15;
if (datas < 10)
	{
	rv = '0' + datas;
	}
else
	{
	rv = 'A' +(datas - 10);
	}
return rv;
}


void ByteToHEX (unsigned char *lpRams,unsigned char datas)
{
lpRams[0] = (ConvBinToASCIIHex(datas>>4));
lpRams[1] = (ConvBinToASCIIHex(datas));
}



void UDToHex_tmp (unsigned char *lpDest, unsigned long datasl, unsigned char cntr)
{
while (cntr)
	{
	lpDest[(cntr-1)] = ConvBinToASCIIHex (datasl & 0x0F);
	datasl = datasl >> 4;
	cntr--;
	}
}


void UShortToHex (unsigned char *lpDest, unsigned short datasl)
{
UDToHex_tmp (lpDest, datasl, 4);
}



void UShortToHex (TDString &dataout, unsigned short datasl)
{
unsigned char Bufs[5];
dataout = "";
UDToHex_tmp (Bufs, datasl, 4);
char Indx = 0;
char dat;
while (Indx < 4)
    {
    dat = Bufs[Indx];
    dataout = dataout + dat;
    Indx++;
    }
}




void UlongToHex (unsigned char *lpDest, unsigned long datasl)
{
UDToHex_tmp (lpDest, datasl, 8);
}




char *CopyStrTo (TEX::BUFPAR *lpDst, char *lpInStr)
{
char *lpRv = 0;
if (lpDst && lpInStr)
    {
    char *lpDstChar = (char*)lpDst->lpRam;
    unsigned long sz = lpDst->Sizes;
    char dt;
    if (lpDstChar && sz)
        {
        lpDstChar[lpDst->Sizes - 1] = 0;
        while (sz)
            {
            dt = lpInStr[0];
            lpDstChar[0] = dt;
            if (!dt) break;
            lpDstChar++;
            lpInStr++;
            sz--;
            }
        }
    lpRv = lpDstChar;
    }
return lpRv;
}





unsigned char *UlongToStr (unsigned char *lpDest, unsigned long datas)
{
unsigned char flagnz=0, resd;
unsigned long delmt = 1000000000;
while (1)
	{
	if (datas>=delmt)
		{
		flagnz=1;
		resd=(datas/delmt);
		datas=datas % delmt;
		}
	else
		{
		resd=0;
		}
	if (flagnz)
		{
		lpDest[0]=resd+48;
		lpDest++;
		}
	delmt=delmt/10;

	if (delmt<=1)
		{
		lpDest[0]=datas+48;
		lpDest++;
        lpDest[0] = 0;
		break;
		}
	}
return lpDest;
}



unsigned char *Uint64ToStr (unsigned char *lpDest, uint64_t datas)
{
unsigned char flagnz = 0, resd;
uint64_t delmt = 10000000000000000000;
while (true)
	{
	if (datas>=delmt)
		{
		flagnz=1;
		resd=(datas/delmt);
		datas=datas % delmt;
		}
	else
		{
		resd=0;
		}
	if (flagnz)
		{
		lpDest[0]=resd+48;
		lpDest++;
		}
	delmt=delmt/10;

	if (delmt<=1)
		{
		lpDest[0]=datas+48;
		lpDest++;
        lpDest[0] = 0;
		break;
		}
	}
return lpDest;
}





bool UlongToStr_M (unsigned long datas, TDString &strout)
{
bool rv = false;
char buf[32];
if (UlongToStr ((unsigned char*)buf, datas))
    {
    AddASCIZZToStr (strout, buf);
    rv = true;
    }
return rv;
}



bool LongToStr_M (long datas, TDString &strout)
{
if (datas < 0)
    {
    datas = abs32 (datas);
    strout = strout + "-";
    }
return UlongToStr_M (datas, strout);
}


TDString LongToStr (long datas)
{
TDString str = "";
bool f_sub = false;
if (datas < 0)
    {
    datas = abs32 (datas);
    f_sub = true;
    }
TDString strout = "";
if (UlongToStr_M (datas, strout))
    {
    if (f_sub) str += "-";
    str += strout;
    }
return str;
}


TDString UlongToStr_V (unsigned long datas)
{
TDString rv = "";
if (!UlongToStr_M (datas, rv)) rv = "err";
return rv;
}





// добавляет к ASCII коду HEX данного цифровую величину
// возвращается ASCII код
unsigned char AddAsciiHex_DecimalValue (char ASCIIHex, unsigned char val)
{
unsigned char datas = ConvertHexVal (ASCIIHex);
return LTetraToHex (datas + val);
}


unsigned char SubAsciiHex_DecimalValue (char ASCIIHex, unsigned char val)
{
unsigned char datas = ConvertHexVal (ASCIIHex);
return LTetraToHex (datas - val);
}








unsigned long Pow10_A (unsigned char Stepnn)
{
unsigned long rv = 1;
if (Stepnn)
	{
	while (Stepnn)
		{
		rv = rv * 10;
		Stepnn--;
		}
	}
return rv;
}



unsigned long Pow16_A (unsigned char Stepnn)
{
unsigned long rv = 1;
if (Stepnn)
	{
	while (Stepnn)
		{
		rv = rv * 16;
		Stepnn--;
		}
	}
return rv;
}






unsigned char *UlongToStr_cnt (unsigned char *lpDest,unsigned long datas,unsigned char cntlens)
{
unsigned char flagnz=0,resd;
unsigned long delmt = 1000000000;
unsigned long StrDelim = Pow10_A (cntlens);
while (1)
	{
	if (datas >= delmt)
		{
		//flagnz=1;
		resd=(datas/delmt);
		datas=datas % delmt;
		}
	else
		{
		resd=0;
		}
	if (StrDelim > delmt) flagnz = 1;
	if (flagnz)
		{
		lpDest[0]=resd+48;
		lpDest++;
		}
	delmt=delmt/10;

	if (delmt<=1)
		{
		lpDest[0]=datas+48;
		lpDest++;
		lpDest[0] = 0;
		break;
		}
	}
return lpDest;
}



void* FillMems (void *lpAdr, char datas, unsigned long sizes)
{
char *lDst = (char*)lpAdr;
while (sizes)
        {
        lDst[0] =  datas;
        lDst++;
        sizes--;
        }
return lDst;
}




char *FindChars (char *lpAdr, char datas)
{
char *rv=0;
char ttg;
while (lpAdr)
        {
        ttg=lpAdr[0];
        if (ttg==datas)
                {
                rv=lpAdr;
                break;
                }
        if (!ttg) break;
        lpAdr++;
        }
return rv;
}



int GetHourTimeFromString (TDString FormatedTimes)
{
int rv=0;
if (FormatedTimes!="")
        {
        TDString copys=FormatedTimes;
        char *lpF = (char*)copys.c_str();
        char *lpZer=FindChars ((char*)copys.c_str(),':');
        if (lpZer)
                {
                lpZer[0]=0;
                TDString numb=(TDString)lpF;
                rv=atoi(numb.c_str());
                }
        }
return rv;
}



int GetMinutesTimeFromString (TDString FormatedTimes)
{
int rv=0;
if (FormatedTimes!="")
        {
        TDString copys=FormatedTimes;
        char *lpF = (char*)copys.c_str();
        char *lpZer=FindChars(lpF,':');
        if (lpZer)
                {
                lpZer++;
                lpF=lpZer;
                lpZer=FindChars(lpZer,':');
                if (lpZer)
                        {
                        lpZer[0]=0;
                        TDString numb=(TDString)lpF;
                        rv=atoi(numb.c_str());
                        }
                }
        }
return rv;
}




int GetSecondTimeFromString (TDString FormatedTimes)
{
int rv=0;
if (FormatedTimes!="")
        {
        TDString copys=FormatedTimes;
        //char *lpF=copys.c_str();
        char *lpZer = FindChars((char*)copys.c_str(),':');
        if (lpZer)
                {
                lpZer++;
                //lpF=lpZer;
                lpZer=FindChars(lpZer,':');
                if (lpZer)
                        {
                        lpZer++;
                        TDString numb=(TDString)lpZer;
                        rv=atoi(numb.c_str());
                        }
                }
        }
return rv;
}



TDString GetSysTimeString_File (SYSTEMTIME *lpSysTime)
{
TDString stre="";
if (lpSysTime)
        {
        stre = IntToStr(lpSysTime->wHour).c_str();
        stre = stre+"_" + IntToStr(lpSysTime->wMinute).c_str();
        stre = stre+"_" + IntToStr(lpSysTime->wSecond).c_str();
        }
return stre;
}




TDString GetSysTimeString (SYSTEMTIME *lpSysTime,char delims)
{
TDString stre="";
if (lpSysTime)
        {
        stre=IntToStr(lpSysTime->wHour).c_str();
        stre=stre+delims+IntToStr(lpSysTime->wMinute).c_str();
        stre=stre+delims+IntToStr(lpSysTime->wSecond).c_str();
        }
return stre;
}



TDString GenerateTimeString (char delims)
{
TDString lastStr = "";
static unsigned long cntstr = 0;
SYSTEMTIME Tim1;
GetLocalTime(&Tim1);
TDString rsltStr = GetSysTimeString (&Tim1,delims);
if (rsltStr == lastStr)
        {
        rsltStr = rsltStr + IntToStr(cntstr).c_str();
        cntstr++;
        }
else
        {
        lastStr = rsltStr;
        cntstr = 0;
        }
return rsltStr;
}



TDString GetSysDayString (SYSTEMTIME *lpSysTime,char delims)
{
TDString stre = "";
if (lpSysTime)
        {
        stre = IntToStr(lpSysTime->wDay).c_str();
        stre = stre + delims + IntToStr(lpSysTime->wMonth).c_str();
        stre = stre + delims + IntToStr(lpSysTime->wYear).c_str();
        }
return stre;
}



TDString GenerateDayString (char delims)
{
TDString lastStr = "";
//unsigned long cntstr = 0;
SYSTEMTIME Tim1;
GetLocalTime(&Tim1);
lastStr = GetSysDayString (&Tim1,delims);
return lastStr;
}



// Изменить в строке первую встречную запятую на точку
void ChangeComaPoint(TDString &str1)
{
char *lpAdr = (char*)str1.c_str();
char tmpb;
while (1)
        {
        tmpb=lpAdr[0];
        if (!tmpb) break;
        if (tmpb==0x2c)
                {
                lpAdr[0]=0x2e;
                break;
                }
        lpAdr++;
        }
}






float StrConvToFloat(TDString str1)
{
ChangeComaPoint(str1);
long double dano=_atold(str1.c_str());
return dano;
}



TDString CutExtFileName (TDString str1)
{
char *lpName = (char*)str1.c_str();
char *lpRes=lpName;
char br;
while (1)
        {
        br=lpName[0];
        if (!br) break;
        if (br==0x2E)
                {
                lpName[0]=0;
                break;
                }
        lpName++;
        }
return ((TDString)lpRes);
}



char* CopyCharString(char *lpDest, char *lpSour, int sizes)
{
if (lpDest && lpSour)
        {
        char tmpb;
        while (sizes)
                {
                tmpb=lpSour[0];
                lpDest[0]=tmpb;
                sizes--;
                lpDest++;
                lpSour++;
                if (!tmpb) break;
                }
        }
return lpDest;
}



unsigned char SWAPBits (unsigned char datas)
{
unsigned char rv=0,cnt7=7;
while (cnt7)
        {
        if (datas & 1) rv=rv | 1;
        datas=datas >> 1;
        rv=rv << 1;
        cnt7--;
        }
if (datas & 1) rv=rv | 1;
return rv;
}



// формирует строку с текущей позиции памяти до указанного разделителя (анализ ведется до конца строки, перевода каретки или искомого разделителя)
char *lpGetNextStrToDelim (TDString &outstr, char *lpCurRams, char DelimChar)
{
char *lpRv = 0;
if (lpCurRams)
        {
        char datas;
        outstr = "";
        unsigned long sizeschar = 0;
        while (true)
                {
                datas = lpCurRams[0];
                if (!datas)
                        {
                        if (!sizeschar) lpCurRams = 0;
                        break;
                        }
                if (datas == 13)
                        {
                        lpCurRams++;
                        datas = lpCurRams[0];
                        if (datas == 10) lpCurRams++;
                        break;
                        }
                if (datas == DelimChar)
                        {
                        lpCurRams++;
                        break;
                        }
                outstr = outstr + datas;
                sizeschar++;
                lpCurRams++;
                }
        lpRv = lpCurRams;
        }
return lpRv;
}





// находит адрес начала указанной строки, индексация идет с 1
char *lpGetAdrLineFromText (char *lpStrInput, unsigned long IndxStr)
{
char *lpRv = 0;
unsigned char curdat,curdat1;
if (IndxStr && lpStrInput)
        {
        unsigned long CurIndxs = 1;

        while (1)       // пропуск необходимых строк
                {
                curdat = lpStrInput[0];
                if (CurIndxs == IndxStr)
                        {
                        lpRv = lpStrInput;
                        break;
                        }
                if (!curdat) break;
                if (curdat == 10) CurIndxs++;
                lpStrInput++;
                }
        }
return lpRv;
}




// возвращает адрес начала следующей строки от текущего адреса
char *lpGetNextAdressLine (char *lpStrInput)
{
char *lpRv = 0;
unsigned char curdat;
if (lpStrInput)
        {
        while (1)       // пропуск необходимых строк
                {
                curdat = lpStrInput[0];
                if (!curdat) break;
                if (curdat == 13)
                        {
                        lpStrInput++;
                        curdat = lpStrInput[0];
                        if (!curdat) break;
                        if (curdat == 10) lpStrInput++;
                        curdat = lpStrInput[0];
                        if (curdat) lpRv = lpStrInput;
                        break;
                        }
                lpStrInput++;
                }
        }
return lpRv;
}




// находит первый значимый символ в строке и возвращает его адрес, функция нужна для пропуска пробелов и табуляций
char *lpFindFistActiveCharFromLine (char *lpStrInput)
{
char *lpRv = 0;
unsigned char curdat;
if (lpStrInput)
        {

        while (1)       // пропуск необходимых строк
                {
                curdat = lpStrInput[0];

                if (!curdat || curdat == 13 || curdat == 10) break;
                if (curdat > 32)
                        {
                        lpRv = lpStrInput;
                        break;
                        }
                lpStrInput++;
                }
        }
return lpRv;
}




char *lpFindFistActiveCharFromLineA (char *lpStrInput)
{
char *lpRv = 0;
unsigned char curdat;
if (lpStrInput)
        {

        while (1)       // пропуск необходимых строк
                {
                curdat = lpStrInput[0];

                if (!curdat) break;
                if (curdat > 32)
                        {
                        lpRv = lpStrInput;
                        break;
                        }
                lpStrInput++;
                }
        }
return lpRv;
}







bool GetStringFromText (TDString inpStr, TDString &outStr, unsigned long IndxStr)
{
bool rv = false;
unsigned char curdat,curdat1;
outStr = "";
if (IndxStr && inpStr!="")
        {
        unsigned long CurIndxs = 1;
        char *lpStrInput = (char*)inpStr.c_str();

        while (true)       // пропуск необходимых строк
                {
                curdat = lpStrInput[0];
                if (CurIndxs == IndxStr) break;
                if (!curdat) break;
                if (curdat == 10) CurIndxs++;
                lpStrInput++;
                }
        if (curdat)
                {
                unsigned long indx = 0;
                char *lpStart_txt = lpStrInput;
                char *lpEnd_txt = lpStrInput;
                while (true)       // копирование строки
                        {
                        (char)curdat = lpStrInput[0];
                        if (curdat == 0 || curdat == 13 || curdat == 10)
                            {
                            lpEnd_txt = lpStrInput;
                            break;
                            }
                        lpStrInput++;
                        indx++;
                        }
                if (lpStart_txt != lpEnd_txt)
                    {
                    char tmp = lpEnd_txt[0];
                    lpEnd_txt[0] = 0;
                    TDString ppt_str = "";
                    ppt_str = lpStart_txt;
                    outStr = ppt_str;
                    lpEnd_txt[0] = tmp;
                    }
                rv = true;
                }
        }
return rv;
}






/*
TSString GetNextString (TSString lpStr,unsigned int ind,unsigned int count)
{
unsigned int lcstr=lpStr.Length()-ind;
if (count>lcstr)
        {
        count=lcstr;
        }
unsigned int cl=0,indxt=ind;
bool fch=false;
for (cl;cl<=count;cl++)
        {
        if (lpStr[ind]!=32)
                {
                if (!fch)
                        {
                        fch=true;
                        indxt=ind;
                        }
                }
        else
                {
                if (fch) break;
                }
        ind++;
        }
if (!fch) return "";
return lpStr.SubString(indxt,ind-indxt);
}
*/




unsigned char GetBitOfs0 (unsigned char cnters)
{
unsigned char rv=1;
cnters=cnters & (unsigned char)7;
while (cnters)
        {
        rv=rv << 1;
        cnters--;
        }
return rv;
}

uchar GetBitValueMasiv(unsigned char *lpAdr,ushort MaxBitMasivs, ushort BeginNumberBits)
{
ushort byteofs=(MaxBitMasivs-BeginNumberBits)/8;
uchar bitofs=GetBitOfs0(BeginNumberBits & 7);
return (lpAdr[byteofs] & bitofs);
}



// Формирует данное из битового масива
ushort GetBitValueFromMasivRam (uchar *lpAdr, ushort MaxBitMasivs, ushort BeginNumberBits, uchar sizebitvalues)
{
ushort rv=0;
while (sizebitvalues)
	{
	rv=rv<<1;
	if (GetBitValueMasiv(lpAdr,MaxBitMasivs,BeginNumberBits))
		{
		rv=rv | 1;
		}
	BeginNumberBits--;
	sizebitvalues--;
	}
return rv;
}




TDString ByteToBinStr (char values)
{
char byteval[9]={0};
char cnt=8;
char bitval=128;
char *lpStr=&byteval[0];
while (cnt)
        {
        if (values & bitval)
                {
                lpStr[0]=49;
                }
        else
                {
                lpStr[0]=48;
                }
        bitval=bitval>>1;
        lpStr++;
        cnt--;
        }
lpStr=&byteval[0];
return (TDString)lpStr;
}


void ByteToBinStr (TDString &strout,unsigned char values)
{
char cnt=8;
unsigned char bitval=128;
strout="";
while (cnt)
        {
        if (values & bitval)
                {
                strout=strout+"1";
                }
        else
                {
                strout=strout+"0";
                }
        bitval=bitval>>1;
        cnt--;
        }
return;
}






unsigned long GetLenStringSz (char *lpAdr, unsigned long sizess)
{
unsigned long rv=0;
if (lpAdr)
        {
        while (sizess)
                {
                if (lpAdr[0]!=0)
                        {
                        rv++;
                        }
                else
                        {
                        break;
                        }
                lpAdr++;
                sizess--;
                }
        }
return rv;
}




unsigned int GetLenStr (char *lpAdr)
{
unsigned int rv=0;
if (lpAdr)
        {
        while (1)
                {
                if (lpAdr[0]!=0)
                        {
                        rv++;
                        }
                else
                        {
                        break;
                        }
                lpAdr++;
                }
        }
return rv;
}


unsigned long GetLenS (char *lpAdr)
{
return GetLenStr (lpAdr);
}


uchar BCDTOBIN (uchar bcddata)
{
uchar tmp=bcddata;
bcddata=((bcddata>>4)&15)*10;
bcddata=bcddata + (tmp & 15);
return bcddata;
}


unsigned char BinToBCDb (unsigned char Bindata)
{
unsigned char BCDData=0;
if (Bindata>9)
        {
        BCDData = ((Bindata/10)<<4) & 0xF0;
        BCDData = BCDData | (Bindata % 10);
        }
else
        {
        BCDData=Bindata;
        }
return BCDData;
}



unsigned int GetStringSizeChar (char *lpStr)
{
unsigned int rv=0;
while (1)
    {
    if (!lpStr[0]) break;
    rv++;
    lpStr++;
    }
return rv;
}


// удаляет пробелы в конце строки
unsigned long CuteEndString_Z (char *lpRamstr, unsigned long sizess)
{
unsigned long rv = 0;
if (sizess)
    {
    char tmpb;
    lpRamstr = lpRamstr + sizess - 1;
    sizess--;
    while (sizess)
        {
        tmpb = lpRamstr[0];
        if (tmpb > 32) break;
        lpRamstr--;
        sizess--;
        }
    rv = sizess;
    lpRamstr[0] = 0;
    }

return rv;
}



char *CopyMemorySDC (char *lpSour, char *lpDest, unsigned int sizes)
{
if (lpSour && lpDest && sizes) {
    while (sizes) {
        *lpDest++ = *lpSour++;
        sizes--;
        }
    }
return lpDest;
}



void MoveDownMemory (char *lpSour, unsigned int sizes, unsigned long BlockSizeDown)
{
if (lpSour && sizes)
    {
    char *lpDest = lpSour + BlockSizeDown - 1;
    lpSour = lpDest - sizes;
    while (BlockSizeDown)
        {
        lpDest[0] = lpSour[0];
        lpDest--;
        lpSour--;
        BlockSizeDown--;
        }
    }
}



unsigned long *CopyMemorySDC_cl (unsigned char *lpSour, unsigned long *lpDest, unsigned int sizes)
{
if (lpSour && lpDest && sizes)
        {
        while (sizes)
                {
                lpDest[0]= lpSour[0];
                lpDest++;
                lpSour++;
                sizes--;
                }
        }
return lpDest;
}



unsigned char *CopyMemorySDC_lc (unsigned long *lpSour, unsigned char *lpDest, unsigned int sizes)
{
if (lpSour && lpDest && sizes)
        {
        while (sizes)
                {
                lpDest[0]= lpSour[0];
                lpDest++;
                lpSour++;
                sizes--;
                }
        }
return lpDest;
}



char *CopyStringToRaM (char *lpDest, unsigned int sizes, TDString datsstr)
{
char *lpRv = lpDest;
if (sizes && lpDest)
        {
        lpDest[0] = 0;
        lpDest[sizes-1] = 0;
        if (datsstr!="")
                {
                char *lpStrRam = (char*)datsstr.c_str();
                char tmpb;
                if (lpStrRam)
                        {
                        while (sizes)
                                {
                                tmpb = lpStrRam[0];
                                lpRv[0] = tmpb;
                                lpRv++;
                                if (!tmpb) break;
                                lpStrRam++;
                                sizes--;
                                }
                        }
                }
        }
return lpRv;
}





unsigned char CompareStringS2 (char *lpStr1,char *lpStr2,unsigned int sizes)
{
unsigned char rv=0;
char bytea,byteb;
if (lpStr1 && lpStr2 && sizes)
        {
        rv=1;
        while (sizes)
                {
                bytea=lpStr1[0];
                byteb=lpStr2[0];
                if (bytea!=byteb || !bytea || !byteb)
                        {
                        rv=0;
                        break;
                        }
                lpStr1++;
                lpStr2++;
                sizes--;
                }
        }
return rv;
}



unsigned char CompareStringS1 (char *lpRam,char *lpStr2,unsigned int sizes)
{
unsigned char rv=0;
char bytea,byteb;
if (lpRam && lpStr2 && sizes)
        {
        rv=1;
        while (sizes)
                {
                bytea=lpRam[0];
                byteb=lpStr2[0];
                if (bytea!=byteb || !bytea)
                        {
                        if (byteb) rv=0;
                        break;
                        }
                lpRam++;
                lpStr2++;
                sizes--;
                }
        }
return rv;
}



// Если, обнаружено совпадение - возвращает адрес строки в начале совпадения стринга, иначе = 0.
TDString FindStringStart (TDString Str1,TDString Str2)
{
TDString rv="";
char *lpRv=0;
if (Str1!="" && Str2!="")
        {
        char *lpStr1 = (char*)Str1.c_str();
        char *lpStr2 = (char*)Str2.c_str();
        char bytea,byteb;
        char indxstr2=0;
        bool flagfs=0;
        while (1)
                {
                bytea=lpStr1[0];
                byteb=lpStr2[indxstr2];
                if (!byteb) break;
                if (!bytea)
                        {
                        lpRv=0;
                        break;
                        }
                if (byteb!=bytea)
                        {
                        indxstr2=0;
                        }
                else
                        {
                        if (!flagfs) lpRv=lpStr1;
                        indxstr2++;
                        }
                lpStr1++;
                }
        }
if (lpRv) rv=(TDString)lpRv;
return rv;
}

// Если, обнаружено совпадение - возвращает адрес строки в конце совпадения стринга, иначе = 0.
TDString FindStringEnd (TDString Str1, TDString Str2)
{
TDString rv="";
char *lpRv=0;
if (Str1!="" && Str2!="")
        {
        char *lpStr1 = (char*)Str1.c_str();
        char *lpStr2 = (char*)Str2.c_str();
        char bytea,byteb;
        char indxstr2=0;
        while (1)
                {
                bytea=lpStr1[0];
                byteb=lpStr2[indxstr2];
                if (!byteb)
                        {
                        break;
                        }
                if (!bytea)
                        {
                        lpRv=0;
                        break;
                        }
                if (byteb!=bytea)
                        {
                        indxstr2=0;
                        }
                else
                        {
                        lpRv=lpStr1+1;
                        indxstr2++;
                        }
                lpStr1++;
                }
        }
if (lpRv) rv=(TDString)lpRv;
return rv;
}


// Формирует стринг с текущей позиции char* указателя, до первого обнаруженого пробела или конца строки
TDString CreateSubString (char *lpAdr)
{
TDString rv="";
if (lpAdr)
        {
        char *lpfindp=lpAdr;
        char *lpChangep=0;
        char byted;
        while (1)
                {
                byted=lpfindp[0];
                if (!byted)
                        {
                        break;
                        }
                else
                        {
                        if (byted==32 || byted == 13)
                                {
                                lpChangep=lpfindp;
                                break;
                                }
                        }
                lpfindp++;
                }
        if (lpChangep) lpChangep[0]=0;
        rv=(TDString)lpAdr;
        if (lpChangep) lpChangep[0]=32;
        }
return rv;
}



// Формирует стринг с текущей позиции char* указателя, до конца строки или перевода каретки
TDString CreateSubString_cor (char *lpAdr)
{
TDString rv="";
if (lpAdr)
    {
    char *lpfindp=lpAdr;
    char *lpChangep=0;
    char byted;
    while (1)
        {
        byted=lpfindp[0];
        if (!byted)
            {
            break;
            }
        else
            {
            if (byted == 13)
                {
                lpChangep=lpfindp;
                break;
                }
            }
        lpfindp++;
        }
    if (lpChangep) lpChangep[0]=0;
    rv=(TDString)lpAdr;
    if (lpChangep) lpChangep[0] = 13;
    }
return rv;
}



// Формирует стринг с текущей позиции char* указателя
bool CreateStringFromLine (TDString &OutStr, char *lpAdr)
{
bool rv = 0;
OutStr = "";
if (lpAdr)
        {
        char *lpfindp=lpAdr;
        //char *lpChangep=0;
        char byted;
        while (1)
                {
                byted=lpfindp[0];
                if (!byted || byted == 13 || byted == 10) break;
                OutStr = OutStr + byted;
                rv = 1;
                lpfindp++;
                }
        }
return rv;
}




void CreateStringFromLine_cnt (TDString &OutStr, char *lpAdr,unsigned long CntBt)
{
OutStr = "";
if (lpAdr)
        {
        char *lpfindp=lpAdr;
        //char *lpChangep=0;
        char byted;
        while (CntBt)
                {
                byted=lpfindp[0];
                if (!byted) break;
                if (byted == 13 || byted == 10) byted = 32;
                OutStr = OutStr + byted;
                lpfindp++;
                CntBt--;
                }
        }
}



// Возвращает количество любых элементов строки разделенных пробелами
unsigned int GetCountStringElement ( TDString Str1 )
{
unsigned int rv=0;
if (Str1!="")
        {
        char *lpAdrStr = (char*)Str1.c_str();
        bool flafed=0;
        char datas;
        while (1)
                {
                datas=lpAdrStr[0];
                if (!datas || datas == 13 || datas == 10)
                        {
                        break;
                        }
                else
                        {
                        if (datas==32)
                                {
                                flafed=0;
                                }
                        else
                                {
                                if (!flafed)
                                        {
                                        flafed=1;
                                        rv++;
                                        }
                                }
                        }
                lpAdrStr++;
                }

        }
return rv;
}



// подсчитывает количество слов в строке или извлекает слово по его индекс-номеру (поиск идет с 0)
long CreateWordStringFromIndxLine (char *lpLinetxt, TDString &outstr,unsigned long Indxx,char delimc)
{
long rv = -1;
if (lpLinetxt)
        {
        unsigned long lcinx = 0;
        char tmpb;
        outstr = "";
        while (true)
                {
                tmpb = lpLinetxt[0];
                if (tmpb == 9) tmpb = 32;
                if (!tmpb || tmpb == 13 || tmpb == 10)
                        {
                        break;
                        }
                else
                        {
                        if (tmpb == delimc)
                                {
                                if (Indxx == lcinx) break;
                                lcinx++;
                                outstr = "";
                                }
                        else
                                {
                                outstr = outstr + tmpb;
                                }
                        }

                lpLinetxt++;
                }
        rv = lcinx;     // номер индекса извлеченного слова или максимальное количество слов в строке + 1
        }
return rv;
}


// находит поле разделенное разделителеми по его индексу, подсчитывает размер этого стринга и передает адрес вхождения
unsigned long GetTagStringDelimIndx (char *lpLinetxt, unsigned long Indxx, char delimc, char **lDest, unsigned long *lPCountField)
{
unsigned long rv = 0;
if (lpLinetxt)
		{
		unsigned long size = 0;
		unsigned long lcinx = 0;
		unsigned long countfield = 0;
		char *lFistInp = 0;
		char tmpb;
		bool f_fist_input = false;
		while (true)
				{
				tmpb = lpLinetxt[0];
				if (tmpb == 9) tmpb = 32;
				if (!tmpb || tmpb == 13 || tmpb == 10)
					{
					if (Indxx != lcinx) f_fist_input = false;
					countfield++;
					break;
					}
				else
					{
					if (tmpb == delimc)
						{
						countfield++;
						if (Indxx == lcinx) 
							{
							break;
							}
						lcinx++;
						f_fist_input = false;
						lFistInp = 0;
						size = 0;
						}
					else
						{
						if (f_fist_input == false)
							{
							lFistInp = lpLinetxt;
							f_fist_input = true;
							}
						size++;
						}
					}
				lpLinetxt++;
				}
		if (!f_fist_input)
			{
			size = 0;
			lFistInp = 0;
			}
		if (lDest) *lDest = lFistInp;
		if (lPCountField) *lPCountField = countfield;
		rv = size;     
		}
return rv;
}



bool GetTagStringDelimIndx (TBUFPARAM *lInput, TBUFPARAM *lOutput, unsigned long Indxx, char delimc, unsigned long *lPCountField) // char **lDest,
{
bool rv = false;
char *lpLinetxt = (char*)lInput->lRam;
unsigned long sz = lInput->sizes;
if (lpLinetxt)
		{
		unsigned long size = 0;
		unsigned long lcinx = 0;
		unsigned long countfield = 0;
		char *lFistInp = 0;
		char tmpb;
		bool f_fist_input = false;
		while (sz)
				{
				tmpb = lpLinetxt[0];
				if (tmpb == 9) tmpb = 32;
				if (!tmpb || tmpb == 13 || tmpb == 10)
					{
					if (Indxx != lcinx) f_fist_input = false;
					countfield++;
					break;
					}
				else
					{
					if (tmpb == delimc)
						{
						countfield++;
						if (Indxx == lcinx) 
							{
							rv = true;
							break; // tag finded
							}
						lcinx++;
						f_fist_input = false;
						lFistInp = 0;
						size = 0;
						}
					else
						{
						if (f_fist_input == false)
							{
							lFistInp = lpLinetxt;
							f_fist_input = true;
							}
						size++;
						}
					}
				lpLinetxt++;
				sz--;
				}
		if (!sz)
			{
			countfield++;
			if (Indxx == lcinx) rv = true;
			}
		if (!f_fist_input)
			{
			size = 0;
			lFistInp = 0;
			}
		if (lPCountField) *lPCountField = countfield;   
		if (lOutput)
			{
			lOutput->lRam = (char*)lFistInp;
			lOutput->sizes = size;
			}
		}
return rv;
}




// подсчитывает количество слов в строке или извлекает слово по его индекс-номеру (поиск идет с 0)
long CreateWordStringFromIndxLineAndCF (char *lpLinetxt, TDString &outstr,unsigned long Indxx,char delimc)
{
long rv = -1;
if (lpLinetxt)
        {
        unsigned long lcinx = 0;
        char tmpb;
        outstr = "";
        bool F_finded_word = false;     // найдено любое слово
        bool F_is_forming = false;      // формируется выходное слово
        bool F_end_str = false;         // признак конца входящего
        while (true)
            {
            tmpb = lpLinetxt[0];
            if (!tmpb) F_end_str = true;
            if (tmpb == 9 || tmpb == 13 || tmpb == 10 || !tmpb) tmpb = ' ';
            if (tmpb == delimc)
                {
                if (F_is_forming)
                    {
                    // формирование стринга завершено
                    rv = lcinx;
                    break;
                    }
                if (F_finded_word)
                    {
                    lcinx++;
                    F_finded_word = false;
                    }
                }
            else
                {
                if (!F_finded_word)
                    {
                    // первый символ в слове
                    if (Indxx == lcinx) F_is_forming = true;
                    F_finded_word = true;
                    }
                if (F_is_forming)
                    {
                    outstr = outstr + tmpb;
                    }
                }
            if (F_end_str)
                {
                //rv = lcinx;
                break;
                }
            lpLinetxt++;
            }
        }
return rv;
}





// подсчитывает количество полей разделенных разделителями до конца строки
unsigned long GetCountStringElementDelim ( char *lpAdrStr, char DelimCode)
{
unsigned long rv=0;
if (lpAdrStr)
        {
        char datas;
        bool F_dat = false;
        while (1)
                {
                datas = lpAdrStr[0];
                if (!datas)
                        {
                        if (F_dat) rv++;
                        break;
                        }
                else
                        {
                        if (datas == 13 || datas == 10)
                                {
                                rv++;
                                break;
                                }
                        else
                                {
                                if (datas == DelimCode)
                                        {
                                        rv++;
                                        F_dat = false;
                                        }
                                else
                                    {
                                    F_dat = true;
                                    }
                                }
                        }
                lpAdrStr++;
                }
        }
return rv;
}





// подсчитывает количество полей разделенных разделителями до конца строки
unsigned long GetCountStringElementDelim ( TDString Str1, char DelimCode )
{
unsigned long rv=0;
if (Str1!="")
        {
        char *lpAdrStr = (char*)Str1.c_str();
        rv = GetCountStringElementDelim (lpAdrStr, DelimCode);
        }
return rv;
}





// Вспомогательная субрутина
unsigned int GetCntWordsEndNumeric (unsigned char *lpAdr)
{
unsigned int rv=0;
if (lpAdr)
        {
        uchar temff;
        while (1)
                {
                temff=lpAdr[0];
                if (temff)
                        {
                        if (temff>=48 && temff<=57)
                                {
                                lpAdr++;
                                rv++;
                                }
                        else
                                {
                                break;
                                }
                        }
                else
                        {
                        break;
                        }
                }
        }

return rv;
}



bool CompareStrings (char *lpString1,char *lpString2)
{
bool rv = false;
char dummy1,dummy2;
while (1)
	{
	dummy1=lpString1[0];
	dummy2=lpString2[0];
    if (dummy1 != dummy2) break;
	if (!dummy1)
        {
        rv = true;
        break;
        }
	lpString1++;
	lpString2++;
	}
return rv;
}



char CheckDiapazon2ValuesStr (char *lpStrZ, unsigned char MinVal1, unsigned char MaxVal1, unsigned char MinVal2, unsigned char MaxVal2)
{
char rv = 0;
if (lpStrZ)
    {
    unsigned char Val;
    while (1)
        {
        Val = lpStrZ[0];
        if (!Val)
            {
            rv = 1;
            break;
            }
        if (((Val >= MinVal1 && Val <= MaxVal1)) || ((Val >= MinVal2) && (Val <= MaxVal2)));
        else break;
        lpStrZ++;
        }
    }
return rv;
}



// если равны = 0, если второй стринг больше = 1, если второй стринг меньше = -1
char CmpString (char *lpString1,char *lpString2)
{
char rv=0;
unsigned char dummy1,dummy2;
while (1)
	{
	dummy1 = lpString1[0];
	dummy2 = lpString2[0];

        // анализ завершения строк
        if (!dummy1)
                {
                if (!dummy2)
                        {
                        // стринги равны rv = 0
                        break;
                        }
                else
                        {
                        rv = 1; // второе имя больше по коду и длине
                        break;
                        }
                }
        else
                {
                if (!dummy2)
                        {
                        //
                        rv = -1;
                        break;
                        }
                else
                        {
                        if (dummy1 != dummy2)
                                {
                                if (dummy2 > dummy1)
                                        {
                                        rv = 1;
                                        }
                                else
                                        {
                                        rv = -1;
                                        }
                                break;
                                }
                        }
                }
	lpString1++;
	lpString2++;
	}
return rv;
}




unsigned char LTetraToHex (unsigned char datas)
{
unsigned char rv;
datas = datas & 0x0F;
if (datas >=  10)
        {
        rv = (datas - 10) + 65;
        }
else
        {
        rv = 48 + datas;
        }
return rv;
}



void DatasToHexs_Str (unsigned char *lpRamData, unsigned int sizesdat, TDString &outptsstr)
{
if (lpRamData)
        {
        unsigned char tmpdat;
        char bufsd[2];
        bufsd[1] = 0;
        while (sizesdat)
                {
                tmpdat = lpRamData[0];
                //outptsstr = outptsstr+"0x";
                bufsd[0] = LTetraToHex (tmpdat>>4);
                outptsstr = outptsstr + (TDString)bufsd;
                bufsd[0] = LTetraToHex (tmpdat);
                outptsstr = outptsstr + (TDString)bufsd;;
                sizesdat--;
                lpRamData++;
                if (sizesdat) outptsstr = outptsstr+",";
                }
        }
}



TDString ToUpperCase (TDString sstr) // for win1251
{
TDString rvstr = "";
unsigned long cnt = sstr.length();
char dat;
char *lInp = (char*)sstr.c_str();
while (cnt)
    {
    dat = *lInp++;
    // engl
    if (dat >= 'a' && dat <= 'z')
        {
        dat -= 32;
        }
    else
        {
        // rus
        if (dat >= 0xE0 && dat <= 0xFF)
            {
            dat -= 32;
            }
        }
    rvstr += dat;
    cnt--;
    }
return rvstr;
}



TDString ToLowerCase (TDString sstr) // for win1251
{
TDString rvstr = "";
unsigned long cnt = sstr.length();
char dat;
char *lInp = (char*)sstr.c_str();
while (cnt)
    {
    dat = *lInp++;
    // engl
    if (dat >= 'A' && dat <= 'Z')
        {
        dat += 32;
        }
    else
        {
        // rus
        if (dat >= 0xC0 && dat <= 0xDF)
            {
            dat += 32;
            }
        }
    rvstr += dat;
    cnt--;
    }
return rvstr;
}



void BinToHex2 (unsigned char *lpRamData, unsigned int sizesdat, TDString &outptsstr)
{
if (lpRamData)
        {
        unsigned char tmpdat;
        char bufsd[2];
        bufsd[1] = 0;
        while (sizesdat)
                {
                tmpdat = lpRamData[0];
                bufsd[0] = LTetraToHex (tmpdat>>4);
                outptsstr = outptsstr + (TDString)bufsd;
                bufsd[0] = LTetraToHex (tmpdat);
                outptsstr = outptsstr + (TDString)bufsd;;
                sizesdat--;
                lpRamData++;
                }
        }
}



unsigned int Cur10Stepen (unsigned int tempp)
{
unsigned int rv=1;
while (tempp)
        {
        rv=rv*10;
        tempp--;
        }
return rv;
}




int ConvertStringToInt ( char *lpStr)
{
int rv=0;
if (lpStr)
        {
        uchar minuszn=0;
        uchar datt;
        while (1)
                {
                datt=lpStr[0];
                if (!datt) break;
                if (datt==' ')
                        {
                        lpStr++;
                        }
                else
                        {
                        break;
                        }
                }
        datt=lpStr[0];
        if (datt)
                {
                if (datt=='-')
                        {
                        minuszn=1;
                        lpStr++;
                        }
                datt=lpStr[0];
                if (datt)
                        {
                        unsigned int tempp=GetCntWordsEndNumeric (lpStr);
                        if (tempp)
                                {
                                unsigned int desst=Cur10Stepen(tempp-1);
                                while (tempp)
                                        {
                                        rv=rv+desst*(lpStr[0]-48);
                                        lpStr++;
                                        if (desst) desst=desst/10;
                                        tempp--;
                                        }
                                if (minuszn) rv=-rv;
                                }
                        }
                }
        }
return rv;
}

// Вспомогательная субрутина
unsigned char *FindNextNumbW1 (unsigned char *lpAdress)
{
unsigned char *lpRam=0;
if (lpAdress)
        {
        bool flagNoNumbWord=0;
        unsigned char tempd;
        while (1)
                {
                tempd=lpAdress[0];
                if (!tempd)
                        {
                        break;
                        }
                else
                        {
                        if (tempd>=48 && tempd<=57)
                                {
                                if (flagNoNumbWord)
                                        {
                                        lpRam=lpAdress;
                                        break;
                                        }
                                }
                        else
                                {
                                flagNoNumbWord=1;
                                }

                        }
                lpAdress++;
                }
        }
return lpRam;
}


unsigned int ConvertTimeStringToSpecialDataInt (TDString striit)
{
// int atoi(const char *s);
unsigned int rv=0;
if (striit!="")
        {
        unsigned char *lpStr = (unsigned char*)striit.c_str();
        unsigned int sminets=0,sseconds=0;
        unsigned int hours=ConvertStringToInt (lpStr);
        lpStr=FindNextNumbW1(lpStr);
        if (lpStr)
                {
                sminets=ConvertStringToInt (lpStr);
                lpStr=FindNextNumbW1(lpStr);
                if (lpStr)
                        {
                        sseconds=ConvertStringToInt (lpStr);
                        }
                }
        rv=(3600*hours)+(sminets*60)+sseconds;
        }
return rv;
}


// Возвращает стринг под номером указаным по 'numbword'
TDString GetStringWord (TDString Str1, int numbword)
{
TDString rv="";
if (Str1!="" && numbword)
        {
        char *lpStr = (char *)Str1.c_str();
        int cnterword=0;
        bool flagw=0;
        char bytes;
        while (1)
                {
                bytes=lpStr[0];
                if (!bytes)
                        {
                        break;
                        }
                else
                        {
                        if (bytes==32)
                                {
                                flagw=0;
                                }
                        else
                                {
                                if (!flagw)
                                        {
                                        flagw=1;
                                        cnterword++;
                                        if (cnterword==numbword)
                                                {
                                                rv=CreateSubString(lpStr);
                                                break;
                                                }
                                        }
                                }
                        }
                lpStr++;
                }

        }
return rv;
}



// Возвращает стринг под номером указаным по 'numbword'
bool GetStringWord (TDString Str1, TDString &outstr, int numbword)
{
bool rv=0;
outstr="";
if (Str1!="" && numbword)
        {
        char *lpStr = (char *)Str1.c_str();
        int cnterword=0;
        bool flagw=0;
        char bytes;
        while (1)
                {
                bytes=lpStr[0];
                if (!bytes)
                        {
                        break;
                        }
                else
                        {
                        if (bytes==32)
                                {
                                flagw=0;
                                }
                        else
                                {
                                if (!flagw)
                                        {
                                        flagw=1;
                                        cnterword++;
                                        if (cnterword==numbword)
                                                {
                                                rv=1;
                                                outstr=CreateSubString(lpStr);
                                                break;
                                                }
                                        }
                                }
                        }
                lpStr++;
                }
        }
return rv;
}




// Возвращает адрес обнаруженного символа
char *FindByte (char *lpSearch, char fb)
{
char *rv=0;
char tmpb;
while (1)
        {
        tmpb=lpSearch[0];
        if (!tmpb) break;
        if (tmpb==fb)
                {
                rv=lpSearch;
                break;
                }
        lpSearch++;
        }
return rv;
}


// Определяет сколько цифр десятичной системе шли подряд от текущего адреса
int GetCntNumber (char *lpStr)
{
int rv=0;
char tmpb;
while (1)
        {
        tmpb=lpStr[0];
        if (tmpb>=0x30 && tmpb<=0x39)
                {
                rv++;
                }
        else
                {
                break;
                }
        lpStr++;
        }
return rv;
}





// сравнивает участки памяти
char *lpCompareRam (char *lpStrBase, char *lpString, unsigned long sizess)
{
char *lpRv = 0;
if (sizess)
        {
        char tmpb;
        while (sizess)
                {
                if (lpStrBase[0] != lpString[0]) break;
                lpStrBase++;
                lpString++;
                sizess--;
                }
        if (!sizess) lpRv = lpStrBase;
        }
return lpRv;
}


// преобразует данные из текста по указаному адресу в unsigned long число
// если число больше чем 10 знакомест или его нет в первом байте указанного адресса, возвращает 0 - (ошибка)
unsigned char *ConvertStrToULong (unsigned char *lpRamData, unsigned long *lpDataOut)
{
unsigned char *lprv = 0;
if (lpRamData)
        {
        // найти конец цифровой строки
        unsigned char dtasd;
        unsigned long cnterN = 0;
        unsigned char *lpTmpAdr = lpRamData;
        while (true)
            {
            dtasd = lpTmpAdr[0];
            if (dtasd < 48 || dtasd > 57) break;
            cnterN++;
            lpTmpAdr++;
            }
        // в cnterN количество цифр
        if (cnterN && cnterN < C_MULTDV_L32_AMOUNT)
            {
            unsigned long Multer;
            unsigned long DigOut = 0;
            while (cnterN)
                {
                Multer = tmpMultData[cnterN-1];
                DigOut = DigOut + (Multer * (lpRamData[0] - 48));
                lpRamData++;
                cnterN--;
                }
            if (lpDataOut) *lpDataOut = DigOut;
            lprv = lpRamData;
            }
        }
return lprv;
}


// преобразует данные из текста по указаному адресу в unsigned long число
// если число больше чем 10 знакомест или его нет в первом байте указанного адресса, возвращает 0 - (ошибка)
unsigned char *ConvertStrToLong (unsigned char *lpRamData, long *lpDataOut)
{
unsigned char *lprv = 0;
bool f_subzero = false;
if (lpRamData)
    {
    lprv = lpRamData;
    if (*lprv == '-')
        {
        lprv++;
        f_subzero = true;
        }
    unsigned long udata;
    long data;
    lprv = ConvertStrToULong (lprv, &udata);
    if (lprv)
        {
        data = (long)udata;
        if (data > 0) {
            if (f_subzero) data *= -1;
            }
        }
    if (lprv && lpDataOut) *lpDataOut = data;
    }
return lprv;
}



unsigned char *ConvertStrToLong_Cnt (unsigned char *lpRamData, long *lpDataOut, unsigned char cntdat)
{
unsigned char *lrv = 0;
static const CMAXBUF = 16;
if (lpRamData && cntdat && cntdat < (CMAXBUF-1))
    {
    lrv = lpRamData;
    unsigned char BufIn[CMAXBUF];
    long DatOut;
    TEX::BUFPAR Dpar;
    Dpar.lpRam = BufIn;
    Dpar.Sizes = cntdat;

    lrv = CopyStrTo (&Dpar, (char*)lrv);
    if (lrv)
        {
        BufIn[cntdat] = 0;
        lrv = ConvertStrToLong (BufIn, &DatOut);
        if (lrv)
            {
            if (lpDataOut) *lpDataOut = DatOut;
            }
        }
    }
return lrv;
}



unsigned char *ConvertStrToULong64 (unsigned char *lpRamData, uint64_t *lpDataOut)
{
unsigned char *lprv = 0;
if (lpRamData && lpDataOut)
        {
        // найти конец цифровой строки
        unsigned char dtasd;
        unsigned char cnterN = 0;
        unsigned char *lpTmpAdr = lpRamData;
        while (true)
                {
                dtasd = lpTmpAdr[0];
                if (dtasd < 48 || dtasd > 57) break;
                cnterN++;
                lpTmpAdr++;
                }
        // в cnterN количество цифр
        if (cnterN && cnterN < C_MULTDV_L64_AMOUNT)
                {
                uint64_t Multer;
                uint64_t DigOut = 0;
                while (cnterN)
                      {
                      Multer = tmpMultData64[cnterN-1];
                      DigOut = DigOut + (Multer * (lpRamData[0] - 48));
                      lpRamData++;
                      cnterN--;
                      }
                lpDataOut[0] = DigOut;
                lprv = lpRamData;
                }
        }
return lprv;
}



unsigned char *ConvertStrToLong64 (unsigned char *lpRamData, int64_t *lpDataOut)
{
char *lprv = lpRamData;
if (lprv && lpDataOut)
        {
        bool f_subzero = false;
        if (*lprv == '-')
            {
            lprv++;
            f_subzero = true;
            }
        uint64_t OutLU;
        int64_t OutL;
        lprv = ConvertStrToULong64 (lprv, &OutLU);
        if (lprv)
            {
            OutL = (int64_t)OutLU;
            if (OutL > 0)
                {
                if (f_subzero) OutL *= -1;
                }
            }
        if (lprv) *lpDataOut = OutL;
        }
return lprv;
}



unsigned char *ConvertStrToLong64_Cnt (unsigned char *lInStr, int64_t *lpDataOut, unsigned char cntdat)
{
static const CMAXBUF = 32;
if (lInStr && cntdat && cntdat < (CMAXBUF-1))
    {
    unsigned char BufIn[CMAXBUF];
    int64_t DatOut;
    TEX::BUFPAR Dpar;
    Dpar.lpRam = BufIn;
    Dpar.Sizes = cntdat;

    lInStr = CopyStrTo (&Dpar, (char*)lInStr);
    if (lInStr)
        {
        BufIn[cntdat] = 0;
        lInStr = ConvertStrToLong64 (BufIn, &DatOut);
        if (lInStr)
            {
            if (lpDataOut) *lpDataOut = DatOut;
            }
        }
    }
else
    {
    lInStr = 0;
    }
return lInStr;
}





unsigned char CompareStringS1Z (char *lpRam,char *lpStr2,unsigned int sizes)
{
unsigned char rv=0;
char bytea,byteb;
if (lpRam && lpStr2 && sizes)
        {
        rv=1;
        while (sizes)
                {
                bytea=lpRam[0];
                byteb=lpStr2[0];
                if (!bytea)
                        {
                        // если в буфере конец строки
                        if (byteb) rv = 0;      // если строка сравнения не закончилась
                        break;
                        }
                if (!byteb)
                        {
                        // если конец строки сравнения
                        if (bytea > 32) rv = 0; // если в буфере слово не прервалось
                        break;
                        }
                if (bytea!=byteb)
                        {
                        // если совпадения нет
                        rv=0;
                        break;
                        }
                lpRam++;
                lpStr2++;
                sizes--;
                }
        }
return rv;
}



// Находит совпадение со строкой и возвращает адрес совпадения строки
char *FindStringZ (char *lpSource,char *lpString,unsigned int sizes)
{
char *lpPoint=0;
unsigned int cursizes=sizes;
char *lpFistAdr=lpSource;
while (1)
        {
        if (lpSource[0]==0) break;
        if (CompareStringS1Z(lpSource,lpString,cursizes))
                {
                lpPoint=lpSource;
                break;
                }
        lpSource++;
        cursizes=sizes-(lpSource-lpFistAdr);
        }
return lpPoint;
}






// Находит совпадение со строкой и возвращает адрес совпадения строки
char *FindString (char *lpSource,char *lpString,unsigned int sizes)
{
char *lpPoint=0;
unsigned int cursizes=sizes;
char *lpFistAdr=lpSource;
while (1)
        {
        if (lpSource[0]==0) break;
        if (CompareStringS1(lpSource,lpString,cursizes))
                {
                lpPoint=lpSource;
                break;
                }
        lpSource++;
        cursizes=sizes-(lpSource-lpFistAdr);
        }
return lpPoint;
}



float StepenDesiatki (char stpn)
{
if (!stpn) return 1;
float rv=1;
while (stpn)
        {
        rv=rv/10;
        stpn--;
        }
return rv;
}



float GetNumberFromString (char *lpStr, char cntb)
{
long double rv=-1;
if (!lpStr || !cntb) return rv;
char tmpb=lpStr[cntb];
lpStr[cntb]=0;
rv=_atold(lpStr);
lpStr[cntb]=tmpb;
return rv;
}


float GetXPosHole(char *lpstr, char cntdatdroba)
{
long double rv=-1;
if (!lpstr || !cntdatdroba) return rv;
char *lpBXC=FindByte(lpstr,0x58);
if (lpBXC)
        {
        lpBXC++;
        char cntcd=GetCntNumber(lpBXC);
        if (cntcd>=cntdatdroba)
                {
                float celchast;
                float drobachast;
                if (cntcd==cntdatdroba)
                        {
                        celchast=0;
                        drobachast=GetNumberFromString(lpBXC,cntdatdroba);
                        }
                else
                        {
                        celchast=GetNumberFromString(lpBXC,(cntcd-cntdatdroba));
                        drobachast=GetNumberFromString(lpBXC+(cntcd-cntdatdroba),cntdatdroba);
                        }
                drobachast=drobachast*(StepenDesiatki(cntdatdroba));
                rv=celchast+drobachast;
                }
        }
return rv;
}

float GetYPosHole(char *lpstr, char cntdatdroba)
{
long double rv=-1;
if (!lpstr || !cntdatdroba) return rv;
char *lpBXC=FindByte(lpstr,0x59);
if (lpBXC)
        {
        lpBXC++;
        char cntcd=GetCntNumber(lpBXC);
        if (cntcd>=cntdatdroba)
                {
                float celchast;
                float drobachast;
                if (cntcd==cntdatdroba)
                        {
                        celchast=0;
                        drobachast=GetNumberFromString(lpBXC,cntdatdroba);
                        }
                else
                        {
                        celchast=GetNumberFromString(lpBXC,(cntcd-cntdatdroba));
                        drobachast=GetNumberFromString(lpBXC+(cntcd-cntdatdroba),cntdatdroba);
                        }
                drobachast=drobachast*(StepenDesiatki(cntdatdroba));
                rv=celchast+drobachast;
                }
        }
return rv;
}


float ExtractDrillDiameter (char *lpstr)
{
long double rv=0;
char tmpb;
char *lpBeginAdr=0;
char *lpEndAdr=0;
char flagn=0;
while (lpstr)
        {
        tmpb=lpstr[0];
        if (!tmpb)
                {
                lpEndAdr=lpstr;
                break;
                }
        if (flagn)
                {
                if (tmpb!=0x2E)
                        {
                        if (tmpb<0x30 || tmpb>0x39)
                                {
                                lpEndAdr=lpstr;
                                break;
                                }
                        }
                }
        if (tmpb==0x43)
                {
                lpBeginAdr=lpstr+1;
                flagn=1;
                }
        lpstr++;
        }
if (lpBeginAdr && lpEndAdr && lpEndAdr>lpBeginAdr)
        {
        tmpb=lpEndAdr[0];
        lpEndAdr[0]=0;
        rv=_atold(lpBeginAdr);
        lpEndAdr[0]=tmpb;
        }
return rv;
}






unsigned char GetByteHexValue2 (char *lpDatas)
{
unsigned char rv=0;
if (lpDatas[0]!=0 && lpDatas[1]!=0)
        {
        rv = ConvertHexVal(lpDatas[0])<<4;
        rv |= ConvertHexVal(lpDatas[1]);
        }
return rv;
}


unsigned char StrHex2ToUChar (char *lpDatas)
{
unsigned char rv=0;
if (lpDatas)
        {
        unsigned char v1=lpDatas[0];
        if (v1)
                {
                unsigned char v2=lpDatas[1];
                if (v2)
                        {
                        rv=ConvertHexVal(v1)*16;
                        rv=rv+ConvertHexVal(v2);
                        }
                else
                        {
                        rv=ConvertHexVal(v1);
                        }
                }
        }
return rv;
}



bool Hex2Bin (char *lInHex, char *outbin, unsigned long hexin_sz)
{
bool rv = false;
if (lInHex && outbin && hexin_sz)
    {
    unsigned long pair_b = hexin_sz / 2;
    unsigned char d1, d2, data;

    while (pair_b)
        {
        if (!CheckHexVal (lInHex[0]) || !CheckHexVal (lInHex[1])) return false;
        *outbin = StrHex2ToUChar (lInHex);
        lInHex += 2; outbin++;
        pair_b--;
        }
    if (hexin_sz & 1)
        {
        if (!CheckHexVal (lInHex[0])) return false;
        *outbin = ConvertHexVal (lInHex[0]);
        }
    rv = true;
    }
return rv;
}


unsigned short StrHex4ToUShort (char *lpDatas)
{
unsigned short rv=0;
if (lpDatas)
        {
        unsigned int cntst=GetLenStr (lpDatas);
        switch (cntst)
                {
                case 1:
                        {
                        rv=ConvertHexVal(lpDatas[0]);
                        break;
                        }
                case 2:
                        {
                        rv=ConvertHexVal(lpDatas[0])*16;
                        rv=rv+ConvertHexVal(lpDatas[1]);
                        break;
                        }
                case 3:
                        {
                        rv=ConvertHexVal(lpDatas[0])*256;
                        rv=rv+ConvertHexVal(lpDatas[1])*16;
                        rv=rv+ConvertHexVal(lpDatas[2]);
                        break;
                        }
                case 4:
                        {
                        rv=ConvertHexVal(lpDatas[0])*4096;
                        rv=rv+ConvertHexVal(lpDatas[1])*256;
                        rv=rv+ConvertHexVal(lpDatas[2])*16;
                        rv=rv+ConvertHexVal(lpDatas[3]);
                        break;
                        }
                }
        }
return rv;
}



unsigned short Hex4ToUShort (char *lpDatas)
{
unsigned short rv=0;
if (lpDatas)
        {
        rv=ConvertHexVal(lpDatas[0])*4096;
        rv=rv+ConvertHexVal(lpDatas[1])*256;
        rv=rv+ConvertHexVal(lpDatas[2])*16;
        rv=rv+ConvertHexVal(lpDatas[3]);
        }
return rv;
}


unsigned int StrHex8ToUInt (char *lpDatas)
{
unsigned int rv=0;
if (lpDatas)
        {
        unsigned int cntst=GetLenStr (lpDatas);
        if (cntst>=5)
                {
                rv=StrHex4ToUShort (lpDatas)*65536;
                rv=rv+StrHex4ToUShort (lpDatas+4);
                }
        else
                {
                rv=StrHex4ToUShort (lpDatas);
                }
        }
return rv;
}


unsigned short GetByteHexValue4 (char *lpDatas)
{
unsigned short rv=0;
if (lpDatas[0]!=0 && lpDatas[1]!=0 && lpDatas[2]!=0 &&  lpDatas[3]!=0)
        {
        rv=GetByteHexValue2(lpDatas)*256;
        rv=rv+GetByteHexValue2(&lpDatas[2]);
        }
return rv;
}

float TransmitM485ValueToFloat (long DValue)
{
float rv=0.0001*DValue;
return rv;
}


unsigned long M_ASCIZZToUlong (unsigned char *lpStr)
{
unsigned long rv=0;
unsigned char sizessindx=GetLenStr(lpStr);
unsigned long multipll=1;
if (sizessindx)
        {
        while (sizessindx)
                {
                rv=rv+(((unsigned long)((lpStr[sizessindx-1])-48))*multipll);
                multipll=multipll*10;
                sizessindx--;
                }
        }
return rv;
}





unsigned char *WriteRamString (unsigned char *lpDest,TDString strm)
{
unsigned char *lpStr = (char*)strm.c_str();
if (lpStr)
	{
        unsigned long clen = GetLenStr (lpStr);
        while (clen)
        	{
                lpDest[0] = lpStr[0];
                lpStr++;
                lpDest++;
                clen--;
                }
        }
return lpDest;
}







void AddKosaLine (TDString &InputString, TDString &OutputString)
{
char *lpInp = (char*)InputString.c_str();
unsigned long Lenss = GetLenStr (lpInp);
char datas;
while (Lenss)
        {
        datas = lpInp[0];
        OutputString = OutputString + datas;
        if (datas == '\\')  OutputString = OutputString + datas;
        lpInp++;
        Lenss--;
        }
}


unsigned long ConvertHexString (char *lpDestHx, char *lpInpute, unsigned long maxsz)
{
unsigned long rv = 0;
if (lpDestHx && lpInpute && maxsz)
        {
        unsigned char datvl;
        while (maxsz)
                {
                if (lpInpute[0] && lpInpute[1])
                        {
                        datvl = GetByteHexValue2 (lpInpute);
                        lpInpute = lpInpute + 2;
                        lpDestHx[0] = datvl;
                        lpDestHx++;
                        rv++;
                        }
                else
                        {
                        break;
                        }
                maxsz--;
                }
        }
return rv;
}






char *CreateWordStrFromText (char *lpRam,TDString &outst)
{
char *lpRv = 0;
char *lpSword = lpFindFistActiveCharFromLine (lpRam);
outst = "";
if (lpSword)
        {
        char tmpb;
        while (lpSword)
                {
                tmpb = lpSword[0];
                if (!tmpb)
                        {
                        break;
                        }
                if (tmpb <= 32)
                        {
                        lpRv = lpSword;
                        break;
                        }
                outst = outst + tmpb;
                lpSword++;
                }
        }
return lpRv;
}



bool CheckHexFormats (char *lpStr, unsigned long sizehex, bool F_Checkmode)
{
bool rv = 0;
if (lpStr)
        {
        if (F_Checkmode) sizehex = sizehex - 2;
        if (F_Checkmode)
                {
                if (lpStr[0]!='0') return 0;
                if (lpStr[1]!='x' && lpStr[1]!='X') return 0;
                lpStr = lpStr + 2;
                }
        rv = 1;
        unsigned char datd;
        while (sizehex)
                {
                datd = lpStr[0];
                if (!CheckHexVal (datd)) return 0;
                lpStr++;
                sizehex--;
                }
        }
return rv;
}



// проверяет символы в строке на соответствие с HEX символами
// Размер действительных HEX тетрад, без учета возможного присутствия текстового префикса " 0x "
// для проверки данных без проверки префикса F_Checkmode должен быть = 0
// размер sizehex устанавливается для нужного количества проверяемых символов, префикс в любом случае F_Checkmode не учитывается
bool CheckHexFormats (TDString strdata, unsigned long sizehex, bool F_Checkmode)
{
bool rv = 0;
if (strdata != "")
        {
        char *lpStr = (char*)strdata.c_str();
        unsigned long lnsz = GetLenStr (lpStr);
        rv =  CheckHexFormats (lpStr, lnsz, F_Checkmode);
        }
return rv;
}



bool CheckHexShortAdr (TDString str1)
{
return CheckHexFormats (str1,4,1) ;
}







TDString GenerateDecTextLine (char *lpRame, unsigned long sizesbyte)
{
 TDString outstr = "";
 unsigned long datas;
 char F_nocoma = 1;
 if (lpRame)
        {
         while (sizesbyte)
                {
                if (!F_nocoma) outstr = outstr + ",";
                datas = lpRame[0];
                outstr = outstr + IntToStr(datas).c_str();
                sizesbyte--;
                F_nocoma = 0;
                }
         }

 return outstr;
}




bool CheckDecValue (TDString strval)
{
bool rv = 0;
if (strval != "")
        {
        char *lpStr = (char*)strval.c_str();
        if (*lpStr == '-') lpStr++;
        unsigned char datt;
        rv = 1;
        while (1)
                {
                datt = lpStr[0];
                if (!datt) break;
                if (datt < '0' || datt > '9')
                        {
                        rv = 0;
                        break;
                        }
                lpStr++;
                }
        }
return rv;
}




bool CheckFloatValue (TDString strval)
{
bool rv = 0;
if (strval != "")
        {
        char *lpStr = (char*)strval.c_str();
        if (*lpStr == '-') lpStr++;
        unsigned char datt;
        unsigned long pntcnt = 0;
        rv = 1;
        while (1)
                {
                datt = lpStr[0];
                if (!datt) break;
                if (datt != '.' && datt != ',')
                        {
                        if (datt < '0' || datt > '9')
                                {
                                rv = 0;
                                break;
                                }
                        }
                else
                        {
                        pntcnt++;
                        if (pntcnt >= 2)
                                {
                                rv = 0;
                                break;
                                }
                        }
                lpStr++;
                }
        }
return rv;
}




long GetNBitmsk (unsigned long btmsk)
{
unsigned long rv = 0;
unsigned char indxs = 0;
unsigned char cntbite = 0;
while (indxs < 32)
        {
        if (btmsk & 1)
                {
                rv = indxs;
                cntbite++;
                }
        btmsk = btmsk >> 1;
        indxs++;
        }
if (cntbite != 1) rv = -1;
return rv;
}



void GetProporcionaleTableValue (unsigned char *lpInProc, unsigned long *lpDestVa, unsigned char TableSizes, unsigned long PropData100)
{
if (TableSizes && lpInProc && lpDestVa)
    {
    unsigned long Indxs = 0;
    float DatP = PropData100, datf;
    DatP = DatP / 100;    // пикселей на процент
    unsigned long MaxW = 0;
    while (Indxs < TableSizes)
        {
        datf = lpInProc[Indxs];
        datf = datf * DatP;
        lpDestVa[Indxs] = datf;
        MaxW = MaxW + lpDestVa[Indxs];
        Indxs++;
        }
    Indxs--;
    // подкорректировать последнюю колонку
    lpDestVa[Indxs] = lpDestVa[Indxs] + (PropData100 - MaxW) - 1;
    }

}



// подсчитывает количество слов в строке или извлекает слово по его индекс-номеру
unsigned long CreateWordStringFromIndxLine (TEX::BUFPAR *InBf, TEX::BUFPAR *OutBuf, unsigned long Indxx,char delimc)
{
unsigned long rv = 0;
if (InBf && OutBuf)
        {
		char *lpLinetxt = (char*) InBf->lpRam;
		unsigned long SizeInBuf = InBf->Sizes;

		unsigned long CntOutBuf = OutBuf->Sizes;	// && lpoutstr
		char *lpoutstr = (char*)OutBuf->lpRam;

        bool Fdat = 0;
        bool iscreate = 0;
        unsigned long lcinx = 1;
        char tmpb;
        //lpoutstr[0] = 0;
        while (SizeInBuf)
                {
                tmpb = lpLinetxt[0];
                if (tmpb == 9) tmpb = 32;
                if (!tmpb || tmpb == 13 || tmpb == 10)
					{
					break;
					}
                else
					{
					if (tmpb == delimc)
						{
						Fdat = 0;
						if (iscreate && Indxx == lcinx) break;
						lcinx++;
						}
					else  Fdat=1;
					}
                if (Indxx == lcinx)
					{
					iscreate = 1;
					if (Fdat)
						{
						lpoutstr[0] = tmpb;
						lpoutstr++;
						CntOutBuf--;
						if (!CntOutBuf) break;
						}
					}
                lpLinetxt++;
				SizeInBuf--;
                }
		lpoutstr[0] = 0;
        rv = lcinx;     // номер индекса извлеченного слова или максимальное количество слов в строке
        }
return rv;
}







// оцифровывает число до 99 999 999     до 1 милиарда
char StrToFloat (float *lpDest, char *lpTxtIn)
{
char rv = 0;
if (lpDest && lpTxtIn)
	{
	TEX::BUFPAR InpBuf;
	unsigned long Sizes = GetLenStr (lpTxtIn);
	InpBuf.lpRam = lpTxtIn;
	InpBuf.Sizes = Sizes;
	if (Sizes && Sizes < 32)
		{
		char rsltOk = 0;
		float Datf = 0;
		unsigned long NLongDroba = 0;
		long NLongCel = 0;
		unsigned long len_drb = 0,len_cel = 0;
		char F_err = 0;
		char Buf[32];
		TEX::BUFPAR OtPBf;
		OtPBf.lpRam = &Buf;
		OtPBf.Sizes = sizeof(Buf);
		unsigned long rslt = CreateWordStringFromIndxLine (&InpBuf,&OtPBf,100,'.');
		switch (rslt)
			{
			case 2:	// дробная присутствует
				{
				rslt = CreateWordStringFromIndxLine (&InpBuf,&OtPBf,2,'.');	// извлечь дробную часть
				if (rslt == 2)
					{
					len_drb = GetLenStr(Buf);
					if (len_drb)
						{
						if (!ConvertStrToULong ((unsigned char *)&Buf,&NLongDroba)) F_err++;
						}
					}
				}
			case 1:	// целая присутствует
				{
				rslt = CreateWordStringFromIndxLine (&InpBuf,&OtPBf,1,'.');	// извлечь целую часть
				if (rslt == 1)
					{
					len_cel = GetLenStr (Buf);
					if (len_cel)
						{
						if (!ConvertStrToLong ((unsigned char *)&Buf,&NLongCel)) F_err++;
						}
					}
				rsltOk = 1;
				break;
				}
			}
		if (rsltOk && !F_err)
			{
			Datf = NLongCel;
			if (len_drb)
				{
				float mult = ((float)(1.0)) / tmpMultData[len_drb];
				float drba = NLongDroba;
				Datf = Datf + (drba * mult);
				}
			lpDest[0] = Datf;
			rv = 1;
			}
		}
	}
return rv;
}




char IncASCII_DIG (char ValDigAscii)
{
ValDigAscii++;
if (ValDigAscii > '9' || ValDigAscii < '0') ValDigAscii = '0';
return ValDigAscii;
}




char DecASCII_DIG (char ValDigAscii)
{
ValDigAscii--;
if (ValDigAscii < '0' || ValDigAscii > '9') ValDigAscii = '9';
return ValDigAscii;
}




unsigned char *DrobaToStr (unsigned char *lpDest, float droba, unsigned char cntr)
{
unsigned char resd;
float delmt = 0.1;
while (cntr)
	{
	if (droba >= delmt)
		{
		resd = (droba / delmt);
		droba = droba - (resd *delmt);
		}
	else
		{
		resd=0;
		}

        lpDest[0] = resd + 48;
        lpDest++;
	delmt = delmt / 10;
        cntr--;
	}
return lpDest;
}


unsigned char *FloatToStrDroba (unsigned char *lpDest,double datas,unsigned char Cntr)
{
unsigned char resd;
double delmt=0.1 - 0.0000000003;
while (Cntr)
	{
	if (datas>=delmt)
		{
		resd=(datas/delmt);
		datas = datas - (delmt*resd);
		}
	else
		{
		resd=0;
		}

	lpDest[0]=resd+48;
	lpDest++;

	delmt = delmt/10;

	Cntr--;
	}
lpDest[0] = 0;
return lpDest;
}


unsigned char *FloatToString (unsigned char *lpRamBuf, double datas,unsigned char pcnt)
{
	unsigned char *lpEnd = 0;
	double DrobOstat;
	unsigned long Celie;
	if (datas < 0)
		{
		lpRamBuf[0] = '-';
		lpRamBuf++;
        datas *= -1;
		}
    Celie = datas;
	lpEnd = UlongToStr (lpRamBuf,Celie);
    if (lpEnd)
        {
        if (pcnt)
            {
            if (pcnt > 9) pcnt = 9;
            DrobOstat = datas - (float)Celie;

            lpEnd[0] = '.';
            lpEnd++;
            lpEnd = FloatToStrDroba (lpEnd,DrobOstat,pcnt);
            }
        lpEnd[0]=0;
        }
return lpEnd;
}


/*
char *FloatToStr(float DataF, TEX::BUFPAR *OutBf, unsigned char DrobSize)
{
char *lpRv = 0;
if (OutBf && OutBf->lpRam && OutBf->Sizes >= 2)
	{
	unsigned long dstsz = OutBf->Sizes;
	char Txt_Cel[16];
	char Txt_Drob[16];
	long len_cel = 0, len_drob = 0;
    char znakMinus = 0;
	// преобразование целых
	unsigned long Celie = DataF;
	unsigned char *lpDst = UlongToStr ((unsigned char*)Txt_Cel,Celie);
	if (DataF < 0) znakMinus = 1;
	len_cel = lpDst - (unsigned char*)Txt_Cel;	// размер целых величин
	// преобразование дробных
	if (DrobSize)
		{
		unsigned long drrslt = 0;
        float divdrob_okr = ((float)(1.0)) / tmpMultData[(DrobSize+1)];
		float Drob = DataF - Celie;
        drrslt = (unsigned long)((float)(Drob / divdrob_okr));
		if (drrslt)
			{
            unsigned char dt;
			unsigned char *lpDst = DrobaToStr ((unsigned char*)Txt_Drob, Drob,(DrobSize+1));
			len_drob = DrobSize;
			// проверка на необходимость округления
			lpDst--;
			dt = lpDst[0];
			lpDst[0] = 0;
			if (dt > 53)  // нужен процесс округления
					{
					divdrob_okr = ((float)(1.0)) / tmpMultData[DrobSize];
					Drob = Drob + divdrob_okr;      // округление до большего
					unsigned char *lpDst = DrobaToStr ((unsigned char*)Txt_Drob,Drob,DrobSize);
					len_drob = lpDst - (unsigned char*)Txt_Drob;
					}
			}
        else
			{
			len_drob = 0;
			}
		}
	// формирование данных в выходном буфере
	unsigned long lencopy = len_cel;
	char Step = 0;
	char *lpOut = (char*)OutBf->lpRam;
	do {
		if (!dstsz) break;
		// знак
		if (znakMinus)
			{
			lpOut[0] = '-';
			lpOut++;
			dstsz--;
			if (!dstsz) break;
			}
		// целые
		if (lencopy > dstsz) lencopy = dstsz;  // CopyMemorySDC
		lpOut = CopyMemorySDC(Txt_Cel,lpOut, lencopy);
		dstsz = dstsz - lencopy;
		if (!dstsz) break;
		Step++;
		// если дроби нет - выход
		if (!DrobSize || !len_drob) break;
		// точка
		lpOut[0] = '.'; lpOut++;
		dstsz--;
		if (!dstsz) break;
		// дробь
		lencopy = len_drob;
		if (len_drob > dstsz) lencopy = dstsz;
		lpOut = CopyMemorySDC(Txt_Drob,lpOut,lencopy);
		dstsz = dstsz - lencopy;
		} while (0);
	if (!Step)
		{
		// формирование завершилось некорректно
		lpOut = lpOut - 2;
		lpOut[0] = '^'; lpOut++;
		lpOut[0] = 0; lpOut++;
		}
	else
		{
		if (dstsz)
			{
			lpOut[0] = 0;
			}
		else
			{
			((char*)OutBf->lpRam)[(OutBf->Sizes-1)] = 0;
			}
		}
	lpRv = lpOut;
	}
return lpRv;
}
*/



bool FloatToStr_M (float DataF, TDString &DstStr, unsigned char DrobSize)
{
bool rv = false;
unsigned char Bufstr[32];
//TEX::BUFPAR Bf1;
//Bf1.lpRam = Bufstr;
//Bf1.Sizes = sizeof(Bufstr) - 1;
DstStr = "";
if (FloatToString (Bufstr, DataF, DrobSize))
    {
    Bufstr[sizeof(Bufstr)-1] = 0;
    AddASCIZZToStr (DstStr, Bufstr);
    rv = true;
    }
return rv;
}



TDString FloatToStr_V (float DataF, unsigned char DrobSize)
{
TDString rv = "";
FloatToStr_M (DataF, rv, DrobSize);
return rv;
}



unsigned char GetNoRleCnt (unsigned char *lpInRam, unsigned long maxsiz)
{
unsigned char rv_cnt = 0,Dt,Dt_prev;
if (maxsiz)
    {
    if (maxsiz == 1)
        {
        rv_cnt = 1;
        }
    else
        {
        if (maxsiz > 64) maxsiz = 64;
        Dt_prev = lpInRam[0];
        while (true)
            {
            lpInRam++;
            maxsiz--;
            if (!maxsiz)
                {
                rv_cnt++;
                break;
                }
            Dt = lpInRam[0];
            if (Dt == Dt_prev) break;
            rv_cnt++;
            Dt_prev = Dt;
            }
        }
    }
return rv_cnt;
}



unsigned char GetRleCnt (unsigned char *lpInRam, unsigned long maxsiz)
{
unsigned char rv_cnt = 0,Dt,Dt_prev;
if (maxsiz)
    {
    if (maxsiz == 1)
        {
        rv_cnt = 1;
        }
    else
        {
        if (maxsiz > 64) maxsiz = 64;
        Dt_prev = lpInRam[0];
        while (true)
            {
            lpInRam++;
            maxsiz--;
            if (!maxsiz)
                {
                rv_cnt++;
                break;
                }
            Dt = lpInRam[0];
            if (Dt != Dt_prev)
                {
                if (rv_cnt) rv_cnt++;
                break;
                }
            rv_cnt++;
            Dt_prev = Dt;
            }
        }
    }
return rv_cnt;
}


/*

Выходная RLE последовательность имеет такой формат:
Биты 7-6: код сжатия
    00  -  RLE последовательность нулевых байт, (5-0) счетчик повторов
    01  -  RLE последовательность, (5-0) счетчик повторов, + 1 байт что повторять
    10  -  Послеловательность байт без повторов, (5-0) сколько байт переслать, + далее следует то количество байт сколько заявленно в счетчике
    11  -  RLE последовательность 0xFF байт, (5-0) счетчик повторов
*/



namespace RLECOD_A{

    const unsigned char CMDMSK = 192;
    const unsigned char ZERO = 0;
    const unsigned char RLE = 64;
    const unsigned char RAW = 128;
    const unsigned char ONES = CMDMSK;
    const unsigned char CNTRMASK = 63;
};





unsigned long RLE_Coding_A (TEX::BUFPAR *InRaw, TEX::BUFPAR *OutCode)
{
unsigned long rv_cnt = 0;
if (InRaw && OutCode && InRaw->lpRam && OutCode->lpRam)
    {
    //unsigned long MaxOutBuf = OutCode->Sizes;
    unsigned long MaxInBuf = InRaw->Sizes;
    unsigned char *lpInp = (unsigned char*)InRaw->lpRam;
    unsigned char *lpOutp = (unsigned char*)OutCode->lpRam;
    if (MaxInBuf)
        {
        unsigned char Dt, Dt_prev;
        unsigned char cntr;
        unsigned char cntcod;
        while (MaxInBuf)
            {
            if (!MaxInBuf) break;
            Dt = lpInp[0];
            cntr = GetNoRleCnt (lpInp,MaxInBuf);    // количество подряд лежащих данных неповторяемых
            if (cntr)
                {
                cntcod = cntr - 1;
                if (cntr == 1)  // одиночные байты
                    {
                    switch (Dt)
                        {
                        case 0:
                            {
                            lpOutp[0] = RLECOD_A::ZERO | cntcod;
                            lpOutp++;
                            rv_cnt++;
                            MaxInBuf--;
                            lpInp++;
                            break;
                            }
                        case 255:
                            {
                            lpOutp[0] = RLECOD_A::ONES | cntcod;
                            lpOutp++;
                            rv_cnt++;
                            MaxInBuf--;
                            lpInp++;
                            break;
                            }
                        default:
                            {
                            lpOutp[0] = RLECOD_A::RAW | cntcod;
                            lpOutp++;
                            lpOutp[0] = Dt;
                            lpOutp++;
                            rv_cnt = rv_cnt + 2;
                            MaxInBuf--;
                            lpInp++;
                            break;
                            }
                        }
                    if (!MaxInBuf) break;
                    }
                else
                    {
                    lpOutp[0] = RLECOD_A::RAW | cntcod;
                    lpOutp++;
                    lpOutp = CopyMemorySDC ((char*)lpInp, (char*)lpOutp, cntr);
                    rv_cnt = rv_cnt + cntr + 1;
                    if (MaxInBuf <= cntr) return rv_cnt;
                    MaxInBuf = MaxInBuf - cntr;
                    lpInp = lpInp + cntr;
                    }
                }
            Dt = lpInp[0];
            cntr = GetRleCnt (lpInp,MaxInBuf);
            if (cntr)
                {
                cntcod = cntr - 1;
                switch (Dt)
                    {
                    case 0:
                        {
                        lpOutp[0] = RLECOD_A::ZERO | cntcod;
                        lpOutp++;
                        rv_cnt++;
                        break;
                        }
                    case 255:
                        {
                        lpOutp[0] = RLECOD_A::ONES | cntcod;
                        lpOutp++;
                        rv_cnt++;
                        break;
                        }
                    default:
                        {
                        lpOutp[0] = RLECOD_A::RLE | cntcod;
                        lpOutp++;
                        lpOutp[0] = Dt;
                        lpOutp++;
                        rv_cnt = rv_cnt + 2;
                        break;
                        }
                    }
                lpInp = lpInp + cntr;
                if (MaxInBuf < cntr) break;
                MaxInBuf = MaxInBuf - cntr;
                }
            if (!MaxInBuf) break;
            }
        }
    }
return rv_cnt;
}





unsigned long RLE_Decoding_A (TEX::BUFPAR *InCode, TEX::BUFPAR *OutRaw)
{
unsigned long rv_cnt = 0;
if (InCode && OutRaw)
    {
    unsigned long MaxOutBuf = OutRaw->Sizes;
    unsigned long MaxInBuf = InCode->Sizes;
    unsigned char *lpInp = (unsigned char*)InCode->lpRam;
    unsigned char *lpOutp = (unsigned char*)OutRaw->lpRam;
    if (MaxInBuf && MaxOutBuf)
        {
        unsigned char Dt,Cod,F_StMem = 0;
        unsigned char cntr,Dt_rep;
        while (MaxInBuf && MaxOutBuf)
            {
            Dt = lpInp[0];
            Cod = Dt & RLECOD_A::CMDMSK;
            cntr = Dt & RLECOD_A::CNTRMASK;
            cntr++;
            switch (Cod)
                {
                case RLECOD_A::ZERO:
                    {
                    Dt_rep = 0;
                    F_StMem = 1;
                    lpInp++;
                    MaxInBuf--;
                    break;
                    }
                case RLECOD_A::RLE:
                    {
                    F_StMem = 1;
                    lpInp++;
                    Dt_rep = lpInp[0];
                    MaxInBuf = MaxInBuf - 2;
                    lpInp++;
                    break;
                    }
                case RLECOD_A::ONES:
                    {
                    Dt_rep = 255;
                    F_StMem = 1;
                    lpInp++;
                    MaxInBuf--;
                    break;
                    }
                }
            if (F_StMem)
                {
                unsigned long maxfill = cntr;
                if (MaxOutBuf < maxfill) maxfill = MaxOutBuf;
                lpOutp = (unsigned char*)FillMems ((char*)lpOutp,Dt_rep,maxfill);
                MaxOutBuf = MaxOutBuf - maxfill;
                rv_cnt = rv_cnt + maxfill;
                F_StMem = 0;
                }
            else
                {
                rv_cnt = rv_cnt + cntr;
                lpInp++;
                MaxInBuf--;
                while (cntr && MaxOutBuf && MaxInBuf)
                    {
                    lpOutp[0] = lpInp[0];
                    lpInp++;
                    lpOutp++;
                    cntr--;
                    MaxOutBuf--;
                    MaxInBuf--;
                    }
                }
            }
        }
    }
return rv_cnt;
}



char ChangeRamValueULong (unsigned long *lpRam,unsigned long MaxValue,unsigned long MinValue,unsigned long IncDecValue,unsigned char CmdChg)
{
char rvcng = TEX::C_NORMAL;
if (IncDecValue)
	{
	unsigned long datas = lpRam[0];
	if (CmdChg == TEX::C_INC)
		{
        if (datas >= MaxValue)
            {
            datas = MaxValue;
            rvcng = TEX::C_MAXVAL;
            }
        else
            {
            if (datas < (datas+IncDecValue)) datas = datas + IncDecValue;
            if (datas > MaxValue)
                {
                datas = MaxValue;
                rvcng = TEX::C_MAXVAL;
                }
            }
		}
	else
		{
		if (CmdChg == TEX::C_DEC)
			{
            if (datas <= MinValue)
                {
                datas = MinValue;
                rvcng = TEX::C_MINVAL;
                }
            else
                {
                if ((datas-IncDecValue) <= datas) datas = datas - IncDecValue;
                if (datas < MinValue)
                    {
                    datas = MinValue;
                    rvcng = TEX::C_MINVAL;
                    }
                }
			}
		}
	lpRam[0] = datas;
	}
return rvcng;
}



char ChangeRamValueUShort (unsigned short *lpRam,unsigned short MaxValue,unsigned short MinValue,unsigned short IncDecValue,unsigned char CmdChg)
{
char rvcng = TEX::C_NORMAL;
if (IncDecValue)
	{
	unsigned short datas = lpRam[0];
	if (CmdChg == TEX::C_INC)
		{
        if (datas >= MaxValue)
            {
            datas = MaxValue;
            rvcng = TEX::C_MAXVAL;
            }
        else
            {
            if (datas < (datas+IncDecValue)) datas = datas + IncDecValue;
            if (datas > MaxValue)
                {
                datas = MaxValue;
                rvcng = TEX::C_MAXVAL;
                }
            }
		}
	else
		{
		if (CmdChg == TEX::C_DEC)
			{
            if (datas <= MinValue)
                {
                datas = MinValue;
                rvcng = TEX::C_MINVAL;
                }
            else
                {
                if ((datas-IncDecValue) <= datas) datas = datas - IncDecValue;
                if (datas < MinValue)
                    {
                    datas = MinValue;
                    rvcng = TEX::C_MINVAL;
                    }
                }
			}
		}
	lpRam[0] = datas;
	}
return rvcng;
}



char ChangeRamValueUChar (unsigned char *lpRam,unsigned char MaxValue,unsigned char MinValue,unsigned char IncDecValue,unsigned char CmdChg)
{
char rvcng = TEX::C_NORMAL;
if (IncDecValue)
	{
	unsigned char datas = lpRam[0];
	if (CmdChg == TEX::C_INC)
		{
        if (datas >= MaxValue)
            {
            datas = MaxValue;
            rvcng = TEX::C_MAXVAL;
            }
        else
            {
            if (datas < (datas+IncDecValue)) datas = datas + IncDecValue;
            if (datas > MaxValue)
                {
                datas = MaxValue;
                rvcng = TEX::C_MAXVAL;
                }
            }
		}
	else
		{
		if (CmdChg == TEX::C_DEC)
			{
            if (datas <= MinValue)
                {
                datas = MinValue;
                rvcng = TEX::C_MINVAL;
                }
            else
                {
                if ((datas-IncDecValue) <= datas) datas = datas - IncDecValue;
                if (datas < MinValue)
                    {
                    datas = MinValue;
                    rvcng = TEX::C_MINVAL;
                    }
                }
			}
		}
	lpRam[0] = datas;
	}
return rvcng;
}



unsigned short abs16 (short datas)
{
unsigned short rv=(unsigned short)datas;
if (rv>=32768)
	{
	rv=(rv ^ 0xFFFF)+1;
	}
return rv;
}




unsigned long abs32 (long datas)
{
unsigned long rv=(unsigned long)datas;
if (rv>=0x80000000)
	{
	rv=(rv ^ 0xFFFFFFFF)+1;
	}
return rv;
}



uint64_t abs64 (int64_t datas)
{
uint64_t rv= (uint64_t)datas;
if (rv>=0x8000000000000000)
	{
	rv=(rv ^ 0xFFFFFFFFFFFFFFFF)+1;
	}
return rv;
}



void AddASCIZZToStr (TDString &str, char *lasciiz)
{
if (lasciiz)
    {
    char dat;
    while (true)
        {
        dat = lasciiz[0];
        if (!dat) break;
        str = str + dat;
        lasciiz++;
        }
    }
}



void AddASCIZZToStr_cnt (TDString &str, char *lasciiz, unsigned long sizee)
{
if (lasciiz && sizee)
    {
    char dat;
    while (sizee)
        {
        dat = lasciiz[0];
        if (!dat) break;
        str = str + dat;
        lasciiz++;
        sizee--;
        }
    }
}



unsigned long ExtractFileNameExt (TDString FileNamee, TDString &outext)
{
outext = "";
char *lTxt = (char*)FileNamee.c_str();
char dat;
unsigned long rv = 0;
bool F_Point = false;
while (true)
    {
    dat = lTxt[0];
    if (!dat) break;
    if (dat == '.')
        {
        F_Point = true;
        }
    else
        {
        if (F_Point)
            {
            outext = outext + dat;
            rv++;
            }
        }
    lTxt++;
    }
return rv;
}



unsigned long ExtractFileNameBase (TDString FileNamee, TDString &outext)
{
outext = "";
char *lTxt = (char*)FileNamee.c_str();
char dat;
unsigned long rv = 0;
while (true)
    {
    dat = lTxt[0];
    if (!dat || dat == '.') break;
    outext = outext + dat;
    rv++;
    lTxt++;
    }
return rv;
}





unsigned long CreateStringToDelim (char *lRam, TDString &outstr, char delim, unsigned long sizes)
{
unsigned long rv = 0;
char dat;
outstr = "";
while (sizes)
    {
    dat = lRam[0];
    if (!dat || dat == delim) break;
    outstr = outstr + dat;
    rv++;
    lRam++;
    sizes--;
    }

return rv;
}



TDString CutFileName (TDString filename, unsigned long sizeASCIZZ)
{
TDString rv = filename;
if (sizeASCIZZ)
    {
    char *lTxt = (char*)filename.c_str();
    unsigned long namesize = GetLenStr (lTxt);
    if (namesize > sizeASCIZZ)
        {
        rv = "";
        // найти расширение файла
        TDString ExtName = "";
        unsigned long SizeExt = ExtractFileNameExt (filename, ExtName);
        //if (SizeExt > 3) SizeExt = 3;
        if (sizeASCIZZ > (SizeExt + 1))
            {
            // найти укороченный размер имени
            unsigned long maxnewlenname = sizeASCIZZ - SizeExt - 1;
            if (maxnewlenname)
                {
                CreateStringToDelim (lTxt, rv, '.', maxnewlenname);
                TDString newext = "";
                CreateStringFromLine_cnt (newext, (char*)ExtName.c_str(), SizeExt);
                if (newext != "")
                    {
                    rv = rv + '.' + newext;
                    }
                }
            }
        }
    }
return rv;
}



// (данных на выходе на 1/4 меньше)
unsigned long SerialDecodingData_6bit  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOut)
{
unsigned long rv = 0;
if (lpInput && lpOut)
	{
	unsigned long rvcnt = 0;
	unsigned long InSize = lpInput->Sizes;
	unsigned char datin6;
	unsigned char *lpIn = (unsigned char*)lpInput->lpRam;
	unsigned char *lpOutData = (unsigned char*)lpOut->lpRam;
	unsigned long SizeOutDat = lpOut->Sizes;
	unsigned char MaskOut8 = 128;
	unsigned char datout8 = 0;
	unsigned char inBcnt6;
	char F_EndOutSize = 0;
	while (InSize && !F_EndOutSize)
		{
		datin6 = lpIn[0];
		lpIn++;
		InSize--;
		inBcnt6 = 6;
		datin6 = datin6 << 2;
		while (inBcnt6)
			{
			if (datin6 & 128) datout8 = datout8 | MaskOut8;
			MaskOut8 = MaskOut8 >> 1;
			datin6 = datin6 << 1;
			if (!MaskOut8)
				{
				lpOutData[0] = datout8;
				rvcnt++;
				datout8 = 0;
				MaskOut8 = 128;
				lpOutData++;
				SizeOutDat--;
				if (!SizeOutDat)
					{
					//F_EndOutSize = 1;
					break;
					}
				}
			inBcnt6--;
			}
		}
	if (!InSize && !F_EndOutSize) rv = rvcnt;
	}
return rv;
}



unsigned long SerialCodingData_6bit  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOutput)
{
unsigned long rv = 0;
if (lpInput && lpOutput)
	{
	unsigned long rvcnt = 0;
	unsigned char *lpIn = (unsigned char*)lpInput->lpRam;
	unsigned long SizeIn = lpInput->Sizes;
	unsigned char datain8;
	unsigned char datBcnt8;
	unsigned char dataout6 = 0;
	unsigned char MaskOut6 = 32;
	unsigned char *lpOut = (unsigned char*)lpOutput->lpRam;
	unsigned long SizeOut = lpOutput->Sizes;
	unsigned long MidleCount = (SizeIn/4 + SizeIn) / 2;
	char F_EndOutSize = 0;
	char F_fisttata = 1;
	while (SizeIn && !F_EndOutSize)
		{
		datain8 = lpIn[0];
		lpIn++;
		datBcnt8 = 8;
		while (datBcnt8)
			{
			if ((datain8 & 128)) dataout6 = dataout6 | MaskOut6;
			datain8 = datain8 << 1;
			MaskOut6 = MaskOut6 >> 1;
			if (!MaskOut6)
				{
				lpOut[0] = dataout6;
				// установка маркеров
				if (F_fisttata)
					{
					F_fisttata = 0;
					}
				else
					{
					// показательный маркер середин
					if (MidleCount > rvcnt)
						{
						lpOut[0] = lpOut[0] | 64;
						}
					else
						{
						lpOut[0] = lpOut[0] | 128;
						}
					}
				lpOut++;
				SizeOut--;
				rvcnt++;
				if (!SizeOut)
					{
					F_EndOutSize = 1;
					break;
					}
				MaskOut6 = 32;
				dataout6 = 0;
				}
			datBcnt8--;
			}
		SizeIn--;
		}
	if (MaskOut6 && MaskOut6 != 32)
		{
		lpOut[0] = dataout6;
		lpOut++;
		rvcnt++;
		}
	// установка маркера конца
	lpOut--;
	lpOut[0] = lpOut[0] | 192;

	if (!F_EndOutSize && !SizeIn) rv = rvcnt;
	}
return rv;
}




unsigned long SerialCodingData_7bit  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOutput)
{
const unsigned char C_StrartBit = 64;
unsigned long rv = 0;
if (lpInput && lpOutput)
	{
	unsigned long rvcnt = 0;
	unsigned char *lpIn = (unsigned char*)lpInput->lpRam;
	unsigned long SizeIn = lpInput->Sizes;
	unsigned char datain8;
	unsigned char datBcnt8;
	unsigned char dataout7 = 0;
	unsigned char MaskOut7 = C_StrartBit;
	unsigned char *lpOut = (unsigned char*)lpOutput->lpRam;
	unsigned long SizeOut = lpOutput->Sizes;
	char F_EndOutSize = 0;
	while (SizeIn && !F_EndOutSize)
		{
		datain8 = lpIn[0];
		lpIn++;
		datBcnt8 = 8;
		while (datBcnt8)
			{
			if ((datain8 & 128)) dataout7 = dataout7 | MaskOut7;
			datain8 = datain8 << 1;
			MaskOut7 = MaskOut7 >> 1;
			if (!MaskOut7)
				{
				lpOut[0] = dataout7;
				lpOut++;
				SizeOut--;
				rvcnt++;
				if (!SizeOut)
					{
					F_EndOutSize = 1;
					break;
					}
				MaskOut7 = C_StrartBit;
				dataout7 = 0;
				}
			datBcnt8--;
			}
		SizeIn--;
		}
	if (MaskOut7 && MaskOut7 != C_StrartBit)
		{
		lpOut[0] = dataout7;
		lpOut++;
		rvcnt++;
		}
	// установка маркера конца
	lpOut--;
	lpOut[0] = lpOut[0] | 128;
	if (!SizeIn) rv = rvcnt;    // !F_EndOutSize &&
	}
return rv;
}



unsigned long SerialDecodingData_7bit  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOut)
{
unsigned long rv = 0;
if (lpInput && lpOut)
	{
	unsigned long rvcnt = 0;
	unsigned long InSize = lpInput->Sizes;
	unsigned char datin7;
	unsigned char *lpIn = (unsigned char*)lpInput->lpRam;
	unsigned char *lpOutData = (unsigned char*)lpOut->lpRam;
	unsigned long SizeOutDat = lpOut->Sizes;
	unsigned char MaskOut8 = 128;
	unsigned char datout8 = 0;
	unsigned char inBcnt7;
	char F_EndOutSize = 0;
    unsigned long MaxOutput = (InSize - (InSize / 8));
    if (InSize % 8) MaxOutput++;
	while (InSize && !F_EndOutSize && rvcnt < MaxOutput)
		{
		datin7 = lpIn[0];
		lpIn++;
		InSize--;
		inBcnt7 = 7;
		datin7 = datin7 << 1;
		while (inBcnt7)
			{
			if (datin7 & 128) datout8 = datout8 | MaskOut8;
			MaskOut8 = MaskOut8 >> 1;
			datin7 = datin7 << 1;
			if (!MaskOut8)
				{
				lpOutData[0] = datout8;
				rvcnt++;
				datout8 = 0;
				MaskOut8 = 128;
				lpOutData++;
				SizeOutDat--;
				if (!SizeOutDat)
					{
					//F_EndOutSize = 1;
					break;
					}
				}
			inBcnt7--;
			}
		}
	if (!InSize && !F_EndOutSize) rv = rvcnt;
	}
return rv;
}



unsigned long SerialDecodingData_7bitC  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOut)
{
unsigned long rv = 0;
if (lpInput && lpOut)
	{
	unsigned long rvcnt = 0;
	unsigned long InSize = lpInput->Sizes;
	unsigned char datin7;
	unsigned char *lpIn = (unsigned char*)lpInput->lpRam;
	unsigned char *lpOutData = (unsigned char*)lpOut->lpRam;
	unsigned long SizeOutDat = lpOut->Sizes;
	unsigned char MaskOut8 = 128;
	unsigned char datout8 = 0;
	unsigned char inBcnt7;
	char F_EndOutSize = 0;
    unsigned long MaxOutput = (InSize - (InSize / 8));
    if (InSize % 8) MaxOutput++;
    unsigned char XorData = 1;
	while (InSize && !F_EndOutSize && rvcnt < MaxOutput)
		{
		datin7 = lpIn[0];
		lpIn++;
		InSize--;
		inBcnt7 = 7;
		datin7 = datin7 << 1;
		while (inBcnt7)
			{
			if (datin7 & 128) datout8 = datout8 | MaskOut8;
			MaskOut8 = MaskOut8 >> 1;
			datin7 = datin7 << 1;
			if (!MaskOut8)
				{
				lpOutData[0] = datout8 ^ XorData;
				rvcnt++;
				datout8 = 0;
				MaskOut8 = 128;
				lpOutData++;
                XorData++;
				SizeOutDat--;
				if (!SizeOutDat)
					{
					//F_EndOutSize = 1;
					break;
					}
				}
			inBcnt7--;
			}
		}
	if (!InSize && !F_EndOutSize) rv = rvcnt;
	}
return rv;
}




unsigned long SerialCodingData_7bitC  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOutput)
{
const unsigned char C_StrartBit = 64;
unsigned long rv = 0;
if (lpInput && lpOutput)
	{
	unsigned long rvcnt = 0;
	unsigned char *lpIn = (unsigned char*)lpInput->lpRam;
	unsigned long SizeIn = lpInput->Sizes;
	unsigned char datain8;
	unsigned char datBcnt8;
	unsigned char dataout7 = 128;
	unsigned char MaskOut7 = C_StrartBit;
	unsigned char *lpOut = (unsigned char*)lpOutput->lpRam;
	unsigned long SizeOut = lpOutput->Sizes;
	char F_EndOutSize = 0;
    unsigned char XorData = 1;
	while (SizeIn && !F_EndOutSize)
		{
		datain8 = lpIn[0] ^ XorData;
		lpIn++;
		datBcnt8 = 8;
		while (datBcnt8)
			{
			if ((datain8 & 128)) dataout7 = dataout7 | MaskOut7;
			datain8 = datain8 << 1;
			MaskOut7 = MaskOut7 >> 1;
			if (!MaskOut7)
				{
				lpOut[0] = dataout7;
				lpOut++;
				SizeOut--;
				rvcnt++;
				if (!SizeOut)
					{
					F_EndOutSize = 1;
					break;
					}
				MaskOut7 = C_StrartBit;
				dataout7 = 128;
				}
			datBcnt8--;
			}
        XorData++;
		SizeIn--;
		}
	if (MaskOut7 && MaskOut7 != C_StrartBit)
		{
		lpOut[0] = dataout7;
		lpOut++;
		rvcnt++;
		}
	// установка маркера конца
	lpOut--;
	lpOut[0] = lpOut[0] & 127;
	if (!SizeIn) rv = rvcnt;    // !F_EndOutSize &&
	}
return rv;
}




TDString Convert_maToStr (unsigned short mavalue)
{
TDString outstr = "";
float maFv = (float)mavalue / 1000;
if (FloatToStr_M (maFv, outstr, 3))
    {
    outstr = outstr + " ma";
    }
else
    {
    outstr = "err.";
    }
return outstr;
}



TDString Convert_celsium_char (char ctemp)
{
TDString outstr = "";
if (LongToStr_M (ctemp, outstr))
    {
    outstr = outstr + " c";
    }
else
    {
    outstr = "err";
    }
return outstr;
}






TDString JSON_GetValue (TDString JSON_txt, TDString JSON_name_param)
{
TDString rv_param = "";
TDString copyjson = JSON_txt;
char *lRam = (char*)copyjson.c_str();
TDString curnameparam = "";
TDString curvalue = "";
unsigned long sizes = GetLenStr (lRam);
if (sizes)
    {
    char datt;
    char type_el = 0;
    unsigned long indx = 0;
    unsigned long counter_char = 0;
    bool arr_chek = false;
    bool F_is_array = false;
    bool F_is_name = false;
    bool F_is_param = false;
    bool F_sub_entered = false;
    bool F_delim_check = false;
    bool F_need_name_param = false;       // false - поиск имени параметра, true - поиск значения
    unsigned long entered_count = 0;

    while (indx < sizes)
        {
        datt = lRam[0];
        if (!datt) break;


        if (F_sub_entered)  // вошли в скобки
            {
            if (!F_need_name_param)     // ищем имя параметра
                {
                if (F_is_name)  // находимся в имени параметра
                    {
                    if (datt == '\"')
                        {
                        // выходим из имени параметра
                        F_is_name = false;
                        F_need_name_param = true;   // будем искать значение параметра
                        F_delim_check = false;      // сбросить флаг обнаружения ':'
                        }
                    else
                        {
                        curnameparam = curnameparam + datt; // формируем имя параметра
                        }
                    }
                else
                    {
                    if (datt == '\"')
                        {
                        curnameparam = "";              // очистить текущее имя
                        F_is_name = true;
                        }
                    }
                }
            else
                {
                // ищем значение параметра
                if (F_is_param)
                    {
                    // находимся в значении параметра
                    bool F_need_reslt_check = false;
                    bool f_copy_char = true;

                    switch (type_el)
                        {
                        case '\"':
                            {
                            if (datt == '\"')
                                {
                                F_need_reslt_check = true;
                                }
                            break;
                            }
                        case '[':
                            {
                            if (datt == '[') counter_char++;
                            if (datt == ']')
                                {
                                if (counter_char)
                                    {
                                    counter_char--;
                                    }
                                else
                                    {
                                    F_need_reslt_check = true;
                                    }
                                }
                            break;
                            }
                        case '{':
                            {
                            if (datt == '{') counter_char++;
                            if (datt == '}')
                                {
                                if (counter_char)
                                    {
                                    counter_char--;
                                    }
                                else
                                    {
                                    F_need_reslt_check = true;
                                    }
                                }
                            break;
                            }
                        case 0:
                            {
                            if (datt == ' ' || datt == ',')
                                {
                                F_need_reslt_check = true;
                                f_copy_char = false;
                                }
                            break;
                            }
                        }
                    if (F_need_reslt_check)
                        {
                        if (f_copy_char) curvalue = curvalue + datt;
                        // выходим из имени параметра
                        // проверка значений и выход если нужное
                        if (JSON_name_param == curnameparam)
                            {
                            if (F_delim_check) rv_param = curvalue;
                            break;
                            }
                        else
                            {
                            F_need_name_param = false;
                            }
                        F_is_param = false;
                        }
                    else
                        {
                        curvalue = curvalue + datt; // формируем имя параметра
                        }
                    }
                else
                    {
                    if (datt > ' ')
                        {
                        if (datt == ':')
                            {
                            F_delim_check = true;
                            }
                        else
                            {
                            if (F_delim_check)
                                {
                                if (datt == '\"' || datt == '[' || datt == '{')
                                    {
                                    type_el = datt;
                                    }
                                else
                                    {
                                    type_el = 0;
                                    }
                                curvalue = datt;
                                F_is_param = true;    // вошли в параметр
                                }
                            }
                        }
                    }
                }
            }
        else
            {
            if (datt == '[') arr_chek = true;
            if (datt == '{')
                {
                if (arr_chek) F_is_array = true;    // находимся в массиве (разрешен индексное извлечение)
                F_sub_entered = true;
                }
            }
        lRam++;
        indx++;
        }
    }
return rv_param;
}


/*
bool JSON_GetItem_param (TDString JSON_txt, long i_array, TEX::JSONARRITEMPAR *lpJS_par)
{
bool rv = false;
TDString copyjson = JSON_txt;
char *lRam = copyjson.c_str();
unsigned long sizes = GetLenStr (lRam);
if (sizes && lpJS_par)
    {
    char datt;
    char type_el = 0;
    long cur_item = 0;
    unsigned long count_char_m = 0;
    unsigned long count_char_o = 0;
    unsigned long count_char = 0;
    lpJS_par->item_amount = 0;
    lpJS_par->prevcoma_ofs = -1;
    long comalast_ofs = -1;
    bool F_sub_entered = false;
    bool F_is_item = false;
    bool f_is_error = false;
    char delim_dat = 0;
    bool F_is_comma_body = false;
    bool F_comma_slash = false;
    unsigned char skip_counter = 0;
    char end_char = -1;

    while (count_char < sizes)
        {
        datt = lRam[0];
        if (!datt) break;


        if (F_sub_entered)  // вошли в массив
            {
            if (F_is_item)  // вошли в элемент массива
                {
                bool f_need_check = false;


                if (!F_is_comma_body)
                    {
                    if (datt == '{') count_char_o++;
                    if (datt == '[') count_char_m++;
                    if (datt == '\"') F_is_comma_body = true; // вход в тело строковой переменной
                    }
                else
                    {
                    if (!F_comma_slash)
                        {
                        switch (datt)
                            {
                            case '\"':
                                {
                                F_is_comma_body = false;
                                skip_counter = 1;
                                break;
                                }
                            case '\\':
                                {
                                F_comma_slash = true;
                                skip_counter = 1;
                                break;
                                }
                            }
                        }
                    else
                        {
                        //
                        switch (datt)
                            {
                            case '/':
                            case 'b':
                            case 'f':
                            case 'n':
                            case 'r':
                            case 't':
                            case '\\':
                            case '\"':
                                {
                                skip_counter = 1;
                                break;
                                }
                            case 'u':
                                {
                                skip_counter = 4;
                                break;
                                }
                            }
                        F_comma_slash = false;  // вставка символа произведена
                        }
                    }


                if (!skip_counter)
                    {
                    if (!F_is_comma_body)
                        {
                        switch (datt)
                            {
                            case '}':
                                {
                                if (count_char_o)
                                    {
                                    count_char_o--;
                                    }
                                else
                                    {
                                    if (end_char == datt)
                                        {
                                        if (lpJS_par->stop_ofs == - 1) lpJS_par->stop_ofs = count_char - 1;
                                        lpJS_par->end_ofs = count_char;     // найден конец данных  +
                                        f_need_check = true;
                                        }
                                    else
                                        {
                                        // Ошибка - найден не тот символ завершения
                                        f_is_error = true;
                                        }
                                    }
                                break;
                                }
                            case ']':
                                {
                                if (count_char_m)
                                    {
                                    count_char_m--;
                                    }
                                else
                                    {
                                    if (end_char == datt)
                                        {
                                        if (lpJS_par->stop_ofs == - 1) lpJS_par->stop_ofs = count_char - 1;
                                        lpJS_par->end_ofs = count_char;     // найден конец данных  +
                                        f_need_check = true;
                                        }
                                    else
                                        {
                                        // Ошибка - найден не тот символ завершения
                                        f_is_error = true;
                                        }
                                    }
                                break;
                                }
                            case ',':
                                {
                                if (!count_char_m && !count_char_o)
                                    {
                                    lpJS_par->stop_ofs = count_char - 1;
                                    lpJS_par->coma_ofs = count_char;
                                    comalast_ofs = count_char;
                                    f_need_check = true;

                                    }
                                else
                                    {
                                    // запятая в теле обьекта или массива
                                    }
                                break;
                                }
                            default:
                                {
                                if (datt > ' ')
                                    {
                                    if (lpJS_par->start_ofs == -1) lpJS_par->start_ofs = count_char;
                                    }
                                break;
                                }
                            }
                        }
                    if (f_need_check)
                        {
                        if (cur_item == i_array)
                            {
                            break;
                            }
                        else
                            {
                            lpJS_par->start_ofs = -1;
                            lpJS_par->stop_ofs = -1;
                            lpJS_par->coma_ofs = -1;
                            lpJS_par->end_ofs = -1;
                            F_is_item = false;
                            }
                        cur_item++;
                        if (cur_item > i_array) break;
                        }
                    }
                else
                    {
                    skip_counter--; // счетчик спец вставок
                    }
                if (f_is_error) break;
                }
            else
                {
                if (datt > ' ')
                    {
                    lpJS_par->start_ofs = -1;
                    lpJS_par->stop_ofs = -1;
                    lpJS_par->coma_ofs = -1;
                    lpJS_par->end_ofs = -1;
                    if (datt != end_char)
                        {
                        count_char_m = 0;
                        count_char_o = 0;
                        if (datt == '[')count_char_m++;
                        if (datt == '{')count_char_o++;
                        if (datt == '\"') F_is_comma_body = true;
                        lpJS_par->prevcoma_ofs = comalast_ofs;
                        lpJS_par->start_ofs = count_char;
                        delim_dat = datt;
                        F_is_item = true;
                        lpJS_par->item_amount++;        // подсчет количества элементов
                        }
                    else
                        {
                        break;
                        }
                    }
                }
            }
        else
            {
            // end_char
            bool f_ent = false;
            switch (datt)
                {
                case '[':
                    {
                    end_char = ']';
                    f_ent = true;
                    break;
                    }
                case '{':
                    {
                    end_char = '}';
                    f_ent = true;
                    break;
                    }
                }
            if (f_ent)
                {
                lpJS_par->m_enter_ofs = count_char;
                F_sub_entered = true;
                }
            }
        lRam++;
        count_char++;
        }
    rv = !f_is_error;
    }
return rv;
}




bool JSON_Item_delete (TDString &JSON_inp,  long i_array)
{
bool rv = false, rsltf;
TEX::JSONARRITEMPAR par;
long el_amount;
if (i_array >= 0)
    {
    unsigned long start_ofs = 0, inc_v = 0;
    rsltf = JSON_GetItem_param (JSON_inp, i_array, &par);
    if (rsltf)
        {
        if (par.prevcoma_ofs != -1)
            {
            start_ofs = par.prevcoma_ofs;
            }
        else
            {
            start_ofs = par.start_ofs;
            inc_v = 1;
            }
        if (par.stop_ofs != -1)
            {
            if (par.stop_ofs >= start_ofs)
                {
                unsigned long sz = 0;
                if (par.coma_ofs != - 1) sz = par.coma_ofs - start_ofs + inc_v;
                if (par.end_ofs != - 1) sz = par.end_ofs - start_ofs;
                if (sz)
                    {
                    JSON_inp.Delete ((start_ofs + 1), sz);
                    rv = true;
                    }
                }
            }
        }
    }
return rv;
}

 */

/*
bool JSON_Item_replace (TDString &JSON_inp, TDString &JSON_arrItem, long i_array)
{
bool rv = false;
TEX::JSONARRITEMPAR par;
long el_amount;
if (i_array >= 0)
    {
    TDString copy_json = JSON_inp;
    if (JSON_GetItem_param (copy_json, i_array, &par))
        {
        if (JSON_Item_insert (copy_json, JSON_arrItem, i_array))
            {
            if (JSON_Item_delete (copy_json, (i_array + 1)))
                {
                JSON_inp = copy_json;
                rv = true;
                }
            }
        }
    }
return rv;
}




bool JSON_Item_insert (TDString &JSON_txt, const TDString &JSON_insert, long i_array)
{
bool rv = false, rsltf;
TEX::JSONARRITEMPAR par;
long el_amount;
if (i_array >= 0)
    {
    rsltf = JSON_GetItem_param (JSON_txt, i_array, &par);
    if (rsltf)
        {
        if (par.stop_ofs != -1)
            {
            if (par.stop_ofs >= par.start_ofs)
                {
                TDString sddstr = "";
                JSON_txt.Insert ((JSON_insert + ','),(par.start_ofs + 1));
                rv = true;
                }
            }
        }
    }
return rv;
}



long JSON_Item_add (TDString &JSON_txt, TDString &JSON_insert)
{
long rv = -1;
TEX::JSONARRITEMPAR par;
long el_amount;

if (JSON_GetItem_param (JSON_txt, 0xFFFFFFF, &par))
    {
    unsigned long amount = par.item_amount;
    if (amount)
        {
        if (JSON_GetItem_param (JSON_txt, (amount - 1), &par))
            {
            JSON_txt.Insert (("," + JSON_insert),(par.end_ofs + 1));
            rv = amount;
            }
        }
    else
        {
        if (par.m_enter_ofs != -1)
            {
            JSON_txt.Insert (JSON_insert,(par.m_enter_ofs + 2));
            rv = 0;
            }
        }
    }

return rv;
}



TDString JSON_Item_get (TDString JSON_txt, long i_array)
{
TDString rv = "";
bool rsltf;
TEX::JSONARRITEMPAR par;
if (i_array >= 0)
    {
    rsltf = JSON_GetItem_param (JSON_txt, i_array, &par);
    if (rsltf)
        {
        if (par.stop_ofs != -1)
            {
            if (par.stop_ofs >= par.start_ofs)
                {
                char *lTxt = JSON_txt.c_str() + par.start_ofs;
                TDString sddstr = "";
                unsigned long CntBt = par.stop_ofs - par.start_ofs + 1;
                CreateStringFromLine_cnt (sddstr, lTxt, CntBt);
                rv = sddstr;
                }
            }
        }
    }
return rv;
}
*/


/*
EJSNTYPE JSON_GetTypeParam (TDString JSON_inp, TDString &outname, TDString &outval)
{
TEX::EJSNTYPE rv = TEX::EJST_NONE;
char *lJsIn = JSON_inp.c_str();
//unsigned long sz_json = GetLenStr (lJsIn);
unsigned long cur_indx = 0;
char dat;
bool F_is_comma_body = false;
char sw_enum = 0;   // 0 - поиск имени, 1 - в имени, 2 - поиск разделителя параметра, 3 - поиск параметра, 4- в параметре
bool F_is_param = false;
bool F_comma_slash = false;
long skip_counter = 0;
char code_data = -1;
bool f_exit = false;

while (true)
    {
    dat = lJsIn[0];
    if (!dat) break;

    switch (sw_enum)
        {
        case 0:     // поиск имени параметра
            {
            if (dat == '\"')
                {
                sw_enum = 1;
                outname = "";
                }
            break;
            }
        case 1:     // в имени параметра
            {
            if (!skip_counter)
                {
                if (!F_comma_slash)
                    {
                    switch (dat)
                        {
                        case '\"':
                            {
                            sw_enum = 2;    // поиск параметра
                            break;
                            }
                        case '\\':
                            {
                            F_comma_slash = true;       // без break
                            }
                        default:
                            {
                            outname += dat;
                            break;
                            }
                        }
                    }
                else
                    {
                    //
                    switch (dat)
                        {
                        case '/':
                        case 'b':
                        case 'f':
                        case 'n':
                        case 'r':
                        case 't':
                        case '\\':
                        case '\"':
                            {
                            skip_counter = 1;
                            break;
                            }
                        case 'u':
                            {
                            skip_counter = 4;
                            break;
                            }
                        }
                    F_comma_slash = false;  // вставка символа произведена
                    outname += dat;
                    }
                }
            else
                {
                skip_counter--;
                outname += dat;
                }

            break;
            }
        case 2:     // поиск разделителя параметра
            {
            if (dat == ':')
                {
                outval = "";
                sw_enum = 3;
                }
            break;
            }
        case 3:     // поиск параметра
            {
            if (dat > ' ')
                {
                switch (dat)
                    {
                    case '\"':
                    case '{':
                    case '[':
                        {
                        code_data = dat;
                        break;
                        }
                    default:
                        {
                        code_data = 0;
                        break;
                        }
                    }
                outval += dat;
                sw_enum = 4;    // в параметре
                }
            break;
            }
        case 4:     // в параметре
            {
            switch (code_data)
                {
                case 0:         // без break
                    {
                    if (dat == ' ')
                        {
                        f_exit = true;
                        break;
                        }
                    }
                default:
                    {
                    outval += dat;
                    break;
                    }
                }
            break;
            }
        }

    if (f_exit) break;
    lJsIn++;
    cur_indx++;
    }
if (sw_enum == 4)
    {
    switch (code_data)
        {
        case 0:
            {
            rv = TEX::EJST_DIG;
            break;
            }
        case '\"':
            {
            rv = TEX::EJST_STRING;
            break;
            }
        case '{':
            {
            rv = TEX::EJST_OBJ;
            break;
            }
        case '[':
            {
            rv = TEX::EJST_ARRAY;
            break;
            }
        }
    }
return rv;
}




long JSON_FindItemParam_fromOBJ (TDString JSON_txt, TDString nameparam, TDString &outParam)
{
long rv = -1;
if (JSON_txt != "" && nameparam != "")
    {
    TDString find_name, find_param, cur_item;
    unsigned long indx = 0;
    bool rslt;
    while (true)
        {
        cur_item = JSON_Item_get (JSON_txt, indx);
        if (cur_item != "")
            {
            TEX::EJSNTYPE type_rv = JSON_GetTypeParam (cur_item, find_name, find_param);
            if (type_rv != TEX::EJST_NONE)
                {
                if (find_name != "")
                    {
                    if (find_name == nameparam)
                        {
                        outParam = find_param;
                        rv = indx;
                        break;
                        }
                    }
                }
            else
                {
                // ошибка - неизвестный тип
                break;
                }
            }
        else
            {
            // конец списка
            break;
            }
        indx++;
        }
    }
return rv;
}





long JSON_ReplaceItemParam_fromOBJ (TDString &JSON_txt, TDString nameparam, TDString newParam)
{
long rv = -1;
if (JSON_txt != "" && nameparam != "")
    {
    TDString find_name = "", find_param = "", cur_item = "";
    unsigned long indx = 0;
    bool rslt;
    while (true)
        {
        cur_item = JSON_Item_get (JSON_txt, indx);
        if (cur_item != "")
            {
            find_name = "", find_param = "";
            TEX::EJSNTYPE type_rv = JSON_GetTypeParam (cur_item, find_name, find_param);
            if (type_rv != TEX::EJST_NONE)
                {
                if (find_name != "")
                    {
                    if (find_name == nameparam)
                        {
                        TDString item_new = "\"" + find_name + "\"" + " : " + newParam;
                        if (JSON_Item_replace (JSON_txt, item_new, indx))
                            {
                            rv = indx;
                            }
                        break;
                        }
                    }
                }
            else
                {
                // ошибка - неизвестный тип
                break;
                }
            }
        else
            {
            // конец списка
            break;
            }
        indx++;
        }
    }
return rv;
}




void JSON_CuteCommaStrParam (TDString &JSON_strparam)
{
TDString rv = "";
char *lpRam = JSON_strparam.c_str();
char dat;
long ofs_start = -1;
long ofs_stop = -1;
unsigned long indx = 0;
while (true)
    {
    dat = lpRam[0];
    if (!dat) break;
    if (dat == '\"')
        {
        if (ofs_start == -1)
            {
            ofs_start = indx;
            }
        else
            {
            ofs_stop = indx;
            }
        }
    indx++;
    lpRam++;
    }
if (ofs_stop > ofs_start)
    {
    rv = JSON_strparam.SubString ((ofs_start + 2), (ofs_stop - ofs_start - 1));
    JSON_strparam = rv;
    }

}

*/




TDString Header_GetValue (TDString hdr_txt, TDString name_param, char delimtr)
{
TDString rv_param = "";
char *lRam = (char*)hdr_txt.c_str();
TDString curnameparam = "";
TDString curvalue = "";
unsigned long sizes = GetLenStr (lRam);
if (sizes)
    {
    char datt;
    unsigned long indx = 0;
    bool F_is_name = false;
    bool F_is_param = false;
    bool F_sub_entered = false;
    //bool F_delim_check = false;
    bool F_need_name_param = false;       // false - поиск имени параметра, true - поиск значения
    unsigned long entered_count = 0;

    while (indx < sizes)
        {
        datt = lRam[0];
        if (!datt) break;


            if (!F_need_name_param)     // ищем имя параметра
                {
                if (F_is_name)  // находимся в имени параметра
                    {
                    if (datt == delimtr)
                        {
                        // выходим из имени параметра
                        F_is_name = false;
                        F_need_name_param = true;   // будем искать значение параметра
                        //F_delim_check = false;      // сбросить флаг обнаружения ':'
                        }
                    else
                        {
                        curnameparam = curnameparam + datt; // формируем имя параметра
                        }
                    }
                else
                    {
                    if (datt > ' ')
                        {
                        curnameparam = datt;              // очистить текущее имя
                        F_is_name = true;
                        }
                    }
                }
            else
                {
                // ищем значение параметра
                //if (datt == ':') F_delim_check = true;  // обнаружен разделитель
                if (F_is_param)
                    {
                    // находимся в значении параметра
                    if (datt == '\r')
                        {
                        // выходим из имени параметра
                        // проверка значений и выход если нужное
                        if (name_param == curnameparam)
                            {
                            rv_param = curvalue;
                            break;
                            }
                        else
                            {
                            F_need_name_param = false;
                            }
                        F_is_param = false;
                        }
                    else
                        {
                        curvalue = curvalue + datt; // формируем имя параметра
                        }
                    }
                else
                    {
                    if (datt > ' ')
                        {
                        curvalue = datt;
                        F_is_param = true;    // вошли в параметр
                        }
                    }
                }
        lRam++;
        indx++;
        }
    }
return rv_param;
}



TDString jsSeqKey_find = "";
char *jslSqKey = 0;
long jsIndx = 0;




// пошагово парсит json ключ пример:  NAME.NAME,NAME[101].ADRESS.POINT[1]
// возвращая тип и имя следующего обьекта 
// три типа: значение, обьект, массив. Для массива третий параметр - индекс
/*
bool FLJSON_getNext (TDString &d_Name, TEX::EJSKEYTYPE &d_type, long &d_indx)
{
bool rv = false;
TDString Str_rslt = "";
TDString str_indx = "";
char dat;
bool F_isPoint = false;
bool F_isArr = false;   // находимся в индексном указателе
bool F_data = false;    // имя обнаружено и сформировано
long DigCount = 0;      // массив обнаружен
if (jslSqKey)
    {
    while (true)
        {
        dat = jslSqKey[0];
        if (dat != 10 && dat != 13 && dat != 9)
            {
            if (!dat)
                {
                jslSqKey = 0;
                break;
                }
            if (dat == '.')
                {
                F_isPoint = true;
                jslSqKey++;
                break;
                }
            else
                {
                if (dat == '[')
                    {
                    DigCount = 0;
                    str_indx = "";
                    F_isArr = true;
                    }
                else
                    {
                    if (dat == ']')
                        {
                        F_isArr = false;
                        }
                    else
                        {
                        if (F_isArr)
                            {
                            str_indx += dat;
                            DigCount++;
                            }
                        else
                            {
                            Str_rslt += dat;
                            F_data = true;
                            }
                        }
                    }
                }
            }
        jslSqKey++;
        }
    if (F_data)
        {
        if (F_isArr)
            {
            // массив обьектов или значение массива
            if (str_indx != "")
                {
                d_indx = str_indx.ToInt();
                d_type = TEX::EJSKEY_ARR;
                rv = true;
                }
            }
        else
            {
            // обьект или значение
            if (Str_rslt != "") // значит это обьект
                {
                d_type = TEX::EJSKEY_OBJ;
                rv = true;
                }
            }
        d_Name = Str_rslt;
        }
    }
return rv;
}



bool FLJSON_getFist (TDString JS_key, TDString &d_Name, TEX::EJSKEYTYPE &d_type, long &d_indx)
{
jsSeqKey_find = JS_key;
jslSqKey = jsSeqKey_find.c_str();
jsIndx = 0;
return FLJSON_getNext (d_Name, d_type, d_indx);
}
*/



// если поле присутствует, то возвращает указатель на элемент
/*
char *FLJSON_check (TDString jsTxt, TDString jsonkey)
{
char *l_rv = 0;
TDString d_Name = "";
TEX::EJSKEYTYPE d_type;
long d_indx = 0;
bool rslt = FLJSON_getFist (jsonkey, d_Name, d_type, d_indx);
while (rslt)
  {
  switch (d_Name)
    {
    case TEX::EJSKEY_VALUE:
        {
        break;
        }
    case TEX::EJSKEY_OBJ:
        {
        break;
        }
    case TEX::EJSKEY_ARRVAL:
        {
        break;
        }
    case TEX::EJSKEY_OBJARR:
        {
        break;
        }
    }
  rslt = FLJSON_getNext (d_Name, d_type, d_indx);
  }

return l_rv;
}
*/



bool FLJSON_Get (TDString const *jsTxt, TDString const *jsonkey, TDString &val)
{
}



bool FLJSON_Set (TDString jsTxt, TDString const *jsonkey, TDString val)
{
}


// находит физические границы обьекта или массива для ограничения парсинга
long JSON_OBJARR_GetSize (char *lCurRam, char objarr_symb)
{
long rv = 0;
if (lCurRam)
    {
    char *lStartRam = 0;
    long o_cnt = 0;
    long a_cnt = 0;
    char dat;
    while (true)
        {
        dat = lCurRam[0];
        if (!dat)
            {

            break;
            }
        else
            {
            if (dat == '[')
                {
                if (objarr_symb == '[' && !a_cnt && !lStartRam) lStartRam = lCurRam;
                a_cnt++;
                }
            if (dat == '{')
                {
                if (objarr_symb == '{' && !o_cnt && !lStartRam) lStartRam = lCurRam;
                o_cnt++;
                }
            if (dat == ']')
                {
                if (a_cnt)
                    {
                    a_cnt--;
                    if (!a_cnt)
                        {
                        if (objarr_symb == '[')
                            {
                            //
                            if (o_cnt == 0) rv = (lCurRam - lStartRam) + 1;
                            break;
                            }
                        }
                    }
                }
            if (dat == '}')
                {
                if (o_cnt)
                    {
                    o_cnt--;
                    if (!o_cnt)
                        {
                        if (objarr_symb == '{')
                            {
                            if (a_cnt == 0) rv = (lCurRam - lStartRam) + 1;
                            break;
                            }
                        }
                    }
                }
            }
        lCurRam++;
        }
    }
return rv;
}


// проверяет число на десятичный формат и  дополнительно возвращает размер , если строка не десятичного проедставления, возвращает 0
unsigned long CheckDecimal (char *lTxt, unsigned long sz)
{
unsigned long rv = 0;
if (lTxt && sz)
	{
	unsigned char datt;
	if (!sz) sz = GetLenStr (lTxt);
	while (sz)
		{
		datt = lTxt[0];
		if (!datt) break;
		if (datt < '0' || datt > '9')
			{
			rv = 0;
			break;
			}
		rv++;
		lTxt++;
		sz--;
		}
	}
return rv;
}



unsigned long GetSizeToDelim (char *lTxt, unsigned long sz, char delim)
{
unsigned long rv = 0;
if (lTxt)
	{
	char dat;
	while (sz)
		{
		dat = *lTxt;
		if (!dat || dat == delim) break;
		rv++;
		lTxt++;
		sz--;
		}
	}
return rv;
}



// преобразует данные из текста по указаному адресу в unsigned long число
// если число больше чем 10 знакомест или его нет в первом байте указанного адресса, возвращает 0 - (ошибка)
bool TxtToULong (unsigned char *lpRamData, unsigned char sz, unsigned long *lpDataOut)
{
bool rv = false;
if (lpRamData)
	{
	unsigned char dtasd;
	unsigned char *lpTmpAdr = lpRamData;
	unsigned char indx = 0;
	unsigned long Sizes = sz;
	if (!Sizes) Sizes = GetLenStr (lpRamData);
	if (CheckDecimal (lpRamData, Sizes) == Sizes)
		{
		if (Sizes < 11)
			{
			unsigned long Multer;
			unsigned long DigOut = 0;
			while (Sizes)
				{
				Multer = tmpMultData[Sizes-1];
				DigOut = DigOut + (Multer * (lpRamData[0] - '0'));
				lpRamData++;
				Sizes--;
				}
			if (lpDataOut) *lpDataOut = DigOut;
			rv = true;
			}
		}
	}
return rv;
}



bool TxtToFloat (float *lpDest, char *lpTxtIn, unsigned long Sizes)
{
bool rv = false;
if (lpDest && lpTxtIn)
	{
	if (!Sizes) Sizes = GetLenStr (lpTxtIn);
	if (Sizes && Sizes < 32)
		{
		float Datf = 0;
		unsigned long NLongDroba = 0;
		unsigned long NLongCel = 0;
		unsigned long len_drb = 0,len_cel = 0;
        bool f_sig = false;
        if (lpTxtIn[0] == '-')
          {
          f_sig = true;
          lpTxtIn++;
          Sizes--;
          }
        if (Sizes)
          {
          len_cel = GetSizeToDelim (lpTxtIn,Sizes,'.');
          if (len_cel)
              {
              len_drb = Sizes - len_cel;
              if (len_drb)
                  {
                  len_drb--;
                  if (!len_drb) return rv;
                  if (!TxtToULong ((lpTxtIn + len_cel + sizeof('.')), len_drb, &NLongDroba)) return rv;
                  }
              if (!TxtToULong (lpTxtIn, len_cel, &NLongCel)) return rv;
              Datf = NLongCel;
              if (len_drb)
                  {
                  float mult = ((float)(1.0)) / tmpMultData[len_drb];
                  float drba = NLongDroba;
                  Datf = Datf + (drba * mult) + 0.0000005;
                  }
              if (f_sig) Datf *= (-1.0);
              lpDest[0] = Datf;
              rv = true;
              }
           }
		}
	}
return rv;
}






unsigned long CreateValueFromBitMassive32out (unsigned char *lInp,  unsigned long CurOffset, unsigned char databitsize)
{
unsigned long rv = 0;
if (lInp  && databitsize && databitsize <= 32)
	{

	unsigned long byte_ofs = CurOffset / 8;
	unsigned char bit_msk_input = 128;
    bit_msk_input = bit_msk_input >> (CurOffset % 8);

    lInp += byte_ofs;

	unsigned long bit_msk_output = 1;
	bit_msk_output = bit_msk_output << (databitsize - 1);
	
	// выходная формирующая маска bit_msk_output
	// входная маска bit_msk_input
	while (databitsize)
		{
		if (lInp[0] & bit_msk_input) rv |= bit_msk_output;
		bit_msk_input = bit_msk_input >> 1;
		if (!bit_msk_input)
            {
            bit_msk_input = 128;
            lInp++;
            }
		bit_msk_output = bit_msk_output >> 1;
		if (!bit_msk_output) bit_msk_output = 0x80000000;
		databitsize--;
		}
	}
return rv;
}


unsigned long CreateValueFromBitMassive (unsigned char *lInp, unsigned long CurOffset, unsigned char databitsize)
{
unsigned long rv = 0;
if (lInp  && databitsize && databitsize <= 32)
	{
	unsigned long byte_ofs = CurOffset / 8;
	unsigned char bit_msk_input = 128;
	unsigned long bit_msk_output = 1;
	bit_msk_output = bit_msk_output << (databitsize - 1);
		
  bit_msk_input = bit_msk_input >> (CurOffset % 8);
  lInp += byte_ofs;

	while (databitsize)
		{
		if (lInp[0] & bit_msk_input) rv |= bit_msk_output;
		bit_msk_input = bit_msk_input >> 1;
		if (!bit_msk_input)
			{
			bit_msk_input = 128;
			lInp++;
			}
		bit_msk_output = bit_msk_output >> 1;
		//if (!bit_msk_output) bit_msk_output = 0x80000000;
		databitsize--;
		}
	}
return rv;
}


unsigned long long CreateValueFromBitMassive64out (unsigned char *lInp,  unsigned long CurOffset, unsigned char databitsize)
{
unsigned long rv = 0;
if (lInp && databitsize && databitsize <= 64)
	{

	unsigned long byte_ofs = CurOffset / 8;
	unsigned char bit_msk_input = 128;
    bit_msk_input = bit_msk_input >> (CurOffset % 8);

    lInp += byte_ofs;

	unsigned long long bit_msk_output = 1;
	bit_msk_output = bit_msk_output << (databitsize - 1);
	
	// выходная формирующая маска bit_msk_output
	// входная маска bit_msk_input
	while (databitsize)
		{
		if (lInp[0] & bit_msk_input) rv |= bit_msk_output;
		bit_msk_input = bit_msk_input >> 1;
		if (!bit_msk_input)
            {
            bit_msk_input = 128;
            lInp++;
            }
		bit_msk_output = bit_msk_output >> 1;
		if (!bit_msk_output) bit_msk_output = 0x8000000000000000;
		databitsize--;
		}
	}
return rv;
}




void CreateValueToBitMassive (void *lpStart, unsigned short d_bits_offset, unsigned long long value_save, unsigned char bits_n)
{
if (lpStart)
    {
    char *lDst = (char*)lpStart;
    // формируем байтовое смещение и выходную битовую маску
	unsigned long byte_ofs = d_bits_offset / 8;
	unsigned char bit_msk_output = 128;
    bit_msk_output = bit_msk_output >> (d_bits_offset % 8);

	unsigned long long bit_msk_input = 1;
	bit_msk_input = bit_msk_input << (bits_n - 1);
    unsigned char dat;

    while (bits_n)
        {
        dat = lDst[byte_ofs];
        if (value_save & bit_msk_input)
            {
            dat |= bit_msk_output;
            }
        else
            {
            dat &= (0xFF ^ bit_msk_output);
            }
        lDst[byte_ofs] = dat;
        bit_msk_output = bit_msk_output >> 1;
        if (!bit_msk_output)
            {
            bit_msk_output = 128;
            byte_ofs++;
            }
        bit_msk_input = bit_msk_input >> 1;
        bits_n--;
        }
    }
}




void SBUSDecodingArrays (unsigned char *lBase, unsigned short Dst[16])
{
unsigned char mask_inp = 1, cnt_loc;
unsigned char out_rc_ix = 0;
unsigned short dat_out, mask_out;
unsigned char dat_in = *lBase;
while (out_rc_ix < 16)
    {
    cnt_loc = 11;
    dat_out = 0;
    mask_out = 1;
    while (cnt_loc)
        {
        if (dat_in & mask_inp) dat_out |= mask_out;
        mask_out <<= 1;
        mask_inp <<= 1;
        if (!mask_inp)
            {
            mask_inp = 1;
            lBase++;
            dat_in = *lBase;
            }
        cnt_loc--;
        }
    Dst[out_rc_ix] = dat_out;
    out_rc_ix++;
    }
}



void SBUSCodingArrays (unsigned char *lBase, unsigned short Src[16])
{
unsigned char mask_out = 1, cnt_loc;
unsigned char cicle = 16;
unsigned short dat_in, mask_in;
unsigned char dat_out = 0;
while (cicle)
    {
    cnt_loc = 11;
    dat_in = *Src++;
    mask_in = 1;
    while (cnt_loc)
        {
        if (dat_in & mask_in) dat_out |= mask_out;
        mask_in <<= 1;
        mask_out <<= 1;
        if (!mask_out)
            {
            *lBase++ = dat_out;
            dat_out = 0;
            mask_out = 1;
            }
        cnt_loc--;
        }
    cicle--;
    }
}



unsigned char *LongToStr (unsigned char *lpDest, long datas)
{
if (lpDest)
	{
	bool m_state = false;
	unsigned long val;
	if (datas < 0)
		{
		m_state = true;
		val = abs32 (datas);
		}
	else
		{
		val = datas;
		}
	if (m_state)
		{
		lpDest[0] = '-';
		lpDest++;
		}
	lpDest = UlongToStr (lpDest, val);
	}
return lpDest;
}



unsigned char *I64ToStr (unsigned char *lpDest, int64_t datas)
{
if (lpDest)
	{
	bool m_state = false;
	uint64_t val;
	if (datas < 0)
		{
		m_state = true;
		val = abs64 (datas);
		}
	else
		{
		val = datas;
		}
	if (m_state)
		{
		lpDest[0] = '-';
		lpDest++;
		}
	lpDest = Uint64ToStr (lpDest, val);
	}
return lpDest;
}



void I64ToStr (int64_t datas, TDString &dst)
{
unsigned char flagnz = 0, resd;
uint64_t delmt = 10000000000000000000;

if (datas < 0)
    {
    dst = "-";
    datas = abs64 (datas);
    }
else
    {
    dst = "";
    }


while (true)
	{
	if (datas>=delmt)
		{
		flagnz=1;
		resd=(datas/delmt);
		datas=datas % delmt;
		}
	else
		{
		resd=0;
		}
	if (flagnz) dst += (resd+48);

	delmt=delmt/10;

	if (delmt<=1)
		{
        dst += (datas+48);
		break;
		}
	}
}



TDString UTF8To1251 (TDString utf8_str)
{
char *lStr_u8 = (char*)utf8_str.c_str();
unsigned int sz = GetLenStr (lStr_u8);
TDString rv = "";
wchar_t *lWch = new wchar_t [sz * 2];
if (lWch && sz)
    {
    long sz_utf8 = MultiByteToWideChar(CP_UTF8, 0, lStr_u8, sz, lWch, sz * 2);
    if (sz_utf8)
        {
        char *lC1251 = new char [sz_utf8 * 2];
        if (lC1251)
            {
            long csz = WideCharToMultiByte(1251, 0, lWch, sz_utf8, lC1251, sz_utf8 * 2, NULL, NULL);
            lC1251[csz] = 0;
            rv = rv + lC1251;
            delete [] lC1251;
            }
        }
    delete []lWch;
    }
return rv;
}



TDString F1251ToUTF8 (TDString uc1251_str)
{
char *lStr_1251 = (char*)uc1251_str.c_str();
unsigned int sz = GetLenStr (lStr_1251);
TDString rv = "";
unsigned long cur_sz = sz * 2;
wchar_t *lWch = new wchar_t [cur_sz];
if (lWch)
    {
    MultiByteToWideChar(1251, 0, lStr_1251, -1, lWch, cur_sz);
    char *lutf8 = new char [cur_sz];
    if (lutf8)
        {
        WideCharToMultiByte(CP_UTF8, 0, lWch, -1, lutf8, cur_sz, NULL, NULL);
        rv = rv + lutf8;
        delete [] lutf8;
        }
    delete []lWch;
    }
return rv;
}



long find_data_array_ul (unsigned long *arr, unsigned long ixcnt, unsigned long data)
{
long rv = -1;
if (arr)
    {
    unsigned long ix = 0;
    while (ix < ixcnt)
        {
        if (arr[ix] == data)
            {
            rv = ix;
            break;
            }
        ix++;
        }
    }
return rv;
}



bool subtimeout (unsigned long &tmr, unsigned long dt)
{
bool rv = false;
if (tmr && dt)
    {
    if (tmr > dt)
        {
        tmr -= dt;
        }
    else
        {
        tmr = 0;
        rv = true;
        }
    }
return rv;
}



static const short utf[ 256 ] = {
    0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
    0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
    0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,
    0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,
    0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,
    0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,
    0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,
    0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x402,0x403,0x201a,0x453,0x201e,
    0x2026,0x2020,0x2021,0x20ac,0x2030,0x409,0x2039,0x40a,0x40c,0x40b,0x40f,0x452,
    0x2018,0x2019,0x201c,0x201d,0x2022,0x2013,0x2014,0,0x2122,0x459,0x203a,0x45a,
    0x45c,0x45b,0x45f,0xa0,0x40e,0x45e,0x408,0xa4,0x490,0xa6,0xa7,0x401,0xa9,0x404,
    0xab,0xac,0xad,0xae,0x407,0xb0,0xb1,0x406,0x456,0x491,0xb5,0xb6,0xb7,0x451,
    0x2116,0x454,0xbb,0x458,0x405,0x455,0x457,0x410,0x411,0x412,0x413,0x414,0x415,
    0x416,0x417,0x418,0x419,0x41a,0x41b,0x41c,0x41d,0x41e,0x41f,0x420,0x421,0x422,
    0x423,0x424,0x425,0x426,0x427,0x428,0x429,0x42a,0x42b,0x42c,0x42d,0x42e,0x42f,
    0x430,0x431,0x432,0x433,0x434,0x435,0x436,0x437,0x438,0x439,0x43a,0x43b,0x43c,
    0x43d,0x43e,0x43f,0x440,0x441,0x442,0x443,0x444,0x445,0x446,0x447,0x448,0x449,
    0x44a,0x44b,0x44c,0x44d,0x44e,0x44f
};


unsigned char W1251ToUTF8b (unsigned char dat, unsigned char dst[2])
{
unsigned char rv = 1;
unsigned short c = utf[ dat];
    if( c < 0x80 ) {
        dst[0] = c;
    }
    else if( c < 0x800 ) {
        dst[ 0] =  c >> 6 | 0xc0;
        dst[ 1 ] = c & 0x3f | 0x80;
        rv++;
    }
return rv;
}



unsigned long GN_CRC32 (void *ldata, unsigned long size)
{
const unsigned long one_sig = 0x724169A3;
const unsigned long zero_sig = 0x28181277;
const unsigned char mask_sig = 0x18;
unsigned long crc = 0;
if (ldata && size)
    {
    unsigned char *src = (char*)ldata;
    unsigned char dat;
    while (size)
        {
        dat = *src++;
        if (dat & mask_sig)
            {
            crc += one_sig;
            }
        else
            {
            crc += zero_sig;
            }
        crc <<= 1;
        crc += dat;
        size--;
        }
    }
return crc;
}




bool subtimer (long &timerr, unsigned long ms)
{
bool rv = false;
if (timerr)
    {
    if (timerr > ms)
        {
        timerr -= ms;
        }
    else
        {
        timerr = 0;
        rv = true;
        }
    }
return rv;
}

