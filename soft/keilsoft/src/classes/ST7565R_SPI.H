#ifndef __H_ST7565R_SPI_H__
#define __H_ST7565R_SPI_H__



#include "LCD132x64.h"
#include "hard_rut.h"
#include "SYSBIOS.H"


extern void Delay_us (unsigned short timss);



const unsigned char C_ST7565R_DELAY_DATA_WR = 1;
const unsigned char C_ST7565R_DELAY_CMD_WR = 1;
const unsigned char C_ST7565R_DELAY_CLK = 10;
const unsigned char C_LCD_PAGE_AMOUNT = LCD_HEIGHT_PIXELS / 8;

enum EST7565P {EST7565P_CS = 0, EST7565P_REST, EST7565P_RS, EST7565P_SCL, EST7565P_SI, EST7565P_ENDENUM = 5};


class TST7565RSPI {
	private:
		S_GPIOPIN *rawpins;
		//static const S_GPIOPIN rawpins[EST7565P_ENDENUM];
		void SpiModeInit ();
		void spi_send (unsigned char bSend);
		void BitStrobeTx (bool val);
		void CSSetTo (bool val);
		void SetDataTo (bool val);
		void RS_To (bool val);
		void StrobeCLK ();
		void ResetTo (bool val);
		
	protected:
		void spi_cmd_single (unsigned char bSend);
		void spi_data_wr (unsigned char bSend);
		char DataBuf[LCD_WIDTH_PIXELS];
		
	public:
		TST7565RSPI (S_GPIOPIN *p);
		void LCD_init (void);
		void Contrast (unsigned char bSend);
		void Set_Page (unsigned char page);
		void Write_Data (char *lSrc, unsigned char sizes);
		void Fill_Data (unsigned char Fdat, unsigned char sizes);
		void LCD_Clear (void);
		void RefreshAllDisplay (TLCDCANVABW *lCanv);
		void RefreshAllDisplay_L (TLCDCANVABW *lCanv);
};




#endif

