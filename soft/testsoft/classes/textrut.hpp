#ifndef TextRutine_1
#define TextRutine_1

#include "stdint.h"
#include <string>

#ifdef ISWINDOWS
    #include "windows.h"
    #include "stdlib.h"
    //#include "vcl.h"
    #define TDString std::string
#else
    #define TDString std::string
#endif

typedef struct {
	void *lRam;
	unsigned long sizes;
}TBUFPARAM;


namespace TEX {

	const unsigned char C_INC = 1;
	const unsigned char C_DEC = 2;
    const char C_NORMAL = 1;
    const char C_MINVAL = 2;
    const char C_MAXVAL = 3;


typedef struct {
    union {
        unsigned char IPADR[4];
        unsigned long l;
        }ip;
} IPDIGSTRING;



typedef struct {
	void *lpRam;
	unsigned long Sizes;
}BUFPAR;

}


//#include "trawbitmap.hpp"


typedef struct {
    TDString getparam;
    TDString host_ip;
    TDString host_port;
    TDString connectmod;
} GETHDR;


bool str_compare (char *lStr1, char *lStr2, unsigned long size);
unsigned long ExtractFileNameBase (TDString FileNamee, TDString &outext);   // отбрасывает из имени файла его расширение после точки, оставляя весь предыдущий путь
unsigned long ExtractFileNameExt (TDString FileNamee, TDString &outext);    // извлекает расширение файла
TDString CutFileName (TDString filename, unsigned long sizeASCIZZ);
TDString GetShortName (TDString str, unsigned long maxlen);                 // сокращает любой стринг если он больше определенной длины, со вставкой точек в конце
TDString GenIPBroadcastAdress (TDString IpAdrStr, TDString IpSubNemtMask);    // по IP адрессу и маске сети формирует броадкаст адресс
void ExtractIPAdress (TDString InpDataStr, TEX::IPDIGSTRING *Dsst);       // преобразует строку IP адреса в цифровой IP
void GetIPAdress (TDString &outstr, TEX::IPDIGSTRING *Inp);               
void GetIPAdress (TDString &outstr, unsigned char *lpIPAdr);
void FileNameChangeExt (TDString &FileName, TDString &NewExt);  // меняет расширения файла на новое
void AddASCIZZToStr (TDString &str, char *lasciiz);
void AddASCIZZToStr_cnt (TDString &str, char *lasciiz, unsigned long sizee);
unsigned long sqrt_cpu_newton (long L);
void ExtractURLandPath (char *lpFullUrl, TDString &UlrOut, TDString &PathOut);
unsigned char LTetraToHex (unsigned char datas);    // преобразует величину 0-15 в HEX ASCII код
unsigned char AddAsciiHex_DecimalValue (char ASCIIHex, unsigned char val);      // добавляет к ASCII коду HEX формата цифровую величину, возвращается ASCII код
unsigned char SubAsciiHex_DecimalValue (char ASCIIHex, unsigned char val);      // вычитает с ASCII кода HEX формата цифровую величину, возвращается ASCII код
void DatasToHexs_Str (unsigned char *lpRamData,unsigned int sizesdat, TDString &outptsstr);
bool CheckHexShortAdr (TDString str1);
long GetNBitmsk (unsigned long btmsk);
TDString GetSysTimeString_File (SYSTEMTIME *lpSysTime);
TDString GetSysDayString (SYSTEMTIME *lpSysTime, char delims);
bool CheckHexVal (unsigned char datas);
char ConvBinToASCIIHex (unsigned char datas);
char FullExtractFilenamePath (char *lpFPFileName, TDString &OutDiskName, TDString &OutPathNames, TDString &OutFileName);
unsigned long GetCntDelimCharsFrString (char *lpStrName, char DelimCh);
unsigned char ConvNDataToASCII (unsigned char datas);
TDString GetCurentEXEFileName (HINSTANCE hInstance);
bool RemoveXServices (TDString strNameRegedit);
bool InstallXServices (TDString SfullName, TDString DfileName);
TDString GetCurrentDirectoryProgram ();
TDString GetExecPath ();
unsigned short abs16 (short datas);
// находит адрес начала указанной строки в тексте, индексация идет с 1
char *lpGetAdrLineFromText (char *lpStrInput, unsigned long IndxStr);
// находит первый значимый символ в строке и возвращает его адрес, функция нужна для пропуска пробелов и табуляций
char *lpFindFistActiveCharFromLine (char *lpStrInput);
char *lpFindFistActiveCharFromLineA (char *lpStrInput);
// возвращает адрес начала следующей строки от текущего адреса
char *lpGetNextAdressLine (char *lpStrInput);
// Формирует стринг с текущей позиции char* указателя
bool CreateStringFromLine (TDString &OutStr, char *lpAdr);
void CreateStringFromLine_cnt (TDString &OutStr, char *lpAdr,unsigned long CntBt);
char *CopyStringToRaM (char *lpDest, unsigned int sizes, TDString datsstr);
char *CopyStrTo (TEX::BUFPAR *lpDst, char *lpInStr);
int ConvertStringToInt (char *lpStr);
unsigned char CompareStringS1Z (char *lpRam,char *lpStr2,unsigned int sizes);
unsigned char *ConvertStrToLong (unsigned char *lpRamData,long *lpDataOut);
unsigned char *ConvertStrToULong (unsigned char *lpRamData,unsigned long *lpDataOut);
unsigned char *ConvertStrToLong64 (unsigned char *lpRamData, int64_t *lpDataOut);
unsigned char *ConvertStrToULong64 (unsigned char *lpRamData, uint64_t *lpDataOut);
unsigned char *ConvertStrToLong_Cnt (unsigned char *lpRamData,long *lpDataOut, unsigned char cntdat);
unsigned char *ConvertStrToULong_Cnt (unsigned char *lpRamData,unsigned long *lpDataOut, unsigned char cntdat);
unsigned char *ConvertStrToLong64_Cnt (unsigned char *lInStr, int64_t *lpDataOut, unsigned char cntdat);
unsigned char *ConvertStrToULong64_Cnt (unsigned char *lInStr, uint64_t *lpDataOut, unsigned char cntdat);
bool GetStringFromText (TDString inpStr, TDString &outStr, unsigned long IndxStr);  // извлекает из текста строку с указаным номером
unsigned int GetLenStr (char *lpAdr);
unsigned long GetLenS (char *lpAdr);
#define lenstr(x) GetLenS (x)
TDString Float_To_Str(float datas,unsigned char ndig);
TDString GetShortStringKBytes (unsigned long sizebyte, bool F_str);
unsigned int GetStringSizeChar (char *lpStr);
char *CopyMemorySDC (char *lpSour, char *lpDest, unsigned int sizes);
void MoveDownMemory (char *lpSour, unsigned int sizes, unsigned long BlockSizeDown);    // делает сдвиг памяти для insert операции
unsigned long *CopyMemorySDC_cl (unsigned char *lpSour, unsigned long *lpDest, unsigned int sizes);
unsigned char *CopyMemorySDC_lc (unsigned long *lpSour, unsigned char *lpDest, unsigned int sizes);
bool CompareStrings (char *lpString1,char *lpString2);
char CmpString (char *lpString1,char *lpString2);       // равны= 0, lpString2 > lpString1 = 1, lpString2 < lpString1 = -1 
unsigned char CompareStringS2 (char *lpStr1,char *lpStr2,unsigned int sizes);
char *lpCompareRam (char *lpStrBase, char *lpString, unsigned long sizess);
TDString GetStringWord (TDString Str1, int numbword);
float StrConvToFloat(TDString str1);
void ChangeComaPoint(TDString &str1);
void ChangeComaPoint(TDString &str1);
TDString CutExtFileName (TDString str1);
void ChangeFileName_Ext (TDString &InpStr, TDString ExtName);       // сменить расширение имени файла на новое
char *FindByte (char *lpSearch, char fb);
char *FindChars (char *lpAdr, char datas);
int GetCntNumber (char *lpStr);
char *FindString (char *lpSource,char *lpString,unsigned int sizes);
char CheckDiapazon2ValuesStr (char *lpStrZ, unsigned char MinVal1, unsigned char MaxVal1, unsigned char MinVal2, unsigned char MaxVal2);
float StepenDesiatki (char stpn);
float GetNumberFromString (char *lpStr, char cntb);
float GetXPosHole(char *lpstr, char cntdatdroba);
float GetYPosHole(char *lpstr, char cntdatdroba);
float ExtractDrillDiameter (char *lpstr);
unsigned char ConvertHexVal(unsigned char datas);
unsigned char GetByteHexValue2 (char *lpDatas);
bool Hex2Bin (char *lInHex, char *outbin, unsigned long hexin_sz);
void BinToHex2 (unsigned char *lpRamData, unsigned int sizesdat, TDString &outptsstr);
// меняет расширение в имени файла
void ChangeFileName_Ext (TDString &InpStr, TDString ExtName);
unsigned char *UlongToStr (unsigned char *lpDest,unsigned long datas);
bool UlongToStr_M (unsigned long datas, TDString &strout);
bool LongToStr_M (long datas, TDString &strout);
TDString LongToStr (long datas);
TDString UlongToStr_V (unsigned long datas);
unsigned char *UlongToStr_cnt (unsigned char *lpDest,unsigned long datas,unsigned char cntlens);
void UlongToHex (unsigned char *lpDest, unsigned long datasl);
void UShortToHex (unsigned char *lpDest, unsigned short datasl);
void UShortToHex (TDString &dataout, unsigned short datasl);
unsigned short Hex4ToUShort (char *lpDatas);
void ByteToHEX (unsigned char *lpRams,unsigned char datas);
char IncASCII_DIG (char ValDigAscii);
char DecASCII_DIG (char ValDigAscii);
// подсчитывает количество полей разделенных разделителями до конца строки
unsigned long GetCountStringElementDelim ( TDString Str1, char DelimCode );
unsigned long GetCountStringElementDelim ( char *lpAdrStr, char DelimCode );
// Возвращает количество любых элементов строки разделенных пробелами
unsigned int GetCountStringElement ( TDString Str1 );
// создает стринг с указанной позиции до указанного символа, но не более указанного размера
unsigned long CreateStringToDelim (char *lRam, TDString &outstr, char delim, unsigned long sizes);
// формирует строку по номеру строки извлекая ее из текста
bool GetStringFromText (TDString inpStr, TDString &outStr, unsigned long IndxStr);
TDString CreateSubString (char *lpAdr);       // Формирует стринг с текущей позиции char* указателя, до первого обнаруженого пробела или конца строки
TDString CreateSubString_cor (char *lpAdr);   // Формирует стринг с текущей позиции char* указателя, до конца строки или перевода каретки
// Если, обнаружено совпадение - возвращает адрес строки в конце совпадения стринга, иначе = 0.
TDString FindStringEnd (TDString Str1, TDString Str2);
TDString GetNextString (TDString lpStr,unsigned int ind,unsigned int count);
// формирует строку с текущей позиции памяти до указанного разделителя (анализ ведется до конца строки, перевода каретки или искомого разделителя)
char *lpGetNextStrToDelim (TDString &outstr, char *lpCurRams, char DelimChar);
// удаляет пробелы в конце строки
unsigned long CuteEndString_Z (char *lpRamstr, unsigned long sizess);
// подсчитывает количество слов в строке или извлекает слово по его индекс-номеру, используя указанный разделитель, конец строки 10,13,0 индекс считается от 1
long CreateWordStringFromIndxLine (char *lpLinetxt, TDString &outstr,unsigned long Indxx,char delimc);
long CreateWordStringFromIndxLineAndCF (char *lpLinetxt, TDString &outstr,unsigned long Indxx,char delimc);
// подсчитывает количество слов в строке или извлекает слово по его индекс-номеру
unsigned long CreateWordStringFromIndxLine (TEX::BUFPAR *InBf, TEX::BUFPAR *OutBuf, unsigned long Indxx,char delimc);
// извлекает слово из текста по индексу  (счет с 1)
//bool CreateWordStrFromText_Indx (char *lpRam,TDString &outst, unsigned long IndxFnd);
// формирует строку из текста, находит данные между разделителями, меняет перевод строки на пробелы
bool CreateStringFrom2Delims (char *lpRam, TDString &outstr, char Bdelin, char Edelim);
// последовательно извлекает из текста слова, разделитель слов - пробел
char *CreateWordStrFromText (char *lpRam,TDString &outst);
TDString DeleteCharFromString (TDString str1, char cutdat);
unsigned long ConvertHexString (char *lpDestHx, char *lpInpute, unsigned long maxsz);
TDString GenerateTimeString (char delims);
TDString GenerateDayString (char delims);
unsigned long GetCountPaths (TDString filenamepaths);
unsigned long CreateAllDirectories (TDString fuldirname);
TDString GenerateDecTextLine (char *lpRame, unsigned long sizesbyte);
unsigned char GetByteHexValue2 (char *lpDatas);
void* FillMems (void *lpAdr, char datas, unsigned long sizes);
bool CheckDecValue (TDString strval);
bool CheckFloatValue (TDString strval);
bool CheckHexFormats (char *lpStr, unsigned long sizehex, bool F_Checkmode);
bool CheckHexFormats (TDString strdata, unsigned long sizehex, bool F_Checkmode);
// оцифровывает число до 99 999 999     до 1 милиарда
char StrToFloat (float *lpDest, char *lpTxtIn);
unsigned char *FloatToString (unsigned char *lpRamBuf, double datas,unsigned char pcnt);
bool FloatToStr_M (float DataF, TDString &DstStr, unsigned char DrobSize);
TDString FloatToStr_V (float DataF, unsigned char DrobSize);
//char *FloatToStr(float DataF, TEX::BUFPAR *OutBf, unsigned char DrobSize);
void GetProporcionaleTableValue (unsigned char *lpInProc, unsigned long *lpDestVa, unsigned char TableSizes, unsigned long PropData100);
// кодирует участок памяти в RLE последовательность
unsigned long RLE_Coding_A (TEX::BUFPAR *InRaw, TEX::BUFPAR *OutCode);
unsigned long RLE_Decoding_A (TEX::BUFPAR *InCode, TEX::BUFPAR *OutRaw);
bool subtimeout (unsigned long &tmr, unsigned long dt);

