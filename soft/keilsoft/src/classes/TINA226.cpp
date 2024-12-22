#include "TINA226.h"
#include <math.h>
#include <stdio.h>


uint16_t TTINA226::c_chip_cnt = 0;
uint16_t TTINA226::work_chip_index = 0;
utimer_t TTINA226::relax_time = 0;


TTINA226::TTINA226 (uint8_t adr, TI2CIFACE *cc_i2c, float shnt_v) : c_shunt_resistance (shnt_v)
{
	static bool f_one_shot = true;
	
	i2c_adress = 0x80 + ((adr & 0x0F)<<1);
	i2c = cc_i2c;
	configure (INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);		// INA226_MODE_SHUNT_BUS_CONT
	
	if (f_one_shot) {
		SYSBIOS::ADD_TIMER_SYS (&relax_time);
		f_one_shot = false;
		}
	f_is_ok = false;
	sw = EINAMOD_MANID;
	obj_chip_index = c_chip_cnt;
	c_chip_cnt++;
}



void TTINA226::next_chip_index ()
{
work_chip_index++;
if (work_chip_index >= c_chip_cnt) work_chip_index = 0;
}



bool TTINA226::writeRegister16 (uint8_t reg, uint16_t val)
{
	uint8_t dat_dst[2];
	dat_dst[0] = val >> 8;
	dat_dst[1] = (uint8_t)val;

	return i2c->WriteFrame_i2c (i2c_adress, reg, dat_dst, 2);
}



bool TTINA226::readRegister(uint8_t SrcReg, uint16_t &dst)
{
	uint8_t dat_dst[2];
	bool rv = i2c->ReadFrame_i2c (i2c_adress, SrcReg, dat_dst, 2);
	if (rv) {
		uint16_t value = dat_dst[0]; value <<= 8;
		value |= dat_dst[1];
		dst = value;
		}
	return rv;
}



bool TTINA226::readManufacturerID (uint16_t &id)
{
	bool rv = false;
	uint16_t data = 0;
	if (readRegister (INA226_MANUFACTURER, data)) 
		{
		id = data;
		rv = true;
		}
  return rv;
}



bool TTINA226::readDieID (uint16_t &id)
{
	bool rv = false;
	uint16_t data = 0;
	if (readRegister (INA226_DIE_ID, data)) 
		{
		id = data;
		rv = true;
		}
  return rv;
}



bool TTINA226::configure (ina226_averages_t avg, ina226_busConvTime_t busConvTime, ina226_shuntConvTime_t shuntConvTime, ina226_mode_t mode)
{
    uint16_t config = 0;
    config |= (avg << 9 | busConvTime << 6 | shuntConvTime << 3 | mode);
    //vBusMax = 36;
    //vShuntMax = 0.08192f;
    writeRegister16(INA226_REG_CONFIG, config);

    return true;
}


/*
bool TTINA226::readShuntCurrent (float &dst)
{
	bool rv = false;
	uint16_t dat = 0;
	if (readRegister(INA226_REG_CURRENT, dat)) {
		dst = (_current_LSB * dat);
		rv = true;
		}
	return rv;
}
*/

bool TTINA226::readShuntVoltage(float &dst)
{
	bool rv = false;
	uint16_t dat = 0;
	if (readRegister(INA226_REG_SHUNTVOLTAGE, dat)) {
		dst = (dat * 2.5e-6);
		rv = true;
		}
	return rv;
}



bool TTINA226::readBusVoltage(float &dst)
{
	bool rv = false;
	uint16_t dat = 0;
	if (readRegister(INA226_REG_BUSVOLTAGE, dat)) {
		dst = 0.00125 * dat;
		rv = true;
		}
	return rv;
}


/*
bool TTINA226::calibrate(float rShuntValue, float iMaxExpected)
{
    uint16_t calibrationValue;
    rShunt = rShuntValue;

    float iMaxPossible, minimumLSB;

    iMaxPossible = vShuntMax / rShunt;

    minimumLSB = iMaxExpected / 32767;

    currentLSB = (uint16_t)(minimumLSB * 100000000);
    currentLSB /= 100000000;
    currentLSB /= 0.0001;
    currentLSB = ceil(currentLSB);
    currentLSB *= 0.0001;

    powerLSB = currentLSB * 25;

    calibrationValue = (uint16_t)((0.00512) / (currentLSB * rShunt));

    writeRegister16(INA226_REG_CALIBRATION, calibrationValue);

    return true;
}
*/



float TTINA226::current ()
{
	return c_shunt;
}



float TTINA226::volt ()
{
	return v_bus;
}



uint16_t TTINA226::manID ()
{
	return data_manid;
}



uint16_t TTINA226::DieID ()
{
	return data_dieid;
}



float *TTINA226::adr_current ()
{
	return &c_shunt;
}



float *TTINA226::adr_volt ()
{
	return &v_bus;
}



bool TTINA226::is_ok ()
{
	return f_is_ok;
}



void TTINA226::Task ()
{
if (work_chip_index != obj_chip_index) return;
if (relax_time) return;
	
	switch (sw)
		{
		case EINAMOD_MANID:
			{
			if (!readManufacturerID (data_manid)) {
				f_is_ok = false;
				next_chip_index ();
				}
			else
				{
				sw = EINAMOD_DIEID;
				}
			relax_time = C_INARELAX_TIME;
			break;
			}
		case EINAMOD_DIEID:
			{
			if (!readDieID (data_dieid)) {
				f_is_ok = false;
				next_chip_index ();
				}
			else
				{
				sw = EINAMOD_VSHUNT;
				}
			break;
			}
		case EINAMOD_VSHUNT:
			{
			if (!readShuntVoltage (v_shunt)) {
				next_chip_index ();
				f_is_ok = false;
				}
			else
				{
				sw = EINAMOD_VBUS;
				}
			c_shunt = v_shunt / c_shunt_resistance;
			relax_time = C_INARELAX_TIME;
			break;
			}
		case EINAMOD_VBUS:
			{
			if (!readBusVoltage (v_bus)) 
				{
				next_chip_index ();
				f_is_ok = false;
				}
			else
				{
				f_is_ok = true;
				}
			sw = EINAMOD_VSHUNT;
			relax_time = C_INARELAX_TIME;
			break;
			}
		default:
			{
			sw = EINAMOD_MANID;
			break;
			}
		}
}


