#include "LCD132x64.h"
#include "SYSBIOS.H"
#include "rutine.h"

#ifdef __cplusplus
 extern "C" {
#endif

extern const unsigned char resname_wendy16_engl_f[];



#ifdef __cplusplus
}
#endif



//unsigned char TLCDCANVABW::BuferLCD[LCD_WIDTH_PIXELS * (LCD_HEIGHT_PIXELS / 8)];
//unsigned char TLCDCANVABW::RLEBufLCD[LCD_RLEBUFSIZE];



TLCDCANVABW::TLCDCANVABW ()
{
FontHeightBytes = 0;
lastX = 0;
lastY = 0;
PPointFloatCod = '.';
LcdPrintMode = 0;
F_FixXWStep = 0;

}





unsigned char *TLCDCANVABW::GetCanva ()
{
return VideoBufer;
}



unsigned long TLCDCANVABW::GetCanvaWidth ()
{
return Canvas_Width;
}



unsigned long TLCDCANVABW::GetCanvaHeight ()
{
return Canvas_Height;
}



TLCDCANVABW::~TLCDCANVABW()
{
//if (VideoBufer) delete VideoBufer;
//if (tmpRLE.lpRam) delete tmpRLE.lpRam;
}



unsigned char *TLCDCANVABW::GetLineAdress (unsigned char Indxss)
{
unsigned char *lpRv = 0;
if (Indxss < CntVerticalLine) lpRv = VideoBufer + (Canvas_Height * CntByteLine);
return lpRv;
}




// Формирует байт строку для LCD дисплеев с вертикальным рядовым расположением
unsigned long TLCDCANVABW::CopyCanva_BLine (BUFPAR *lpDst, unsigned char IndxLine)
{
unsigned long rv = 0;
if (IndxLine < CntVerticalLine && lpDst)
	{
	unsigned long rlsize = Canvas_Width;
	if (lpDst->sizes < rlsize) rlsize = lpDst->sizes;
	unsigned long IdxB = 0;
	unsigned long ofsbb = CntByteLine * 8;
	ofsbb = ofsbb * IndxLine;
	unsigned char *lpAdr = VideoBufer + ofsbb, *lpTmp;
	unsigned char Dt,Hmask = 128, Vmask;
	unsigned char *lpDest = (unsigned char *) lpDst->lRam;
	unsigned long DstSize = rlsize;
	while (IdxB < CntByteLine && DstSize)
		{
		//cnt8 = 8;
		// формирование вертикального байта из горизонтально расположенных линий
		lpTmp = lpAdr;
		Dt = 0;
		Vmask = 1;
		while (Vmask)
			{
			if (lpTmp[0] & Hmask) Dt = Dt | Vmask;
			Vmask = Vmask << 1;
			lpTmp = lpTmp + CntByteLine;
			//cnt8--;
			}
		Hmask = Hmask >> 1;
		if (!Hmask)
			{
			lpAdr++;
			IdxB++;
			Hmask = 128;
			}
		lpDest[0] = Dt;
		lpDest++;
		DstSize--;
		}
	rv = rlsize;
	}
return rv;
}





unsigned long TLCDCANVABW::CopyCanva_BLine_RLE (BUFPAR *Dst, unsigned char IndxLine)
{
unsigned long rv = 0;
unsigned long szline = CopyCanva_BLine (&tmpRLE,IndxLine);
if (szline)
	{
	BUFPAR LOPR1;
	LOPR1.lRam = tmpRLE.lRam;
	LOPR1.sizes = szline;
	rv = RLE_Coding_A (&LOPR1,Dst);
	}
return rv;
}




void TLCDCANVABW::SetFonts (MaxFontMicro *lpFont)
{
SystemFont = lpFont;
FontHeightBytes = lpFont->Height;
}



MaxFontMicro *TLCDCANVABW::GetFonts ()
{
return (MaxFontMicro *)SystemFont;
}




void TLCDCANVABW::Init()
{
SetFonts ((MaxFontMicro*)resname_wendy16_engl_f);
Canvas_Width = LCD_WIDTH_PIXELS;
Canvas_Height = LCD_HEIGHT_PIXELS;
CntByteLine = Canvas_Width / 8;
if (Canvas_Width % 8) CntByteLine++;
CntVerticalLine = Canvas_Height / 8;
if (Canvas_Height % 8) CntVerticalLine++;

SizeCanvaBytes = Canvas_Height * CntByteLine;
VideoBufer = &BuferLCD[0];

tmpRLE.sizes = LCD_RLEBUFSIZE;
tmpRLE.lRam = RLEBufLCD;
}





void TLCDCANVABW::GetMaskAndOffset (short Xk, short Yk, unsigned long *lpOfsOut, unsigned char *lpMaskOut)
{
unsigned char Maskb=128;
lpOfsOut[0] =(unsigned long)VideoBufer + Yk*CntByteLine + Xk/8;
Maskb = Maskb >>  (Xk & 7);
lpMaskOut[0] = Maskb;
}








char TLCDCANVABW::GetPoint (short Xk,short Yk)
{
char rv = 0;
if (Xk>=0 && Xk<Canvas_Width && Yk>=0 && Yk<Canvas_Height)
	{
	//unsigned char scrmd = LcdPrintMode & 15;
	unsigned char *lpAdrCanv = VideoBufer + Yk*CntByteLine + Xk/8;
	unsigned char Maskb = 128;
	Maskb = Maskb >>  (Xk & 7);
    if (lpAdrCanv[0] & Maskb) rv = 1;
	}
return rv;
}



void TLCDCANVABW::SetNormalMode ()
{
LcdPrintMode = 0;
}




void TLCDCANVABW::Point_Bufer (short Xk,short Yk)
{
if (Xk>=0 && Xk<Canvas_Width && Yk>=0 && Yk<Canvas_Height)
	{
	unsigned char scrmd = LcdPrintMode & 15;
	unsigned char *lpAdrCanv = VideoBufer + Yk*CntByteLine + Xk/8;
	unsigned char Maskb=128;

	Maskb = Maskb >>  (Xk & 7);
	switch (scrmd)
		{
		case 0:
		case 2:
			{
			lpAdrCanv[0]=lpAdrCanv[0] | Maskb;
			break;
			}
		case 4:
			{
			lpAdrCanv[0] = lpAdrCanv[0] ^ Maskb;
			break;
			}
		case 8:
			{
			Maskb = Maskb ^ 255;
			lpAdrCanv[0] = lpAdrCanv[0] & Maskb;
			break;
			}
		}
	}
}




// Узнает ширину маски текущего шрифта для текущего символа
unsigned char TLCDCANVABW::GetDrawMaskWidth (unsigned char datas)
{
unsigned char rv_width;
unsigned char *lpOfsstrChar;
lpOfsstrChar = ((unsigned char *)SystemFont) + SystemFont->OffsetBitMask[datas];
rv_width = ((MicroFWidth*)lpOfsstrChar)->Width;
return rv_width;
}



unsigned char TLCDCANVABW::GetDrawMaskWidth (MaxFontMicro *lpFont, unsigned char datas)
{
unsigned char rv_width;
unsigned char *lpOfsstrChar;
lpOfsstrChar = ((unsigned char *)lpFont) + lpFont->OffsetBitMask[datas];
rv_width = ((MicroFWidth*)lpOfsstrChar)->Width;
return rv_width;
}



