#ifndef _h_rutine_lib_h_
#define _h_rutine_lib_h_

#include "stddef.h"
#include <stdint.h>

#ifdef M_PI
# undef M_PI
#endif
#define M_PI      (3.141592653589793f)

#ifdef M_PI_2
# undef M_PI_2
#endif
#define M_PI_2    (M_PI / 2)

#define M_GOLDEN  1.6180339f

#define M_2PI         (M_PI * 2)

#define DEG_TO_RAD      (M_PI / 180.0f)
#define RAD_TO_DEG      (180.0f / M_PI)

typedef struct {
	uint8_t *lRam;
	uint16_t sizes;
} BUFPAR;




unsigned long lenstr (const char *lsrt);
unsigned short abs16 (short datas);
unsigned long abs32 (long datas);
void *memset (void *lDst, unsigned char dat, long sizes);
void *fillmem (void *lDst, unsigned char dat, long sizes);
//void *memcpy ( void * destination, const void * source, long num );
//uint32_t HAL_GetTick ();
//unsigned long HAL_RCC_GetPCLK1Freq ();
unsigned long str_size (const char *lsrt);
long strncmp( const char * string1, const char * string2, long num );
unsigned long GetTagStringDelimIndx (char *lpLinetxt, unsigned long Indxx, char delimc, char **lDest, unsigned long *lPCountField);
bool str_compare (char *lStr1, char *lStr2, unsigned long size);
bool TxtToFloat (float *lpDest, char *lpTxtIn, unsigned long Sizes);
unsigned long CheckDecimal (char *lTxt, unsigned long sz);
bool TxtToULong (char *lpRamData, unsigned char sz, unsigned long *lpDataOut);
unsigned short SwapShort (unsigned short dat);
void HardwareReset (void);
unsigned char *UlongToStr (unsigned char *lpDest, unsigned long datas);
unsigned char *LongToStr (unsigned char *lpDest, long datas);
char *FloatToStr (char *lDst, float val, unsigned char dig_n);
char *CopyMemorySDC (char *lpSour, char *lpDest, unsigned int sizes);
unsigned long CreateValueFromBitMassive (unsigned char *lInp, unsigned long CurOffset, unsigned char databitsize);
unsigned char SWAPBits (unsigned char datas);
unsigned long SwapBitsLong (unsigned long val, unsigned char bitsize);
uint32_t LittleEndian_get_n (const uint8_t *lInp, uint32_t CurOffset, uint8_t databitsize);
bool quant_value (float base, float base_jit, float val);
bool subval_u32 (uint32_t &v, uint32_t sb);
bool addval_u32 (uint32_t &v, uint32_t av);
unsigned long RLE_Coding_A (BUFPAR *InRaw, BUFPAR *OutCode);
unsigned char *UlongToStr_cnt (unsigned char *lpDest,unsigned long datas,unsigned char cntlens);
void UlongToHex (unsigned char *lpDest, unsigned long datasl);
uint32_t CalcCRC32Data (uint8_t *lSrc, uint32_t siz);
uint32_t lenstr_max (const char *lsrt, uint32_t maxsz);


#endif



