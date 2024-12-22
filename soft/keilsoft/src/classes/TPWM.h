#ifndef _H_STM32HALL_PWM_CLASS_H_
#define _H_STM32HALL_PWM_CLASS_H_


#include "SYSBIOS.H"
#include "TFTASKIF.h"
#include "stm32f4xx_hal.h"
#include "hard_rut.h"
#include "ITIMINT.h"

enum ETIMCH {ETIMCH_1 = 0, ETIMCH_2 = 1, ETIMCH_3 = 2, ETIMCH_4 = 3, EPWMCH_ENDENUM = 4};

typedef struct {
	S_GPIOPIN port;
	ETIMCH ch;
	#if (HRDCPU == 4)
		E_GPIO_AF af;
	#endif
} S_PWM_INIT_LIST_T;



class TPWMIFC {
	public:
		virtual void enable (bool v) = 0;
		virtual void set_pwm (float val) = 0;
};



typedef struct {
	uint32_t period;
	uint32_t freq_clk;
	ESYSTIM e_tim;
	TIM_HandleTypeDef    *TimHandle;
	bool f_is_32Bit;
} S_BASEPWM_INF_T;



class TPWMSCHAN: public TPWMIFC {
		virtual void enable (bool v) override;
		virtual void set_pwm (float val) override;
	
		uint32_t pwm;
		bool f_output_active;

		TIM_OC_InitTypeDef sConfig;
		const uint32_t C_CHAN_ID;
		S_BASEPWM_INF_T const &tim;
	
		uint32_t calculate_pwm (float val);
	
	public:
		TPWMSCHAN (S_BASEPWM_INF_T &t, uint32_t ch);
		
};



class TPWM  {
		static const uint32_t chanpwmlist[EPWMCH_ENDENUM];
		void init_base ();

		S_BASEPWM_INF_T info;
		TIM_HandleTypeDef    TimHandle;
	
		TPWMSCHAN *ch_array[EPWMCH_ENDENUM];
	protected:
		void tim_gpio_init (S_PWM_INIT_LIST_T *ls, uint8_t pn);
		
	
	public:
		TPWM (ESYSTIM t, uint32_t period, uint32_t hz_clk, S_PWM_INIT_LIST_T *ls, uint8_t pn);
		TPWMIFC *getChanel (ETIMCH c);
		void set_period (uint32_t mks);
};





#endif
