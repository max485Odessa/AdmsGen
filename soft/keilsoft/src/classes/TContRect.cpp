#include "TcontRect.h"



TCONTRECT::TCONTRECT (S_GPIOPIN *pnout, S_GPIOPIN *pnin, ESYSTIM tt, uint32_t freq_hz, uint8_t cmagcnt) : c_magnet_cnt (cmagcnt), IEXTINT_ISR(pnin, EGPINTMOD_RISING_FALLING), ITIM_ISR (tt)
{
	timer_init (0xFFFFFFFF, freq_hz);
	enable_timer_oc (EPWMCHNL_PWM1, true);
	enable_timer_oc (EPWMCHNL_PWM2, true);
	
	c_pin_out = pnout;
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
	f_phase_value_new_isr = false;
	f_phase_value_new_sys = false;
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
			if (!f_phase_value_new_sys)
				{
				float val_quant = period_180; val_quant /= 180.0F;
				tick_phase_rectifier_sys_on = program_rect_angle_on * val_quant;
				tick_phase_rectifier_sys_off = program_rect_angle_off * val_quant;
				f_phase_value_new_sys = true;
				}	
			if (f_phase_value_new_isr && !f_oc_interupt_enabled) oc_control_enabled (true);
			}
		else
			{
			if (f_oc_interupt_enabled) oc_control_enabled (false);
			}
		}
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
					{
					gpio_rectifier_enable (true);
					break;
					}
				case EPWMCHNL_PWM2:
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
		set_timer_oc_value (EPWMCHNL_PWM1, tcnreg + tick_phase_rectifier_isr_on);
		set_timer_oc_value (EPWMCHNL_PWM2, tcnreg + tick_phase_rectifier_isr_off);
		}
}




void TCONTRECT::isr_gpio_cb_int (uint8_t isr_n, bool pinstate)
{
	uint32_t cur_meas_tick = get_timer_counter ();
	timer_timeout_ext_isr.set (1000);
	
	if (f_phase_value_new_sys)
		{
		tick_phase_rectifier_isr_on = tick_phase_rectifier_sys_on;
		tick_phase_rectifier_isr_off = tick_phase_rectifier_sys_off;
		f_phase_value_new_sys = false;
		f_phase_value_new_isr = true;
		}
	bool f_error = false;
	switch (rpmcontr_sw)
		{
		case ERPMSW_FRONTE_0_360_ANGLE:
			{
			if (pinstate)
				{
				tick_delta_360_Angle = cur_meas_tick - tick_0_angle;		// прошлое значение фронта
				tick_0_angle = cur_meas_tick;
					
				if (f_phase_control_active && f_phase_value_new_isr) update_phase_points (cur_meas_tick);
					
				rpmcontr_sw = ERPMSW_FALL_180_ANGLE;
				}
			else
				{
				f_error = true;
				}
			break;
			}
		case ERPMSW_FALL_180_ANGLE:
			{
			if (!pinstate)
				{
				tick_180_angle = cur_meas_tick;
					
				if (f_phase_control_active && f_phase_value_new_isr) update_phase_points (cur_meas_tick);
					
				rpmcontr_sw = ERPMSW_FRONTE_0_360_ANGLE;
				local_rpm_tick_counter++;
				}
			else
				{
				f_error = true;
				}
			break;
			}
		default:
			{
			f_error = true;
			break;
			}
		}
	if (f_error)
		{
		rpmcontr_sw = ERPMSW_FRONTE_0_360_ANGLE;
		f_phase_control_active = false;
		f_phase_value_new_sys = false;
		f_phase_value_new_isr = false;
		}
}


