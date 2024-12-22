#include "THALLDIG.h"
#include "rutine.h"
#include "TGlobalISR.h"


THALLDIG::THALLDIG (TTIM_MKS_ISR *t, TEXTINT_ISR *ei, EPWMCHNL usdch,  uint8_t npnt, uint8_t phas_n): c_points_n ((!npnt)?1:npnt), c_phases_n (phas_n)
{
	points = new S_HALPOINT_T[c_points_n];
	add_points_ix = 0;
	phase_cb = 0;
	sync_cb = 0;
	etim = t;
	extisr = ei;
	used_ch = usdch;
	//c_pin_out = c_pout;
	etim->set_tim_cb (used_ch, this);
	etim->enable_timer_oc (used_ch, true);
	etim->enable_timer_isr (true);
	c_treshold_step = 10;
	c_offset_angl = 0;
	acc_current_period_value = 0;
	
	timcb_isr_handle_1000ms = SYSBIOS::TCBMANAGER::create_cb_slot (true);
	timcb_isr_handle_1000ms->Set_CB (this);
	timcb_isr_handle_1000ms->Set_ID (EHLDGTIMUID_1000MS);
	timcb_isr_handle_1000ms->Start_Periodic (1000);
}



void THALLDIG::timer_cb (uint32_t id)
{
	switch (id)
		{
		case EHLDGTIMUID_1000MS:
			{
			acc_current_period_value = 0;
			if (sync_cb) sync_cb->cb_ifhallsync_pulse (EHALLPULSESYNC_TIMEOUT, acc_current_period_value);
			break;
			}
		}
}



long THALLDIG::find_point_index (uint32_t e)
{
long rv = -1, ix = 0;
while (ix < add_points_ix) {
	if (points[ix].en_name == e) {
		rv = ix;
		break;
		}
	ix++;
	}
return rv;	
}



void THALLDIG::sorted_point ()
{
if (add_points_ix > 1)
	{
	uint32_t fist_ix = 0;
	float value;
	bool f_find;
	while (fist_ix < (add_points_ix-1))
		{
		value = points[fist_ix].angl;
		uint32_t s_ix = fist_ix + 1;
		uint32_t find_ix = s_ix;
		float find_value;
		f_find = false;
		while (s_ix < add_points_ix)
			{
			find_value = points[s_ix].angl;
			if (find_value < value)
				{
				find_ix = s_ix;
				value = find_value;
				f_find = true;
				}
			s_ix++;
			}
		if (f_find)
			{
			// swap places
			S_HALPOINT_T tag = points[fist_ix];
			points[fist_ix] = points[find_ix];
			points[find_ix] = tag;
			}
		fist_ix++;
		}
	}
}



bool THALLDIG::add_replace_point (float angl, uint32_t enname)
{
	bool rv = false;
	long ix = find_point_index (enname);
	if (ix < 0) {
		if (add_points_ix < c_points_n) ix = add_points_ix++;
		}
	if (ix >= 0) 
		{
		points[ix].angl = angl;
		points[ix].en_name = enname;
		points[ix].enable = true;
		points[ix].counter = 0;
		points[ix].oc_offset = 0;
		rv = true;
		}
	return rv;
}



void THALLDIG::enable_point (uint32_t enname, bool enbl)
{
	long ix = find_point_index (enname);
	if (ix >= 0) points[ix].enable = enbl;
}



float THALLDIG::get_point (uint32_t enname)
{
	float rv = 0;
	long ix = find_point_index (enname);
	if (ix >= 0) rv = points[ix].angl;
	return rv;
}



void THALLDIG::enable (bool v)
{
	f_enable = v;
}



void THALLDIG::set_phase_cb (IFHALLPHASECB *c)
{
	phase_cb = c;
}



void THALLDIG::set_sync_cb (IFHALLSYNCPULSECB *c)
{
	sync_cb = c;
}


void THALLDIG::add_acc_period (uint32_t v)
{
acc_current_period_value = v;
/*
if (v > c_treshold_step)
	{
	acc_current_period_value = v;
	}
else
	{
	acc_current_period_value += ((prev_period_value + v)/2);
	}
*/
}



void THALLDIG::isr_gpio_cb_isr (uint8_t isr_n, bool pinstate)
{
	// need control rpm
	TGLOBISR::disable ();
	uint32_t cv = etim->get_timer_counter ();
	uint32_t value = etim->get_delta (prev_period_value, cv);
	add_acc_period (value);
	prev_period_value = cv;
	
	c_period_quantangle = acc_current_period_value;
	c_period_quantangle = c_period_quantangle / 360.0F;
	
	recalculate_point_offsets (cv);
	
	if (add_points_ix)
		{
		search_ix = 0;
		etim->set_timer_oc_value (used_ch, points[search_ix].oc_offset);
		}
	timcb_isr_handle_1000ms->Restart_period ();
	if (sync_cb) sync_cb->cb_ifhallsync_pulse (EHALLPULSESYNC_OK, acc_current_period_value * c_phases_n);
	TGLOBISR::enable ();
}



void THALLDIG::recalculate_point_offsets (uint32_t scnnt)
{
long ix = 0;
uint32_t val;
while (ix < add_points_ix)
	{
	val = ((points[ix].angl + c_offset_angl) * c_period_quantangle) + scnnt; 
	points[ix].oc_offset = val;
	ix++;
	}
}



void THALLDIG::tim_comp_cb_user_isr (ESYSTIM t, EPWMCHNL ch)
{
	if (f_enable == false) return;
	if (add_points_ix && search_ix < add_points_ix)
		{
		etim->set_timer_oc_value (used_ch, points[search_ix + 1].oc_offset);
		if (phase_cb) phase_cb->cb_ifhall_phase (points[search_ix].en_name);
		search_ix++;
		}
}



void THALLDIG::setoffset (float angl)
{
	c_offset_angl = angl;
}



uint32_t THALLDIG::getrpm ()
{
return 60 * get_freq ();
}



float THALLDIG::get_freq ()
{
	float rv = 0;
	uint32_t loc_period = acc_current_period_value;
	if (loc_period) rv = calculate_herz_from_mks (loc_period * c_phases_n);
	return rv;
}



float THALLDIG::get_param_f (uint32_t ix)
{
	float rv = 0;
	switch (ix)
		{
		case EHALPAPAM_RPM:
			{
			rv = getrpm ();
			break;
			}
		default: break;
		}
	return rv;
}



uint32_t THALLDIG::get_param_u32 (uint32_t ix)
{
	uint32_t rv = 0;
	switch (ix)
		{
		case EHALPAPAM_RPM:
			{
			rv = getrpm ();
			break;
			}
		default: break;
		}
	return rv;
}



int32_t THALLDIG::get_param_i32 (uint32_t ix)
{
	int32_t rv = 0;
	switch (ix)
		{
		case EHALPAPAM_RPM:
			{
			rv = getrpm ();
			break;
			}
		default: break;
		}
	return rv;
}





