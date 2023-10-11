#ifndef _H_CONTROL_RECTIFIER_H_
#define _H_CONTROL_RECTIFIER_H_



#include "stm32f4xx_hal.h"
#include "hard_rut.h"
#include "IEXTINT.h"
#include "ITIMINT.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"



enum ERPMSW {ERPMSW_NONE = 0, ERPMSW_FRONTE_0_360_ANGLE, ERPMSW_FALL_180_ANGLE, ERPMSW_ENDENUM};




class TCONTRECT: public ITIM_ISR, public IEXTINT_ISR, public TFFC {

		uint32_t tick_delta_360_Angle;
		uint32_t tick_0_angle;					// rise
		uint32_t tick_180_angle;				// rise

		uint32_t speed_start_sys;			// for 180 angle tick period
		uint32_t speed_stop_sys;			// for 180 angle tick period
		bool f_speed_control_enabled;
	
		uint32_t tick_phase_rectifier_sys_on;			// for 180 angle tick period
		uint32_t tick_phase_rectifier_sys_off;			// for 180 angle tick period
		bool f_phase_value_new_sys;
		uint32_t tick_phase_rectifier_isr_on;				// for 180 angle tick period
		uint32_t tick_phase_rectifier_isr_off;			// for 180 angle tick period
		bool f_phase_value_new_isr;
		
		bool f_phase_control_active;
	
		SYSBIOS::Timer timer_timeout_ext_isr;
	
		float program_rect_angle_on;
		float program_rect_angle_off;
	
		ERPMSW rpmcontr_sw;
		void update_phase_points (uint32_t tcnreg);
	
		S_GPIOPIN *c_pin_out;
	
		virtual void tim_comp_cb_isr (ESYSTIM t, EPWMCHNL ch) override;
		virtual void isr_gpio_cb_int (uint8_t isr_n, bool pinstate) override;
		const uint8_t c_magnet_cnt;
		virtual void Task () override;
	
	protected:
	
		bool f_rect_enabled;
	
	public:
		TCONTRECT (S_GPIOPIN *pnout, S_GPIOPIN *pnin, ESYSTIM tt, uint32_t freq_hz, uint8_t cmagcnt);
	
		float get_rpm ();
		void control_rectifier (float strt_angl, float stp_angl);
		void rectifier_enabled (bool val);
		float get_angle_now ();
		
};



#endif

