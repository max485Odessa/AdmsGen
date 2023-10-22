#ifndef _H_STM32_STRING_H_
#define _H_STM32_STRING_H_



#include "stdint.h"
#ifdef IS_WINDOWS_OS
    #include "textrut.hpp"
		using namespace TEX;
#else
		#include "rutine.h"
#endif



class TSTMSTRING {
private:
      static unsigned char *FloatToStrDroba (unsigned char *lpDest,float datas,unsigned char Cntr);
	    static unsigned long GetSizeToDelim (char *lTxt, unsigned long sz, char delim);
        
protected:

        char *lpStrRam;
        unsigned long BuferSize;
        unsigned long size_str;

        void AddStringP (char* lpramsadds);
        void SetStr (char* lpramsadds);
        void InsChar (char datas);
        void ZeroStr ();
        void clear_preconstructor ();

public:

        TSTMSTRING ();

        explicit TSTMSTRING (char *lStr, void *lDRam, unsigned long size);
        explicit TSTMSTRING (void *lDRam, unsigned long size);
        explicit TSTMSTRING (const char *lstr);
        explicit TSTMSTRING (char datas, void *lDRam, unsigned long size);
        explicit TSTMSTRING (BUFPAR *param);

        void set_space (void *lDRam, unsigned long size);
        void set_context (void *lDRam, unsigned long size);

        TSTMSTRING operator+(char dt);
        TSTMSTRING operator+(char *lpRams);
        TSTMSTRING operator+(const char *lpRams);
        TSTMSTRING operator+=(char *lpRams);
        TSTMSTRING operator+=(const char *lpRams);
        bool operator!=(char *lpRams);
        bool operator!=(const char *lpRams);
        TSTMSTRING operator+=(long val);
        TSTMSTRING operator+=(unsigned long val);
        bool operator==(TSTMSTRING &val);
        bool operator==(const char *lp);
        //bool operator!=(const char *lp);
        //bool operator!=(char *lp);
        TSTMSTRING operator+=(short val);
        TSTMSTRING operator+=(unsigned short val);
        TSTMSTRING operator+=(unsigned char val);

        TSTMSTRING operator=(const char *lpRams);
        TSTMSTRING& operator=(char dt);
        TSTMSTRING operator=(TSTMSTRING &dt);
        TSTMSTRING operator+=(TSTMSTRING &dt);

        void operator+=(char val);
        void Insert_Binary_L (unsigned long val, unsigned char bit_cnt);
        void Insert_Long (long val);
        void Insert_ULong (unsigned long val);
        void Add_ULong (unsigned long val);
        void Add (char val_char);
        void Insert_Time_sek (unsigned long val, char delimiter);
        void Insert_Time_ms (unsigned long val, char delimiter);
        void Insert_Float (float val, unsigned char dec_n);
        void Insert_Hex_T (void *lsrc, unsigned short sz);
        void Insert_Hex_C (char dat);
        void Insert_Hex_S (unsigned short dat);
        void Insert_Hex_L (unsigned long dat);
        void Insert (char *linp, unsigned long ofs, unsigned long ins_size);
        void Insert (TSTMSTRING *linp, unsigned long ofs);
        void Insert (TSTMSTRING &str, unsigned long ofs);
        void Delete (unsigned long indx, unsigned long cnt);
        void Add_String (const char *lpsrc);
        void Add_String (const char *lpsrc, unsigned long sz);
        unsigned long alloc_space ();
        void SubString (unsigned long indx, unsigned long sz);

        void ChangeChars (char schar, char dchar);

        bool ToLong (long &ul_dat);
        bool ToULong (uint32_t &ul_dat);
        bool ToFloat (float &f_dat);
        bool HexToUint64 (uint64_t &ul_dat);

        bool getcomastring_indx (TSTMSTRING *lOutput, unsigned long Indxx, char delimc, unsigned long *lPCountField);
        bool getstring_indx (TSTMSTRING *lOutput, unsigned long Indxx, unsigned long *lPCountField);

        char *c_str(void);
        unsigned long Length ();
        unsigned long size ();

        static bool hex1bin (uint8_t dat, uint8_t &rslt);
        static char *SkipBlank (char *lsrc, uint32_t maxsz);
        static unsigned char *FloatToString (unsigned char *lpRamBuf, float datas, unsigned char pcnt);
        static unsigned char *UlongToStr (unsigned char *lpDest, uint32_t datas);
        static char *CopyMemorySDC (char *lpSour, char *lpDest, uint32_t sizes);
        static bool str_compare (char *lStr1, char *lStr2, uint32_t size);
        static uint32_t lenstr (char *lpAdr);
        static unsigned char *LongToStr (unsigned char *lpDest, long datas);
        static char ConvBinToASCIIHex (unsigned char datas);
        static void ByteToHEX (unsigned char *lpRams,unsigned char datas);
        static unsigned char *ConvertStrToLong (unsigned char *lpRamData, long *lpDataOut);
        static unsigned char *ConvertStrToULong (unsigned char *lpRamData, uint32_t *lpDataOut);
        static bool TxtToFloat (float *lpDest, char *lpTxtIn, uint32_t Sizes);
        static bool TxtToULong (unsigned char *lpRamData, unsigned char sz, uint32_t *lpDataOut);
        static uint32_t abs32 (long datas);
        static uint32_t CheckDecimal (char *lTxt, uint32_t sz);
				static uint32_t lenstr_max (const char *lsrt, uint32_t maxsz);
        //operator  char*() ;
};

#define TString(name,x) \
    char buf[x];\
    TSTMSTRING name(buf,sizeof(buf));

extern TSTMSTRING operator+( char* str, const TSTMSTRING& tsStr);



#endif

