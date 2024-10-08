#include "ST7565R_SPI.H"






TST7565RSPI::TST7565RSPI (S_GPIOPIN *p)
{
	rawpins = p;
	_pin_low_init_out_pp (rawpins, EST7565P_ENDENUM, EHRTGPIOSPEED_HI);
}



void TST7565RSPI::SetDataTo (bool val)
{
if (val)
	{
	rawpins[EST7565P_SI].port->BSRR = rawpins[EST7565P_SI].pin;	
	}
else
	{
	rawpins[EST7565P_SI].port->BSRR = rawpins[EST7565P_SI].pin << 16;	
	}
}



void TST7565RSPI::ResetTo (bool val)
{
if (val)
	{
	rawpins[EST7565P_REST].port->BSRR = rawpins[EST7565P_REST].pin;	
	}
else
	{
	rawpins[EST7565P_REST].port->BSRR = rawpins[EST7565P_REST].pin << 16;	
	}
}


void TST7565RSPI::BitStrobeTx (bool val)
{
SetDataTo (val);
//Delay_us (C_ST7565R_DELAY_CLK);
//StrobeCLK ();
rawpins[EST7565P_SCL].port->BSRR = rawpins[EST7565P_SCL].pin;	
rawpins[EST7565P_SCL].port->BSRR = rawpins[EST7565P_SCL].pin << 16;	
}



void TST7565RSPI::StrobeCLK ()
{
rawpins[EST7565P_SCL].port->BSRR = rawpins[EST7565P_SCL].pin;	
rawpins[EST7565P_SCL].port->BSRR = rawpins[EST7565P_SCL].pin << 16;	
//GPIOB->BSRR = GPIO_Pin_10;
//GPIOB->BRR = GPIO_Pin_10;
//GPIO_WriteBit (GPIOB, GPIO_Pin_10, Bit_SET);
//GPIO_WriteBit (GPIOB, GPIO_Pin_10, Bit_RESET);

}




void TST7565RSPI::CSSetTo (bool val)
{
if (val)
	{
	rawpins[EST7565P_CS].port->BSRR = rawpins[EST7565P_CS].pin;	
	//GPIOA->BSRR = GPIO_Pin_4;
	//GPIO_WriteBit (GPIOA, GPIO_Pin_4, Bit_SET);
	}
else
	{
	rawpins[EST7565P_CS].port->BSRR = rawpins[EST7565P_CS].pin << 16;	
	//GPIOA->BRR = GPIO_Pin_4;
	//GPIO_WriteBit (GPIOA, GPIO_Pin_4, Bit_RESET);
	}
//Delay_us (C_ST7565R_DELAY_CLK);
}



void TST7565RSPI::RS_To (bool val)
{
if (val)
	{
	rawpins[EST7565P_RS].port->BSRR = rawpins[EST7565P_RS].pin;	
	//GPIOA->BSRR = GPIO_Pin_7;
	//GPIO_WriteBit (GPIOA, GPIO_Pin_7, Bit_SET);
	}
else
	{
	rawpins[EST7565P_RS].port->BSRR = rawpins[EST7565P_RS].pin << 16;	
	//GPIO_WriteBit (GPIOA, GPIO_Pin_7, Bit_RESET);
	//GPIOA->BRR = GPIO_Pin_7;
	}
}



void TST7565RSPI::spi_send (unsigned char bSend)
{
unsigned char cnt = 8;
while (cnt)
	{
	BitStrobeTx (bSend & 128);
	bSend = bSend << 1;
	cnt--;
	}
}


/*
void TST7565RSPI::spi_cmd_param (unsigned char cmd, unsigned char params)
{
	RS_To (0);		// cmd reg set
	CSSetTo (0);
	spi_send (cmd);
	spi_send (params);
	CSSetTo (1);
	RS_To (1);
	Delay_us (C_ST7565R_DELAY_CMD_WR);
}
*/

void TST7565RSPI::spi_data_wr (unsigned char bSend)
{
	CSSetTo (0);
	spi_send (bSend);
	CSSetTo (1);
	//Delay_us (C_ST7565R_DELAY_CMD_WR);
}



