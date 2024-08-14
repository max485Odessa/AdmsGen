#include "I2CSOFTWARE.H"



TI2CIFACE::TI2CIFACE (const S_GPIOPIN *p, uint8_t w) : c_i2c_wait (w)
{
	pins = p;
	_pin_low_init_out_od (const_cast<S_GPIOPIN*>(pins), EARRI2CPIN_ENDENUM, EHRTGPIOSPEED_HI);
}



void TI2CIFACE::I2C_INIT()
{
	pins[EARRI2CPIN_SCK].port->BSRR = pins[EARRI2CPIN_SCK].pin;
	pins[EARRI2CPIN_SDA].port->BSRR = pins[EARRI2CPIN_SDA].pin;
}



void TI2CIFACE::I2C_SDA_1 ()
{
	pins[EARRI2CPIN_SDA].port->BSRR = pins[EARRI2CPIN_SDA].pin;
}



void TI2CIFACE::I2C_SDA_0 ()
{
	pins[EARRI2CPIN_SDA].port->BSRR = pins[EARRI2CPIN_SDA].pin << 16;
}



void TI2CIFACE::I2C_SCK_1 ()
{
	pins[EARRI2CPIN_SCK].port->BSRR = pins[EARRI2CPIN_SCK].pin;
}



void TI2CIFACE::I2C_SCK_0 ()
{
	pins[EARRI2CPIN_SCK].port->BSRR = pins[EARRI2CPIN_SCK].pin << 16;
}



void TI2CIFACE::I2C_SDA_OUT ()
{
	I2C_SDA_1 ();
}



void TI2CIFACE::I2C_SDA_IN ()
{
	I2C_SDA_1 ();
}


	
bool TI2CIFACE::I2C_SDA_READ ()
{
	bool rv = false;
	if ((pins[EARRI2CPIN_SDA].port->IDR & pins[EARRI2CPIN_SDA].pin)) rv = true;
	return rv;
}



bool TI2CIFACE::I2C_SCL_READ ()
{
	bool rv = false;
	if ((pins[EARRI2CPIN_SCK].port->IDR & pins[EARRI2CPIN_SCK].pin)) rv = true;
	return rv;
}


void TI2CIFACE::Pause_I2C ()
{
volatile unsigned char cnt = c_i2c_wait;	// 50 3 текущая пауза на 330 кгц настроена  было 10
while (cnt)
	{
	cnt--;
	}
}



void TI2CIFACE::Start_I2C()
{
Pause_I2C ();
I2C_SDA_1 ();
Pause_I2C ();
I2C_SCK_1 ();
Pause_I2C ();
I2C_SDA_0 ();
Pause_I2C ();
I2C_SCK_0 ();
Pause_I2C ();
}



void TI2CIFACE::Stop_I2C ()
{
I2C_SCK_0 ();
I2C_SDA_0 ();
Pause_I2C ();
I2C_SCK_1 ();
Pause_I2C ();
I2C_SDA_1 ();
Pause_I2C ();
}



void TI2CIFACE::I2CPulse ()
{
Pause_I2C ();
I2C_SCK_0 ();	
Pause_I2C ();
I2C_SCK_1 ();
Pause_I2C ();
I2C_SCK_0 ();
Pause_I2C ();
}



bool TI2CIFACE::DataOut_I2C (unsigned char datas)
{
unsigned char cnwh=8;
I2C_SDA_OUT ();
bool I2C_ASK = false;
while (cnwh)
	{
	if (datas & 128)
		{
		I2C_SDA_1 ();
		}
	else
		{
		I2C_SDA_0 ();
		}
	datas=datas<<1;
	I2CPulse ();
	cnwh--;
	}
I2C_SDA_IN ();
//I2C_SDA_0 ();
Pause_I2C ();
I2C_SCK_1 ();
Pause_I2C ();
if (I2C_SDA_READ()) I2C_ASK = true;
I2C_SCK_0 ();
Pause_I2C ();
I2C_SDA_1 ();
I2C_SDA_OUT ();
Pause_I2C ();
return I2C_ASK;
}



bool TI2CIFACE::DataOut_SMBUS (unsigned char datas)
{
unsigned char cnwh=8;
I2C_SDA_OUT ();
bool I2C_ASK = false;
while (cnwh)
	{
	if (datas & 128)
		{
		I2C_SDA_1 ();
		}
	else
		{
		I2C_SDA_0 ();
		}
	datas=datas<<1;
		
	Pause_I2C ();
	I2C_SCK_1 ();
	Pause_I2C ();
	
	if (!I2C_SCL_READ())
		{
		// wait
		uint32_t lasttick = SYSBIOS::GetTickCountLong ();
		while (!I2C_SCL_READ())
			{
			if ((SYSBIOS::GetTickCountLong () - lasttick) > C_SMBUS_TIMEOUT) return true; //break;
			}
		Pause_I2C ();
		}
	
	I2C_SCK_0 ();
	Pause_I2C ();
	cnwh--;
	}
I2C_SDA_IN ();
Pause_I2C ();
I2C_SCK_1 ();
Pause_I2C ();
// check sck wait state
if (!I2C_SCL_READ())
	{
	// wait
	uint32_t lasttick = SYSBIOS::GetTickCountLong ();
	while (!I2C_SCL_READ())
		{
		if ((SYSBIOS::GetTickCountLong () - lasttick) > C_SMBUS_TIMEOUT) return true;//break;
		}
	Pause_I2C ();
	}
	
if (I2C_SDA_READ()) I2C_ASK = true;
I2C_SCK_0 ();
Pause_I2C ();
I2C_SDA_1 ();
I2C_SDA_OUT ();
Pause_I2C ();
return I2C_ASK;
}