// Узнает ширину в пикселах для целой строки
unsigned long TLCDCANVABW::GetDrawStringWidth (char *lpString)
{
unsigned long rv_sized = 0;
unsigned char datas;
while (1)
	{
	datas = lpString[0];
	if (!datas) break;
	rv_sized = rv_sized + GetDrawMaskWidth (datas);
	lpString++;
	}
return rv_sized;
}



// Узнает ширину в пикселах для целой строки
unsigned long TLCDCANVABW::GetDrawStringWidth (MaxFontMicro *lpFont, char *lpString)
{
unsigned long rv_sized = 0;
unsigned char datas;
while (true)
	{
	datas = lpString[0];
	if (!datas) break;
	rv_sized = rv_sized + GetDrawMaskWidth (lpFont, datas);
	lpString++;
	}
return rv_sized;
}



// Отрисовывает вертикальную линию по высоте текущего шрифта
void TLCDCANVABW::DrawVerticalBlank (unsigned char xk, unsigned char yk)
{
if (xk<Canvas_Width && yk<Canvas_Height)
	{
	unsigned char cntb = SystemFont->Height;
	unsigned long Ofsetcnva;
	unsigned char cmask,invcmask;
	unsigned char *lpCnvAdr;

	lastX = xk;
	if ((yk+cntb) >= Canvas_Height)
		{
		cntb = Canvas_Height - yk;
		}
	GetMaskAndOffset (xk,yk,&Ofsetcnva,&cmask);
	lpCnvAdr = (unsigned char*)Ofsetcnva;
	invcmask = cmask ^ 255;
	while (cntb)
		{
		if (!(LcdPrintMode & VID::PGInverse))
			{
			lpCnvAdr[0] = lpCnvAdr[0] & invcmask;
			}
		else
			{
			lpCnvAdr[0] = lpCnvAdr[0] | cmask;
			}
		lpCnvAdr = lpCnvAdr + CntByteLine;
		cntb--;
		}
	lastY = yk;
	lastX++;
	}
}


// отрисовывает группу вертикальных пробелов
void TLCDCANVABW::DrawVerticalBlankCnt (unsigned char sizes)
{
while (sizes)
	{
	DrawVerticalBlank (lastX,lastY);
	sizes--;
	}
}



// отрисовывает ASCII символ
void TLCDCANVABW::DrawCharSlow (unsigned char datas)
{
long realwidth,tmpwidth,realheight, orgWidth;
unsigned char lenByteOfsLineFnt, f_draw = true;
char *lpOfsstrChar = 0, *lpOfs;
//unsigned char *lpOfs;
unsigned char OrMskFnt = 128;
long Xppos;
if (!SystemFont->OffsetBitMask[datas]) return;
lpOfsstrChar = ((char*)SystemFont) + SystemFont->OffsetBitMask[datas];
lpOfs = lpOfsstrChar + sizeof(MicroFWidth);
realheight = SystemFont->Height;
realwidth = ((MicroFWidth*)lpOfsstrChar)->Width;
orgWidth = realwidth;

lenByteOfsLineFnt = realwidth / 8;

if (!lenByteOfsLineFnt)
    {
    lenByteOfsLineFnt = 1;
    }
else
    {
    if ((realwidth & 7))lenByteOfsLineFnt++;
    }
if (lastX < Canvas_Width)
        {
        if ((Canvas_Width - lastX ) < realwidth)
                {
                realwidth = Canvas_Width - lastX;
                }
        }
else
        {
        lastX = lastX + realwidth;
        return;
        }
if (lastY < Canvas_Height)
        {
        if ((Canvas_Height - lastY) < realheight)
                {
                realheight = Canvas_Height - lastY;
                }
        }
else
        {
        lastY = lastY + FontHeightBytes;
        return;
        }
Xppos = lastX;
if (lastX < 0)
    {
    unsigned long ofsMsk = abs32(lastX);
    unsigned long boffs = ofsMsk / 8;
    unsigned long bitror = ofsMsk & 7;
    if (realwidth > ofsMsk)
        {
        realwidth = realwidth - ofsMsk;
        if (bitror) OrMskFnt = OrMskFnt >> bitror;
        lpOfs = lpOfs + boffs;
        Xppos = 0;
        }
    else
        {
        f_draw = false;
        }
    }

        unsigned char grmode = LcdPrintMode & 15;
        unsigned long Ofsetcnva;
        unsigned char cmask,invcmask,maskoriginal, CurFntMsk, FDat;
        unsigned char *lpCnvAdr, *lpCnvAdrTMP;
        long Yppos = lastY;
        if (lastY < 0)
            {
            unsigned long OfsL = abs32(lastY);
            Yppos = 0;
            lpOfs = lpOfs + (OfsL *lenByteOfsLineFnt);
            if (realheight > OfsL)
                {
                realheight = realheight - OfsL;
                }
            else
                {
                f_draw = false;
                }
            }
    if (f_draw)
        {
        GetMaskAndOffset (Xppos,Yppos,&Ofsetcnva,&maskoriginal);
        lpCnvAdr = (unsigned char*)Ofsetcnva;
        char *lOrgMaskOfs = lpOfs;
        while (realheight)
            {
            tmpwidth = realwidth;
            lpCnvAdrTMP = lpCnvAdr;
            cmask = maskoriginal;
            invcmask = cmask ^ 255;
            lpOfs = lOrgMaskOfs;
            CurFntMsk = OrMskFnt;
            FDat = lpOfs[0];
            while (tmpwidth)
                {
                if (!CurFntMsk)
                    {
                    lpOfs++;
                    FDat = lpOfs[0];
                    CurFntMsk = 128;
                    }
                switch (grmode)
                    {
                    case 0:
                        {
                        if (!(FDat & CurFntMsk))
                            {
                            lpCnvAdrTMP[0] = lpCnvAdrTMP[0] & invcmask;
                            }
                        else
                            {
                            lpCnvAdrTMP[0] = lpCnvAdrTMP[0] | cmask;
                            }
                        break;
                        }
                    case 1:
                        {
                        if (FDat & CurFntMsk)
                            {
                            lpCnvAdrTMP[0] = lpCnvAdrTMP[0] & invcmask;
                            }
                        else
                            {
                            lpCnvAdrTMP[0] = lpCnvAdrTMP[0] | cmask;
                            }
                        break;
                        }
                    case 2:
                        {
                        if (FDat & CurFntMsk) lpCnvAdrTMP[0] = lpCnvAdrTMP[0] | cmask;
                        break;
                        }
                    case 3:
                        {
                        // over + inverse
                        if (!(FDat & CurFntMsk)) lpCnvAdrTMP[0] = lpCnvAdrTMP[0] | cmask;
                        break;
                        }
                    case 4:
                        {
                        // xor
                        if (lpCnvAdrTMP[0] & cmask)
                            {
                            if (FDat & CurFntMsk)
                                {
                                lpCnvAdrTMP[0] = lpCnvAdrTMP[0] & invcmask;
                                }
                            else
                                {
                                lpCnvAdrTMP[0] = lpCnvAdrTMP[0] | cmask;
                                }
                            }
                        else
                            {
                            if (!(FDat & CurFntMsk))
                                {
                                lpCnvAdrTMP[0] = lpCnvAdrTMP[0] & invcmask;
                                }
                            else
                                {
                                lpCnvAdrTMP[0] = lpCnvAdrTMP[0] | cmask;
                                }
                            }
                        break;
                        }
                    default:
                        {
                        if (!(FDat & CurFntMsk))
                            {
                            lpCnvAdrTMP[0] = lpCnvAdrTMP[0] & invcmask;
                            }
                        else
                            {
                            lpCnvAdrTMP[0] = lpCnvAdrTMP[0] | cmask;
                            }
                        break;
                        }
                    }
                CurFntMsk = CurFntMsk >> 1;
                //maskbt = maskbt<<1;
                cmask = cmask >> 1;
                invcmask = cmask ^ 255;
                if (!cmask)
                    {
                    cmask = 128;
                    invcmask = 127;
                    lpCnvAdrTMP++;
                    }
                tmpwidth--;
                }
            lOrgMaskOfs = lOrgMaskOfs + lenByteOfsLineFnt;
            lpCnvAdr = lpCnvAdr + CntByteLine;
            realheight--;
            }
        }

if (F_FixXWStep)
	{
	lastX = lastX + F_FixXWStep;
	}
else
	{
	lastX = lastX + orgWidth;
	}
}



