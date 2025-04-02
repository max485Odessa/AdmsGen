#ifndef _H_STM32HALL_AIN_CLASS_H_
#define _H_STM32HALL_AIN_CLASS_H_



#include "hard_rut.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"



enum EAINCH {EAINCH_IN_A = 0, EAINCH_OUT = 1, EAINCH_IN_B = 2,  EAINCH_VREF = 3, EAINCH_ENDENUM = 4};
enum EAINPIN {EAINPIN_IN_A = 0, EAINPIN_OUT = 1, EAINPIN_IN_B = 2, EAINPIN_ENDENUM = 3};



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
