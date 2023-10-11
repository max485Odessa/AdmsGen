#include "STMSTRING.h"

#ifdef IS_WINDOWS_OS
	using namespace TEX;
#endif
static const unsigned long C_MULTDV_L32_AMOUNT = 10;
static const unsigned long tmpMultData[C_MULTDV_L32_AMOUNT] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};


void TSTMSTRING::set_space (void *lDRam, unsigned long size)
{
    BuferSize = size;
    size_str = 0;
    lpStrRam = (char*)lDRam;
}

                                                               

void TSTMSTRING::clear_preconstructor ()
{
BuferSize = 0;
lpStrRam = 0;
ZeroStr ();
}



TSTMSTRING::TSTMSTRING ()
{
clear_preconstructor ();
}



TSTMSTRING::TSTMSTRING (char *lStr, void *lDRam, unsigned long size)
{
    clear_preconstructor ();
    set_space (lDRam, size);
    AddStringP (lStr);
}



void TSTMSTRING::set_context (void *lDRam, unsigned long size)
{
if (size && lDRam)
    {
    BuferSize = size;
    size_str = size;
    lpStrRam = (char*)lDRam;
    }
}



TSTMSTRING::TSTMSTRING (const char *lstr)
{
clear_preconstructor ();
set_context ((void*)lstr, lenstr ((char*)lstr));
}




unsigned long TSTMSTRING::alloc_space ()
{
return BuferSize;
}



TSTMSTRING::TSTMSTRING (BUFPAR *param)
{
    clear_preconstructor ();
	set_space (param->lRam, param->sizes);
}


TSTMSTRING::TSTMSTRING (void *lDRam, unsigned long size)
{
    clear_preconstructor ();
    set_space (lDRam, size);
}



void TSTMSTRING::ZeroStr ()
{
size_str = 0;
}



TSTMSTRING::TSTMSTRING (char datas, void *lDRam, unsigned long size)
{
    clear_preconstructor ();
   set_space (lDRam, size);
	InsChar (datas);
}



char *TSTMSTRING::c_str(void)
{
if (lpStrRam) lpStrRam[size_str] = 0;
return lpStrRam;
}



void TSTMSTRING::ChangeChars (char schar, char dchar)
{
if (lpStrRam)
    {
    unsigned long ix = 0;
    while (ix < size_str)
        {
        if (lpStrRam[ix] == schar) lpStrRam[ix] = dchar;
        ix++;
        }
    }
}




unsigned long TSTMSTRING::Length ()
{
unsigned long rv = 0;
if (lpStrRam) rv = size_str;
return rv;
}



unsigned long TSTMSTRING::size ()
{
return Length ();
}




void TSTMSTRING::SetStr (char* lpRams)
{

if (lpStrRam && lpRams)
		{
		unsigned long lnstr = lenstr (lpRams);
		size_str = lnstr;
        char *lpDst = CopyMemorySDC (lpRams,lpStrRam,lnstr);
        lpDst[0] = 0;
		}
}



void TSTMSTRING::Insert (char *linp, unsigned long ofs, unsigned long ins_size)
{
if (!lpStrRam) return;
if (linp)
    {
    if (ofs < BuferSize)
        {
        //unsigned long ins_size = GetLenS (linp);
        unsigned long free_sz = BuferSize - size_str;
        if (free_sz > ins_size)
            {
            unsigned long ix_copy_dst = size_str + ins_size;
            unsigned long ix_copy_src = size_str;
            if (ofs < size_str)
                {
                ix_copy_dst = size_str + ins_size;
                ix_copy_src = size_str;
                // сместили вниз
                unsigned long sz = size_str - ofs;
                while (sz)
                    {
                    lpStrRam[ix_copy_dst++] = lpStrRam[ix_copy_src++];
                    sz--;
                    }
                // скопировали
                size_str += ins_size;
                while (ins_size)
                    {
                    lpStrRam[ofs++] = *linp++;
                    ins_size--;
                    }
                }
            }
        }
    }
}