void TLCDCANVABW::SetXorMode (char datas)
{
if (datas)
	{
	LcdPrintMode = LcdPrintMode | VID::PGXor;
	}
else
	{
	LcdPrintMode = LcdPrintMode & (255- VID::PGXor);
	}
}


// Устанавливает режим инверсии
void TLCDCANVABW::SetInverseMode (char datas)
{
if (datas)
	{
	LcdPrintMode = LcdPrintMode | VID::PGInverse;
	}
else
	{
	LcdPrintMode = LcdPrintMode & (255- VID::PGInverse);
	}
}


// Устанавливает режим ИЛИ
void TLCDCANVABW::SetOverMode  (char datas)
{
if (datas)
	{
	LcdPrintMode = LcdPrintMode | VID::PGSet;
	}
else
	{
	LcdPrintMode = LcdPrintMode & (255- VID::PGSet);
	}
}




// печатает целую строку ASCIIZ
void TLCDCANVABW::PrintString (const char *lpString)
{
char tmp;
if (lpString)
    {
    while (1)
        {
        tmp=lpString[0];
        if (!tmp) break;
        DrawCharSlow (tmp);
        lpString++;
        }
    }
}



void TLCDCANVABW::DrawString_Merge (char *lData, char *lMask, unsigned long sizess)
{
if ((lData || lMask) && sizess)
	{
	char d_data, d_msk;
	char dats, F_draw = true;
	while (sizess && F_draw)
		{
		F_draw = false;
		d_data = 0;
		d_msk = 0;
		dats = 0;
		if (lData) d_data = lData[0];
		if (lMask) d_msk = lMask[0];
		if (!d_data && !d_msk) break;
		switch (d_msk)
			{
			case 0:
				{
				dats = d_data;
				lData++;
				F_draw = true;
				break;
				}
			case 1:
				{
				if (d_data)
					{
					dats = d_data;
					lData++;
					}
				else
					{
					dats = ' ';
					}
				lMask++;
				F_draw = true;
				break;
				}
			default:
				{
				dats = d_msk;
				lMask++;
				F_draw = true;
				break;
				}
			}
		if (F_draw) DrawCharSlow (dats);

		sizess--;
		}
	}
}



void TLCDCANVABW::DrawStringAtSlow (short Xk,short Yk,unsigned char *lpString)
{
lastX = Xk;
lastY = Yk;
PrintString ((const char*)lpString);
}



void TLCDCANVABW::DrawStringAtSlow_Next (unsigned char *lpString)
{
PrintString ((const char*)lpString);
}






// Возвращает ширину текущего шрифта
unsigned char TLCDCANVABW::GetFontHeight ()
{
unsigned char rv = FontHeightBytes;
return rv;
}


// рисует круг
void TLCDCANVABW::DrawCircle(short x0, short y0, short radius)
{
        short x = 0;
        short y = radius;
        short delta = 2 - 2 * radius;
        short error = 0;
        while(y >= 0) {
                Point_Bufer(x0 + x, y0 + y);
                Point_Bufer(x0 + x, y0 - y);
                Point_Bufer(x0 - x, y0 + y);
                Point_Bufer(x0 - x, y0 - y);
                error = 2 * (delta + y) - 1;
                if(delta < 0 && error <= 0) {
                        ++x;
                        delta += 2 * x + 1;
                        continue;
                }
                error = 2 * (delta - x) - 1;
                if(delta > 0 && error > 0) {
                        --y;
                        delta += 1 - 2 * y;
                        continue;
                }
                ++x;
                delta += 2 * (x - y);
                --y;
        }
}


// рисует прерывистую линию
void TLCDCANVABW::DrawLineCute(short x1, short y1, short x2, short y2,unsigned char cutes)
{
unsigned char tmp_cutes=cutes;
unsigned char curcut=0;
    short deltaX = abs16(x2 - x1);
    short deltaY = abs16(y2 - y1);
    short signX = x1 < x2 ? 1 : -1;
    short signY = y1 < y2 ? 1 : -1;
    short error1 = deltaX - deltaY;
	long error2;

    for (;;)
    {
	if (curcut) Point_Bufer (x1,y1);
	if (tmp_cutes)
		{
		tmp_cutes--;
		if (!tmp_cutes)
			{
			tmp_cutes=cutes;
			curcut=!curcut;
			}
		}
    if(x1 == x2 && y1 == y2) break;
    error2 = error1 * 2;
	if(error2 > -deltaY)
		{
		error1 -= deltaY;
		x1 += signX;
		}
	if(error2 < deltaX)
		{
		error1 += deltaX;
		y1 += signY;
		}
    }
}



// рисует линию
void TLCDCANVABW::DrawLine(short x1, short y1, short x2, short y2)
{
    short deltaX = abs16(x2 - x1);
    short deltaY = abs16(y2 - y1);
    short signX = x1 < x2 ? 1 : -1;
    short signY = y1 < y2 ? 1 : -1;
    short error1 = deltaX - deltaY;
	long error2;

    for (;;)
    {
	Point_Bufer (x1,y1);
	if(x1 == x2 && y1 == y2)
		break;
	error2 = error1 * 2;
	if(error2 > -deltaY)
		{
		error1 -= deltaY;
		x1 += signX;
		}
	if(error2 < deltaX)
		{
		error1 += deltaX;
		y1 += signY;
		}
    }
}





// устанавливает штрину и высоту текущего графического контекста
void TLCDCANVABW::SetWH (unsigned short Wi,unsigned short He)
{
Canvas_Width=Wi;
Canvas_Height=He;
CntByteLine=Canvas_Width/8;
}



// отрисовывает прямоугольник
void TLCDCANVABW::Rectagle (VID::TMRect *lpRect)
{
DrawLine(lpRect->X,lpRect->Y,lpRect->X+lpRect->Width-1,lpRect->Y); // up
DrawLine(lpRect->X,lpRect->Y+lpRect->Height-1,lpRect->X+lpRect->Width-1,lpRect->Y+lpRect->Height-1); // down
DrawLine(lpRect->X,lpRect->Y,lpRect->X,lpRect->Y+lpRect->Height-1); // left
DrawLine(lpRect->X+lpRect->Width-1,lpRect->Y,lpRect->X+lpRect->Width-1,lpRect->Y+lpRect->Height-1); // right
}



