#include "THALLRATIOMETRIC.h"
#include "math.h"
#include "rutine.h"



TLINEARHALL::TLINEARHALL (uint16_t *adrdata_sa, uint16_t *adrdata_sb, float quant_volt, float mvgaus, IFHALL *c, ESYSTIM t) : ain_quant_volt (quant_volt), sens_mlvolt_g (mvgaus)
{
	cb = c;
	etim = t;
	ain_sens_a = adrdata_sa;
	ain_sens_b = adrdata_sb;
	timisr = new ITIM_MKS_ISR (t, 0xFFFFFFFF, 1000000);
	//timisr->timer_init (10, 1000000);
	sync ();
	asintab = new float[C_ASINTAB_SIZE];
	asintab_generate ();
	clear_dir_status ();
}



void TLINEARHALL::sync ()
{
	ctrlpos = EHALLCTRLPOS_SYNC;
}



float TLINEARHALL::asingen (float sval)
{
	float rv = asin (sval) * (180.0/3.141592653589793238463);
	return rv;
}



void TLINEARHALL::asintab_generate ()
{
	uint16_t ix = 0;
	float cur_sval = 0;
	const float angl_step = 1.0F / C_ASINTAB_SIZE;
	float sinval, asin;
	while (ix < C_ASINTAB_SIZE)
		{
		sinval = cur_sval / C_ASINTAB_SIZE;
		asin = asingen (sinval);
		asintab[ix++]	 = asin;
		cur_sval += angl_step; 
		}
}



void TLINEARHALL::clear_dir_status ()
{
	cur_dir = EMOVEDIR_NONE;
	count_dir_left = 0;
	count_dir_right = 0;
	a_point_dir[ECTRLRPM_90] = EMOVEDIR_NONE;
	a_point_dir[ECTRLRPM_270] = EMOVEDIR_NONE;
	dir_timeout.set (0);
}



void TLINEARHALL::tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch)
{
	uint16_t raw_sa = *ain_sens_a;
	uint16_t raw_sb = *ain_sens_b;
	curent_angle = calc_angle (raw_sa, raw_sb);
	
	// dir control
	if (dirctrl_prev_angle != curent_angle)
		{
		EMOVEDIR c_tmp_dir = EMOVEDIR_NONE;
		uint16_t dlt = abs16 (dirctrl_prev_angle - curent_angle);
		if (dlt < 320)
			{
			if (curent_angle > dirctrl_prev_angle)
				{
				count_dir_right++;
				if (count_dir_left) count_dir_left--;
				}
			else
				{
				count_dir_left++;
				if (count_dir_right) count_dir_right--;
				}
				
			if (count_dir_right > 5 && !count_dir_left) c_tmp_dir = EMOVEDIR_RIGHT;
			if (count_dir_left > 5 && !count_dir_right) c_tmp_dir = EMOVEDIR_LEFT;
			cur_dir = c_tmp_dir;
				
			if (curent_angle > (90-C_A_CTRL_GIST) && curent_angle < (90+C_A_CTRL_GIST)) {
				if (a_point_dir[ECTRLRPM_90] == EMOVEDIR_NONE) a_point_dir[ECTRLRPM_90] = c_tmp_dir;
				}
			if (curent_angle > (270-C_A_CTRL_GIST) && curent_angle < (270+C_A_CTRL_GIST)) {
				if (a_point_dir[ECTRLRPM_270] == EMOVEDIR_NONE) a_point_dir[ECTRLRPM_270] = c_tmp_dir;
				}
			}
		else
			{
			// переполнение угла <360>
			if (a_point_dir[ECTRLRPM_90] == a_point_dir[ECTRLRPM_270]) {
				if (a_point_dir[ECTRLRPM_90] != EMOVEDIR_NONE) local_cnt_hz++;
				}
			a_point_dir[ECTRLRPM_90] = EMOVEDIR_NONE;
			a_point_dir[ECTRLRPM_270] = EMOVEDIR_NONE;
			}
		dir_timeout.set (300);
		dirctrl_prev_angle = curent_angle;
		}
		
	if (rpm_period_cnt)
		{
		rpm_period_cnt--;
		}
	else
		{
		rpm_period_cnt = 100000;
		result_hz = local_cnt_hz;
		local_cnt_hz = 0;
		}
	
	
	int16_t angl = curent_angle; 
	angl += angl_offset;
	angl = angl % 360;
	bool f_internal = false;
	bool f_cb_execute = false;
	if (angl > c_contr_angl_start && angl < c_contr_angl_stop) f_internal = true;
	EHALLPOINT pstate = EHALLPOINT_ENDENUM;
	if (ctrlpos == EHALLCTRLPOS_SYNC)
		{
		if (f_internal)
			{
			pstate = EHALLPOINT_START;
			ctrlpos = EHALLCTRLPOS_INTERNAL;
			f_cb_execute = true;
			}
		else
			{
			ctrlpos = EHALLCTRLPOS_EXTERNAL;
			}
		}
	
	switch (ctrlpos)
		{
		case EHALLCTRLPOS_INTERNAL:
			{
			if (!f_internal)
				{
				ctrlpos = EHALLCTRLPOS_EXTERNAL;
				pstate = EHALLPOINT_STOP;
				f_cb_execute = true;
				}
			break;
			}
		case EHALLCTRLPOS_EXTERNAL:
			{
			if (f_internal)
				{
				pstate = EHALLPOINT_START;
				ctrlpos = EHALLCTRLPOS_INTERNAL;
				f_cb_execute = true;
				}
			break;
			}
		default: break;
		}
	if (f_cb_execute) cb->cb_ifhall (curent_angle, pstate);
}



