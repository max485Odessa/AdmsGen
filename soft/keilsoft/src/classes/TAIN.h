#ifndef _H_STM32HALL_AIN_CLASS_H_
#define _H_STM32HALL_AIN_CLASS_H_



#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"



enum EAINCH {EAINCH_320V = 0, EAINCH_BAT = 1, EAINCH_SRC24V = 2,  EAINCH_VREF = 3, EAINCH_ENDENUM = 4};
enum EAINPIN {EAINPIN_320V = 0, EAINPIN_BAT = 1,  EAINPIN_SRC24V = 2,  EAINPIN_ENDENUM = 3};



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