void TLCDCANVABW::FillRect (VID::TMRect *lpRect)
{
unsigned char cntsz = lpRect->Height;
short StrY = lpRect->Y;
char tmp = LcdPrintMode;
short LstX = lpRect->X+lpRect->Width - 1;
if (cntsz && lpRect->Width)
	{
	while (cntsz)
		{
		DrawLine (lpRect->X,StrY, LstX,StrY);
		StrY++;
		cntsz--;
		}
	}
LcdPrintMode = tmp;
}



char TLCDCANVABW::GetFreeHorLine (short Xk,short Yk, unsigned short wddhh)
{
char rv = 1;
while (wddhh)
    {
    if (GetPoint (Xk,Yk))
        {
        rv = 0;
        break;
        }
    Xk++;
    wddhh--;
    }
return rv;
}




char TLCDCANVABW::GetFreePoint (short Xk,short Yk)
{
char rv = 0;
if (!GetPoint (Xk + 1,Yk) && !GetPoint (Xk - 1,Yk))
    {
    if (GetFreeHorLine (Xk - 1,Yk + 1, 3)) rv = 1;
    }

return rv;
}



void TLCDCANVABW::HorisontalGrayLine (short Xk, short Yk, unsigned short sizLen)
{
short LstX = Xk + sizLen - 1;
if (Yk & 1) Xk++;

while (Xk < LstX)
    {
    Xk = Xk + 2;
    Point_Bufer (Xk,Yk);
    }
}



void TLCDCANVABW::VerticalGrayLine (short Xk, short Yk, unsigned short sizLen)
{
short LstY = Yk + sizLen - 1;
while (Yk < LstY)
    {
    Yk = Yk + 2;
    Point_Bufer (Xk,Yk);
    }
}



void TLCDCANVABW::FillRectGray_Light (VID::TMRect *lpRect)
{
unsigned char cntsz = lpRect->Height;
short StrY = lpRect->Y;
//short Wdd = lpRect->Width;
char tmp = LcdPrintMode;
short LstX = lpRect->X+lpRect->Width;
short Px,Py;
if (cntsz && lpRect->Width)
	{
    while (cntsz)
        {
        //Wdd = lpRect->Width;

        Py = StrY;
        Px = lpRect->X;
        if (Py & 1) Px++;
        while (Px < LstX)
            {
            if (GetFreePoint (Px,Py)) Point_Bufer (Px,Py);
            Px = Px + 2;
            }
        StrY++;
        cntsz--;
        }
	}
LcdPrintMode = tmp;
}




void TLCDCANVABW::FillRectGray_Lock (VID::TMRect *lpRect)
{
VID::TMRect RC1 = lpRect[0];
RC1.Y += 2;
RC1.Height -= 4;
FillRectGray_OR (&RC1);
/*
if (lpRect->Height >= 3 && lpRect->Width >=3)
    {

    VID::TMRect RC1;
    RC1.Width = 3;
    RC1.Height = 3;
    RC1.X = lpRect->X;
    RC1.Y = lpRect->Y;
    //FillRect (&RC1);
    RC1.Y = RC1.Y + lpRect->Height - 3;
    //FillRect (&RC1);
    RC1.X = lpRect->X + lpRect->Width - 3;
    //FillRect (&RC1);
    //RC1.Y = lpRect->Y;
    //FillRect (&RC1);

    HorisontalGrayLine (lpRect->X,lpRect->Y, lpRect->Width);
    HorisontalGrayLine (lpRect->X,lpRect->Y + lpRect->Height - 1, lpRect->Width);
    VerticalGrayLine (lpRect->X, lpRect->Y, lpRect->Height - 1);
    VerticalGrayLine (lpRect->X + lpRect->Width - 1, lpRect->Y, lpRect->Height - 1);
    }
*/


}




void TLCDCANVABW::FillRectGray_OR (VID::TMRect *lpRect)
{
unsigned char cntsz = lpRect->Height;
short StrY = lpRect->Y;
//short Wdd = lpRect->Width;
char tmp = LcdPrintMode;
short LstX = lpRect->X+lpRect->Width;
short Px,Py;
if (cntsz && lpRect->Width)
	{
    while (cntsz)
        {
        //Wdd = lpRect->Width;

        Py = StrY;
        Px = lpRect->X;
        if (Py & 1) Px++;
        while (Px < LstX)
            {
            Point_Bufer (Px,Py);
            Px = Px + 2;
            }
        StrY++;
        cntsz--;
        }
	}
LcdPrintMode = tmp;
}



void TLCDCANVABW::RectagleClear (VID::TMRect *lpRect)
{
char tmp = LcdPrintMode;
LcdPrintMode = VID::PGClear;
FillRect (lpRect);
LcdPrintMode = tmp;
}



void TLCDCANVABW::RectagleSet (VID::TMRect *lpRect)
{
char tmp = LcdPrintMode;
LcdPrintMode = VID::PGSet;
FillRect (lpRect);
LcdPrintMode = tmp;
}



// устанавливает позицию вывода текста
void TLCDCANVABW::DrawToSet(short x0, short y0)
{
lastX=x0;
lastY=y0;
}



// отрисовывает линию от последней X,Y координаты (удобно рисовать осцилограммы)
void TLCDCANVABW::DrawTo (short x0, short y0)
{
DrawLine(lastX,lastY,x0,y0);
lastX=x0;
lastY=y0;
}




// очищает весь экран
void TLCDCANVABW::Cls (unsigned char bitdata)
{
unsigned char *lpRam = VideoBufer;
unsigned short sizesdat = SizeCanvaBytes;
while (sizesdat)
	{
	lpRam[0]=bitdata;
	lpRam++;
	sizesdat--;
	}
}







// печатает цифру в десятичном формате
void TLCDCANVABW::Print_Long (long datas)
{
unsigned char Bufrtv[16];
unsigned char *lpEnd = Bufrtv;
if (datas < 0)
	{
	lpEnd[0] = '-';
	lpEnd++;
	datas = abs32 (datas);
	}
lpEnd = UlongToStr (lpEnd,datas);
lpEnd[0]=0;			
PrintString ((const char*)Bufrtv);
}





// печатает цифру в десятичном формате
void TLCDCANVABW::Print_Ulong (unsigned long datas)
{
unsigned char Bufrtv[16];
unsigned char *lpEnd = Bufrtv;
lpEnd = UlongToStr (Bufrtv,datas);
lpEnd[0]=0;			
PrintString ((const char *)Bufrtv);
}


void TLCDCANVABW::Print_Ulong_Cnt (unsigned long datas, unsigned char lendigs)
{
unsigned char Bufrtv[16];
unsigned char *lpEnd = Bufrtv;
lpEnd = UlongToStr_cnt (Bufrtv,datas,lendigs);
lpEnd[0]=0;			
PrintString ((const char *)Bufrtv);
}



// печатает цифру в HEX формате
void TLCDCANVABW::Print_Hex (unsigned long datas)
{
unsigned char Bufrtv[16];
UlongToHex (Bufrtv,datas);
Bufrtv[8]=0;
PrintString ((const char *)Bufrtv);
}



unsigned char *TLCDCANVABW::FloatToStrDroba (unsigned char *lpDest,float datas,unsigned char Cntr)
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
return lpDest;
}