void TSTMSTRING::Delete (unsigned long indx_start, unsigned long cnt)
{
if (!lpStrRam) return;
if (indx_start < size_str && cnt)
    {
    unsigned long indx_stop = indx_start + cnt;
    if (indx_stop > size_str) indx_stop = size_str;
    unsigned long real_move_up = size_str - indx_stop;
    unsigned long real_sub = indx_stop - indx_start;

    unsigned long dst_ix = indx_start;
    unsigned long src_ix = indx_stop;
    while (real_move_up)
        {
        lpStrRam[dst_ix++] = lpStrRam[src_ix++];
        real_move_up--;
        }
    size_str -= real_sub;
    }
}



void TSTMSTRING::Insert ( TSTMSTRING *linp, unsigned long ofs)
{
if (linp) {
    Insert (linp->c_str(), ofs, linp->Length());
    }
}



void TSTMSTRING::Insert (TSTMSTRING &str, unsigned long ofs)
{
Insert (str.c_str(), ofs, str.Length());
}



void TSTMSTRING::AddStringP (char* lpramsadds)
{
if (lpramsadds && lpStrRam)
	{
	unsigned long lnstr = lenstr (lpramsadds);
	if (lnstr)
		{
		char *lpDst = &lpStrRam[size_str];
		unsigned long free_sz = BuferSize - size_str;
		unsigned long copy_size = lnstr;
		if (copy_size > free_sz) copy_size = free_sz;	
		//unsigned long crntsz = size_str + copy_size;
		
		lpDst = CopyMemorySDC (lpramsadds, lpDst, copy_size);
		lpDst[0] = 0;
		size_str += copy_size;
		}
	}
}




void TSTMSTRING::SubString (unsigned long indx, unsigned long sz)
{
if (size_str > indx)
    {
    unsigned long free_sz = size_str - indx;
    if (sz > free_sz) sz = free_sz;
    lpStrRam += indx;
    size_str = sz;
    }
}







void TSTMSTRING::Add_String (const char *lpsrc)
{
AddStringP ((char*)lpsrc);
}



void TSTMSTRING::Add_String (const char *lpsrc, unsigned long sz)
{
while (sz)
    {
    InsChar (*lpsrc++);
    sz--;
    }
}




bool TSTMSTRING::operator==(TSTMSTRING &val)
{
bool rv = false;
if (size_str == val.size_str) {
    if (size_str) {
        rv = str_compare (lpStrRam, val.lpStrRam, size_str);
        }
    else
        {
        rv = true;  // нулевая длина у всех
        }
    }
return rv;
}




bool TSTMSTRING::operator==(const char *lp)
{
bool rv = false;
if (lp)
    {
    unsigned long sz_in = lenstr ((char*)lp);
    if (size_str == sz_in) {
        if (size_str) {
            rv = str_compare (lpStrRam, (char*)lp, size_str);
            }
        }
    }
return rv;
}



bool TSTMSTRING::operator!=(const char *lp)
{
bool rv = false;
if (lp)
    {
    unsigned long sz_in = lenstr ((char*)lp);
    if (size_str == sz_in)
        {
        if (size_str)
            {
            rv = !str_compare (lpStrRam, (char*)lp, size_str);
            }
        }
    else
        {
        rv = true;
        }
    }
else
    {
    rv = true;
    }
return rv;
}



bool TSTMSTRING::operator!=(char *lp)
{
bool rv = false;
if (lp)
    {
    unsigned long sz_in = lenstr ((char*)lp);
    if (size_str == sz_in)
        {
        if (size_str)
            {
            rv = !str_compare (lpStrRam, (char*)lp, size_str);
            }
        }
    else
        {
        rv = true;
        }
    }
else
    {
    rv = true;
    }
return rv;
}



TSTMSTRING TSTMSTRING::operator+(char *lpRams)
{
if (lpStrRam && lpRams) {
			AddStringP (lpRams);
			}
return *this;
}



TSTMSTRING TSTMSTRING::operator+(const char *lpRams)
{
if (lpStrRam && lpRams)
			{
			AddStringP ((char*)lpRams);
			}
return *this;
}



