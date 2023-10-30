#include "TcontRect.h"
#include "TGlobalISR.h"


TCONTRECT::TCONTRECT (S_GPIOPIN *pnout, S_GPIOPIN *pnin, ESYSTIM tt, uint32_t freq_hz, uint8_t cmagcnt) : c_magnet_cnt (cmagcnt), IEXTINT_ISR(pnin, EGPINTMOD_RISING), ITIM_ISR (tt)
{
	//f_is_new_control_angle = false;
	timer_init (0xFFFFFFFF, freq_hz);
	enable_timer_oc (EPWMCHNL_PWM1, true);
	enable_timer_oc (EPWMCHNL_PWM2, true);
	enable_timer_oc (EPWMCHNL_PWM3, true);
	enable_timer_oc (EPWMCHNL_PWM4, true);
	
	c_pin_out = pnout;
	_pin_low_init_out_pp (c_pin_out, 1);
	timer_timeout_ext_isr.set (0);
	f_speed_control_enabled = false;

	enable_timer_isr (false);
	rectifier_enabled (false);

	local_rpm_tick_counter = 0;
	result_rpm_tick_counter = 0;
	
	rpm_handler = create_timer_ev (this);
	timer_start_periodic (rpm_handler, 1000);
}



void TCONTRECT::periodic_cb (SYSBIOS::HPERIOD h)
{
	if (rpm_handler == h)
		{
		result_rpm_tick_counter = local_rpm_tick_counter;
		local_rpm_tick_counter = 0;
		}
}



float TCONTRECT::get_rpm ()
{
	return result_rpm_tick_counter;
}



void TCONTRECT::control_rectifier (float strt_angl, float stp_angl)
{
	program_rect_angle_on = strt_angl;
	program_rect_angle_off = stp_angl;
}



void TCONTRECT::rectifier_enabled (bool val)
{
	oc_control_enabled (val);
	f_rectifier_system_enabled = val;
}



void TCONTRECT::gpio_rectifier_enable (bool val)
{
	_pin_output (c_pin_out, val);
}



//bool f_oc_interupt_enabled
void TCONTRECT::oc_control_enabled (bool val)
{
	rpmcontr_sw = ERPMSW_NONE;
	//f_phase_value_new_isr = false;
	//f_phase_value_new_sys = false;
	f_oc_interupt_enabled = false;
	f_phase_control_active = false;
	timer_timeout_ext_isr.set (0);

	enable_timer_isr (val);

	gpio_rectifier_enable (false);
	f_oc_interupt_enabled = val;

}



void TCONTRECT::Task ()
{
	
	if (!f_rectifier_system_enabled) return;
	
	if (!timer_timeout_ext_isr.get ())
		{
		f_phase_control_active = false;
		}
	else
		{
		uint32_t period_180 = tick_delta_360_Angle / 2;
		if (f_speed_control_enabled)
			{
			if (speed_stop_sys < period_180)
				{
				// speed is low
				f_phase_control_active = false;
				}
			else
				{
				if (speed_start_sys > period_180) f_phase_control_active = true;				
				}
			}
		else
			{
			f_phase_control_active = true;
			}
		if (f_phase_control_active)
			{

			if (!f_oc_interupt_enabled) oc_control_enabled (true);
			}
		else
			{
			if (f_oc_interupt_enabled) oc_control_enabled (false);
			}
		}
}



float TCONTRECT::get_freq ()
{
	float rv = 1.0F;
	double meas = 0.000001 * tick_delta_360_Angle;
	return rv / meas;
}



void TCONTRECT::tim_comp_cb_isr (ESYSTIM t, EPWMCHNL ch)
{
	if (t == ESYSTIM_TIM2)
		{
		if (f_oc_interupt_enabled)
			{
			switch (ch)
				{
				case EPWMCHNL_PWM1:
				case EPWMCHNL_PWM3:
					{
					gpio_rectifier_enable (true);
					break;
					}
				case EPWMCHNL_PWM2:
				case EPWMCHNL_PWM4:
					{
					gpio_rectifier_enable (false);
					break;
					}
				}
			}
		else
			{
			gpio_rectifier_enable (false);
			}
		}
}



void TCONTRECT::update_phase_points (uint32_t tcnreg)
{
	if (f_oc_interupt_enabled)
		{
		set_timer_oc_value (EPWMCHNL_PWM1, tcnreg + tick_phase_rectifier_isr_on_a);
		set_timer_oc_value (EPWMCHNL_PWM2, tcnreg + tick_phase_rectifier_isr_off_a);
		set_timer_oc_value (EPWMCHNL_PWM3, tcnreg + tick_phase_rectifier_isr_on_b);
		set_timer_oc_value (EPWMCHNL_PWM4, tcnreg + tick_phase_rectifier_isr_off_b);
		}
}





void TCONTRECT::isr_gpio_cb_int (uint8_t isr_n, bool pinstate)
{
TGLOBISR::disable ();
	uint32_t cur_meas_tick = get_timer_counter (), tmp_360meas;
	
	timer_timeout_ext_isr.set (1000);
	
		/*
		tick_phase_rectifier_isr_on_a = tick_phase_rectifier_sys_on_a;
		tick_phase_rectifier_isr_off_a = tick_phase_rectifier_sys_off_a;
		tick_phase_rectifier_isr_on_b = tick_phase_rectifier_sys_on_b;
		tick_phase_rectifier_isr_off_b = tick_phase_rectifier_sys_off_b;
	*/

	switch (rpmcontr_sw)
		{
		case ERPMSW_FRONTE_0_360_ANGLE:
			{
			tick_delta_360_Angle = cur_meas_tick - tick_0_angle;		// прошлое значение фронта tick_delta_360_Angle
			if (tick_delta_360_Angle > 100)
				{
				tick_0_angle = cur_meas_tick;
				uint32_t period_180 = tick_delta_360_Angle / 2;
			
				float val_quant = period_180; val_quant /= 180.0F;
				tick_phase_rectifier_isr_on_a = program_rect_angle_on * val_quant;
				tick_phase_rectifier_isr_off_a = program_rect_angle_off * val_quant;
				tick_phase_rectifier_isr_on_b = period_180 + tick_phase_rectifier_isr_on_a;
				tick_phase_rectifier_isr_off_b = period_180 + tick_phase_rectifier_isr_off_a;
				if (f_phase_control_active) update_phase_points (cur_meas_tick);
				}
			break;
			}
		default:
			{
			rpmcontr_sw = ERPMSW_FRONTE_0_360_ANGLE;
			break;
			}
		}
TGLOBISR::enable ();
}