void TST7565RSPI::spi_cmd_single (unsigned char bSend)
{
	RS_To (0);		// cmd reg set
	CSSetTo (0);
	spi_send (bSend);
	CSSetTo (1);
	RS_To (1);
	//Delay_us (C_ST7565R_DELAY_CMD_WR);
}



void TST7565RSPI::Contrast (unsigned char bSend)
{
spi_cmd_single (0x81);
spi_cmd_single (bSend >> 2);
}



void TST7565RSPI::SpiModeInit ()
{
	_pin_low_init_out_pp ((S_GPIOPIN*)&rawpins, EST7565P_ENDENUM, EHRTGPIOSPEED_HI);

	rawpins[EST7565P_SCL].port->BSRR = rawpins[EST7565P_SCL].pin << 16;			// clk = 0
	CSSetTo (1);
	RS_To (1);
	ResetTo (0);
	SYSBIOS::Wait (50);
	ResetTo (1);
	SYSBIOS::Wait (50);
}



void TST7565RSPI::LCD_init (void)
{
	SpiModeInit ();

  spi_cmd_single(0xA2);   
  spi_cmd_single(0xA1);                                                   
	spi_cmd_single(0x26);  
  spi_cmd_single(0x040);		          	             	           
  spi_cmd_single(0x0a1);		           
  spi_cmd_single(0x0c8);		   //spi_cmd_single(0x0c8),	            
  spi_cmd_single(0x0a6);		  // spi_cmd_single(0x0a6);	       
  spi_cmd_single(0x0a2);		              
  spi_cmd_single(0x02f);		            
	spi_cmd_single(0x010);
  
	SYSBIOS::Wait (50);
	//Delay_us (50000);
	
	spi_cmd_single(0xAF);
	spi_cmd_single(0xF8);
	spi_cmd_single(0);
		
	Contrast (90);
}



void TST7565RSPI::Write_Data (char *lSrc, unsigned char sizes)
{
if (lSrc && sizes)
	{
	lSrc += (sizes - 9);
	while (sizes)
		{
		spi_data_wr (lSrc[0]);
		lSrc--;
		sizes--;
		}
	}
}



void TST7565RSPI::Fill_Data (unsigned char Fdat, unsigned char sizes)
{
if (sizes)
	{
	while (sizes)
		{
		spi_data_wr (Fdat);
		sizes--;
		}
	}
}



void TST7565RSPI::LCD_Clear (void)
{
	unsigned char Indx = 0;
	while (Indx < C_LCD_PAGE_AMOUNT)
		{
		Set_Page (Indx);
		Fill_Data (0, LCD_WIDTH_PIXELS);
		Indx++;
		}
}



void TST7565RSPI::Set_Page (unsigned char page)
{
	spi_cmd_single( 0xB0 + (page & 0x0F));
	spi_cmd_single( 4);			// 0
	spi_cmd_single( 0x10);	// 0
}



void TST7565RSPI::RefreshAllDisplay (TLCDCANVABW *lCanv)
{
unsigned char Indx = 0;
unsigned long sz;
BUFPAR bpr;
bpr.lRam = (unsigned char*)DataBuf;
bpr.sizes = sizeof(DataBuf);
while (Indx < C_LCD_PAGE_AMOUNT)
	{
	sz = lCanv->CopyCanva_BLine (&bpr, Indx);
	if (sz)
		{
		Set_Page (Indx);
		Write_Data (DataBuf, sizeof(DataBuf));
		}
	Indx++;
	}
}



void TST7565RSPI::RefreshAllDisplay_L (TLCDCANVABW *lCanv)
{
static unsigned char Indx = 0;
unsigned long sz;
BUFPAR bpr;
bpr.lRam = (unsigned char*)DataBuf;
bpr.sizes = sizeof(DataBuf);

	sz = lCanv->CopyCanva_BLine (&bpr, Indx);
	if (sz)
		{
		Set_Page (Indx);
		Write_Data (DataBuf, sizeof(DataBuf));
		}
	Indx++;
	
if (Indx >= C_LCD_PAGE_AMOUNT) Indx = 0;
}