uint8_t TI2CIFACE::DataIn_SMBUS (char RawHardAsk)
{
uint8_t rv = 0;
uint8_t cnwh=8;
I2C_SDA_IN ();
while (cnwh)
	{
	rv = rv << 1;
	I2C_SCK_1 ();
	Pause_I2C ();
	
	if (!I2C_SCL_READ())
		{
		// wait
		uint32_t lasttick = SYSBIOS::GetTickCountLong ();
		while (!I2C_SCL_READ())
			{
			if ((SYSBIOS::GetTickCountLong () - lasttick) > C_SMBUS_TIMEOUT) break;
			}
		Pause_I2C ();
		}
	
	if (I2C_SDA_READ()) rv = rv | 1;
	I2C_SCK_0 ();
	Pause_I2C ();
	cnwh--;
	}
I2C_SDA_OUT ();
if (RawHardAsk)
	{
	I2C_SDA_1 ();
	}
else
	{
	I2C_SDA_0 ();
	}
Pause_I2C ();
I2C_SCK_1 ();
Pause_I2C ();
	if (!I2C_SCL_READ())
		{
		// wait
		uint32_t lasttick = SYSBIOS::GetTickCountLong ();
		while (!I2C_SCL_READ())
			{
			if ((SYSBIOS::GetTickCountLong () - lasttick) > C_SMBUS_TIMEOUT) break;
			}
		Pause_I2C ();
		}
I2C_SCK_0 ();
Pause_I2C ();
I2C_SDA_IN ();
Pause_I2C ();
return rv;
}




unsigned char TI2CIFACE::DataIn_I2C (char RawHardAsk)
{
unsigned char rv = 0;
unsigned char cnwh=8;
I2C_SDA_IN ();
while (cnwh)
	{
	rv = rv << 1;
	I2C_SCK_1 ();
	Pause_I2C ();
	if (I2C_SDA_READ()) rv = rv | 1;
	I2C_SCK_0 ();
	Pause_I2C ();
	cnwh--;
	}
I2C_SDA_OUT ();
if (RawHardAsk)
	{
	I2C_SDA_1 ();
	}
else
	{
	I2C_SDA_0 ();
	}
Pause_I2C ();
I2C_SCK_1 ();
Pause_I2C ();
I2C_SCK_0 ();
Pause_I2C ();
I2C_SDA_IN ();
Pause_I2C ();
return rv;
}








/*
// без сдвига 8 бит
void TI2CIFACE::SetSlaveAdress (unsigned char dev_adr)
{
	DEVADRESS = dev_adr;
}
*/


bool TI2CIFACE::WriteFrame_i2c (unsigned char DEVADRESS, unsigned char DestReg, unsigned char *lTx, unsigned short tx_size)
{
	bool rv = false;
	if (tx_size)
		{
		//DEVADRESS = DEVADRESS << 1;
		Start_I2C ();
		do {
				if (DataOut_I2C (DEVADRESS)) break;		// WRITE BIT
				if (DataOut_I2C (DestReg)) break;
				while (tx_size)
					{
					if (DataOut_I2C (lTx[0])) break;
					lTx++;
					tx_size--;
					}
				if (!tx_size) rv = true;
				} while (false);
		Stop_I2C ();
		}
	return rv;
}



bool TI2CIFACE::ReadFrame_i2c (unsigned char DEVADRESS, unsigned char SrcReg, unsigned char *lDest, unsigned short rx_size)
{
bool rv = false;
	if (rx_size)
		{
		Start_I2C ();
		do {
				bool ack_level = false;
				if (DataOut_I2C (DEVADRESS)) break;		// WRITE BIT
				if (DataOut_I2C (SrcReg)) break;
				Start_I2C ();
				if (DataOut_I2C (DEVADRESS | 1)) break;;		// READ BIT
				while (rx_size)
					{
					if (rx_size == 1) ack_level = true;
					lDest[0] = DataIn_I2C (ack_level);
					lDest++;
					rx_size--;
					}
				rv = true;
				} while (false);
		Stop_I2C ();
		}
	return rv;
}




bool TI2CIFACE::ReadFrame_smbus (unsigned char DEVADRESS, unsigned char SrcReg, unsigned char *lDest, unsigned short rx_size)
{
bool rv = false;
	if (rx_size)
		{
		//DEVADRESS = DEVADRESS << 1;
		Start_I2C ();
		do {
				bool ack_level = false;
				if (DataOut_SMBUS (DEVADRESS)) break;		// WRITE BIT
				if (DataOut_SMBUS (SrcReg)) break;
				Start_I2C ();
				if (DataOut_SMBUS (DEVADRESS | 1)) break;;		// READ BIT
				while (rx_size)
					{
					//if (rx_size == 1) ack_level = true;
					lDest[0] = DataIn_SMBUS (ack_level);
					lDest++;
					rx_size--;
					}
				rv = true;
				} while (false);
		Stop_I2C ();
		}
	return rv;
}



bool TI2CIFACE::WriteFrame_smbus (unsigned char DEVADRESS, unsigned char DestReg, unsigned char *lTx, unsigned short tx_size)
{
	bool rv = false;
	if (tx_size)
		{
		//DEVADRESS = DEVADRESS << 1;
		Start_I2C ();
		do {
				if (DataOut_SMBUS (DEVADRESS)) break;		// WRITE BIT
				if (DataOut_SMBUS (DestReg)) break;
				while (tx_size)
					{
					if (DataOut_SMBUS (lTx[0])) break;
					lTx++;
					tx_size--;
					}
				if (!tx_size) rv = true;
				} while (false);
		Stop_I2C ();
		}
	return rv;
}