void TLCDCANVABW::PrintStringCentrRect (const char *lpString, short Xk, short Yk, unsigned short widdtlc, char BlankMode)
{
long wsize,frvblank,backblank,curaswd;
unsigned long lens;
unsigned char tmb;
wsize = GetDrawStringWidth ((char*)lpString);
lens = lenstr ((char*)lpString);
if (wsize > widdtlc)
	{
	frvblank = 0;
	backblank = 0;
	}
else
	{
	frvblank = (widdtlc - wsize) / 2;
	backblank = widdtlc - (wsize + frvblank);
	}
lastY = Yk;
if (BlankMode)
	{
	lastX = Xk;
	DrawVerticalBlankCnt (frvblank);
	}
else
	{
	lastX = Xk + frvblank;
	}
while (lens)
	{
	tmb = lpString[0];
	curaswd = GetDrawMaskWidth (tmb);
	if (curaswd > wsize)
		{
		// печатается символ который обрезан, т.е. последний в строке
		if (Flash & 512)
			{
			tmb = ' ';
			}
		else
			{
			tmb = '>';
			}		
		DrawCharSlow (tmb);
		break;
		}
    DrawCharSlow (tmb);
	wsize = wsize - curaswd;
	lpString++;		
	lens--;
	}
if (BlankMode) DrawVerticalBlankCnt (backblank);
}



unsigned char TLCDCANVABW::GetMode ()
{
return LcdPrintMode;
}




void TLCDCANVABW::SetMode (unsigned char modcc)
{
LcdPrintMode = modcc;
}



// отрисовывает от нужной позиции цифры, центрируя относительно ширины заданой области
void TLCDCANVABW::PrintUlongtCentRect (unsigned long datas, short Xk, short Yk,unsigned short widdtlc)
{
unsigned char Bufrtv[16];
unsigned char *lpEnd = Bufrtv;
lpEnd = UlongToStr (Bufrtv,datas);
lpEnd[0]=0;
PrintStringCentrRect ((const char*)Bufrtv,Xk,Yk,widdtlc,0);				
}




void TLCDCANVABW::PrintProcentsCenter (unsigned long datas, short Xk, short Yk,unsigned short widdtlc)
{
unsigned char Bufrtv[16];
unsigned char *lpEnd;
lpEnd = UlongToStr (Bufrtv,datas);
lpEnd[0]='%';
lpEnd[1]=0;
PrintStringCentrRect ((const char*)Bufrtv,Xk,Yk,widdtlc,0);				
}




unsigned short TLCDCANVABW::GetBitmapWidth (const unsigned char *lpRamBitmap)
{
unsigned short rvwidth = 0;
VID::TMBitmap *lpParmBitmp = (VID::TMBitmap *)lpRamBitmap;
rvwidth = lpParmBitmp->Width;
return rvwidth;
}




// отрисовывает BITMAP
void TLCDCANVABW::DrawBitmap (long Xk, long Yk,const unsigned char *lpRamBitmap,char InversMode)
{
long Wbit,Hbit;
unsigned long lenBtLine,Ofsetcnva,tmp_pfl,RealWidth,RealHeight;
const unsigned char *lpStartRaw, *lptmpBMP;
unsigned char *lpDestCanv;
unsigned char BMaskStart_Canva, BMaskStart_Bitmap, tmp_bmBMP, tmp_bmCANV,datab;
VID::TMBitmap *lpParmBitmp = (VID::TMBitmap *)lpRamBitmap;
Wbit = lpParmBitmp->Width;
Hbit = lpParmBitmp->Height;				
lenBtLine = Wbit/8;
if (Wbit%8) lenBtLine++;
lpStartRaw = lpRamBitmap + sizeof(VID::TMBitmap);

if (Yk < 0)
	{
	if ((Yk + Hbit) <= 0 ) return;										
	RealHeight = Hbit + Yk;
	lpStartRaw = lpStartRaw + ((abs32(Yk))*lenBtLine);
	Yk = 0;
	}
else
	{
	if (Yk >= Canvas_Height)	return;
	if ((Yk + Hbit) >= Canvas_Height)
		{
		RealHeight = Canvas_Height - Yk;
		}
	else
		{
		RealHeight = Hbit;
		}
	}

if (Xk < 0)
	{
	if ((Wbit + Xk) <= 0) return;
	RealWidth = Wbit + Xk;
	Xk = abs32 (Xk);		
	lpStartRaw = lpStartRaw + (Xk / 8);
	BMaskStart_Bitmap = 128 >>  (Xk & 7);
	Xk = 0;
	}
else
	{
	if (Xk >= Canvas_Width)	return;
	if ((Xk + Wbit) >= Canvas_Width)
		{
		RealWidth = Canvas_Width - Xk;
		}										
	else
		{
		RealWidth = Wbit;
		}
	BMaskStart_Bitmap = 128;
	}
GetMaskAndOffset (Xk,Yk,&Ofsetcnva,&BMaskStart_Canva);
while (RealHeight)
	{			
	tmp_pfl = RealWidth;					// ширина картинки в пикселах
	tmp_bmBMP = BMaskStart_Bitmap; 			// начальная маска BMP картинки
	lptmpBMP = lpStartRaw;

	tmp_bmCANV = BMaskStart_Canva;
	lpDestCanv = (unsigned char *)Ofsetcnva;
	while (tmp_pfl)
		{
		datab = lpDestCanv[0];
		if (lptmpBMP[0] & tmp_bmBMP)
			{
			if (InversMode)
				{
				datab = datab & (255 ^ tmp_bmCANV);
				}
			else
				{
				datab = datab | tmp_bmCANV;
				}
			}
		else
			{
			if (InversMode)
				{
				datab = datab | tmp_bmCANV;
				}
			else
				{
				datab = datab & (255 ^ tmp_bmCANV);
				}
			}
		lpDestCanv[0] = datab;
	    tmp_bmCANV = tmp_bmCANV >> 1;
		tmp_bmBMP = tmp_bmBMP >> 1;
		if (!tmp_bmCANV)
			{
			tmp_bmCANV = 128;
			lpDestCanv++;
			}
		if (!tmp_bmBMP)
			{
			tmp_bmBMP = 128;
			lptmpBMP++;
			}				
		tmp_pfl--;
		}				
	lpStartRaw = lpStartRaw + lenBtLine;
	Ofsetcnva = Ofsetcnva + CntByteLine;
	RealHeight--;
	}
}






