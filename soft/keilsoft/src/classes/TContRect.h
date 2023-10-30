#ifndef _H_CONTROL_RECTIFIER_H_
#define _H_CONTROL_RECTIFIER_H_



#include "stm32f4xx_hal.h"
#include "hard_rut.h"
#include "IEXTINT.h"
#include "ITIMINT.h"
#include "SYSBIOS.H"
#include "TFTASKIF.h"



enum ERPMSW {ERPMSW_NONE = 0, ERPMSW_FRONTE_0_360_ANGLE,  ERPMSW_ENDENUM};		// ERPMSW_FALL_180_ANGLE, ERPMSW_RESYNC,




class TCONTRECT: public ITIM_ISR, public IEXTINT_ISR, public TFFC, public SYSBIOS::Periodic {
		uint32_t local_rpm_tick_counter;
		uint32_t result_rpm_tick_counter;
		virtual void periodic_cb (SYSBIOS::HPERIOD h) override;
		SYSBIOS::HPERIOD rpm_handler;
	
		uint32_t tick_delta_360_Angle;
		uint32_t tick_0_angle;					// rise
		uint32_t tick_180_angle;				// rise

		uint32_t speed_start_sys;			// for 180 angle tick period
		uint32_t speed_stop_sys;			// for 180 angle tick period
		bool f_speed_control_enabled;
	
		uint32_t tick_phase_rectifier_sys_on_a;			// for 180 angle tick period
		uint32_t tick_phase_rectifier_sys_off_a;			// for 180 angle tick period
		uint32_t tick_phase_rectifier_sys_on_b;			// for 180 angle tick period
		uint32_t tick_phase_rectifier_sys_off_b;			// for 180 angle tick period
		//bool f_phase_value_new_sys;
		uint32_t tick_phase_rectifier_isr_on_a;				// for 180 angle tick period
		uint32_t tick_phase_rectifier_isr_off_a;			// for 180 angle tick period
		uint32_t tick_phase_rectifier_isr_on_b;				// for 180 angle tick period
		uint32_t tick_phase_rectifier_isr_off_b;			// for 180 angle tick period
		//bool f_phase_value_new_isr;
		
		bool f_phase_control_active;
	
		bool f_oc_interupt_enabled;
		void oc_control_enabled (bool val);
	
		SYSBIOS::Timer timer_timeout_ext_isr;
	
		float program_rect_angle_on;
		float program_rect_angle_off;
	
		ERPMSW rpmcontr_sw;
		void update_phase_points (uint32_t tcnreg);
	
		S_GPIOPIN *c_pin_out;
		
		void gpio_rectifier_enable (bool val);
		//bool f_is_new_control_angle;
	
		virtual void tim_comp_cb_isr (ESYSTIM t, EPWMCHNL ch) override;
		virtual void isr_gpio_cb_int (uint8_t isr_n, bool pinstate) override;
		const uint8_t c_magnet_cnt;
		virtual void Task () override;
	
		
	protected:
	
		bool f_rectifier_system_enabled;
	
	public:
		TCONTRECT (S_GPIOPIN *pnout, S_GPIOPIN *pnin, ESYSTIM tt, uint32_t freq_hz, uint8_t cmagcnt);
	
		float get_rpm ();
		void control_rectifier (float strt_angl, float stp_angl);
		void rectifier_enabled (bool val);
		float get_angle_now ();
		float get_freq ();
};



#endif

