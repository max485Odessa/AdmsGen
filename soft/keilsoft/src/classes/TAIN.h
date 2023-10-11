#ifndef _H_STM32HALL_AIN_CLASS_H_
#define _H_STM32HALL_AIN_CLASS_H_


#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"


enum EAINCH {EAINCH_VBUS1 = 0, EAINCH_VBUS2 = 1, EAINCH_VBAT = 2, EAINCH_JX = 3, EAINCH_JY = 4, EAINCH_VREF = 5, EAINCH_TEMP = 6, EAINCH_VREF2 = 7, EAINCH_ENDENUM = 8};
enum EAINPIN {EAINPIN_VBUS1 = 0, EAINPIN_VBUS2 = 1, EAINPIN_VBAT = 2, EAINPIN_JX = 3, EAINPIN_JY = 4, EAINPIN_ENDENUM = 5};

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
		
};


#endif
