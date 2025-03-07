#include "TPIDMOTOR.h"




// float const *fr_hz,  
TPIDPWM::TPIDPWM (TPWMIFC *pwmi)
{
	pwm = pwmi;
	freq_mult_value = 1.0F;
	c_set_cur_i = 0;
	c_set_cur_p = 0;
	acc_cur_i = 0;
	acc_cur_d = 0;
	f_enable_sys = false;
	//f_enable_p = true;
	//f_enable_i = true;
	f_enable_d = true;
	
	last_pid_state = EPIDSTATE_OFF;
}



void TPIDPWM::set_freq (float hz)
{
	need_freq = hz;
	stab_cinetic_power = need_freq * need_freq;
}



void TPIDPWM::set_p (float p)
{
	c_set_cur_p = p;
}



void TPIDPWM::set_i (float i)
{
	c_set_cur_i = i;
}



void TPIDPWM::enable_sys (bool val)
{
	if (pwm) pwm->enable (val);
	f_enable_sys = val;
}




void TPIDPWM::enable_i (bool val)
{
	f_enable_i = val;
}



void TPIDPWM::enable_d (bool val)
{
	f_enable_d = val;
}



void TPIDPWM::Task ()
{
	if (!f_enable_sys) 
		{
		last_pid_state = EPIDSTATE_OFF;
		return;
		}
	if (f_update_sync)
		{
		float cur_freq = calculate_herz_from_mks (last_delta_mks);
		f_update_sync = false;
			
		float orig_error = (need_freq * freq_mult_value) - cur_freq; // скорость меньше - ошибка положительна€, скорость больше ошибка отрицательна€
		float pid_val = orig_error;
		
		pid_val *= (c_set_cur_p + acc_cur_i + acc_cur_d);

		
			
		if (pwm) {
			if (pid_val > 1.0F) pid_val = 1.0F;
			pwm->set_pwm (pid_val);
			}
		
		bool f_error_need_plus_i = false;
		bool f_error_need_minus_i = false;
		if (last_error_freq > 0 && orig_error > 0)  f_error_need_plus_i = true;
		if (last_error_freq < 0 && orig_error < 0) f_error_need_minus_i = true;	

		if (f_error_need_plus_i) 
			{
			if (f_enable_i) acc_cur_i += c_set_cur_i;	// ошибка в одну сторону, означает что надо увеличивать интегральную составл€ющую
			seq_pole_error_plus_cnt++;
			seq_pole_error_minus_cnt = 0;
	
			if (seq_pole_error_plus_cnt >= 3)
				{
				last_pid_state = EPIDSTATE_INC;
				}
			else
				{
				last_pid_state = EPIDSTATE_INC;
				}
			}			
			
			if (f_error_need_minus_i)
				{
				seq_pole_error_plus_cnt = 0;
				seq_pole_error_minus_cnt++;
				if (seq_pole_error_minus_cnt >= 3)
					{
					last_pid_state = EPIDSTATE_DEC;
					}
				else
					{
					last_pid_state = EPIDSTATE_STAB;
					}
				if (f_enable_i)
					{
					// ошибка помен€ла направление, означает уменьшать интегральную составл€ющую
					if (acc_cur_i > c_set_cur_i)
						{
						acc_cur_i -= c_set_cur_i;
						}
					else
						{
						acc_cur_i = 0;
						}
					}
				}
		if (!f_error_need_minus_i && !f_error_need_plus_i) last_pid_state = EPIDSTATE_STAB;
		last_error_freq = orig_error;
		
		}
}



void TPIDPWM::freq_mult (float v)
{
	
}



void TPIDPWM::cb_ifhallsync_pulse (EHALLPULSESYNC rslt, uint32_t delt_mks)
{
	last_sync_rslt = rslt;
	last_delta_mks = delt_mks;
	f_update_sync = true;
}



EPIDSTATE TPIDPWM::is_state ()
{
	return last_pid_state;

}