TSTMSTRING TSTMSTRING::operator+=(TSTMSTRING &dt)
{
AddStringP (dt.c_str ());
return *this;
}


TSTMSTRING TSTMSTRING::operator=(TSTMSTRING &dt)
{
set_space (dt.c_str (), dt.Length());
AddStringP (dt.c_str ());
return *this;
}


TSTMSTRING TSTMSTRING::operator+=(char *lpRams)
{
if (lpStrRam && lpRams)
			{
			AddStringP (lpRams);
			}
return *this;
}



TSTMSTRING TSTMSTRING::operator+=(const char *lpRams)
{
if (lpStrRam && lpRams) {
			AddStringP ((char*)lpRams);
			}
return *this;
}

/*
TSTMSTRING& TSTMSTRING::operator=(char *lpRams)
{
SetStr (lpRams);
return *this;
}
*/



TSTMSTRING TSTMSTRING::operator=(const char *lpRams)
{
SetStr ((char*)lpRams);
return *this;
}




TSTMSTRING TSTMSTRING::operator+(char date)
{
InsChar (date);

return *this;
}



void TSTMSTRING::Add (char val_char)
{
InsChar (val_char);
}




void TSTMSTRING::operator+=(char dat)
{
InsChar (dat);
}



TSTMSTRING TSTMSTRING::operator+=(unsigned char dat)
{
InsChar (dat);
return *this;
}





void TSTMSTRING::InsChar (char date)
{
if (lpStrRam)
		{
		unsigned long free_size = BuferSize - size_str;
		if (free_size >= 2)
			{
			lpStrRam[size_str++] = date; 
			lpStrRam[size_str] = 0;
			}
		}
}



TSTMSTRING& TSTMSTRING::operator=(char date)
{
InsChar (date);
return *this;
}


/*
TSTMSTRING::operator char*()
{
   return (char*)lpStrRam;
}
*/



void TSTMSTRING::Insert_Long (long val)
{
unsigned long freem = BuferSize - size_str;
if (freem >= 10)
	{
		unsigned char *end_str = LongToStr ((unsigned char *)&lpStrRam[size_str], val);
		unsigned long inc_sz = end_str - (unsigned char *)&lpStrRam[size_str];
		size_str += inc_sz;
	}
}





TSTMSTRING TSTMSTRING::operator+=( unsigned short date)
{
Insert_ULong (date);
return *this;
}






TSTMSTRING TSTMSTRING::operator+=( long date)
{
Insert_Long (date);
return *this;
}




// три поля: часы, минуты, секунды
void TSTMSTRING::Insert_Time_sek (unsigned long val_sek, char delimiter)
{
	unsigned long hours = val_sek / 3600;
	unsigned long mins = (val_sek % 3600) / 60;
	unsigned long sek = val_sek % 60;
	
	Insert_ULong (hours); InsChar (delimiter);
	Insert_ULong (mins); InsChar (delimiter);
	Insert_ULong (sek); //InsChar (delimiter);
	
}



// четыре поля: часы, минуты, секунды, милисекунды
void TSTMSTRING::Insert_Time_ms (unsigned long val, char delimiter)
{
	unsigned long val_sek = val / 1000;
	unsigned long ms_val = val % 1000;
	
	Insert_Time_sek (val_sek, delimiter);
	InsChar (delimiter);
	Insert_ULong (ms_val / 100);
}




void TSTMSTRING::Insert_Float (float val, unsigned char dec_n)
{
unsigned long freem = BuferSize - size_str;
if (freem >= 12)
	{
    char *end_str = (char *)FloatToString ((unsigned char *)&lpStrRam[size_str], val, dec_n);
    unsigned long inc_sz =  end_str - (char *)&lpStrRam[size_str];
    size_str += inc_sz;
	}
}



TSTMSTRING TSTMSTRING::operator+=(unsigned long date)
{
Insert_ULong (date);
return *this;
}