char ChangeRamValueULong (unsigned long *lpRam,unsigned long MaxValue,unsigned long MinValue,unsigned long IncDecValue,unsigned char CmdChg);
char ChangeRamValueUShort (unsigned short *lpRam,unsigned short MaxValue,unsigned short MinValue,unsigned short IncDecValue,unsigned char CmdChg);
char ChangeRamValueUChar (unsigned char *lpRam,unsigned char MaxValue,unsigned char MinValue,unsigned char IncDecValue,unsigned char CmdChg);

unsigned short SwapShort (unsigned short datas);
unsigned long SwapLong (unsigned long datas);
uint64_t SwapI64 (uint64_t datas);
bool GetBitMasiv (TEX::BUFPAR *lInp, unsigned long BitN, bool &GetValue);
bool SetBitMasiv (TEX::BUFPAR *lDst, unsigned long BitN, bool valdat);
unsigned long SerialDecodingData_6bit (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOut);
unsigned long SerialCodingData_6bit  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOutput);
unsigned long SerialDecodingData_7bit (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOut);
unsigned long SerialCodingData_7bit  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOutput);
unsigned long SerialDecodingData_7bitC (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOut);
unsigned long SerialCodingData_7bitC  (TEX::BUFPAR *lpInput, TEX::BUFPAR *lpOutput);

