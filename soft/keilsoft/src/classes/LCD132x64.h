#ifndef __HH_LCDBLACKWHITE_H_
#define __HH_LCDBLACKWHITE_H_

#include "rutine.h"
#include "maxfont.h"
#include "STMSTRING.h"

#define LCD_WIDTH_PIXELS 136 //136
#define LCD_HEIGHT_PIXELS 72
#define LCD_RLEBUFSIZE (LCD_WIDTH_PIXELS * 3)



extern const unsigned char resname_varfont_f[];
extern unsigned char resname_flora_f[];
extern const unsigned char resname_off_f[];
extern const unsigned char resname_up_f[];
extern const unsigned char resname_voiseminus_f[];
extern const unsigned char resname_voiseplus_f[];
extern const unsigned char resname_yacen_f[];
extern const unsigned char resname_impact_20_Dig_f[];
extern const unsigned char resnameF7x1212f[];

extern const unsigned char resname_down_scr_f[];
extern const unsigned char resname_left_scr_f[];
extern const unsigned char resname_right_scr_f[];
extern const unsigned char resname_up_scr_f[];


typedef struct {
	char *lpBuf;
	unsigned char size;
	unsigned char NLine;
	unsigned char XOfs;
}RleDisp;


namespace VID {
	typedef struct {
			long X;
			long Y;
			long Width;
			long Height;
	} TMRect;

    typedef struct {
            unsigned char Mask_L;
            unsigned long CntMxFF;
            unsigned char Mask_R;
    } PRM_L1;

    const unsigned char SzPimpa = 1;

	typedef struct {
		unsigned short Width;
		unsigned short Height;
	} TMBitmap;

enum ALIGN {LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3};
enum ESTRALIGN {A_LEFT = 0, A_MIDLE = 1, A_RIGHT = 2};
enum ECURSSTYLE {CRS_GRAY = 0};

const unsigned char C_BLINESIZE = 136;
const unsigned char C_SEGROWCNT = 8;
const unsigned char C_SEGCOLCNT = 8;
const unsigned char PGInverse = 1;
const unsigned char PGSet = 2;
const unsigned char PGXor = 4;
const unsigned char PGClear = 8;

//const unsigned char MaskXLine_L[8] = {0,127,63,31,15,7,3,1};
const unsigned char MaskXLine_R[8] = {0,128,192,224,240,248,252,254};
};




#pragma pack(push,4)

class TLCDCANVABW {
	private:
		/*
    	unsigned char CntCharF_Celie;
        unsigned char CntCharF_Droba;
    	char BufStr[64];
        unsigned char IndxStr;
        void FloatToRam (char *lpDest, float datf, unsigned char drobcnt);
	*/

	protected:
		unsigned char BuferLCD[LCD_WIDTH_PIXELS * (LCD_HEIGHT_PIXELS / 8)];
		unsigned char RLEBufLCD[LCD_RLEBUFSIZE];
		unsigned char FontHeightBytes;							// высота шрифта в пикселах (размерность шрифта в байтах)
		unsigned char CntByteLine;
		unsigned long SizeCanvaBytes;
		long Canvas_Width;
		long Canvas_Height;							// текущая ширина и высота канвы
		unsigned char *VideoBufer;								// созданный участок памяти для целого экрана (КАНВЫ)
		BUFPAR tmpRLE;
		MaxFontMicro *SystemFont;
		char PPointFloatCod;
		//unsigned char CntByteLine;
		unsigned char CntVerticalLine;
		unsigned long LcdPrintMode;
		unsigned long Flash;
		unsigned char *FloatToStrDroba (unsigned char *lpDest,float datas,unsigned char Cntr);
		unsigned char *GetLineAdress (unsigned char Indxss);

		unsigned char TAILLINE[VID::C_BLINESIZE];
		unsigned char TAILLINE_RLE[(VID::C_BLINESIZE * 2)];
		long CRC_TileCalc (unsigned long Row, unsigned long Col);
		void CRC_AllTailCal (long lDst[VID::C_SEGROWCNT][VID::C_SEGCOLCNT]);
		void CRC_MoveToPrev ();
		long CRCTILE_PREV [VID::C_SEGROWCNT][VID::C_SEGCOLCNT];		// контрольные суммы тайлов экрана прошлые
		long CRCTILE [VID::C_SEGROWCNT][VID::C_SEGCOLCNT];			// контрольные суммы тайлов экрана текущие
		void GetGuteRect (VID::TMRect *inp_rct, VID::TMRect *out_rct);
    void GetLineParam_msk (unsigned long Xk, unsigned long Wdth, VID::PRM_L1 *lDst);

		long InxGtChngSeg_Row;
		long InxGtChngSeg_Col;

	public:

		TLCDCANVABW ();
		~TLCDCANVABW();
		
		void ClearTileCRC ();
		char GetFistChangeSeg (RleDisp &Ldest, char RleMod);
		char GetNextChangeSeg (RleDisp &Ldest, char RleMod);

        unsigned char *GetCanva ();


		long lastX;
		long lastY;
		long F_FixXWStep;

		unsigned long CopyCanva_BLine (BUFPAR *lpDst, unsigned char IndxLine);			// копирует и преобразовывает часть канвы в BYTE-строку
		unsigned long CopyCanva_BLine_RLE (BUFPAR *lpDst, unsigned char IndxLine);		// тоже самое + RLE сжатие

		void SetFonts (MaxFontMicro *lpFont);
		MaxFontMicro *GetFonts (void);
		unsigned char *GetFontMaskAdr (unsigned char datas);
		unsigned char CP1251To866 (unsigned char datas);