void TSTMSTRING::Insert_ULong (unsigned long val)
{
unsigned long freem = BuferSize - size_str;
if (freem >= 10)
	{
		unsigned char *end_str = UlongToStr ((unsigned char *)&lpStrRam[size_str], val);
		unsigned long inc_sz = end_str - (unsigned char *)&lpStrRam[size_str];
		size_str += inc_sz;
	}
}




void TSTMSTRING::Add_ULong (unsigned long val)
{
	Insert_ULong (val);
}



void TSTMSTRING::Insert_Binary_L (unsigned long val, unsigned char bit_cnt)
{
	char dat;
	unsigned long c_mask = 1;
	while (bit_cnt)
		{
		if (val & c_mask)
			{
			dat = '1';
			}
		else
			{
			dat = '0';
			}
		InsChar (dat);
		bit_cnt--;
		c_mask <<= 1;
		}
}




void TSTMSTRING::Insert_Hex_T (void *lsrc, unsigned short sz)
{
	char *lSrc = (char*)lsrc;
	while (sz)
		{
		Insert_Hex_C (*lSrc++);
		sz--;
		}
}


void TSTMSTRING::Insert_Hex_C (char dat)
{
	unsigned char buf[3];
	ByteToHEX (buf, dat);
	buf[2] = 0;
	Add_String ((const char *)buf);
}



void TSTMSTRING::Insert_Hex_S (unsigned short dat)
{
	Insert_Hex_C (dat >> 8);
	Insert_Hex_C (dat);
}



void TSTMSTRING::Insert_Hex_L (unsigned long dat)
{
	Insert_Hex_C (dat >> 24);
	Insert_Hex_C (dat >> 16);
	Insert_Hex_C (dat >> 8);
	Insert_Hex_C (dat);
}



bool TSTMSTRING::ToLong (long &ul_dat)
{
bool rv = false;
if (ConvertStrToLong ((unsigned char*)lpStrRam, &ul_dat)) rv = true;
return rv;
}



bool TSTMSTRING::ToULong (uint32_t &ul_dat)
{
bool rv = false;
if (ConvertStrToULong ((unsigned char*)lpStrRam, &ul_dat)) rv = true;
return rv;
}



bool TSTMSTRING::ToFloat (float &f_dat)
{
bool rv = false;
if (size_str) rv = TxtToFloat (&f_dat, lpStrRam, size_str);
return rv;
}