// отрисовывает BITMAP
void TLCDCANVABW::DrawBitmap_Strech (long Xk, long Yk,const unsigned char *lpRamBitmap,char InversMode,float StrechSet)
{
//long MulOrDiv10 = 0;
long Wbit,Hbit,ControlX,ControlY;
unsigned long lenBtLine,Ofsetcnva;
long RealWidth,RealHeight,tmp_pfl;
const unsigned char *lpStartRaw, *lptmpBMP;
unsigned char *lpDestCanv;
unsigned char BMaskStart_Canva, BMaskStart_Bitmap, tmp_bmBMP, tmp_bmCANV,datab;
float tmpstrHeight,tmpstrWidth,StepY;//,StrStepFloat;
VID::TMBitmap *lpParmBitmp = (VID::TMBitmap *)lpRamBitmap;
//StrStepFloat = StrechSet;
//if (StrechSet >= 1) MulOrDiv10 = 1;
//if (StrStepFloat <= 0) StrStepFloat = 1;
Wbit = lpParmBitmp->Width;
Hbit = lpParmBitmp->Height;		
RealWidth = Wbit;		
RealHeight = Hbit;
lenBtLine = Wbit/8;
if (Wbit%8) lenBtLine++;
lpStartRaw = lpRamBitmap + sizeof(VID::TMBitmap);

if (Yk < 0)
	{
	if ((Yk + Hbit) <= 0 ) return;										
	//RealHeight = Hbit + Yk;
	lpStartRaw = lpStartRaw + ((abs32(Yk))*lenBtLine);
	Yk = 0;
	}
else
	{
	if (Yk >= Canvas_Height)	return;
	if ((Yk + Hbit) >= Canvas_Height)
		{
		//RealHeight = Const_PrinterPixelHeight - Yk;
		}
	else
		{
		RealHeight = Hbit;
		}
	}

if (Xk < 0)
	{
	if ((Wbit + Xk) <= 0) return;
	//RealWidth = Wbit + Xk;
	Xk = abs32 (Xk);		
	lpStartRaw = lpStartRaw + (Xk / 8);
	BMaskStart_Bitmap = 128 >>  (Xk & 7);
	Xk = 0;
	}
else
	{
	if (Xk >= Canvas_Width)	return;
	if ((Xk + Wbit) >= Canvas_Width)
		{
		//RealWidth = Const_PrinterPixelWidth - Xk;
		}										
	else
		{
		RealWidth = Wbit;
		}
	BMaskStart_Bitmap = 128;
	}

GetMaskAndOffset (Xk,Yk,&Ofsetcnva,&BMaskStart_Canva);
StepY = 1 / StrechSet;
tmpstrHeight = StepY;
ControlY = Yk;
while (RealHeight > 0 && ControlY < Canvas_Height)
	{			
	tmp_pfl = RealWidth;					// ширина картинки в пикселах
	tmp_bmBMP = BMaskStart_Bitmap; 			// начальная маска BMP картинки
	lptmpBMP = lpStartRaw;

	tmp_bmCANV = BMaskStart_Canva;
	lpDestCanv = (unsigned char *)Ofsetcnva;
	tmpstrWidth = StrechSet;
	ControlX = Xk;
	while (tmp_pfl > 0)
		{		
		if (tmpstrWidth >= 1)
			{
			unsigned long cntduble = tmpstrWidth;
			tmpstrWidth = tmpstrWidth - cntduble;
			while (cntduble)
				{
				datab = lpDestCanv[0];
				if (lptmpBMP[0] & tmp_bmBMP)
					{
					if (InversMode)
						{
						datab = datab & (255 ^ tmp_bmCANV);
						}
					else
						{
						datab = datab | tmp_bmCANV;
						}
					}
				else
					{
					if (InversMode)
						{
						datab = datab | tmp_bmCANV;
						}
					else
						{
						datab = datab & (255 ^ tmp_bmCANV);
						}
					}
				lpDestCanv[0] = datab;
				
				tmp_bmCANV = tmp_bmCANV >> 1;
				if (!tmp_bmCANV)
					{
					tmp_bmCANV = 128;
					lpDestCanv++;
					}
				cntduble--;
				ControlX++;
				if (ControlX >= Canvas_Width)
					{
					tmp_pfl = 1;
					break;
					}
				}
			}
		tmp_bmBMP = tmp_bmBMP >> 1;
		if (!tmp_bmBMP)
			{
			tmp_bmBMP = 128;
			lptmpBMP++;
			}
		tmpstrWidth = tmpstrWidth + StrechSet;
		tmp_pfl--;
		}		
		
	if (tmpstrHeight >= 1)
		{
		unsigned long cntduble = tmpstrHeight;
		tmpstrHeight = tmpstrHeight - cntduble;
		lpStartRaw = lpStartRaw + lenBtLine*cntduble;	
		RealHeight = RealHeight - cntduble;
		}
	Ofsetcnva = Ofsetcnva + CntByteLine;
	tmpstrHeight = tmpstrHeight + StepY;
	ControlY++;
	}
}



void TLCDCANVABW::SCRMidleDisplay (const unsigned char *lpBitmaps, float strch)
{
long Wbit,Hbit,Xk,Yk;
VID::TMBitmap *lpParmBitmp = (VID::TMBitmap *)lpBitmaps;
Wbit = strch * lpParmBitmp->Width;
Hbit = strch * lpParmBitmp->Height;
Xk = (132 - Wbit) / 2;
Yk = (64 - Hbit) / 2;	
DrawBitmap_Strech (Xk,Yk,lpBitmaps,0,strch);
}



void TLCDCANVABW::ProgressBar_Vlow (short Xk, short Yk, short WidV, float curVal, float maxVal, unsigned short HHeigg)
{
// ( Sizes * 100 ) / ProgresBarBixels;
char tmpmd = LcdPrintMode;
float MxSize = maxVal * 100;
float SzToPix = MxSize / WidV;
unsigned short LineCnt = (curVal * 100 ) / SzToPix;
LcdPrintMode = 0;

SetXorMode (1);

if (LineCnt)
	{
	short strX = Xk,strY = Yk;
	while (LineCnt)
		{
		DrawLine (strX,strY,strX,strY+HHeigg);
		LineCnt--;
		strX++;
		}
	}
LcdPrintMode = tmpmd;
}



void TLCDCANVABW::DrawSome_Bat_H (VID::TMRect *lrct1, unsigned char Procents, char F_txt)
{
if (lrct1)
    {
    VID::TMRect rc1 = lrct1[0], rc2;
    unsigned long Hbut = rc1.Height - 4;
    rc2.X = rc1.X;
    rc2.Y = rc1.Y + 2;
    rc2.Width = VID::SzPimpa;
    rc2.Height = Hbut;
    Rectagle(&rc2);  // отрисовка плюсового контакта
    rc1.X += VID::SzPimpa;
    rc1.Width -= VID::SzPimpa;
    DrawSome_Progress (&rc1, Procents, VID::RIGHT, F_txt); // VID::RIGHT LEFT
    }
}




void TLCDCANVABW::DrawSome_Progress (VID::TMRect *lrct1, unsigned char Procents, VID::ALIGN Align, char F_txt)
{
if (lrct1)
    {
    unsigned char mdlast = GetMode ();
    VID::TMRect rc1 = lrct1[0];
    if (rc1.Width > 5)
        {
        char BufStr[5];
        MaxFontMicro *prevFont = GetFonts();
        //SetFonts((MaxFontMicro*)resname_wendyDIG09_f);
        RectagleClear (lrct1);
        Rectagle(lrct1);  // отрисовка общего корпуса
        // расчет пропорции заполнения
        if (Procents > 100) Procents = 100;
        switch (Align)
            {
            case VID::RIGHT:
            case VID::LEFT:
                {
                float WtoProc = rc1.Width;
                WtoProc = WtoProc / 100;
                rc1.Width = WtoProc * Procents;
                if (Align == VID::RIGHT) rc1.X = rc1.X + (lrct1->Width - rc1.Width);
                break;
                }
            case VID::UP:
            case VID::DOWN:
                {
                float WtoProc = rc1.Height;
                WtoProc = WtoProc / 100;
                rc1.Height = WtoProc * Procents;
                if (Align == VID::DOWN) rc1.Y = rc1.Y + (lrct1->Height - rc1.Height);
                break;
                }
            }
        FillRect(&rc1);  // закрашивание оюласти "процента заполнения"
        if (F_txt)
            {
            unsigned char *lDst = UlongToStr ((unsigned char*)&BufStr, Procents);
            if (lDst)
                {
                lDst[0] = '%';
                lDst[1] = 0;
                long Vofs = (lrct1->Y + (lrct1->Height - 7)/2)-5;
                SetNormalMode ();
                SetXorMode(true);
                PrintStringCentrRect ((const char*)&BufStr, lrct1->X + 1, Vofs, lrct1->Width, false);
                SetXorMode(false);
                }
            }
        SetFonts(prevFont);
        }
    SetMode (mdlast);
    }
}



