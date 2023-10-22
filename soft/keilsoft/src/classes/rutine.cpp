#include "rutine.h"
#include "stddef.h"
#include "stdint.h"

static const uint8_t mask8[8] = {1,3,7,15,31,63,127,255};
const static unsigned long tmpMultData[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
extern uint32_t SystemCoreClock; 


void HardwareReset ()
{
typedef void (*cb_reset) (void);	
	cb_reset reset = 0;
	reset ();
}


uint32_t GN_CRC32 (void *ldata, uint32_t size)
{
const uint32_t one_sig = 0x724169A3;
const uint32_t zero_sig = 0x28181277;
const uint8_t mask_sig = 0x18;
uint32_t crc = 0;
if (ldata && size)
    {
    uint8_t *src = (uint8_t*)ldata;
    uint8_t dat;
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


static const uint8_t C_8BIT_POL = 0x41;
static const uint32_t C_BITMASK_HB = 0x80000000;
uint32_t CalcCRC32Data (uint8_t *lSrc, uint32_t siz)
{
	uint32_t rv = 0;
	uint8_t dat;
	uint32_t z_cnt = 0;
	while (siz)
		{
		dat = *lSrc;
		if (!dat)
			{
			rv += siz;
			z_cnt++;
			}
		else
			{
			if (dat & C_8BIT_POL)
				{
				rv += 0x24;
				}
			else
				{
				rv += 0x33;
				}
			}
		if (rv & C_BITMASK_HB)
			{
			rv <<= 1;
			rv |= 1;
			}
		else
			{
			rv <<= 1;
			}
		rv += dat;
		lSrc++;
		siz--;
		}
	rv += z_cnt;
return rv;
}


unsigned short SwapShort (unsigned short dat)
{
	unsigned char hibyte = (dat & 0xff00) >> 8;
	unsigned char lobyte = (dat & 0xff);
	unsigned short rv = lobyte;	rv = rv << 8; rv |= hibyte;
	return rv;
}



void *fillmem (void *lDst, unsigned char dat, long sizes)
{
char *lRam = (char*)lDst;
while (sizes)
	{
	*lRam = dat;
	lRam++;
	sizes--;
	}
return lRam;
}



unsigned long SwapLong (unsigned long dat)
{
	char *lSrc = (char*)&dat;
	lSrc += 3;
	unsigned long rv = 0;
	char *lDst = (char*)&rv;
	unsigned char cnt4 = 4;
	while (cnt4)
		{
		lDst[0] = lSrc[0];
		lSrc--;
		lDst++;
		cnt4--;
		}
	
	return rv;
}



char *CopyMemorySDC (char *lpSour, char *lpDest, unsigned int sizes)
{
if (lpSour && lpDest && sizes)
    {
    while (sizes)
        {
        lpDest[0]=lpSour[0];
        lpDest++;
        lpSour++;
        sizes--;
        }
    }
return lpDest;
}



unsigned long lenstr (const char *lsrt)
{
	unsigned long sz = 0;
	char dat;
	while (true)
		{
		dat = lsrt[0];
		if (!dat) break;
		sz++;	
		lsrt++;
		}
return sz;
}



void *memset (void *lDst, unsigned char dat, long sizes)
{
char *lRam = (char*)lDst;
while (sizes)
	{
	*lRam = dat;
	lRam++;
	sizes--;
	}
return lRam;
}



long strncmp( const char * string1, const char * string2, long num)
{
	long rv = 0;
	char d1, d2;
	if (num > 0)
		{
		while (num)
			{
				d1 = string1[0];
				d2 = string2[0];
			if (d1 != d2)
				{
				if (d1 > d2)
					{
					rv = 1;
					}
				else
					{
					rv = -1;
					}
				break;
				}
			string1++;
			string2++;
			num--;
			}
		}
	return rv;
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



uint8_t bitmask_lsb (uint8_t vi)
{
	uint8_t rv = 0;
	if (vi < 8) rv = mask8[vi];
	return rv;
}



long rounding_lsb (long val, uint8_t r)
{
long rv = val;
if (r && r < 8)
	{
	bool f_znak_minus = false;
	if (val < 0) f_znak_minus = true;
	uint32_t nz_val = abs32 (val);
	if (r == 1)
		{
		nz_val &=0x7FFFFFFE;
		}
	else
		{
		uint16_t smth_mask_lsb = bitmask_lsb (r);
		uint32_t smth_mask_half_lsb = smth_mask_lsb / 2;
		uint32_t smth_data = nz_val & smth_mask_lsb;
		nz_val &= (0x7FFFFFFF ^ smth_mask_lsb);		// lsb to zero
		smth_mask_lsb++;		// smooth inc value
		if (smth_data > smth_mask_half_lsb) 
			{
			nz_val += smth_mask_lsb;
			if (nz_val & 0x80000000) return val;
			}
		}
	rv = nz_val;
	if (f_znak_minus) rv *= -1;
	}
return rv;
}



bool str_compare (char *lStr1, char *lStr2, unsigned long size)
{
	bool rv = false;
	if (size)
		{
		rv = true;
		while (size)
			{
			if (lStr1[0] != lStr2[0])
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



unsigned long str_size (const char *lsrt)
{
	unsigned long sz = 0;
	char dat;
	while (true)
		{
		dat = lsrt[0];
		if (!dat) break;
		sz++;	
		lsrt++;
		}
return sz;
}


/*
void *memcpy ( void * destination, const void * source, long num )
{
	char *lDst = (char*)destination, *lSrc = (char*)source;
while (num)
	{
	*lDst++ = lSrc[0];
	lSrc++;
	num--;
	}
return lDst;
}
*/

static unsigned long GetOffsetDay (long y, long m, long d)
{
    static const long yearNotLeap[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    static const long yearIsLeap[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    long days = 0;
    unsigned long rv = 0;

    if (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))
			{
			for (int i = 0; i < m - 1; i++)
				{
				days += yearIsLeap[i];
				}
			}
    else
			{
        for (int i = 0; i < m - 1; i++)
					{
					days += yearNotLeap[i];
					}
			}
    if (m == 1)
        {
        rv = d;
        }
    else
        {
        rv = days + d;
        }
    return rv;
}


static unsigned long GetDayDeltaToYear1980 (unsigned long y_new)
{
unsigned long rv = 0;
unsigned long delta_y = y_new - 1980 - 1;
if (delta_y >= 4) rv = 1461 * (delta_y / 4);    // дней в 4-рех годовом цикле
unsigned long ofs = (delta_y % 4) * 365;
rv += ofs;
return rv;
}



unsigned long GetUTCDaysToDate (long Yr, long Mn, long Days)
{
    unsigned long rv = GetOffsetDay (Yr, Mn, Days);
    rv = rv + GetDayDeltaToYear1980 (Yr);
    return rv;
}





unsigned long HAL_RCC_GetPCLK1Freq ()
{
	return SystemCoreClock / 2;
}



unsigned long GetCountStringElementDelim ( char *lpAdrStr, char DelimCode )
{
unsigned long rv=0;
if (lpAdrStr)
	{
	char datas;
	bool F_dat = false;
  while (true)
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



// Возвращает указатель вхождения на нужный стринг по указаному индексу, возвращает размер текстового поля по этому индексу.
// Можно вычислить количество полей записи указав большой индекс поиска, количество полей возвращается по указателю lPCountField,
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





unsigned long CheckDecimal (char *lTxt, unsigned long sz)
{
unsigned long rv = 0;
if (lTxt && sz)
	{
	unsigned char datt;
		if (!sz) sz = str_size (lTxt);
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




bool TxtToULong (char *lpRamData, unsigned char sz, unsigned long *lpDataOut)
{
bool rv = false;
if (lpRamData)
	{
	unsigned long Sizes = sz;
	if (!Sizes) Sizes = str_size ((char*)lpRamData);
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



bool TxtToFloat (float *lpDest, char *lpTxtIn, unsigned long Sizes)
{
bool rv = false;
if (lpDest && lpTxtIn)
	{
	if (!Sizes) Sizes = str_size (lpTxtIn);
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



unsigned long long CreateValueFromBitMassive64out (unsigned char *lInp, unsigned long MaxMasive, unsigned long CurOffset, unsigned char databitsize)
{
unsigned long rv = 0;
if (lInp && MaxMasive > CurOffset && databitsize && databitsize <= 64)
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



static unsigned char *FloatToStrDroba (unsigned char *lpDest,float datas,unsigned char Cntr)
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




char *FloatToStr (char *lDst, float val, unsigned char dig_n)
{
char *lrv = lDst;
if (lDst)
	{
	long Celie = val;
	lrv = (char *)LongToStr ((unsigned char*)lDst, Celie);
	
	if (dig_n)
		{
		if (dig_n > 6) dig_n = 6;
		float DrobOstat = val - Celie;

		lrv[0] = '.';
		lrv++;
		lrv = (char *)FloatToStrDroba ((unsigned char *)lrv, DrobOstat, dig_n);
		}
	}
return lrv;
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



char *FloatToStr(float DataF, BUFPAR *OutBf, unsigned char DrobSize)
{
char *lpRv = 0;
if (OutBf && OutBf->lRam && OutBf->sizes >= 2)
	{
	unsigned long dstsz = OutBf->sizes;
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
	char *lpOut = (char*)OutBf->lRam;
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
			((char*)OutBf->lRam)[(OutBf->sizes-1)] = 0;
			}
		}
	lpRv = lpOut;
	}
return lpRv;
}




static unsigned char HexCharToBin (unsigned char datas)
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



unsigned char ConvertHex2Val (unsigned char *lpRams)
{
unsigned char rv = (HexCharToBin(lpRams[0]) << 4);
rv = rv | HexCharToBin(lpRams[1]);
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
		if (!bit_msk_output) bit_msk_output = 0x80000000;
		databitsize--;
		}
	}
return rv;
}



unsigned long SwapBitsLong (unsigned long val, unsigned char bitsize)
{
unsigned long rv = 0;
if (bitsize)
	{
	if (bitsize == 1)
		{
		rv = val;
		}
	else
		{
		unsigned long dst_mask = 1;
		unsigned long src_mask = 1;
		src_mask = src_mask << (bitsize - 1);
		while (bitsize)
			{
			if (val & src_mask) rv |= dst_mask;
			dst_mask <<= 1;
			src_mask >>= 1;
			bitsize--;
			}
		}
	}
return rv;
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



void BitInverse_ULong (unsigned long &data_dword, unsigned char b_ofs)
{
unsigned long B_mask = (unsigned long)1 << b_ofs;
if (data_dword & B_mask)
	{
	data_dword &= (~B_mask);
	}
else
	{
	data_dword |= B_mask;
	}
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



unsigned char ConvertHexVal(unsigned char datas)
{
unsigned char rv=0;
do
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
    } while (false);
return rv;
}

bool CheckHexVal (unsigned char datas)
{
bool rv = 0;
do
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
        } while (false);
return rv;
}


unsigned char ConvBinToASCIIHex (unsigned char datas)
{
unsigned char rv;
datas &= 0x0F;
if (datas >= 10)
   {
   datas -= 10;
   rv = 'A';
   }
else
   {
   rv = '0';
   }
rv = rv + datas;
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





void UlongToHex (unsigned char *lpDest, unsigned long datasl)
{
UDToHex_tmp (lpDest, datasl, 8);
}



// Возвращает указатель вхождения на нужный стринг по указаному индексу, возвращает размер текстового поля по этому индексу.
// Можно вычислить количество полей записи указав большой индекс поиска, количество полей возвращается по указателю lPCountField,
bool GetTagStringDelimIndx (BUFPAR *lInput, BUFPAR *lOutput, unsigned long Indxx, char delimc, unsigned long *lPCountField) // char **lDest,
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
			lOutput->lRam = (uint8_t*)lFistInp;
			lOutput->sizes = size;
			}
		}
return rv;
}





// char delimc
unsigned long GetCRLFStringDelimIndx (char *lpLinetxt, unsigned long Indxx, char **lDest, unsigned long *lPCountField)
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
				//if (tmpb == 9) tmpb = 32;
				if (tmpb != '\r')	// пропустить 0x0D
					{
					if (!tmpb)
						{
						if (Indxx != lcinx) f_fist_input = false;
						break;
						}
					else
						{
						if (tmpb == '\n')
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




namespace RLECOD_A{

    const unsigned char CMDMSK = 192;
    const unsigned char ZERO = 0;
    const unsigned char RLE = 64;
    const unsigned char RAW = 128;
    const unsigned char ONES = CMDMSK;
    const unsigned char CNTRMASK = 63;
};





unsigned long RLE_Coding_A (BUFPAR *InRaw, BUFPAR *OutCode)
{
unsigned long rv_cnt = 0;
if (InRaw && OutCode && InRaw->lRam && OutCode->lRam)
    {
    //unsigned long MaxOutBuf = OutCode->Sizes;
    unsigned long MaxInBuf = InRaw->sizes;
    unsigned char *lpInp = (unsigned char*)InRaw->lRam;
    unsigned char *lpOutp = (unsigned char*)OutCode->lRam;
    if (MaxInBuf)
        {
        unsigned char Dt;			// Dt_prev
        unsigned char cntr;
        unsigned char cntcod;
        while (MaxInBuf)
            {
            if (!MaxInBuf) break;
            Dt = lpInp[0];
            cntr = GetNoRleCnt (lpInp,MaxInBuf);   
            if (cntr)
                {
                cntcod = cntr - 1;
                if (cntr == 1)  
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
                    lpOutp = (unsigned char*)CopyMemorySDC ((char*)lpInp, (char*)lpOutp, cntr);
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





unsigned long RLE_Decoding_A (BUFPAR *InCode, BUFPAR *OutRaw)
{
unsigned long rv_cnt = 0;
if (InCode && OutRaw)
    {
    unsigned long MaxOutBuf = OutRaw->sizes;
    unsigned long MaxInBuf = InCode->sizes;
    unsigned char *lpInp = (unsigned char*)InCode->lRam;
    unsigned char *lpOutp = (unsigned char*)OutRaw->lRam;
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
                lpOutp = (unsigned char*)fillmem ((char*)lpOutp,Dt_rep,maxfill);
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


bool GetCRLFStringDelimIndx_HTTP (BUFPAR *lInput, BUFPAR *lOutput, unsigned long Indxx, unsigned long *lPCountField)  // char **lDest,
{
bool rv = false;
if (lInput)
	{
	char *lpLinetxt = (char*)lInput->lRam;
	unsigned long sz_exn = lInput->sizes;
	if (lpLinetxt)
		{
		unsigned long size = 0;
		unsigned long lcinx = 0;
		unsigned long countfield = 0;
		char *lFistInp = 0;
		char tmpb;
		bool f_fist_input = false;
		while (sz_exn)
				{
				tmpb = lpLinetxt[0];
				//if (tmpb == 9) tmpb = 32;
				if (tmpb != '\r')	// пропустить 0x0D
					{
					if (!tmpb)
						{
						if (Indxx != lcinx) f_fist_input = false;
						break;
						}
					else
						{
						if (tmpb == '\n')
							{
							countfield++;
							if (Indxx == lcinx) 
								{
								rv = true;
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
					}
				lpLinetxt++;
				sz_exn--;
				}
		if (!sz_exn)
			{
			countfield++;
			if (Indxx == lcinx) rv = true;
			}
		if (!f_fist_input)
			{
			size = 0;
			lFistInp = 0;
			}
		if (lOutput) 
			{
			lOutput->lRam = ( uint8_t*)lFistInp;
			lOutput->sizes = size;
			}
		if (lPCountField) *lPCountField = countfield;
		rv = size;     
		}
	}
return rv;
}



char *SkipStartChars (char *ltxt, unsigned short max_sz)
{
	char *lrv;
	char dat;
	while (max_sz)
		{
		dat = *ltxt;
		if (dat > ' ') break;
		ltxt++;
		max_sz--;
		}
	lrv = ltxt;
	return lrv;
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



void CreateValueToBitMassive_R (void *lpStart, unsigned short d_bits_offset, unsigned long long value_save, unsigned char bits_n)
{
if (lpStart)
    {
    char *lDst = (char*)lpStart;
    // формируем байтовое смещение и выходную битовую маску
	unsigned long byte_ofs = d_bits_offset / 8;
	unsigned char bit_msk_output = 1;
    bit_msk_output = bit_msk_output << (d_bits_offset % 8);

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
						
						
        bit_msk_output = bit_msk_output << 1;
        if (!bit_msk_output)
            {
            bit_msk_output = 1;
            byte_ofs++;
            }
        bit_msk_input = bit_msk_input >> 1;
        bits_n--;
        }
    }
}



uint16_t crc_xmodem_update(uint16_t crc, uint8_t data)
{
	crc = crc ^ ((uint16_t)data << 8);
	for (uint16_t i=0; i<8; i++)
	{
		if(crc & 0x8000) {
			crc = (crc << 1) ^ 0x1021;
		} else {
			crc <<= 1;
        }
	}

	return crc;
}

uint16_t crc_xmodem(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0;
    for (uint16_t i=0; i<len; i++) {
        crc = crc_xmodem_update(crc, data[i]);
    }
    return crc;
}



float radians(float deg)
{
    return deg * DEG_TO_RAD;
}



void WriteBitArray (void *base, unsigned long bit_ofs, unsigned char bitsz, void *s)
{
	unsigned char *dst = (unsigned char *)base;
	unsigned char *src = (unsigned char *)s;
	unsigned long byte_ofs = bit_ofs / 8;
	unsigned long ofsbyte = bit_ofs % 8;
	unsigned char src_mask = 0x80;
	unsigned char dest_mask = 0x80;
	dest_mask = dest_mask >> ofsbyte;
	dst = dst + byte_ofs;
	unsigned char d_data = *dst;
	unsigned char s_data = *src; 
	while (bitsz)
		{
		if (s_data & src_mask) 
			{
			d_data = d_data | dest_mask;
			}
		else
			{
			d_data = d_data & (0xFF - dest_mask);
			}
		src_mask >>= 1;
		dest_mask >>= 1;
		if (!src_mask)
			{
			src_mask = 0x80;
			src++;
			s_data = *src; 
			}
		if (!dest_mask)
			{
			dest_mask = 0x80;
			*dst++ = d_data;
			d_data = *dst;
			}
		bitsz--;
		}
	*dst = d_data;
}


uint32_t RamCodingSpiro (void *In, void *Out, uint32_t sizes, const char *lpPassCode, uint8_t pass_ofs)
{
uint32_t rv = 0;
if (In && Out && sizes)
	{
	
	uint8_t curbyted, maskleft = 1, maskright = 0x80;
	uint32_t pass_lenght = lenstr (lpPassCode);
	if (pass_lenght) lpPassCode += (pass_ofs % pass_lenght);
	uint8_t *lpCurPassdata = (uint8_t*)lpPassCode;
	uint8_t *In_c = (uint8_t*)In;
	uint8_t *Out_c = (uint8_t*)Out;

	while (sizes)
		{
		curbyted = *In_c++;

		if (pass_lenght)
			{
			curbyted=curbyted ^ *lpCurPassdata;
			lpCurPassdata++;
			if (!(*lpCurPassdata)) lpCurPassdata = (uint8_t*)lpPassCode;
			}

		curbyted = curbyted ^ maskright;
		curbyted = curbyted ^ maskleft;

		maskright = maskright >> 1;
		if (!maskright) maskright = 0x80;
		maskleft = maskleft << 1;
		if (!maskleft) maskleft = 1;

		curbyted = curbyted ^ (uint8_t)rv;

		*Out_c++ = curbyted;

		sizes--;
		rv++;
		}
	}
return rv;
}


uint32_t LittleEndian_get_n (const uint8_t *lInp, uint32_t CurOffset, uint8_t databitsize)
{
uint32_t rv = 0;
if (lInp  && databitsize && databitsize <= 32)
	{
	uint32_t byte_ofs = CurOffset / 8;
	uint8_t bit_msk_input = 1;
	uint32_t bit_msk_output = 1;

  bit_msk_input = bit_msk_input << (CurOffset % 8);
  lInp += byte_ofs;

	while (databitsize)
		{
		if (*lInp & bit_msk_input) rv |= bit_msk_output;
		bit_msk_input = bit_msk_input << 1;
		if (!bit_msk_input)
			{
			bit_msk_input = 1;
			lInp++;
			}
		bit_msk_output = bit_msk_output << 1;
		databitsize--;
		}
	}
return rv;
}



bool quant_value (float base, float base_jit, float val)
{
	bool rv = false;
	float min_v = base - base_jit;
	float max_v = base + base_jit;
	if (val >= min_v && val <= max_v) rv = true;
	return rv;
}



bool subval_u32 (uint32_t &v, uint32_t sb)
{
	bool rv = false;
	uint32_t rslt = v - sb;
	if (rslt <= v)
		{
		v = rslt;
		rv = true;
		}
	else
		{
		v = 0;
		}
	return rv;
}



bool addval_u32 (uint32_t &v, uint32_t av)
{
	bool rv = false;
	uint32_t rslt = v + av;
	if (rslt >= v)
		{
		v = rslt;
		rv = true;
		}
	else
		{
		v = 0xFFFFFFFF;
		}
	return rv;
}



uint32_t lenstr_max (const char *lsrt, uint32_t maxsz)
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