uint32_t TLINEARHALL::getrpm ()
{
	return result_hz;
}



void TLINEARHALL::Task ()
{
	// dir status generate
	if (!dir_timeout.get()) clear_dir_status ();
}



EMOVEDIR TLINEARHALL::getdir ()
{
	return cur_dir;
}



///static const uint8_t arrpole[]
float TLINEARHALL::calc_angle (uint16_t raw_a, uint16_t raw_b)
{
	float rv = 0;
	uint8_t sel = 0;
	int16_t valp_a = raw_a;
	int16_t valp_b = raw_b;
	
	valp_a -= c_zero_gaus;
	valp_b -= c_zero_gaus;
	if (valp_a >= 0) sel |= 1;
	if (valp_b >= 0) sel |= 2;
	if (valp_a < 0) valp_a *= -1;
	if (valp_a > c_peack_gaus) valp_a = c_peack_gaus;
	float valsin_a = valp_a;
	valsin_a /= c_peack_gaus;

	float angl_asin = asingen (valsin_a);
	
	switch (sel)
		{
		case ESPOLE_0_89:
			{
			rv = angl_asin;
			break;
			}
		case ESPOLE_90_179:
			{
			rv = 180.0F - angl_asin;
			break;
			}
		case ESPOLE_180_269:
			{
			rv = 180.0F + angl_asin;
			break;
			}
		case ESPOLE_270_359:
			{
			rv = 360.0F - angl_asin;
			break;
			}
		}
return rv;
}



void TLINEARHALL::setbaund (uint16_t p1_val, uint16_t p2_val)
{
	uint16_t bnd;
	if (p1_val >= p2_val)
		{
		bnd = p1_val - p2_val;
		c_zero_gaus = p2_val;
		}
	else
		{
		bnd = p2_val - p1_val;
		c_zero_gaus = p1_val;
		}
	bnd /= 2;
	c_zero_gaus += bnd;
	c_peack_gaus = bnd;
}



void TLINEARHALL::setangle_start (float angl)
{
	c_contr_angl_start = angl;
}



void TLINEARHALL::setangle_stop (float angl)
{
	c_contr_angl_stop = angl;
}



int16_t TLINEARHALL::getangle_now ()
{
	int16_t rv = -1;
	if (f_enabled) rv = curent_angle;
	return rv;
}



void TLINEARHALL::enabled (bool v)
{
	if (f_enabled != v)
		{
		if (v) sync ();
		f_enabled = v;
		timisr->enable_timer_isr (f_enabled);
		}
}


		