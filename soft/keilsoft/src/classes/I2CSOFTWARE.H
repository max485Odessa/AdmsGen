#ifndef __H_I2C_SOFTWARE_H__
#define __H_I2C_SOFTWARE_H__

#include <stdint.h>
#include "SYSBIOS.H"
#include "stm32f4xx_hal.h"
#include "I2CSOFTWARE.H"
#include "hard_rut.h"

enum EARRI2CPIN {EARRI2CPIN_SCK = 0, EARRI2CPIN_SDA = 1, EARRI2CPIN_ENDENUM = 2};


#define C_SMBUS_TIMEOUT (25+1)

class TI2CIFACE {
	private:
		const uint8_t c_i2c_wait;
		void Pause_I2C ();
		void I2CPulse ();
		const S_GPIOPIN *pins;
		void Hard_Init ();
		
	protected:
		void I2C_INIT ();
		void I2C_SDA_1 ();
		void I2C_SDA_0 ();
		void I2C_SCK_1 ();
		void I2C_SCK_0 ();
		void I2C_SDA_OUT ();
		void I2C_SDA_IN ();
		bool I2C_SDA_READ ();	
		bool I2C_SCL_READ ();	
	
	public:
		TI2CIFACE (const S_GPIOPIN *p, uint8_t wait_us);
		
		void Start_I2C();
		void Stop_I2C ();
		bool DataOut_I2C (unsigned char datas);
		bool DataOut_SMBUS (unsigned char datas);
		uint8_t DataIn_SMBUS (char RawHardAsk);
		unsigned char DataIn_I2C (char RawHardAsk);

		bool WriteFrame_i2c (unsigned char DEVADRESS, unsigned char DestReg, unsigned char *lTx, unsigned short tx_size);
		bool ReadFrame_i2c (unsigned char DEVADRESS, unsigned char SrcReg, unsigned char *lRx, unsigned short rx_size);
		bool ReadFrame_smbus (unsigned char DEVADRESS, unsigned char SrcReg, unsigned char *lDest, unsigned short rx_size);
		bool WriteFrame_smbus (unsigned char DEVADRESS, unsigned char DstReg, unsigned char *l_src, unsigned short tx_size);
		
	
};




#endif


