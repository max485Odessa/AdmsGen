#ifndef _H_TIMERS_INTERRUPT_CLASS_H_
#define _H_TIMERS_INTERRUPT_CLASS_H_



#include "stm32f4xx_hal.h"
#include "hard_rut.h"



//enum ESYSTIM {ESYSTIM_TIM2 = 0, ESYSTIM_TIM5 = 1, ESYSTIM_ENDENUM = 2};
enum EPWMCHNL {EPWMCHNL_PWM1 = 0, EPWMCHNL_PWM2 = 1, EPWMCHNL_PWM3 = 2, EPWMCHNL_PWM4 = 3, EPWMCHNL_UPDATE = 4, EPWMCHNL_ENDENUM = 5};
#define C_RECTIFIRE_TIM ESYSTIM_TIM2



class ITIM_ISR {
		bool f_active_isr[EPWMCHNL_ENDENUM];
		uint32_t a_pwmvalue[EPWMCHNL_ENDENUM];
		TIM_HandleTypeDef TimHandle;
		ESYSTIM tim_ix;
	
	protected:

		void timer_init (uint32_t period, uint32_t hz);
		virtual void tim_comp_cb_isr (ESYSTIM t, EPWMCHNL ch) = 0;		// isr context executed
	
		void clr_tim ();		// очистить таймер 
		uint32_t get_timer_counter ();
		void enable_timer_isr (bool st);
		void enable_timer_oc (EPWMCHNL c, bool state);
		void set_timer_oc_value (EPWMCHNL c, uint32_t v);
		
		
		ITIM_ISR (ESYSTIM t);
	
	public:
		
		void isr_tim ();
};



#endif
