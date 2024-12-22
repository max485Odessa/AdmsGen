#include "TPIDMOTOR.h"




// float const *fr_hz,  
TPIDPWM::TPIDPWM (TPWMIFC *pwmi)
{
	pwm = pwmi;
	//time_check.set (tm);
	c_set_cur_i = 0;
	c_set_cur_p = 1.0F;
	acc_cur_i = 1.0F;
	f_enable_sys = false;
	f_enable_pd = false;
	f_enable_i = false;
}



void TPIDPWM::set_freq (float hz)
{
	need_freq = hz;
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



void TPIDPWM::enable_pd (bool val)
{
	f_enable_pd = val;
}



void TPIDPWM::enable_i (bool val)
{
	f_enable_i = val;
}



void TPIDPWM::Task ()
{
	if (!f_enable_sys) return;
	if (f_update_sync)
		{
		f_update_sync = false;
			
		float cur_freq = calculate_herz_from_mks (last_delta_mks);
		float err_val = need_freq - cur_freq;	// скорость меньше - ошибка положительна€, скорость больше ошибка отрицательна€
		float cur_pwm = err_val;
		if (f_enable_pd) cur_pwm *= c_set_cur_p;
		if (f_enable_i) cur_pwm *= acc_cur_i;
			
		if (pwm) {
			if (cur_pwm > 1.0F) cur_pwm = 1.0F;
			pwm->set_pwm (cur_pwm);
			}
		if (f_enable_i)	// включение интегральной составл€ющей
			{
			if ((last_error_freq > 0 && err_val > 0) || (last_error_freq < 0 && err_val < 0)) 
				{
				// ошибка в одну сторону, означает что надо увеличивать интегральную составл€ющую
				acc_cur_i += c_set_cur_i;
				}
			else
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
		last_error_freq = err_val;
		
		}
}



void TPIDPWM::cb_ifhallsync_pulse (EHALLPULSESYNC rslt, uint32_t delt_mks)
{
	last_sync_rslt = rslt;
	last_delta_mks = delt_mks;
	f_update_sync = true;
}