void TLCDCANVABW::ProgressBar_V (short Xk, short Yk, short WidV, unsigned long curVal, unsigned long maxVal)
{
// ( Sizes * 100 ) / ProgresBarBixels;
VID::TMRect retpr;
char tmpmd = LcdPrintMode;
unsigned long MxSize = maxVal * 100;
unsigned long SzToPix = MxSize / WidV;
unsigned short LineCnt = (curVal * 100 ) / SzToPix;

unsigned long MxSizeProc = maxVal * 100;
unsigned long SzToPixProc = MxSizeProc / 100;
unsigned short CurProc = (curVal * 100 ) / SzToPixProc;

unsigned char Bufrtv[16];
unsigned char *lpEnd = Bufrtv;

retpr.X = Xk;
retpr.Y = Yk;
retpr.Width = WidV;
retpr.Height = GetFontHeight ();
SetInverseMode (0);
SetOverMode (1);
RectagleClear (&retpr);
Rectagle (&retpr);

if (LineCnt)
	{
	short strX = Xk,strY = Yk;
	while (LineCnt)
		{
		DrawLine (strX,strY,strX,strY+retpr.Height-1);
		LineCnt--;
		strX++;
		}
	}

lpEnd = UlongToStr (Bufrtv,CurProc);
lpEnd[0]='%';
lpEnd[1]=0;
LcdPrintMode = 0;
SetXorMode (1);
PrintStringCentrRect ((const char*)Bufrtv,Xk,Yk,WidV,0);
SetXorMode (0);
LcdPrintMode = tmpmd;
}


// очищает контрольные суммы таблиц "tail segs"
void TLCDCANVABW::ClearTileCRC ()
{
fillmem ((char*)&CRCTILE, 0, sizeof(CRCTILE));
fillmem ((char*)&CRCTILE_PREV, 0, sizeof(CRCTILE_PREV));
}



// подсчитывает контрольную сумму указанного "tail segment"
long TLCDCANVABW::CRC_TileCalc (unsigned long Row, unsigned long Col)
{
long crc = 0;
unsigned char dt;
unsigned long dtxor = 0;
unsigned long SzCrcDat = Canvas_Width / VID::C_SEGCOLCNT;	// сколько байт подсчитывать
unsigned char *lpRam = VideoBufer + (Row * Canvas_Width) + (Col * SzCrcDat);
while (SzCrcDat)
	{
	dt = lpRam[0];
	crc = crc + dt;
	dtxor = (dtxor ^ dt) + dt;
	lpRam++;
	SzCrcDat--;
	}
dtxor = dtxor << 16;
crc = crc + dtxor;
return crc;
}



// подсчитывает контрольные суммы "tail segment"
void TLCDCANVABW::CRC_AllTailCal (long lDst[VID::C_SEGROWCNT][VID::C_SEGCOLCNT])
{
if (lDst)
	{
	long indxRow = 0, indCol, crctl;
	while (indxRow < VID::C_SEGROWCNT)
		{
		indCol = 0;
		while (indCol < VID::C_SEGCOLCNT)
			{
			crctl = CRC_TileCalc (indxRow, indCol);
			lDst[indxRow][indCol] = crctl;
			indCol++;
			}
		indxRow++;
		}
	}
}



// перемещает текущюю таблицу контрольных сумм тайлов в старую таблицу
void TLCDCANVABW::CRC_MoveToPrev ()
{
CopyMemorySDC ((char*)&CRCTILE, (char*)&CRCTILE_PREV, sizeof(CRCTILE_PREV));
//CopyMemory((char*)&CRCTILE_PREV, (char*)&CRCTILE, sizeof(CRCTILE_PREV));
}



// находит первый сегмент с обновленной(изменившейся) графикой
char TLCDCANVABW::GetFistChangeSeg (RleDisp &Ldest, char RleMod)
{
InxGtChngSeg_Row = 0;
InxGtChngSeg_Col = 0;
CRC_AllTailCal (CRCTILE);	// подсчет контрольных сумм Tail секций в текущюю таблицу
return GetNextChangeSeg (Ldest, RleMod);
}



// находит следующий сегмент с изменившейся графикой
char TLCDCANVABW::GetNextChangeSeg (RleDisp &Ldest, char RleMod)
{
char rv = 0, F_end = 0;
long Strt_Col = -1, Col_cnt = 0;
while (InxGtChngSeg_Row < VID::C_SEGROWCNT)
	{
	if (InxGtChngSeg_Col >= VID::C_SEGCOLCNT)
		{
		InxGtChngSeg_Col = 0;
		InxGtChngSeg_Row++;
		if (InxGtChngSeg_Row >= VID::C_SEGROWCNT)
			{
			F_end = 1;
			break;
			}
		}
	while (InxGtChngSeg_Col < VID::C_SEGCOLCNT)
		{
		if (CRCTILE[InxGtChngSeg_Row][InxGtChngSeg_Col] != CRCTILE_PREV[InxGtChngSeg_Row][InxGtChngSeg_Col])
			{
			if (Strt_Col == -1)
				{
				Strt_Col = InxGtChngSeg_Col;
				Col_cnt = 1;
				}
			else
				{
				Col_cnt++;
				}
			}
		else
			{
			if (Col_cnt)
				{
				InxGtChngSeg_Col++;
				break;
				}
			}
		InxGtChngSeg_Col++;
		}
	if (Col_cnt) break;
	}
if (!F_end && Col_cnt)
	{
	BUFPAR bout1;
	unsigned long SzCrcDat = Canvas_Width / VID::C_SEGCOLCNT;			// скаолько байт в сегменте
	bout1.lRam = TAILLINE;
	bout1.sizes = sizeof(TAILLINE);										// размер целой строки
	unsigned long rawtailsize = Col_cnt * SzCrcDat;
	unsigned long rvsize = CopyCanva_BLine (&bout1, InxGtChngSeg_Row);	// перенести байт-линию в буфер
	if (rvsize)
		{
		if (RleMod)
			{
			BUFPAR LOPR1, RLout;
			LOPR1.lRam = (((unsigned char*)bout1.lRam) + Strt_Col*SzCrcDat);	// высчитать начальное смещение -
			LOPR1.sizes = rawtailsize;									// и текущий размер
			RLout.lRam = TAILLINE_RLE;									// параметры -
			RLout.sizes = sizeof(TAILLINE_RLE);							// выходного буфера
			rvsize = RLE_Coding_A (&LOPR1, &RLout);						// RLE кодировка
			if (rvsize)
				{
				Ldest.lpBuf = (char*)TAILLINE_RLE;
				Ldest.size = rvsize;
				Ldest.NLine = InxGtChngSeg_Row;
				Ldest.XOfs = Strt_Col*SzCrcDat;
				rv = 1;
				}
			}
		else
			{
			Ldest.lpBuf = (((char*)bout1.lRam) + Strt_Col*SzCrcDat);
			Ldest.size = rawtailsize;
			Ldest.NLine = InxGtChngSeg_Row;
			Ldest.XOfs = Strt_Col*SzCrcDat;
			rv = 1;
			}
		}
	}
if (F_end) CRC_MoveToPrev ();	// если конец поиска - сделать контрольные суммы устаревшими
return rv;
}