// Возвращает указатель вхождения на нужный стринг по указаному индексу, возвращает размер текстового поля по этому индексу.
// Можно вычислить количество полей записи указав большой индекс поиска, количество полей возвращается по указателю lPCountField,
bool TSTMSTRING::getcomastring_indx (TSTMSTRING *lOutput, unsigned long Indxx, char delimc, unsigned long *lPCountField)
{
bool rv = false;
if (size_str && lpStrRam)
    {
    char *lpLinetxt = (char*)lpStrRam;
    unsigned long sz = size_str;

		unsigned long size = 0;
		unsigned long lcinx = 0;
		unsigned long countfield = 0;
		char *lFistInp = 0;
		char tmpb;
		bool f_fist_input = false;
		while (sz)
				{
				tmpb = lpLinetxt[0];
				if (tmpb == 9) tmpb = ' ';
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
		if (lOutput) {
            lOutput->Add_String ((const char *)lFistInp, size);
			}

    }
return rv;
}



bool TSTMSTRING::hex1bin (uint8_t dat, uint8_t &rslt)
{
bool rv = false;
if (dat >= 'A' && dat <= 'F')
    {
    rslt = dat - 'A';
    rslt += 10;
    rv = true;
    }
else
    {
    if (dat >= 'a' && dat <= 'f')
        {
        rslt = dat - 'a';
        rslt += 10;
        rv = true;
        }
    else
        {
        if (dat >= '0' && dat <= '9')
            {
            rslt = dat - '0';
            rv = true;
            }
        }
    }
return rv;
}



bool TSTMSTRING::HexToUint64 (uint64_t &ul_dat)
{
bool rv = false;
if (size_str && lpStrRam)
    {
    if (size_str <= 16)
        {
        uint64_t mult = 1, datrslt, rslt64 = 0;
        uint8_t cnt = size_str, rslt8;
        char *scrch = lpStrRam;
        //uint8_t dat;
        while (cnt)
            {
            mult = 1 << ((cnt - 1) * 4);
            if (!hex1bin (*scrch++, rslt8)) return rv;
            datrslt = rslt8; datrslt *= mult; rslt64 += datrslt;
            cnt--;
            }
        ul_dat = rslt64;
        rv = true;
        }
    }
return rv;
}




bool TSTMSTRING::getstring_indx (TSTMSTRING *lOutput, unsigned long Indxx, unsigned long *lPCountField)
{
bool rv = false;
if (size_str && lpStrRam)
    {
    char *lpLinetxt = (char*)lpStrRam;
    unsigned long sz = size_str;

    unsigned long lcinx = 0;
    char tmpb;
    bool f_in_str = false;  // is no into string
    bool f_forming = false; // is no create string
    if (lOutput) lOutput->ZeroStr();

		while (sz)
				{
				tmpb = lpLinetxt[0];
                if (!tmpb || tmpb == 13 || tmpb == 10)
                    {
                    if (f_forming) rv = true;
                    if (f_in_str) lcinx++;
                    break;
                    }
                if (tmpb == ' ')
                    {
                    if (f_in_str) lcinx++;
                    if (f_forming)
                        {
                        rv = true;
                        break;
                        }
                    f_in_str = false;   // вышли со строки
                    }
                else
                    {
                    if (!f_in_str)
                        {
                        if (lcinx == Indxx) f_forming = true;
                        f_in_str = true;
                        }
                    }
                if (f_forming)
                    {
                    if (lOutput) lOutput->Add (tmpb);
                    }

				lpLinetxt++;
				sz--;
				}
        if (!sz)
            {
            if (f_forming) rv = true;
            if (f_in_str) lcinx++;
            }

        if (lPCountField) *lPCountField = lcinx;

    }
return rv;
}



char *TSTMSTRING::SkipBlank (char *lsrc, uint32_t maxsz)
{
while (maxsz)
    {
    if (*lsrc != ' ') break;
    lsrc++;
    maxsz--;
    }
return lsrc;
}


unsigned char *TSTMSTRING::FloatToString (unsigned char *lpRamBuf, float datas, unsigned char pcnt)
{
	unsigned char *lpEnd = 0;
	float DrobOstat;
	unsigned long Celie;
    lpRamBuf = (unsigned char *)SkipBlank ((char*)lpRamBuf, 16);
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
            if (pcnt > 6) pcnt = 6;
            DrobOstat = datas - (float)Celie;

            lpEnd[0] = '.';
            lpEnd++;
            lpEnd = FloatToStrDroba (lpEnd,DrobOstat,pcnt);
            }
        lpEnd[0]=0;
        }
return lpEnd;
}


