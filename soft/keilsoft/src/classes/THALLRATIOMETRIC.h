#ifndef _H_STM32HALL_LINEARHALL_EFFECT_SENSORS_H_
#define _H_STM32HALL_LINEARHALL_EFFECT_SENSORS_H_



// UGN3503UA, AH3503, SS49, DRV5055
// два сенсора сдвинуты друг от друга на 90 градусов
/*       A       B
0-89    (0 or +, +)
90-179  (+, 0 or -)
180-269	(0 or -, -)
270-359 (-, 0 or +)
*/
enum ESPOLE {ESPOLE_180_269 = 0, ESPOLE_90_179 = 1, ESPOLE_270_359 = 2, ESPOLE_0_89 = 3};



#include "stm32f4xx_hal.h"
#include "hard_rut.h"
#include "IEXTINT.h"
#include "ITIMINT.h"
#include "TFTASKIF.h"
#include "SYSBIOS.H"



enum EHALLSENS {EHALLSENS_A = 0, EHALLSENS_B = 1, EHALLSENS_ENDENUM};
enum EHALLPOINT {EHALLPOINT_START = 0, EHALLPOINT_STOP = 1, EHALLPOINT_ENDENUM};
enum EHALLCTRLPOS {EHALLCTRLPOS_SYNC, EHALLCTRLPOS_INTERNAL, EHALLCTRLPOS_EXTERNAL, EHALLCTRLPOS_ENDENUM};

#define C_ASINTAB_SIZE 2048

class IFHALL {
	public:
		virtual void cb_ifhall (float angl, EHALLPOINT ps) = 0;
};



class TLINEARHALL: public TFFC, public ITIMCB {
		uint16_t *ain_sens_a;
		uint16_t *ain_sens_b;
		const float ain_quant_volt;
		const float sens_mlvolt_g;
	
		uint16_t c_peack_gaus;
		uint16_t c_zero_gaus;
	
		IFHALL *cb;
		ESYSTIM etim;
		bool f_enabled;
	
		float calc_angle (uint16_t raw_a, uint16_t raw_b);
		float asingen (float sval);
		void asintab_generate ();
		float *asintab;
	
		virtual void Task () override;
		virtual void tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch) override;
		ITIM_ISR *timisr;
	
		EHALLCTRLPOS ctrlpos;
	
	public:
		TLINEARHALL (uint16_t *adrdata_sa, uint16_t *adrdata_sb, float quant_volt, float mvgaus, IFHALL *c, ESYSTIM t);
		void setbaund (uint16_t min, uint16_t max);
		void setangle_start (float angl);
		void setangle_stop (float angl);
		void enabled (bool v);
};



#endif