void TLCDCANVABW::GetGuteRect (VID::TMRect *inp_rct, VID::TMRect *out_rct)
{
long dimens;
VID::TMRect InR1 = inp_rct[0];
InR1.Width = abs32 (InR1.Width);
InR1.Height = abs32 (InR1.Height);
out_rct->Width = 0;
out_rct->Height = 0;
do {
    if (InR1.X < 0)
        {
        InR1.X = 0;
        dimens = abs32 (InR1.X);
        if (dimens >= InR1.Width) break;
        InR1.Width = InR1.Width - dimens;
        }
    if ((InR1.X + InR1.Width) > Canvas_Width) InR1.Width = Canvas_Width;

    if (InR1.Y < 0)
        {
        InR1.Y = 0;
        dimens = abs32 (InR1.Y);
        if (dimens >= InR1.Height) break;
        InR1.Height = InR1.Height - dimens;
        }

    if ((InR1.Y + InR1.Height) > Canvas_Height) InR1.Height = Canvas_Height;
    out_rct[0] = InR1;
    } while (0);
}



// вычисляет для линии параметры масочного наложения при переносе данных
void TLCDCANVABW::GetLineParam_msk (unsigned long Xk, unsigned long Wdth, VID::PRM_L1 *lDst)
{
unsigned char lofs = Xk & 7;
lDst->Mask_L = 0;
lDst->CntMxFF = 0;
lDst->Mask_R = 0;
if (Wdth)
    {
    do {
        if (lofs)
            {
            unsigned char StrtBit = 128 >> lofs;
            unsigned char cntL = 7 - lofs;
            while (cntL && Wdth)
                {
                StrtBit = StrtBit | (StrtBit >> 1);
                cntL--;
                Wdth--;
                }
            lDst->Mask_L = StrtBit;
            if (!Wdth) break;
            }
        if (Wdth >= 8)
            {
            lDst->CntMxFF = Wdth / 8;
            Wdth = Wdth % 8;
            if (!Wdth) break;
            }
        lDst->Mask_R = VID::MaskXLine_R[Wdth];
        } while (0);
    }
}



void TLCDCANVABW::MoveHorisontalLine (long Dest_X, long Dest_Y, long Src_Y, unsigned long wdth)
{
unsigned long dimns;
//unsigned char F_Blank = false;

    do {
        if (Dest_Y >= Canvas_Height || Dest_Y < 0) break;
        if (Dest_X >= Canvas_Width) break;
        if (Dest_X < 0)
            {
            dimns = abs32 (Dest_X);
            if (dimns >= wdth) break;
            wdth = wdth - dimns;
            Dest_X = 0;
            }
        if (wdth > Canvas_Width) wdth = Canvas_Width;

        unsigned long DestAdr, SrcAdr;
        unsigned char Dest_Mask, Src_Mask;

        if (Src_Y >= Canvas_Height || Src_Y < 0)
            {
            unsigned char lastmod = LcdPrintMode;
            LcdPrintMode = VID::PGClear;
            DrawLine(Dest_X, Dest_Y, Dest_X + wdth, Dest_Y);
            LcdPrintMode = lastmod;
            }
        else
            {
            VID::PRM_L1 PrLine;
            GetLineParam_msk (Dest_X, wdth, &PrLine);
            if (!PrLine.Mask_L && !PrLine.CntMxFF & !PrLine.Mask_R) break;

            GetMaskAndOffset (Dest_X, Dest_Y, &DestAdr, &Dest_Mask);
            GetMaskAndOffset (Dest_X, Src_Y , &SrcAdr, &Src_Mask);
            if (PrLine.Mask_L)
                {
                unsigned char Dat = ((unsigned char*)DestAdr)[0] & (0xFF - PrLine.Mask_L);
                ((unsigned char*)DestAdr)[0] = Dat | (((unsigned char*)SrcAdr)[0] & PrLine.Mask_L);
                SrcAdr++;
                DestAdr++;
                }
            while (PrLine.CntMxFF)
                {
                ((unsigned char*)DestAdr)[0] = ((unsigned char*)SrcAdr)[0];
                SrcAdr++;
                DestAdr++;
                PrLine.CntMxFF--;
                }
            if (PrLine.Mask_R)
                {
                unsigned char Dat = ((unsigned char*)DestAdr)[0] & (0xFF - PrLine.Mask_R);
                ((unsigned char*)DestAdr)[0] = Dat | (((unsigned char*)SrcAdr)[0] & PrLine.Mask_R);
                }
            }
    } while (0);
}




void TLCDCANVABW::RotateRect_Up (VID::TMRect *lrct1, unsigned long steprout)
{
VID::TMRect Rct1 = lrct1[0];
if (steprout)
    {
    unsigned long Y_PhLast = Rct1.Y + Rct1.Height;
    unsigned long Y_dest = lrct1->Y;
    unsigned long Y_ScrLine = Rct1.Y + steprout;
    if (Y_PhLast > Y_ScrLine)
        {
        unsigned long Hcnt = Y_PhLast - Y_ScrLine;
        while (Hcnt)    // переместить данные
            {
            MoveHorisontalLine (lrct1->X, Y_dest, Y_ScrLine, lrct1->Width);
            Y_dest++;
            Y_ScrLine++;
            Hcnt--;
            }
        // стереть нижнюю часть экрана
        unsigned char lastmod = LcdPrintMode;
        long LsttX = lrct1->X + lrct1->Width - 1;
        LcdPrintMode = VID::PGClear;
        while (steprout)
            {
            DrawLine (lrct1->X, Y_dest, LsttX, Y_dest);
            Y_dest++;
            steprout--;
            }
        LcdPrintMode = lastmod;
        }
    }
}




void TLCDCANVABW::PrintString_W (char *lpString, unsigned short wddttpixels)
{
if (lpString)
	{
    unsigned short locWpix = 0;
    char ddt;
    while (true)
    	{
        ddt = lpString[0];
        if (!ddt) break;
        locWpix = locWpix + GetDrawMaskWidth (ddt);
        if (locWpix > wddttpixels) break;
        DrawCharSlow (ddt);
        lpString++;
        }
    }
}



void TLCDCANVABW::DrawCursor (VID::ECURSSTYLE stile, unsigned char Wcurrs)
{
VID::TMRect rct;
switch (stile)
	{
    case VID::CRS_GRAY:
    	{
        break;
        }
    default:
    	{
        break;
        }
    }
rct.X = lastX;
rct.Y = lastY;
rct.Width = Wcurrs;
rct.Height = GetFontHeight ();
FillRectGray_Light (&rct);
}



void TLCDCANVABW::PrintString_W_Cursor (char *lpString, unsigned short wddtt_pixels, unsigned char Curs)
{
if (lpString)
	{
    unsigned short locWpix = 0;
    char ddt;
    unsigned char curdrW;
    unsigned short Indx = 0;
    unsigned char lastmod = LcdPrintMode;
    SetOverMode (1);
    while (true)
    	{
        ddt = lpString[Indx];
        if (!ddt) break;
        curdrW = GetDrawMaskWidth (ddt);
        locWpix = locWpix + curdrW;
        if (locWpix > wddtt_pixels) break;
        if (Curs)
        	{
        	if (Indx == (Curs - 1))
            	{
            	DrawCursor (VID::CRS_GRAY, curdrW);
                }
            }
        DrawCharSlow (ddt);
        Indx++;
        }
    LcdPrintMode = lastmod;
    }
}





