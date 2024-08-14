#ifndef _H_TIMERS_INTERRUPT_CLASS_H_
#define _H_TIMERS_INTERRUPT_CLASS_H_



#include "stm32f4xx_hal.h"
#include "hard_rut.h"



//enum ESYSTIM {ESYSTIM_TIM2 = 0, ESYSTIM_TIM5 = 1, ESYSTIM_ENDENUM = 2};
enum EPWMCHNL {EPWMCHNL_PWM1 = 0, EPWMCHNL_PWM2 = 1, EPWMCHNL_PWM3 = 2, EPWMCHNL_PWM4 = 3, EPWMCHNL_UPDATE = 4, EPWMCHNL_ENDENUM = 5};
#define C_RECTIFIRE_TIM ESYSTIM_TIM2


class ITIMCB {
	public:
		virtual void tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch) = 0;
};



class TTIM_MKS_USER_A {
		void timer_init (uint32_t period, uint32_t hz);
		
	protected:
		const uint32_t c_value_period;
		const uint32_t c_value_freq;
		TIM_HandleTypeDef TimHandle;
		ESYSTIM tim_ix;
		void clr_tim ();
		
	
	public:
		TTIM_MKS_USER_A (ESYSTIM t, uint32_t prd, uint32_t fr);
		uint32_t get_timer_counter ();
		uint32_t get_delta (uint32_t prv, uint32_t cur);
		uint32_t get_period ();
		uint32_t get_freq ();
		ESYSTIM get_tim ();
};



class TTIM_MKS_ISR: public TTIM_MKS_USER_A {
		bool f_active_isr[EPWMCHNL_ENDENUM];
		bool f_one_short[EPWMCHNL_ENDENUM];
		uint32_t a_pwmvalue[EPWMCHNL_ENDENUM];
		
		ITIMCB *callback_user[EPWMCHNL_ENDENUM];
	
	protected:

	
	public:
		TTIM_MKS_ISR (ESYSTIM t, uint32_t prd, uint32_t fr);
		void set_tim_cb (EPWMCHNL c, ITIMCB *cb);
		void isr_tim ();
		
		void enable_timer_isr (bool st);
		void enable_timer_oc (EPWMCHNL c, bool state);
		void set_timer_oc_value (EPWMCHNL c, uint32_t v);
		void start_one_short (EPWMCHNL c, uint32_t dly_mks);
};



#endif