TDString Convert_maToStr (unsigned short mavalue);
TDString Convert_celsium_char (char ctemp);
unsigned long abs32 (long datas);
uint64_t abs64 (int64_t datas);
void I64ToStr (int64_t datas, TDString &dst);
unsigned char *Uint64ToStr (unsigned char *lpDest, uint64_t datas);
unsigned char *I64ToStr (unsigned char *lpDest, int64_t datas);
bool ParseHDR (TDString inp_str, GETHDR *lparse_out);
TDString Header_GetValue (TDString hdr_txt, TDString name_param, char delimtr);
unsigned char *LongToStr (unsigned char *lpDest, long datas);
TDString ToUpperCase (TDString sstr);
TDString ToLowerCase (TDString sstr);

long find_data_array_ul (unsigned long *arr, unsigned long ixcnt, unsigned long data);


// ---- JSON !!!
// ITEM - запись в обрабатываемом обьекте разделенной запятыми

//bool JSON_GetItem_param (TDString JSON_txt, long i_array, TEX::JSONARRITEMPAR *lpJS_par);     // возвращает текстовые параметры расположения записи в обьекте или массиве по индексу
//TDString JSON_GetValue (TDString JSON_txt, TDString JSON_name_param);

//long JSON_FindItemParam_fromOBJ (TDString JSON_txt, TDString nameparam, TDString &outParam);          // ++
//long JSON_ReplaceItemParam_fromOBJ (TDString &JSON_txt, TDString nameparam, TDString newParam);       // ++
//void JSON_CuteCommaStrParam (TDString &JSON_strparam);

