#ifndef _H_STM32_STDLIB_INA226_H_
#define _H_STM32_STDLIB_INA226_H_

#include "TFTASKIF.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
#include "hard_rut.h"
#include "I2CSOFTWARE.H"
#include "SYSBIOS.H"


#define INA226_REG_CONFIG           0x00
#define INA226_REG_SHUNTVOLTAGE     0x01
#define INA226_REG_BUSVOLTAGE       0x02
#define INA226_REG_POWER            0x03
#define INA226_REG_CURRENT          0x04
#define INA226_REG_CALIBRATION      0x05
#define INA226_REG_MASKENABLE       0x06
#define INA226_REG_ALERTLIMIT       0x07
#define INA226_MANUFACTURER         0xFE
#define INA226_DIE_ID               0xFF

#define INA226_BIT_SOL              (0x8000)
#define INA226_BIT_SUL              (0x4000)
#define INA226_BIT_BOL              (0x2000)
#define INA226_BIT_BUL              (0x1000)
#define INA226_BIT_POL              (0x0800)
#define INA226_BIT_CNVR             (0x0400)
#define INA226_BIT_AFF              (0x0010)
#define INA226_BIT_CVRF             (0x0008)
#define INA226_BIT_OVF              (0x0004)
#define INA226_BIT_APOL             (0x0002)
#define INA226_BIT_LEN              (0x0001)

typedef enum
{
    INA226_AVERAGES_1             = 0,
    INA226_AVERAGES_4             = 1,
    INA226_AVERAGES_16            = 2,
    INA226_AVERAGES_64            = 3,
    INA226_AVERAGES_128           = 4,
    INA226_AVERAGES_256           = 5,
    INA226_AVERAGES_512           = 6,
    INA226_AVERAGES_1024          = 7
} ina226_averages_t;

typedef enum
{
    INA226_BUS_CONV_TIME_140US    = 0,
    INA226_BUS_CONV_TIME_204US    = 1,
    INA226_BUS_CONV_TIME_332US    = 2,
    INA226_BUS_CONV_TIME_588US    = 3,
    INA226_BUS_CONV_TIME_1100US   = 4,
    INA226_BUS_CONV_TIME_2116US   = 5,
    INA226_BUS_CONV_TIME_4156US   = 6,
    INA226_BUS_CONV_TIME_8244US   = 7
} ina226_busConvTime_t;

typedef enum
{
    INA226_SHUNT_CONV_TIME_140US   = 0,
    INA226_SHUNT_CONV_TIME_204US   = 1,
    INA226_SHUNT_CONV_TIME_332US   = 2,
    INA226_SHUNT_CONV_TIME_588US   = 3,
    INA226_SHUNT_CONV_TIME_1100US  = 4,
    INA226_SHUNT_CONV_TIME_2116US  = 5,
    INA226_SHUNT_CONV_TIME_4156US  = 6,
    INA226_SHUNT_CONV_TIME_8244US  = 7
} ina226_shuntConvTime_t;

typedef enum
{
    INA226_MODE_POWER_DOWN      = 0,
    INA226_MODE_SHUNT_TRIG      = 1,
    INA226_MODE_BUS_TRIG        = 2,
    INA226_MODE_SHUNT_BUS_TRIG  = 3,
    INA226_MODE_ADC_OFF         = 4,
    INA226_MODE_SHUNT_CONT      = 5,
    INA226_MODE_BUS_CONT        = 6,
    INA226_MODE_SHUNT_BUS_CONT  = 7,
} ina226_mode_t;



enum EINAMOD {EINAMOD_MANID = 0, EINAMOD_DIEID, EINAMOD_VSHUNT, EINAMOD_VBUS, EINAMOD_ENDENUM}; // EINAMOD_CSHUNT
#define C_INARELAX_TIME 100

class TTINA226 : public TFFC {
	
		TI2CIFACE *i2c;
		uint8_t i2c_adress;
		bool readRegister(uint8_t reg, uint16_t &dst);
		bool writeRegister16 (uint8_t reg, uint16_t val);
		virtual void Task ();
		static utimer_t relax_time;
	
		const float c_shunt_resistance;
	
		float v_shunt;
		float v_bus;
		float c_shunt;
		bool f_is_ok;
		uint16_t data_manid;
		uint16_t data_dieid;
	
		EINAMOD sw;
		bool setMaxCurrentShunt (float maxCurrent, float shunt, bool normalize);
		bool readShuntCurrent (float &dst);
		bool readShuntVoltage(float &dst);
		bool readBusVoltage(float &dst);
		bool readDieID (uint16_t &id);
		bool readManufacturerID (uint16_t &id);
		
		static uint16_t c_chip_cnt;
		static uint16_t work_chip_index;
		static void next_chip_index ();
		uint16_t obj_chip_index;
		
	protected:
		
	
	public:
		TTINA226 (uint8_t adr, TI2CIFACE *cc_i2c, float shnt_v);
		bool configure (ina226_averages_t avg, ina226_busConvTime_t busConvTime, ina226_shuntConvTime_t shuntConvTime, ina226_mode_t mode);
		bool is_ok ();
		float current ();
		float volt ();
		uint16_t manID ();
		uint16_t DieID ();
	
		float *adr_current ();
		float *adr_volt ();
};


#endif


