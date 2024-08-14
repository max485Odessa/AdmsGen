#ifndef _H_STM32HALL_AIN_CLASS_H_
#define _H_STM32HALL_AIN_CLASS_H_


#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"


enum EAINCH {EAINCH_VBUS_24V = 0, EAINCH_M_REFORCE_V = 1, EAINCH_G_REFORCE_V = 2, EAINCH_RATIO_A = 3, EAINCH_RATIO_B = 4, EAINCH_VREF = 5, EAINCH_TEMP = 6, EAINCH_VREF2 = 7, EAINCH_ENDENUM = 8};
enum EAINPIN {EAINPIN_VBUS_24V = 0, EAINPIN_M_REFORCE_V = 1, EAINPIN_G_REFORCE_V = 2, EAINPIN_RATIO_A = 3, EAINPIN_RATIO_B = 4, EAINPIN_ENDENUM = 5};

class TAIN: public TFFC {
		virtual void Task () override;
		SYSBIOS::Timer relax_timer;
		float quant_calc ();
		float vcc_calc ();
	
	protected:
		float quant_value;
		float vcc_value;
		float voltage[EAINCH_ENDENUM];
	
	public:
		TAIN ();
		float *adr_voltage (EAINPIN ch);
		uint16_t *adr_voltage_raw (EAINPIN ch);
		
};


#endif