		void Init(void);
        // графические операции
		unsigned long GetCanvaWidth ();
		unsigned long GetCanvaHeight ();
		void SetOverMode  (char datas);
		void SetInverseMode (char datas);
		void SetXorMode (char datas);
        void SetNormalMode ();
        unsigned char GetMode ();
        void SetMode (unsigned char modcc);
        void Cls (unsigned char bitdata);
		void Font_SetPixelSize (unsigned char sizespix);
		void GetMaskAndOffset (short Xk, short Yk, unsigned long *lpOfsOut, unsigned char *lpMaskOut);
        char GetPoint (short Xk,short Yk);
        char GetFreePoint (short Xk,short Yk);
        char GetFreeHorLine (short Xk,short Yk, unsigned short wddhh);
		void Point_Bufer (short Xk,short Yk);
		void Point_Bufer_Set (short Xk,short Yk);
		void Point_Bufer_Clr (short Xk,short Yk);
		void DrawVerticalByte (unsigned char xk, unsigned char yk,unsigned char bytedata);
		void DrawVertical_8bit (unsigned char xk, unsigned char yk,unsigned char bytedata);
		void DrawVerticalByteNext (unsigned char bytedata);
		void WriteVerticalData (unsigned char *lpRom1,unsigned char counts);
		void DrawVerticalBlank (unsigned char xk, unsigned char yk);
		void DrawVerticalBlankCnt (unsigned char sizes);

        // стринговые операции
        unsigned char GetFontHeight (void);
		unsigned char GetDrawMaskWidth (unsigned char datas);
        unsigned char GetDrawMaskWidth (MaxFontMicro *lpFont, unsigned char datas);
		unsigned long GetDrawStringWidth (char *lpString);
        unsigned long GetDrawStringWidth (MaxFontMicro *lpFont, char *lpString);

        // простые стринги
        void DrawCursor (VID::ECURSSTYLE stile, unsigned char Wcurrs);
		void DrawCharSlow (unsigned char datas);
		void PrintString (const char *lpString);
    void PrintString_W (char *lpString, unsigned short wddtt_pixels);		// 24.2.18
    void PrintString_W_Cursor (char *lpString, unsigned short wddtt_pixels, unsigned char Curs);		// 24.2.18
		void DrawStringAtSlow (short Xk,short Yk,unsigned char *lpString);
		void DrawStringAtSlow_Next (unsigned char *lpString);
    void PrintStringCentrRect (const char *lpString, short Xk, short Yk,unsigned short widdtlc, char BlankMode);

        // цифры
		void Print_Long (long datas);
    //void Print_Float_WdthAlign (float datas, unsigned char pcnt, unsigned char wdthp, VID::ESTRALIGN allgn);
    //void Print_Float_WdthAlign_Cursor (float datas, unsigned char pcnt, unsigned char wdthp, VID::ESTRALIGN allgn, unsigned char TetrCursor);
		void Print_Hex (unsigned long datas);
		void Print_Ulong (unsigned long datas);
		void Print_Ulong_Cnt (unsigned long datas, unsigned char lendigs);

    // геометрические примитивы
		void DrawCircle(short x0, short y0, short radius);
		void DrawLineCute(short x1, short y1, short x2, short y2,unsigned char cutes);
		void DrawLine(short x1, short y1, short x2, short y2);
		void SetWH (unsigned short Wi,unsigned short He);
		void Rectagle (VID::TMRect *lpRect);
		void FillRect (VID::TMRect *lpRect);
		void FillRectGray_Light (VID::TMRect *lpRect);
		void FillRectGray_OR (VID::TMRect *lpRect);
		void FillRectGray_Lock (VID::TMRect *lpRect);
		void HorisontalGrayLine (short Xk, short Yk, unsigned short sizLen);
		void VerticalGrayLine (short Xk, short Yk, unsigned short sizLen);
		void RectagleClear (VID::TMRect *lpRect);
		void RectagleSet (VID::TMRect *lpRect);
		void DrawToSet(short x0, short y0);
		void DrawTo (short x0, short y0);

    // растровые операции
		void DrawBitmap (long Xk, long Yk,const unsigned char *lpRamBitmap,char InversMode);
		void DrawBitmap_Strech (long Xk, long Yk,const unsigned char *lpRamBitmap,char InversMode,float StrechMul);
    unsigned short GetBitmapWidth (const unsigned char *lpRamBitmap);


		void DrawString_Merge (char *lStr, char *lMask, unsigned long sizess);
		void PrintUlongtCentRect (unsigned long datas, short Xk, short Yk,unsigned short widdtlc);
		void PrintProcentsCenter (unsigned long datas, short Xk, short Yk,unsigned short sizes);
		void ProgressBar_V (short Xk, short Yk, short WidV, unsigned long curVal, unsigned long maxVal);
		void ProgressBar_Vlow (short Xk, short Yk, short WidV, float curVal, float maxVal, unsigned short HHeigg);

		void SCRMidleDisplay (const unsigned char *lpBitmaps, float strch);

		void DrawSome_Bat_H (VID::TMRect *lrct1, unsigned char Procents, char F_txt);
		void DrawSome_Progress (VID::TMRect *lrct1, unsigned char Procents, VID::ALIGN Align, char F_txt);
		void RotateRect_Up (VID::TMRect *lrct1, unsigned long steprout);
		void MoveHorisontalLine (long Dest_X, long Dest_Y, long Src_Y, unsigned long wdth);





};
#pragma pack (pop)



#endif