//TEX::EJSNTYPE JSON_GetTypeParam (TDString JSON_inp, TDString &outname, TDString &outval);

//TDString JSON_Item_get (TDString JSON_txt, long i_array);    // извлекает запись по индексу
//bool JSON_Item_delete (TDString &JSON_arr_inp,  long i_array);         // удаляет элемент
//bool JSON_Item_replace (TDString &JSON_inp, TDString &JSON_NewItem, long i_array);     // заменяет элемент
//bool JSON_Item_insert (TDString &JSON_txt, const TDString &JSON_insert, long i_array);      // вставляет элемент
//long JSON_Item_add (TDString &JSON_txt, TDString &JSON_insert);                      // добавляет элемент

//long JSON_OBJARR_GetSize (char *lCurRam, char objarr_symb);
//bool FLJSON_Get (TDString const *jsTxt, TDString const *jsonkey, TDString &val);
//bool FLJSON_Set (TDString jsTxt, TDString const *jsonkey, TDString val);


unsigned long GetTagStringDelimIndx (char *lpLinetxt, unsigned long Indxx, char delimc, char **lDest, unsigned long *lPCountField);
bool GetTagStringDelimIndx (TBUFPARAM *lInput, TBUFPARAM *lOutput, unsigned long Indxx, char delimc, unsigned long *lPCountField) ;
bool TxtToFloat (float *lpDest, char *lpTxtIn, unsigned long Sizes);
TDString UTF8To1251 (TDString utf8_str);
TDString F1251ToUTF8 (TDString uc1251_str);
unsigned char W1251ToUTF8b (unsigned char dat, unsigned char dst[2]);
unsigned long GN_CRC32 (void *ldata, unsigned long size);
bool subtimer (long &timerr, unsigned long ms);
unsigned long CreateValueFromBitMassive32out (unsigned char *lInp,  unsigned long CurOffset, unsigned char databitsize);
unsigned long CreateValueFromBitMassive (unsigned char *lInp,  unsigned long CurOffset, unsigned char databitsize);
unsigned char SWAPBits (unsigned char datas);
void CreateValueToBitMassive (void *lpStart, unsigned short d_bits_offset, unsigned long long value_save, unsigned char bits_n);
void SBUSDecodingArrays (unsigned char *lBase, unsigned short Dst[16]);
void SBUSCodingArrays (unsigned char *lBase, unsigned short Src[16]);



#endif

