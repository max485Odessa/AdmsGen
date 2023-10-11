//#pragma once
#ifndef MMFNTT_
#define MMFNTT_

#pragma pack(push, 1)


// Структура файла


// Стуктура символьного буфера
struct MaxChar
{
	int Width;	// Ширина символа в пиксилях
};


typedef struct {
	unsigned char Width;	// Ширина символа в пиксилях
} MicroFWidth;


// Структура идёт с начала файла
struct MaxFont
{
	char Label[8];							// Спецальная метка "MaxFonts"
	int Height;								// Высота всех символов данного шрифта  в пиксилях.
	int OffsetFromBeginFileToMaxChar[256];	// Масив смещений от начала файла структур MaxChar.
};



typedef struct {
	unsigned long Height;		       // Высота всех символов данного шрифта  в пиксилях.
	unsigned long OffsetBitMask[256];      // Масив смещений от начала файла структур MaxChar.
} MaxFontMicro;
#pragma pack(pop)

#endif