unsigned char *TSTMSTRING::UlongToStr (unsigned char *lpDest, uint32_t datas)
{
unsigned char flagnz=0, resd;
uint32_t delmt = 1000000000;
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



unsigned char *TSTMSTRING::FloatToStrDroba (unsigned char *lpDest,float datas,unsigned char Cntr)
{
unsigned char resd;
float delmt=0.1;
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




char *TSTMSTRING::CopyMemorySDC (char *lpSour, char *lpDest, unsigned int sizes)
{
if (lpSour && lpDest && sizes) {
    while (sizes) {
        *lpDest++ = *lpSour++;
        sizes--;
        }
    }
return lpDest;
}



bool TSTMSTRING::str_compare (char *lStr1, char *lStr2, uint32_t size)
{
	bool rv = false;
	if (lStr1 && lStr2 && size)
		{
		rv = true;
		while (size)
			{
			if (*lStr1 != *lStr2)
				{
				rv = false;
				break;
				}
			lStr1++;
			lStr2++;
			size--;
			}
		}
	return rv;
}



uint32_t TSTMSTRING::lenstr (char *lpAdr)
{
uint32_t rv = 0;
if (lpAdr)
    {
    while (true)
        {
        if (*lpAdr++ == 0) break;
        rv++;
        }
    }
return rv;
}



uint32_t TSTMSTRING::lenstr_max (const char *lsrt, uint32_t maxsz)
{
	uint32_t sz = 0;
	if (lsrt)
		{
		while (maxsz)
			{
			if (!*lsrt++) break;
			sz++;	
			maxsz--;
			}
		}
return sz;
}



unsigned char *TSTMSTRING::LongToStr (unsigned char *lpDest, long datas)
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



char TSTMSTRING::ConvBinToASCIIHex (unsigned char datas)
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


void TSTMSTRING::ByteToHEX (unsigned char *lpRams,unsigned char datas)
{
lpRams[0] = (ConvBinToASCIIHex(datas>>4));
lpRams[1] = (ConvBinToASCIIHex(datas));
}



// преобразует данные из текста по указаному адресу в unsigned long число
// если число больше чем 10 знакомест или его нет в первом байте указанного адресса, возвращает 0 - (ошибка)
unsigned char *TSTMSTRING::ConvertStrToLong (unsigned char *lpRamData, long *lpDataOut)
{
unsigned char *lprv = 0;
bool f_subzero = false;
if (lpRamData)
    {
    lpRamData = (unsigned char*)SkipBlank ((char*)lpRamData, 16);
    lprv = lpRamData;
    if (*lprv == '-')
        {
        lprv++;
        f_subzero = true;
        }
    uint32_t udata;
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






// преобразует данные из текста по указаному адресу в unsigned long число
// если число больше чем 10 знакомест или его нет в первом байте указанного адресса, возвращает 0 - (ошибка)
unsigned char *TSTMSTRING::ConvertStrToULong (unsigned char *lpRamData, uint32_t *lpDataOut)
{
unsigned char *lprv = 0;
if (lpRamData)
        {
        // найти конец цифровой строки
        unsigned char dtasd;
        uint32_t cnterN = 0;
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



bool TSTMSTRING::TxtToFloat (float *lpDest, char *lpTxtIn, uint32_t Sizes)
{
bool rv = false;
if (lpDest && lpTxtIn)
	{
	if (!Sizes) Sizes = lenstr (lpTxtIn);
	if (Sizes && Sizes < 32)
		{
		float Datf = 0;
		uint32_t NLongDroba = 0;
		uint32_t NLongCel = 0;
		uint32_t len_drb = 0,len_cel = 0;
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
                  if (!TxtToULong ((unsigned char*)(lpTxtIn + len_cel + sizeof('.')), len_drb, &NLongDroba)) return rv;
                  }
              if (!TxtToULong ((unsigned char*)lpTxtIn, len_cel, &NLongCel)) return rv;
              Datf = NLongCel;
              if (len_drb)
                  {
                  float mult = ((float)(1.0)) / tmpMultData[len_drb];
                  float drba = NLongDroba;
                  Datf = Datf + (drba * mult) + (float)0.0000005;
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



unsigned long TSTMSTRING::GetSizeToDelim (char *lTxt, unsigned long sz, char delim)
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
bool TSTMSTRING::TxtToULong (unsigned char *lpRamData, unsigned char sz, uint32_t *lpDataOut)
{
bool rv = false;
if (lpRamData)
	{

	unsigned long Sizes = sz;
	if (!Sizes) Sizes = lenstr ((char*)lpRamData);
	if (CheckDecimal ((char*)lpRamData, Sizes) == Sizes)
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


uint32_t TSTMSTRING::abs32 (long datas)
{
uint32_t rv=(uint32_t)datas;
if (rv>=0x80000000)
	{
	rv=(rv ^ 0xFFFFFFFF)+1;
	}
return rv;
}


uint32_t TSTMSTRING::CheckDecimal (char *lTxt, uint32_t sz)
{
uint32_t rv = 0;
if (lTxt && sz)
	{
	unsigned char datt;
	if (!sz) sz = lenstr (lTxt);
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


/*
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
*/










